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

#pragma once
#include <gl\glew.h>
#include <Windows.h>
#include <FreeImage.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "utils.h"

struct vertex
{
	float x, y, z;
	float s, t;
	float nx, ny, nz;
};

class Terrain
{
public:
	Terrain(const char* heightmapfile, const char* terraintexturefile, const float& initialH, const unsigned int& width, const unsigned int& lenght, const unsigned short& detail);
	bool createGLBuffers();
	void draw(const GLuint& drawProgram, const glm::mat4& modelViewMat);
	inline float getBaseHeight(){return initialheight;}
	~Terrain(void);

private:
	GLubyte * Terrain::loadImage(const char* heightmapfile);
	bool Terrain::loadTexture(const char* texturefile);
	void createTerrain(unsigned char*& heightdata);
	void computeNormals();
	unsigned char* heightData;
	unsigned int twidth, theight, vwidth, vheight;
	unsigned short detail;

	float initialheight;
	GLuint vao;
	GLuint texture;
	GLuint vbo;
	vertex* vertexData;
	std::vector<unsigned int> indexData;
	unsigned int indexNumber;
	GLubyte * texData;
	unsigned int texwidth, texheight;

	

};

