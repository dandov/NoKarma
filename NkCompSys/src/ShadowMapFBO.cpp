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

#include "ShadowMapFBO.h"


ShadowMapFBO::ShadowMapFBO(int resWidth, int resHeight) : resHeight(resHeight), resWidth(resWidth)
{


 glGenFramebuffers(1, &this->fbo);
 glBindFramebuffer(GL_FRAMEBUFFER, fbo);

 glGenTextures(1, &this->textureId);
 glBindTexture(GL_TEXTURE_2D, textureId);
 glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, resWidth, resHeight, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
 glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);
 
 glDrawBuffer(GL_NONE);
 
 if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
  std::cout << "ERROR FRAMEBUFFER STATUS " << std::endl;

 }

 createShader("Shaders\\shadowMapping.vert", "Shaders\\shadowMapping.frag");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

int ShadowMapFBO::createShader(const char* vShader, const char* fShader) {
	GLuint vertexshader = shader_utils::createShaderFromFile(vShader, GL_VERTEX_SHADER);
	GLuint  fragmentshader = shader_utils::createShaderFromFile(fShader, GL_FRAGMENT_SHADER);
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexshader);
    glAttachShader(shaderProgram, fragmentshader);

    glLinkProgram(shaderProgram);
	if (!shader_utils::checkLinkError(shaderProgram)) return -1;

    glUseProgram(shaderProgram);
   
    //Camara
	depthMatLoc = glGetUniformLocation(shaderProgram, "depthMVP");
	glUniform1i(glGetUniformLocation(shaderProgram, "samplerCube"), 0);
	glUseProgram(0);
	return shaderProgram;
}


void ShadowMapFBO::renderShadowPass(Canvas* canvas, 
	const std::vector<c_handle<NewRenderComponent>>& components, const glm::vec3& lightDir) {

	//logGLError("FBOStart");
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// Render on the whole framebuffer, complete from the lower left corner to the upper right
		glViewport(0,0,1024,1024); 

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		//logGLError("FBOUSe");

		glm::vec3 lightInvDir = glm::vec3(0.5f,2,2);

		// Compute the MVP matrix from the light's point of view
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
		glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
		// or, for spot light :
		//glm::vec3 lightPos(5, 20, 20);
		//glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
		//glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));

	//glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
	//glm::mat4 depthViewMatrix = glm::lookAt(lightDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
 
	glUniformMatrix4fv(depthMatLoc, 1, GL_FALSE, &depthMVP[0][0]);
	//logGLError("FBO0");
	std::vector<c_handle<NewRenderComponent>> compHandles = 
	 GameObjectFactory::getComponentHandlesOfType<NewRenderComponent>();


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	for (c_handle<NewRenderComponent> &handle : compHandles) {
		glBindVertexArray(handle->mData.mVAO);
		//logGLError("FBO01");

		glDrawElements(canvas->getDevice()->getPrimitive(handle->mPrimitive), 
		handle->mData.mUsedIndices, GL_UNSIGNED_SHORT, 
		(GLvoid*)(handle->mDrawOffset*sizeof(unsigned int)));
		//logGLError("FBO1");
		//logGLError("FBO2");
		glBindVertexArray(0);
	}

	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//logGLError("SSD");
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//logGLError("End");

	glm::mat4 biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);
	depthBiasMVP = biasMatrix*depthMVP;
}


ShadowMapFBO::~ShadowMapFBO(void)
{
}
