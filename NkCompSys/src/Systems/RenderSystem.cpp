/* 
Copyright 2015 Alvaro Herrasti and Daniel Dovali

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "GameSystems/RenderSystem.h"
#include "GameComponents/NewRenderComponent.h"
#include "GameComponents/NewTextureComponent.h"
#include "GameComponents/ShaderComponent.h"


int createShaderr(const char* vShader, const char* fShader) {
	GLuint vertexshader = shader_utils::createShaderFromFile(vShader, GL_VERTEX_SHADER);
	GLuint  fragmentshader = shader_utils::createShaderFromFile(fShader, GL_FRAGMENT_SHADER);
    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexshader);
    glAttachShader(shaderProgram, fragmentshader);

    glLinkProgram(shaderProgram);
	if (!shader_utils::checkLinkError(shaderProgram)) return -1;

    glUseProgram(shaderProgram);
   
    //Camara
	GLuint viewMatLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint projMatLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniform1i(glGetUniformLocation(shaderProgram, "samplerCube"), 0);
	glUseProgram(0);
	return shaderProgram;
}

RenderSystem::RenderSystem()
: mCanvas() {
	skyBox.create();
	skyBox.createShader("shaders\\skybox.vert", "shaders\\skybox.frag");
	std::string filenames[6];

	filenames[0] = "skybox\\yellowcloud_ft.jpg";
	filenames[1] = "skybox\\yellowcloud_bk.jpg";
	filenames[2] = "skybox\\yellowcloud_up.jpg";
	filenames[3] = "skybox\\yellowcloud_dn.jpg";
	filenames[4] = "skybox\\yellowcloud_lf.jpg";
	filenames[5] = "skybox\\yellowcloud_rt.jpg";

	skyBox.createTextures(filenames);

	shaderProgram = createShaderr("shaders\\MiniMapShader.vert", "shaders\\MiniMapShader.frag");
}

void RenderSystem::setCanvas(Canvas canvas) {
	this->mCanvas = canvas;
}


RenderSystem::~RenderSystem(void)
{
}


void RenderSystem::update(double time, SystemMessageSender* msgSender)
{
	std::vector<c_handle<NewRenderComponent>> renderComps = 
		GameObjectFactory::getComponentHandlesOfType<NewRenderComponent>();

	std::vector<c_handle<CameraComponent>> cameraHandles = GameObjectFactory::getComponentHandlesOfType<CameraComponent>();
	c_handle<CameraComponent> cameraHandle;
	if (cameraHandles.size() > 0) {
		cameraHandle = cameraHandles[0];
	}
	_ASSERT(cameraHandle);

	float shininess = 50;
	float attenuation = 0.00009f;
	glm::vec4 ambient(0.15f, 0.15f, 0.15f, 1.0f);
	glm::vec4 diffuse(0.9f, 0.9f, 0.9f, 1.0f);

	glm::mat4 projMat = cameraHandle->projectionMatrix;
	glm::mat4 viewMat = cameraHandle->getViewMatrix(cameraHandle);

	skyBox.draw(viewMat, projMat);
	glm::vec4 lightPosV = viewMat * glm::vec4(-60.0f, 8.0f, -20.0f, 1.0f);

	//shadowFBO.renderShadowPass(compHandles, lightPosV.xyz);

	//intt totalRenders = GameStateMachine::getCaptureFrames()? 2: 1;

	int totalRenders = 1;

	for (int count = 0; count < totalRenders; count++) {

	// TODO: set viewport on canvas
	if (count == 1) {
		glViewport(0,0, frameFBO.resWidth, frameFBO.resHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, frameFBO.fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//skyBox.draw(viewMat, projMat);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, GLsizei(cameraHandle->width), GLsizei(cameraHandle->height));
	}

	static int debug = 0;
	if (debug++ < 60) {
		logGLError("Draw:");
	}

	//glEnable (GL_BLEND);

	for (c_handle<NewRenderComponent> &renderH : renderComps)
	{
		c_handle<GameEntity> parentP = renderH->getParentEntity();
		if (renderH && renderH->getParentEntity()){
			c_handle<TransformComponent> transformHandle = 
				GameObjectFactory::getComponentBrother<TransformComponent>(renderH);
			c_handle<ShaderComponent> shaderH = 
				GameObjectFactory::getComponentBrother<ShaderComponent>(renderH);
			c_handle<NewTextureComponent> texHandle = 
				GameObjectFactory::getComponentBrother<NewTextureComponent>(renderH);
			
			if(transformHandle && shaderH)
			{
				TransformComponent& transform = transformHandle.get();

				std::vector<UniformBind> uniforms;
				uniforms.push_back(UniformsFuncs::createUniform<float>(
					UniformsFuncs::set1f, "shininess", shininess));
				uniforms.push_back(UniformsFuncs::createUniform<float, float, float, float>(
					UniformsFuncs::set4f, "specularColor", 1.0f, 1.0f, 1.0f, 1.0f));
				uniforms.push_back(UniformsFuncs::createUniform<int, const float*>(
					UniformsFuncs::set3fv, "lightpos", 1, &lightPosV[0])); 
				//TODO: check why not &lightPosV[0]

				uniforms.push_back(UniformsFuncs::createUniform<int, bool, const float*>(
					UniformsFuncs::setMat4fv, "viewMatrix", 1, false, &viewMat[0][0]));

				uniforms.push_back(UniformsFuncs::createUniform<int, bool, const float*>(
					UniformsFuncs::setMat4fv, "projectionMatrix", 1, false,
					&cameraHandle->projectionMatrix[0][0]));

				glm::vec3 offset=glm::vec3();
				glm::vec3 offsetRot=glm::vec3();
				float signo = 1.0f;

				c_handle<OffsetComponent> offsetH = 
					GameObjectFactory::getComponentBrother<OffsetComponent>(renderH);
				if(offsetH)
				{
					offset = offsetH->offsetMov;
					offsetRot = offsetH->offsetRot;
					glm::mat4 identity;
				}
					
				glm::vec3 orient = signo * (glm::eulerAngles(transform.orientation) 
					+ offsetRot.xyz);
				glm::quat quaternion = glm::quat(orient);

				glm::mat4 rotationTranslate = glm::translate(offset);

				glm::mat4 rotation = glm::mat4_cast(quaternion);
				glm::mat4 mat = glm::translate(transform.position) * rotation * rotationTranslate 
					* glm::scale(transform.scale);

				uniforms.push_back(UniformsFuncs::createUniform<int, bool, const float*>(
					UniformsFuncs::setMat4fv, "modelMatrix", 1, false, &transform.worldModelMat[0][0]));

				std::vector<Texture2D> texs;
				if (texHandle) {
					glm::vec4& Kd = texHandle->mKd;
					glm::vec4& Ka = texHandle->mKa;

					uniforms.push_back(UniformsFuncs::createUniform<float, float, float, float>(
						UniformsFuncs::set4f, "lightIntensity", diffuse.x * Kd.x, diffuse.y * Kd.y,
						diffuse.z * Kd.z, diffuse.w * Kd.w));
					uniforms.push_back(UniformsFuncs::createUniform<float, float, float, float>(
						UniformsFuncs::set4f, "ambientIntensity", ambient.x*Ka.x, ambient.y*Ka.y,
						ambient.z*Ka.z, ambient.w*Ka.w));

					uniforms.push_back(UniformsFuncs::createUniform<float>(UniformsFuncs::set1f,
						"lightAttenuation", attenuation * texHandle->mLightAttenuation));

					texs = std::vector<Texture2D>(texHandle->mTexs);
				}
				else {
					uniforms.push_back(UniformsFuncs::createUniform<int, const float*>(
						UniformsFuncs::set4fv, "lightIntensity", 1, &diffuse[0]));
					uniforms.push_back(UniformsFuncs::createUniform<int, const float*>(
						UniformsFuncs::set4fv, "ambientIntensity", 1, &ambient[0]));
					uniforms.push_back(UniformsFuncs::createUniform<float>(UniformsFuncs::set1f,
						"lightAttenuation", attenuation));
				}

				// agregar las uniforms de cada render component
				for (auto it = renderH->mProperties.begin(); it != renderH->mProperties.end(); ++it) {
					UniformBind uni = it->second->bind(it->first);
					uniforms.push_back(uni);
				}
					
				this->mCanvas.draw(shaderH->shaderProgramId, renderH->mPrimitive, 
					renderH->mData.mVAO, renderH->mData.mUsedIndices, (void*)renderH->mDrawOffset, 
					texs, uniforms);
			}
		}
	}

	//glDisable(GL_BLEND);

		if (count == 1) {
			frameFBO.saveFrame("frame");
		}

	}

	c_handle<TransformComponent> camPosH = 
		GameObjectFactory::getComponentBrother<TransformComponent>(cameraHandle);
	bool flag = true;

	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderSystem::init(void)
{
}


void RenderSystem::send(const Message& msg) {

}