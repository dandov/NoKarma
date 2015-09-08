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

/*
 * ModelLoader.h
 *
 *  Created on: Feb 18, 2013
 *      Author: Alvaro
 */
#ifndef MODELLOADER_H_
#define MODELLOADER_H_

#pragma once

#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <deque>
#include <limits>

#include <GL/glew.h>
#include <Windows.h>
// pinche FreeImage que incluya Windows.h antes sino se redefinen tipos
#include <FreeImage.h>
//#include <FreeImage.h>

#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include <sys/types.h>
//#include <stdarg.h>

#include "utils.h"

/*template <typename V, typename T, typename I>
struct ModelData
{
	typedef V TEXT_YPE;
	typedef T VERT_TYPE;
	typedef I INDX_TYPE;

	vector<V> vertexMem;
	unsigned int* vertexnum;
    vector<T> textMem;
    unsigned int* textureWH;
	vector<I> indxMem;
	unsigned int * indxnum;

    unsigned int* vsize;
    unsigned int* vtsize;
    unsigned int* vnsize;

};*/
const unsigned int INITIAL_VERTEX_MEM = 3600;
const unsigned int INITIAL_INDEX_MEM = 1000;
const unsigned int SIZEMULT = 1;
const unsigned int TOTAL_TEXTURE_TYPES = 2;

enum TEXUTE_TYPE {
	DIFFUSE = 0,
	NORMAL = 1
};

struct Texture {
	int width;
	int height;
	int channels;
	FIBITMAP* bm;
	GLubyte* data;
	int index;
	Texture(int w = 0, int h = 0, int ch = 0, GLubyte* pixels = 0, FIBITMAP* bm = NULL,  int index = 0) : width(w), height(h), channels(ch), data(pixels), bm(bm), index(index) {
	}
};

struct ModelMaterial {
	float Ka[3];
	float Kd[3];
	//int textureIndex;
	unsigned int textureIndices[TOTAL_TEXTURE_TYPES];
	GLuint indexOffset;
	GLuint totalIndices;
	ModelMaterial() : indexOffset(0), totalIndices(0) {
		memset(textureIndices, -1, sizeof(textureIndices));
	}

};

struct Component {
	unsigned short vsize;
	unsigned short vtsize;
	unsigned short vnsize;
};


struct ModelData
{
	std::vector<GLfloat> vertexMem;
	unsigned int vertexnum;

	unsigned short groupNumber;
    std::vector<ModelMaterial> groups;

	std::vector<GLuint> indxMem;
	unsigned int indxnum;

    Component components;
    std::vector<Texture> textures;
    unsigned int textCount;
    //vector<Texture> animTextures;
   // unsigned int animCount;
	GLenum primitives;

    unsigned int size;

	ModelData(int initialIndexMemSize = INITIAL_INDEX_MEM, int initialVertexMemSize = INITIAL_VERTEX_MEM) :vertexMem(SIZEMULT*initialVertexMemSize), indxMem(SIZEMULT*initialIndexMemSize),
			groups(), vertexnum(0), indxnum(0), components(), groupNumber(1),
			size(0), textCount(0), primitives(GL_TRIANGLES)
	
	{

	}
	~ModelData()
	{
		/*for (std::vector<Texture>::iterator it = textures.begin(); it < textures.end(); it++) {
			if (it->bm)
				FreeImage_Unload(it->bm);
		}*/
	}
};

/*class model_handle {
	model_handle(unsigned int index)
}*/

class ModelLoader
{
public:
	ModelLoader();

	static int loadModel(const char* fileName,
						 ModelData& data,
					 	 bool ignoreGroups = true);

	static int createTexturedSquare(const char* textureName,
									ModelData& data);

	static int createSquare(ModelData& data);

	static int createHeightField(unsigned char*& heightdata,
									unsigned int twidth, 
									unsigned int theight,
									unsigned short detail,
									const char* textureFilename,
									ModelData& data,
									btScalar& maxHeight,
									btScalar& minHeight);
	static int ModelLoader::createHeightFieldTriangles(unsigned char*& heightData, 
								   unsigned int twidth,
								   unsigned int theight,
								   unsigned short detail,
								   const glm::vec3& scale,
								   const char* textureFilename,
								   ModelData& data,
								   btScalar& maxHeight,
								   btScalar& minHeight,
								   float* outHdata);

	static int loadTexture(const char* textureName,  ModelData& data);

	static unsigned int getInitialModels() { return initialModels; }

	static int LOG(const char * format, ... );

	static void unloadTextures();

	~ModelLoader(void);
private:
	//static ModelData data;
	static void computeHeightFieldNormals(ModelData& data, int compnumber, int vwidth, int vheight);
	static void computeHeightFieldTriangleNormals(ModelData& data, int compnumber, int vwidth, int vheight);

	static std::unordered_map<std::string, Texture> loadedTextures;
	static bool resizeData(ModelData& data, int indx);
	static unsigned int initialModels;



};


#endif /* MODELLOADER_H_ */
