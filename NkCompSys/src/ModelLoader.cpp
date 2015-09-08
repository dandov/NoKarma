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
 * ModelLoader.cpp
 *
 *  Created on: Feb 18, 2013
 *      Author: Alvaro
 */

#include "ModelLoader.h"


unsigned int ModelLoader::initialModels = 2;

std::unordered_map<std::string, Texture> ModelLoader::loadedTextures;

ModelLoader::ModelLoader() {
}

int ModelLoader::LOG(const char * format, ... ) {
	/*va_list arg;
    int done;

	va_start (arg, format);
	done = printf(format, arg);
	va_end (arg);
	return done;*/
	return 0;
}

void ModelLoader::unloadTextures() {
	for (std::unordered_map<std::string, Texture>::iterator it = loadedTextures.begin(); it != loadedTextures.end(); it++) {
		if (it->second.bm) {
			FreeImage_Unload(it->second.bm);
		}
	}
	loadedTextures.clear();
}

bool ModelLoader::resizeData(ModelData& data, int indx) {
	LOG("data resize %d", SIZEMULT*indx);
	/*data.vertexnum.resize(SIZEMULT*indx);
	data.indxnum.resize(SIZEMULT*indx);
	data.groupNumber.resize(SIZEMULT*indx);
	data.compontents.resize(SIZEMULT*indx);
	data.groups.resize(SIZEMULT*indx);
	data.vertexMemOffset.resize(SIZEMULT*indx);
	data.indexMemOffset.resize(SIZEMULT*indx);
	//data.modelTextureNum.resize(indx*SIZEMULT);*/


	return true;
}

/*int ModelLoader::deleteModel(const char* filename) {
	if (!filename || data.loadedModels.count(filename) == 0) {
		return -1;
	}
	unsigned int index = data.loadedModels[filename];
	unsigned int vMemIndx = data.vertexMemOffset[index];
	unsigned int vsize = data.vertexnum[index];
	data.vertexMem.erase(data.vertexMem.begin()+vMemIndx, data.vertexMem.begin()+vMemIndx+vsize);
}*/

int ModelLoader::createSquare(ModelData& data) {
	const float vertices[] = { -0.5, -0.5, 0.0, 	0.0, 0.0,
								0.5, -0.5, 0.0, 	1.0, 0.0,
								0.5,  0.5, 0.0, 	1.0, 1.0,
								-0.5, 0.5, 0.0,		0.0, 1.0 };
	const unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };


	 unsigned int objectnumber = 1;
	 unsigned int groupnumber = 1;
	 unsigned int vsumber = 3;
	 unsigned int vnnumber = 0;
	 unsigned int vtnumber = 2;

	 unsigned int compnumber = 5;
	 char w = 0;

	 unsigned int totalvnumber =  (sizeof(vertices)/sizeof(float))/compnumber;
		 LOG("Loader total verts %d", totalvnumber);

	 data.groupNumber = groupnumber;

	 unsigned int totalVData = totalvnumber*compnumber;
	 if (data.vertexMem.size() < totalVData) {
		 data.vertexMem.resize(totalVData);
	 }

	 data.vertexnum = totalvnumber;

	 memcpy(&data.vertexMem[0], vertices, sizeof(vertices));

	 unsigned int totalindx = sizeof(indices)/sizeof(int);

	 if (data.indxMem.size() < totalindx)
		 data.indxMem.resize(totalindx);


	 data.indxnum = totalindx;

	 memcpy(&data.indxMem[0], indices, sizeof(indices));

	 
	 data.components.vsize = 3;
	 data.components.vtsize = 2;
	 data.components.vnsize = 0;


	 ModelMaterial group;
	 group.indexOffset = 0;

	 //group.textureIndex = -1;

	 data.groups.push_back(group);

	 data.size++;

	 return 1;
}

int ModelLoader::createTexturedSquare(const char* textureName,
									  ModelData& data) {
	/*const float vertices[] = { -5000.5, -5000.5, 0.0, 	0.0, 0.0,
								5000.5, -5000.5, 0.0, 	1.0, 0.0,
								5000.5,  5000.5, 0.0, 	1.0, 1.0,
								-5000.5, 5000.5, 0.0,		0.0, 1.0 };*/
	const float vertices[] = { -0.5, -0.5, 0.0, 	0.0, 0.0,
								0.5, -0.5, 0.0, 	1.0, 0.0,
								0.5,  0.5, 0.0, 	1.0, 1.0,
								-0.5, 0.5, 0.0,		0.0, 1.0 };
	const unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };


	 unsigned int objectnumber = 1;
	 unsigned int groupnumber = 1;
	 unsigned int vsumber = 3;
	 unsigned int vnnumber = 0;
	 unsigned int vtnumber = 2;

	 unsigned int compnumber = 5;
	 char w = 0;

	 unsigned int totalvnumber =  (sizeof(vertices)/sizeof(float))/compnumber;
	 	 LOG("Loader total verts %d", totalvnumber);

	 data.groupNumber = groupnumber;

	 unsigned int totalMem = totalvnumber*compnumber;
	 if (data.vertexMem.size() < totalMem) {
		 data.vertexMem.resize(totalMem);
	 }

	 data.vertexnum = totalvnumber;

	 memcpy(&data.vertexMem[0], vertices, sizeof(vertices));

	 unsigned int totalindx = sizeof(indices)/sizeof(int);
	 if (data.indxMem.size() < totalindx)
		 data.indxMem.resize(totalindx);


	 data.indxnum = totalindx;
	 memcpy(&data.indxMem[0], indices, sizeof(indices));


	 data.components.vsize = 3;
	 data.components.vtsize = 2;
	 data.components.vnsize = 0;

	 ModelMaterial group;
	 group.indexOffset = 0;
	 group.textureIndices[TEXUTE_TYPE::DIFFUSE] = loadTexture(textureName, data);

	 data.groups.push_back(group);

	 data.size++;

	 return 1;

}


int ModelLoader::createHeightField(unsigned char*& heightData, 
								   unsigned int twidth,
								   unsigned int theight,
								   unsigned short detail,
								   const char* textureFilename,
								   ModelData& data,
								   btScalar& maxHeight,
								   btScalar& minHeight)
{
	 unsigned int compnumber = 8;
	 unsigned int vwidth = twidth/detail;
	 unsigned int vheight = theight/detail;
	 unsigned int totalvnumber =  (vwidth*vheight);
	 unsigned int totalMem = totalvnumber*compnumber;
	 minHeight = std::numeric_limits<float>::max();
	 maxHeight = std::numeric_limits<float>::min();
	 if (data.vertexMem.size() < totalMem) {
		 data.vertexMem.resize(totalMem);
	 }

	 data.vertexnum = totalvnumber;
	//unsigned int step = d

	 float cx = 0.5 * twidth;
	 float cz = 0.5 * theight;

	for (int i = 0; i < twidth; i+= detail)
	{
		for (int j = 0; j < theight; j+= detail)
		{
			unsigned int indx = (i/detail)*vwidth + j/detail;
			data.vertexMem[indx*compnumber] = cx - i; // x
			data.vertexMem[indx*compnumber+2] = j - cx; // z
			float y = (float)heightData[i*(twidth) + j]; 
			y = 0.0f;
			data.vertexMem[indx*compnumber+1] = y; //y

			if (y < minHeight) {
				minHeight = y;
			}
			if (y > maxHeight) {
				maxHeight = y;
			}
			
			data.vertexMem[indx*compnumber+3] = (float)i/twidth; //s
			data.vertexMem[indx*compnumber+4] = (float)j/theight; //t
		}
	}


	int j = 0;
	int k = 0;

	if (data.indxMem.size() > 0) {
		data.indxMem.clear();
	}

	data.indxMem.push_back((vwidth-1)*vwidth);
	data.indxMem.push_back((vwidth-2)*vwidth);
	
	for (int i = vwidth-1, col = 0; i > 0; i--, col++)
	{
		for (k=1; k < vheight; k++, j++)			// indices para triangle strips de derecha a izquierda empezando en columna (twidth -1)
		{
				unsigned int i1 = ((i)*vwidth)+k*((col+1)%2) + (col%2)*(vheight-k-1); // cuando es par 
				unsigned int i2 = ((i-1)*vwidth)+k*((col+1)%2) + (col%2)*(vheight-k-1);
				
				data.indxMem.push_back(i1);
				data.indxMem.push_back(i2);		
		}
		data.indxMem.push_back(((i-2)*vwidth)+(k-1)*((col+1)%2) + ((col+2)%2)*(vheight-k));
		k = 1;
	}

	data.indxMem.erase(data.indxMem.end()-1);

	data.indxnum = data.indxMem.size();

	computeHeightFieldNormals(data, compnumber, vwidth, vheight);

	data.components.vsize = 3;
	data.components.vtsize = 2;
	data.components.vnsize = 3;

	data.primitives = GL_TRIANGLE_STRIP;

	ModelMaterial group;
	group.indexOffset = 0;
	group.textureIndices[TEXUTE_TYPE::DIFFUSE] = loadTexture(textureFilename, data);
	group.totalIndices = data.indxnum;
	data.groupNumber = 1;

	data.groups.push_back(group);
	data.size++;

	return data.size-1;
}


int ModelLoader::createHeightFieldTriangles(unsigned char*& heightData, 
								   unsigned int twidth,
								   unsigned int theight,
								   unsigned short detail,
								   const glm::vec3& scale,
								   const char* textureFilename,
								   ModelData& data,
								   btScalar& maxHeight,
								   btScalar& minHeight,
								   float* outHdata)
{
	 unsigned int compnumber = 8 + 3;
	 unsigned int vwidth = twidth/detail;
	 unsigned int vheight = theight/detail;
	 unsigned int totalvnumber =  (vwidth*vheight);
	 unsigned int totalMem = totalvnumber*compnumber;
	 minHeight = std::numeric_limits<float>::max();
	 maxHeight = std::numeric_limits<float>::min();
	 if (data.vertexMem.size() < totalMem) {
		 data.vertexMem.resize(totalMem);
	 }

	 data.vertexnum = totalvnumber;
	//unsigned int step = d

	 float cx = 0.5 * (vwidth -1);
	 float cz = 0.5 * (vheight -1);

	 for (int i = 0; i < vheight; i++)
	{
		for (int j = 0; j < vwidth; j++)
		{
			unsigned int indx = i*vwidth + j;
			
			// Vertice 1
			data.vertexMem[indx*compnumber] = (j - cx) * scale.x; // x
			data.vertexMem[indx*compnumber+2] = (i - cz) * scale.z; // z
			float y = (heightData[indx*detail]/255.0f)*scale.y; 
			//y = 0.0f;
			outHdata[indx] = y;
			data.vertexMem[indx*compnumber+1] = y; //y

			if (y < minHeight) {
				minHeight = y;
			}
			if (y > maxHeight) {
				maxHeight = y;
			}

			data.vertexMem[indx*compnumber+3] = 40.f*(float)i/vwidth; //s
			data.vertexMem[indx*compnumber+4] = 40.f*(float)j/vheight; //t
		}
	}




	int j = 0;
	int k = 0;

	int totalIndices = (vwidth -1) * (vheight -1) * 2 * 3; // numero de quads * 2 triangulos cada quad * 3 indices para cada triangulo

	if (data.indxMem.size() > 0) {
		data.indxMem.clear();
	}

	for (int i = 0; i < vheight - 1; i++ ) {
		for (int j = 0; j < vwidth - 1; j++) {

			// Triangulo 1
			data.indxMem.push_back( i*vwidth + j);
			data.indxMem.push_back( (i+1)*vwidth + j);
			data.indxMem.push_back( i*vwidth + j+1);

			// Triangulo 2
			data.indxMem.push_back( i*vwidth + j+1);
			data.indxMem.push_back( (i+1)*vwidth + j);
			data.indxMem.push_back( (i+1)*vwidth + j+1);
		}
	}
	
	assert(data.indxMem.size() == totalIndices);
	data.indxnum = data.indxMem.size();

	computeHeightFieldTriangleNormals(data, compnumber, vwidth, vheight);

	data.components.vsize = 3;
	data.components.vtsize = 2;
	data.components.vnsize = 3;

	data.primitives = GL_TRIANGLES;

	ModelMaterial group;
	group.indexOffset = 0;
	group.textureIndices[TEXUTE_TYPE::DIFFUSE] = loadTexture(textureFilename, data);
	group.totalIndices = data.indxnum;
	data.groupNumber = 1;

	data.groups.push_back(group);
	data.size++;

	return data.size-1;
}


void ModelLoader::computeHeightFieldTriangleNormals(ModelData& data, int compnumber, int vwidth, int vheight) { 
	//unsigned int size = twidth*theight;
	srand(static_cast <unsigned> (0));

	for (int i = 0; i < vwidth*vheight; i++)
	{	

		int m = i/vwidth;
		int j = i%vwidth;

		glm::vec3 normal(0);


		glm::vec3 v1(data.vertexMem[i*compnumber], data.vertexMem[i*compnumber+1], data.vertexMem[i*compnumber+2]);
		glm::vec3 v2;
		glm::vec3 v3;

		int index;
		if (m - 1 >= 0 ) {

			index = (m-1)*vwidth + j;

			v2.x = data.vertexMem[index*compnumber]; v2.y = data.vertexMem[index*compnumber+1]; v2.z = data.vertexMem[index*compnumber+2];

			if (j + 1 < vwidth) {
				index = m*vwidth + j+1;
				v3.x = data.vertexMem[index*compnumber]; v3.y = data.vertexMem[index*compnumber+1]; v3.z = data.vertexMem[index*compnumber+2];
				normal += glm::cross(glm::normalize(v3 - v1), glm::normalize(v2 - v1));
			}

			if (j - 1 >= 0) {
				index = m*vwidth + j-1;
				v3.x = data.vertexMem[index*compnumber]; v3.y = data.vertexMem[index*compnumber+1]; v3.z = data.vertexMem[index*compnumber+2];
				normal += glm::cross(glm::normalize(v2 - v1), glm::normalize(v3 - v1));
			}

		}

		if (m + 1 < vheight ) {

			index = (m+1)*vwidth + j;

			v2.x = data.vertexMem[index*compnumber]; v2.y = data.vertexMem[index*compnumber+1]; v2.z = data.vertexMem[index*compnumber+2];

			if (j + 1 < vwidth) {
				index = m*vwidth + j+1;
				v3.x = data.vertexMem[index*compnumber]; v3.y = data.vertexMem[index*compnumber+1]; v3.z = data.vertexMem[index*compnumber+2];
				normal += glm::cross(glm::normalize(v2 - v1), glm::normalize(v3 - v1));
			}

			if (j - 1 >= 0) {
				index = m*vwidth + j-1;
				v3.x = data.vertexMem[index*compnumber]; v3.y = data.vertexMem[index*compnumber+1]; v3.z = data.vertexMem[index*compnumber+2];
				normal += glm::cross(glm::normalize(v3 - v1), glm::normalize(v2 - v1));
			}

		}
		
		normal = glm::normalize(normal);
		normal = (normal.y < 0)? glm::vec3(0) - normal: normal; 
		data.vertexMem[i*compnumber + 5] = normal.x;
		data.vertexMem[i*compnumber + 6] = normal.y;
		data.vertexMem[i*compnumber + 7] = normal.z;

		int iii = i;
		//data.vertexMem[i*compnumber + 5] = (iii % 3 == 0) ? 1.0f : 0.0f;
		//data.vertexMem[i*compnumber + 6] = (iii % 3 == 1) ? 1.0f : 0.0f;
		//data.vertexMem[i*compnumber + 7] = (iii % 3 == 2) ? 1.0f : 0.0f;
		//float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//iii += (r < 0.7f) ? 0 : (r < 0.85f) ? 1 : 2;
		data.vertexMem[i*compnumber + 8] = (iii % 3 == 0) ? 1.0f : 0.0f;
		data.vertexMem[i*compnumber + 9] = (iii % 3 == 1) ? 1.0f : 0.0f;
		data.vertexMem[i*compnumber + 10] = (iii % 3 == 2) ? 1.0f : 0.0f;

	}
}


void ModelLoader::computeHeightFieldNormals(ModelData& data, int compnumber, int vwidth, int vheight) { 
	//unsigned int size = twidth*theight;
	unsigned int col = 0;
	int offset = 0;
	int prev =  data.indxMem[2];
	int next =0, target = 0;
	for (int i = 0; i < data.indxnum; i++)
	{
		if (i+1 == data.indxnum)		/// solo para el ultimo caso
			next = target;
		else
			next = data.indxMem[i+1];

		target = data.indxMem[i];
		

		glm::vec3 v1(data.vertexMem[target*compnumber], data.vertexMem[target*compnumber+1], data.vertexMem[target*compnumber+2]);
		glm::vec3 v2(data.vertexMem[prev*compnumber], data.vertexMem[prev*compnumber+1], data.vertexMem[prev*compnumber+2]);
		glm::vec3 v3(data.vertexMem[next*compnumber], data.vertexMem[next*compnumber+1], data.vertexMem[next*compnumber+2]);

		glm::vec3 dir1(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
		dir1 = glm::normalize(dir1);
		glm::vec3 dir2(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
		dir2 = glm::normalize(dir2);

		glm::vec3 normal = glm::cross(dir1, dir2);

		normal = (normal.y < 0)? glm::vec3(0) - normal: normal; 

		data.vertexMem[target*compnumber+5] = normal.x;
		data.vertexMem[target*compnumber+6] = normal.y;
		data.vertexMem[target*compnumber+7] = normal.z;

		col++;
		
		int cond = col/(2*vheight-1)%2;
		int cond2 = ((i+1)/(2*vheight-1))%2;
		col = (col == 2*vheight-1)? 0: col;

		prev = target;
		if (cond && cond2)
			prev -= (vheight+1);
		else if (cond && !cond2)
			prev -= (vheight-1);
	}
}


//template <typename vetexType, typename textType, typename indxType>
 int ModelLoader::loadModel( const char* fileName,
		 	 	 	 	 	 ModelData& data,
		 	 	 	 	 	 bool ignoreGroups) {
	LOG("MODELLOADER: Loading model \"%s\" ...", fileName);
		
		off_t start = 0, length = 0;
		FILE* file;
		fopen_s(&file, fileName, "rb");
		if (!file) {
			LOG(" No such file \"%s\"\n",fileName);
			return 0;
		}

		fseek(file, start, SEEK_SET);
		unsigned char magicnumber[2];

		fread(magicnumber, sizeof(char), 2*sizeof(char), file);
		if (magicnumber[0] != 0xB0 || magicnumber[1] != 0xFF) {
				LOG(" Error: Unexpected File Format in file \"%s\"",fileName);
				fclose(file);
				return -1;
		}
		else {
			LOG("Binary Object File Format!");
		}
		LOG("magicnumber 0x%X 0x%X", magicnumber[0], magicnumber[1]);

		char version[4];
		fread(version,  sizeof(char), sizeof(char)*4, file);

		LOG(" Reading file version: %d.%d.%d.%d Supported reading for version: 1.1 ", (int)version[0], (int)version[1] ,(int)version[2], (int)version[3]);
		unsigned int headeroffset = sizeof(char)*6;

		unsigned int objectnumber = 0;
		fread((char*)&objectnumber, sizeof(unsigned int), 1, file);
		unsigned int groupnumber = 0;
		fread((char*)&groupnumber, sizeof(unsigned int), 1, file);
		unsigned int vnumber = 0;
		fread((char*)&vnumber, sizeof(unsigned int), 1, file);
		unsigned int vnnumber = 0;
		fread((char*)&vnnumber, sizeof(unsigned int), 1, file);
		unsigned int vtnumber = 0;
		fread((char*)&vtnumber, sizeof(unsigned int), 1, file);
		char w = 0;
		fread(&w, sizeof(char), 1, file);
		unsigned int totalvnumber = 0;
		fread((char*)&totalvnumber, sizeof(unsigned int), 1, file);


		LOG("vertnum: %d\n", totalvnumber);
		data.vertexnum = totalvnumber;

		unsigned int optGroupNum = (ignoreGroups)? 1: groupnumber;

		/*if (optGroupNum > data.groups.size()) {
			LOG("indx: %d\n", optGroupNum);
			data.groups.resize(optGroupNum);
			LOG("indx: %d\n", optGroupNum);
		}*/
		data.groupNumber = optGroupNum;
		//vertexnumber = totalvnumber;

		int size = 3;

		if (vnnumber != 0)
		{
			size += 3;
			data.components.vnsize = 3;
		}
		if(vtnumber != 0)
		{
			if (w != 0)
			{
				size += 3;
				data.components.vtsize = 3;
			}
			else
			{
				size += 2;
				data.components.vtsize = 2;
			}
		}
		 data.components.vsize = 3;

		LOG("components vs %d vt %d vn %d total %d ",data.components.vsize, data.components.vtsize, data.components.vnsize, size);
		unsigned int totalMem = size*totalvnumber;
		if (data.vertexMem.size() < totalMem) {
			data.vertexMem.resize(totalMem);
		}

		size_t offset = sizeof(unsigned int)*6 + sizeof(char) + headeroffset;
		//file.seekg(offset);

		fread((char*)&data.vertexMem[0], size*sizeof(float), totalvnumber, file);

		//std::shared_ptr<unsigned int> trinum = std::shared_ptr<unsigned int>(new unsigned int[groupnumber]);
		unsigned int* trinum = new unsigned int[groupnumber];


		//file.seekg(offset + size*(totalvnumber*sizeof(float)));
		fread((char*)trinum, sizeof(unsigned int), groupnumber, file);


		unsigned int totaltri = 0;

		int initialGroupIndex = data.groups.size();
		for (unsigned int i = 0; i < groupnumber; i++)
		{
			 //cout << " Read index" << trinum[i] << endl;
			if (!ignoreGroups || i == 0) {
				ModelMaterial group;
				group.indexOffset = totaltri*3;
				group.totalIndices = trinum[i]*3;
				data.groups.push_back(group);
				//data.groups[indx+i].indexOffset = totaltri*3;
				LOG("ModelLoader: Group %d triangles %d ", i, trinum[i]);
			}
			totaltri += trinum[i];
		}

		if (ignoreGroups) {
			data.groups[initialGroupIndex].totalIndices = totaltri*3;
		}

		LOG(" total %d ", totaltri);
		  //cout << " Vertex number " << vertexnumber << endl;

		data.indxnum = totaltri*3;
		if (data.indxMem.size() < data.indxnum)
			data.indxMem.resize(data.indxnum);


		data.indxnum = totaltri*3;

		long long p = totaltri*3*sizeof(unsigned int);

		fread((char*)&data.indxMem[0], 3*sizeof(unsigned int), totaltri, file);

		LOG("total %d", totaltri);


		LOG(" ob %d v %d n %d t %d has w %d total %d size %d", objectnumber, vnumber, vnnumber, vtnumber, (int)w, totalvnumber, size);

		char dtexturename[256];


		size_t blocks = 0;
		fread((char*)&blocks, sizeof(size_t), 1, file);
		size_t curblock = 0;
		unsigned long pstart = ftell(file);
		LOG("sizeof size_t %d", sizeof(size_t));

		if (groupnumber == 1)
		{
			LOG("groupno1 : %d ", groupnumber);
			size_t matoff = 12*sizeof(float) + sizeof(short);
			fseek(file, matoff, SEEK_CUR);
			char namesize = 0;
			char mapType[2];

			fread(mapType, 2, 1, file);
			fread(&namesize, 1, 1, file);
			fread((char*)dtexturename, namesize, 1, file);
			dtexturename[(int)namesize] = 0;

			//data.groups[indx].Ka

			data.groups[initialGroupIndex].textureIndices[TEXUTE_TYPE::DIFFUSE] = loadTexture(dtexturename, data);

		}
		else
		{
			LOG("groupno : %d ", groupnumber);
			char namesize = 0;
			unsigned long toff = 0;
			unsigned long cdistance = (long)ftell(file) - pstart;
			unsigned int materialIndx = initialGroupIndex;


			if (ignoreGroups) {
				 char namesize = 0;
						unsigned long toff = 0;
						unsigned long cdistance = (long)ftell(file) - pstart;

						int loaded = 0;

						int count = 0;

						
						while (loaded < TOTAL_TEXTURE_TYPES && (cdistance < blocks+groupnumber*sizeof(size_t)))
						{
							count++;
							fread((char*)&curblock, sizeof(size_t), 1, file);

							unsigned long begp = ftell(file);
							size_t matoff = 12*sizeof(float) + sizeof(short);
							fseek(file, matoff, SEEK_CUR);

							int readMaps = 0;
							char mapType[2];
							fread(mapType, 2, 1, file); //  Kd difuse
							fread(&namesize, 1, 1, file);
							if (namesize != 0 && mapType[0] == 'k' && mapType[1] == 'd') // Tipo ka, o kd
							{
								fread((char*)dtexturename, (size_t)namesize, 1, file);
								dtexturename[(int)namesize] = 0;
								data.groups[materialIndx].textureIndices[TEXUTE_TYPE::DIFFUSE] = loadTexture(dtexturename, data);
								loaded++;
							}

							
							fread(mapType, 2, 1, file); // Ka
							fread(&namesize, 1, 1, file);
							if (namesize != 0) {
								fread((char*)dtexturename, (size_t)namesize, 1, file);
								dtexturename[(int)namesize] = 0;
								if (data.groups[materialIndx].textureIndices[TEXUTE_TYPE::DIFFUSE] == -1) {
									data.groups[materialIndx].textureIndices[TEXUTE_TYPE::DIFFUSE] = loadTexture(dtexturename, data);
									loaded++;
								}
							}
							
							fread(mapType, 2, 1, file);
							fread(&namesize, 1, 1, file);
							if (namesize != 0 && mapType[0] == 'm' && mapType[1] == 'b') { // Tipo bump map como Normal Map
								fread((char*)dtexturename, (size_t)namesize, 1, file);
								dtexturename[(int)namesize] = 0;
								data.groups[materialIndx].textureIndices[TEXUTE_TYPE::NORMAL] = loadTexture(dtexturename, data);
								loaded++;
							}

							toff = ftell(file);
							//Para saltar al siguiente material
							fseek(file, begp, SEEK_SET);
							fseek(file, curblock, SEEK_CUR);
							cdistance = (long)ftell(file) - pstart;
						}
			}
			else {
				while ((cdistance < blocks+groupnumber*sizeof(size_t)))
				{
					fread((char*)&curblock, sizeof(size_t), 1, file);
					unsigned long begp = ftell(file);
					size_t matoff = 12*sizeof(float) + sizeof(short);
					fseek(file, matoff, SEEK_CUR);

					char mapType[2];
					fread(mapType, 2, 1, file);
					fread(&namesize, 1, 1, file);
					LOG("matindx %d namesize %d", materialIndx, namesize);
					if (namesize != 0 && mapType[0] == 'k' && (mapType[1] == 'a' || mapType[1] == 'd'))
					{
						fread((char*)dtexturename, (size_t)namesize, 1, file);
						dtexturename[(int)namesize] = 0;
						std::string name(dtexturename);

						data.groups[materialIndx].textureIndices[TEXUTE_TYPE::DIFFUSE] = loadTexture(dtexturename, data);
					}


					toff = ftell(file);
					fseek(file, begp, SEEK_SET);
					fseek(file, curblock, SEEK_CUR);
					cdistance = (long)ftell(file) - pstart;
					materialIndx++;
				}
			}

		}


		LOG(" Texture file: \"%s\"", dtexturename );
		std::string troute("Textures\\");
		troute += dtexturename;

		fclose(file);

		LOG("group vector size %d ", data.groups.size());

		LOG(" Model Successfully loaded! " );

		data.size++;
		LOG("Total models: %d", data.size);


		delete[] trinum;
		return data.groups.size()-1;
}

 int ModelLoader::loadTexture(const char* fileName,  ModelData& data) {

	 if (!fileName) { 
		 return 0;
	 }
	std::string name(fileName);
	if (!loadedTextures.count(name)){

		LOG("MODELLOADER: Loading texture \'%s\'...", fileName);
		FREE_IMAGE_FORMAT f = FreeImage_GetFileType(fileName);
		FIBITMAP* bm = FreeImage_Load(f, fileName);
		if (bm != NULL) {
			//Para asegurar que este en RGB de 8 bits cada color
		
			//bm = FreeImage_ConvertTo24Bits(bm);
			unsigned int width = FreeImage_GetWidth(bm);
			unsigned int height = FreeImage_GetHeight(bm);
			int channels = FreeImage_GetBPP(bm);
			GLubyte * pixels = (GLubyte*)FreeImage_GetBits(bm);	
			LOG("pixels %d %d %d %d", (int)pixels[0], (int)pixels[1], (int)pixels[2], (int)pixels[3]);
			LOG("TextureInfo width %d height %d\n", width, height);

			LOG("textcount %d pixels %d\n", data.textCount, pixels );

			Texture text = Texture(width, height, channels, pixels, bm, data.textures.size());
			data.textures.emplace_back(width, height, channels, pixels, bm);
			data.textures[text.index].index = text.index;
			data.textCount++;

			loadedTextures[name] = text;
			return text.index;
		}
		else {
			LOG("Unable to load texture!\n");
		}
	}
	else {
		return loadedTextures[name].index;
	}
 }



	
ModelLoader::~ModelLoader(void)
{


}


