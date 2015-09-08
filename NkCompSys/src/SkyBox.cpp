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

#include "SkyBox.h"


SkyBox::SkyBox(void)
{
}

int SkyBox::createShader(const char* vShader, const char* fShader) {
	GLuint vertexshader = shader_utils::createShaderFromFile("Shaders\\skybox.vert", GL_VERTEX_SHADER);
    GLuint  fragmentshader = shader_utils::createShaderFromFile("Shaders\\skybox.frag", GL_FRAGMENT_SHADER);
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexshader);
    glAttachShader(shaderProgram, fragmentshader);

    glLinkProgram(shaderProgram);
	if (!shader_utils::checkLinkError(shaderProgram)) return -1;

    glUseProgram(shaderProgram);
   
    //Camara
	viewMatLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
	projMatLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniform1i(glGetUniformLocation(shaderProgram, "samplerCube"), 0);
	glUseProgram(0);
	return shaderProgram;
}


void SkyBox::setProjMatrix(glm::mat4 projMat) {
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, &projMat[0][0]);
	glUseProgram(0);
}
void SkyBox::draw(glm::mat4 viewMat, glm::mat4 projMat) {
	viewMat[3].x = 0.0;
	viewMat[3].y = 0.0;
	viewMat[3].z = 0.0;
	glDepthMask(GL_FALSE);
	//logGLError("0");
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, &viewMat[0][0]);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, &projMat[0][0]);
	glBindVertexArray(vao);
	//logGLError("vao");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);
	//logGLError("bind");
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//logGLError("draw");
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glBindVertexArray(0);
	glUseProgram(0);

	glDepthMask(GL_TRUE);
}
void SkyBox::create() {
	SVertexData vd;
	vd.vertices.push_back(glm::vec3(-1.0,1.0,1.0));
	vd.vertices.push_back(glm::vec3(-1.0,-1.0,1.0));
	vd.vertices.push_back(glm::vec3(1.0,-1.0,1.0));	
	vd.vertices.push_back(glm::vec3(1.0,1.0,1.0));
	vd.vertices.push_back(glm::vec3(-1.0,1.0,-1.0));	
	vd.vertices.push_back(glm::vec3(-1.0,-1.0,-1.0));
	vd.vertices.push_back(glm::vec3(1.0,-1.0,-1.0));
	vd.vertices.push_back(glm::vec3(1.0,1.0,-1.0));

	/*vd.vertices.push_back(glm::vec3(-100.0,100.0,100.0));
	vd.vertices.push_back(glm::vec3(-100.0,-100.0,100.0));
	vd.vertices.push_back(glm::vec3(100.0,-100.0,100.0));	
	vd.vertices.push_back(glm::vec3(100.0,100.0,100.0));
	vd.vertices.push_back(glm::vec3(-100.0,100.0,-100.0));	
	vd.vertices.push_back(glm::vec3(-100.0,-100.0,-100.0));
	vd.vertices.push_back(glm::vec3(100.0,-100.0,-100.0));
	vd.vertices.push_back(glm::vec3(100.0,100.0,-100.0));*/
		
		//front face
		vd.indices.push_back(0);
		vd.indices.push_back(1);
		vd.indices.push_back(2);
		
		vd.indices.push_back(0);
		vd.indices.push_back(3);
		vd.indices.push_back(2);
		
		
		//left face
		vd.indices.push_back(2);
		vd.indices.push_back(3);
		vd.indices.push_back(6);
		
		vd.indices.push_back(3);
		vd.indices.push_back(7);
		vd.indices.push_back(6);
		
		//back face
		vd.indices.push_back(4);
		vd.indices.push_back(5);
		vd.indices.push_back(6);
		
		vd.indices.push_back(4);
		vd.indices.push_back(7);
		vd.indices.push_back(6);
		
		//right face
		vd.indices.push_back(0);
		vd.indices.push_back(1);
		vd.indices.push_back(5);
		
		vd.indices.push_back(0);
		vd.indices.push_back(4);
		vd.indices.push_back(5);
		
		
		//top face
		vd.indices.push_back(0);
		vd.indices.push_back(3);
		vd.indices.push_back(4);
		
		vd.indices.push_back(3);
		vd.indices.push_back(4);
		vd.indices.push_back(7);
		
		
		//bottom face
		vd.indices.push_back(1);
		vd.indices.push_back(2);
		vd.indices.push_back(6);
		
		vd.indices.push_back(1);
		vd.indices.push_back(5);
		vd.indices.push_back(6);

	float* p = &vd.vertices[0].x;

	for (int  i = 0; i < 6; i++) {
		printf("ssss: %f\n", p[i]);
	}


	GLuint buffs[2];
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, buffs);
	GLuint vbo = buffs[0];
	this->ibo = buffs[1];
	printf("vbo %d ibo %d ", vbo, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
		3 * vd.vertices.size()* sizeof(float),
		&vd.vertices[0].x, GL_STATIC_DRAW);

	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	printf(
		"sizeof(verts): %d, indices: %d\n", vd.vertices.size(), vd.indices.size());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			vd.indices.size() * sizeof(unsigned int),
			&vd.indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
	logGLError("Buffers");

	printf("FINISHED BUFFER CREATION firsts vbo %d ibo %d\n", vbo, ibo);

}

int SkyBox::createBluredTextures(std::string* filenames) {
	glGenTextures(1, &this->blurCubeMapId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, blurCubeMapId);
	
	for(int i=0;i<6;i++)
	{
		FREE_IMAGE_FORMAT f = FreeImage_GetFileType(filenames[i].c_str(), 0);
		FIBITMAP* bm = FreeImage_Load(f, filenames[i].c_str());

		GLubyte * pixels = NULL;
		unsigned int twidth = 0;
		unsigned int theight = 0;
		if (bm != NULL)
		{

			bm = FreeImage_ConvertTo24Bits(bm);
			twidth = FreeImage_GetWidth(bm);
			theight = FreeImage_GetHeight(bm);
			pixels = (GLubyte*)FreeImage_GetBits(bm);
			printf(" Texture loaded! ");


		}
		else {
			printf( " could not find texture ");
			return -1;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,twidth,theight,0,GL_BGR,GL_UNSIGNED_BYTE,pixels);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		FreeImage_Unload(bm);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP,0);
	return blurCubeMapId;
}

int SkyBox::createTextures(std::string* filenames) {
	glGenTextures(1, &this->cubeMapId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);
	
	for(int i=0;i<6;i++)
	{
		FREE_IMAGE_FORMAT f = FreeImage_GetFileType(filenames[i].c_str(), 0);
		FIBITMAP* bm = FreeImage_Load(f, filenames[i].c_str());

		GLubyte * pixels = NULL;
		unsigned int twidth = 0;
		unsigned int theight = 0;
		if (bm != NULL)
		{

			int bpp = FreeImage_GetBPP(bm);
			std::cout << " Skybox bpp: " << bpp << std::endl;
			unsigned red_mask, green_mask, blue_mask;
			red_mask = FreeImage_GetRedMask(bm);
			bm = FreeImage_ConvertTo24Bits(bm);
			twidth = FreeImage_GetWidth(bm);
			theight = FreeImage_GetHeight(bm);
			pixels = (GLubyte*)FreeImage_GetBits(bm);
			printf(" Texture loaded! ");


		}
		else {
			printf( " could not find texture ");
			return -1;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,twidth,theight,0,GL_BGR,GL_UNSIGNED_BYTE,pixels);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		FreeImage_Unload(bm);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP,0);
	return cubeMapId;
}

SkyBox::~SkyBox(void)
{
}
