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

#include "GameComponents/NewRenderComponent.h"

NewRenderComponent::NewRenderComponent()
: mData()
, mPrimitive(Device::Triangles)
, mDrawOffset(0)
, mProperties() { }

// number of vertices and indices refers to number of elements and not number of bytes
NewRenderComponent::NewRenderComponent(Device* device, const uint8_t* data, size_t numVertices,
	const uint16_t* indices, size_t numIndices, const std::vector<Attribute> attrs,
	Device::Primitive primitive, size_t offset)
	: mData()
	, mPrimitive(primitive)
	, mDrawOffset(offset)
	, mProperties() {
	device->createGPUData(&this->mData, data, numVertices, indices, numIndices, attrs);
}

NewRenderComponent::NewRenderComponent(Device::GPUData data, Device::Primitive primitive, 
	size_t offset)
: mData(data)
, mPrimitive(primitive)
, mDrawOffset(offset)
, mProperties() { }

NewRenderComponent::NewRenderComponent(Device* device, const ModelData& modelData, int groupIndex)
	: mData()
	, mPrimitive(Device::Triangles)
	, mDrawOffset(0)
	, mProperties() {
	unsigned short totalComp = modelData.components.vnsize + modelData.components.vsize +
		modelData.components.vtsize;

	std::vector<Attribute> attrs;
	attrs.emplace_back(0, modelData.components.vsize, Attribute::float_VA,
		sizeof(float)* totalComp, (void*)0);

	if (modelData.components.vtsize) {
		attrs.emplace_back(1, modelData.components.vtsize, Attribute::float_VA,
			sizeof(float)* totalComp, (void*)(sizeof(float)*modelData.components.vsize));
	}
	if (modelData.components.vnsize) {
		attrs.emplace_back(2, modelData.components.vnsize, Attribute::float_VA,
			sizeof(float)* totalComp,
			(void*)(sizeof(float)*(modelData.components.vsize + modelData.components.vtsize)));
	}

	// convert indices to uint16_t
	uint16_t* indices = new uint16_t[modelData.indxnum];
	for (size_t i = 0; i < modelData.indxnum; i++) {
		int value = modelData.indxMem[i];
		assert(value <= 65536);
		indices[i] = (uint16_t)(value < 0 ? 0 : value);
	}

	device->createGPUData(&this->mData, reinterpret_cast<const uint8_t*>(&modelData.vertexMem[0]), 
		modelData.vertexnum, indices, modelData.indxnum, attrs);

	delete[] indices;

	if (modelData.groups.size() > groupIndex) {
		this->mData.mIndexSize = modelData.groups[groupIndex].totalIndices;
		this->mDrawOffset = modelData.groups[groupIndex].indexOffset;
	}
	else {
		this->mData.mIndexSize = modelData.indxnum;
		this->mDrawOffset = 0;
	}

	this->mData.mVertexSize = modelData.vertexnum;
	switch (modelData.primitives) {
	case GL_TRIANGLES:
	default:
		this->mPrimitive = Device::Triangles;
		break;
	case GL_TRIANGLE_STRIP:
		this->mPrimitive = Device::Triangle_Strip;
		break;
	case GL_TRIANGLE_FAN:
		this->mPrimitive = Device::Triangle_Fan;
		break;
	}
}

void Render::loadModelComponents(Model* model, const char* fileName, Device* device) {
	bool ignoreGroups = true;

	off_t start = 0, length = 0;
	FILE* file;
	fopen_s(&file, fileName, "rb");
	fseek(file, start, SEEK_SET);
	unsigned char magicnumber[2];

	fread(magicnumber, sizeof(char), 2 * sizeof(char), file);
	if (magicnumber[0] != 0xB0 || magicnumber[1] != 0xFF) {
		printf(" Error: Unexpected File Format in file \"%s\"", fileName);
		fclose(file);
		return;
	}
	else {
		printf("Binary Object File Format!\n");
	}

	printf("magicnumber 0x%X 0x%X\n", magicnumber[0], magicnumber[1]);

	char version[4];
	fread(version, sizeof(char), sizeof(char)* 4, file);

	printf(" Reading file version: %d.%d.%d.%d Supported reading for version: 1.1\n",
		(int)version[0], (int)version[1], (int)version[2], (int)version[3]);
	unsigned int headeroffset = sizeof(char)* 6;

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

	GLsizei numVertex = totalvnumber;

	unsigned int optGroupNum = (ignoreGroups) ? 1 : groupnumber;

	//this->groups.resize(optGroupNum);

	//this->groupNumber = optGroupNum;

	int size = 3;

	if (vnnumber != 0)
	{
		size += 3;
		//this->compontents.vnsize = 3;
	}
	if (vtnumber != 0)
	{
		if (w != 0)
		{
			size += 3;
			//this->compontents.vtsize = 3;
		}
		else
		{
			size += 2;
			//this->compontents.vtsize = 2;
		}
	}
	//this->compontents.vsize = 3;

	numVertex = totalvnumber;
	//printf("components vs %d vt %d vn %d total %d \n",this->compontents.vsize, 
	//this->compontents.vtsize,this->compontents.vnsize, size);

	std::vector<GLuint> vertexMem = std::vector<GLuint>();
	vertexMem.resize(size*totalvnumber);

	size_t offset = sizeof(unsigned int)* 6 + sizeof(char)+headeroffset;
	fread((char*)&vertexMem[0], size*sizeof(float), totalvnumber, file);
	unsigned int* trinum = new unsigned int[groupnumber];

	//fseek(file, offset + size*(totalvnumber*sizeof(float)), SEEK_SET);
	fread((char*)trinum, sizeof(unsigned int), groupnumber, file);


	unsigned int totaltri = 0;

	for (unsigned int i = 0; i < groupnumber; i++)
	{
		//if (!ignoreGroups || i == 0) {
		//	//ModelMaterial group;
		//	//group.indexOffset = totaltri*3;
		//	//this->groups.push_back(group);
		//	printf("ModelLoader: Group %d triangles %d \n", i, trinum[i]);
		//}
		totaltri += trinum[i];
	}

	printf(" total %d ", totaltri);
	//cout << " Vertex number " << vertexnumber << endl;

	GLsizei numIndex = totaltri * 3;

	std::vector<GLfloat> indxMem = std::vector<GLfloat>();
	indxMem.resize(totaltri * 3);

	long long p = totaltri * 3 * sizeof(unsigned int);
	fread((char*)&indxMem[0], 3 * sizeof(unsigned int), totaltri, file);
	printf("total %d", totaltri);

	Render::Component comps = Render::Component();
	comps.vnsize = 3;
	comps.vsize = 3;
	comps.vtsize = 2;

	//RenderComponentParams params = RenderComponentParams((GLfloat*)&vertexMem[0], numVertex, 
	//	(GLuint*)&indxMem[0], numIndex, GL_TRIANGLES, comps);

	unsigned short totalComp = comps.vnsize + comps.vsize + comps.vtsize;
	std::vector<Attribute> attrs;
	attrs.emplace_back(0, comps.vsize, Attribute::float_VA, sizeof(float)* totalComp, (void*)0);
	attrs.emplace_back(1, comps.vtsize, Attribute::float_VA, sizeof(float)* totalComp,
		(void*)(sizeof(float)*comps.vsize));
	attrs.emplace_back(2, comps.vnsize, Attribute::float_VA, sizeof(float)* totalComp,
		(void*)(sizeof(float)*(comps.vsize + comps.vtsize)));

	// convert indices to uint16_t
	uint16_t* indices = new uint16_t[numIndex];
	for (size_t i = 0; i < numIndex; i++) {
		int value = indxMem[i];
		assert(value <= 65536);
		indices[i] = (uint16_t)(value < 0 ? 0 : value);
	}

	device->createGPUData(&model->mGpuData, reinterpret_cast<const uint8_t*>(vertexMem.data()), 
		numVertex, indices, numIndex, attrs);

	delete[] indices;

	//c_handle<RenderComponent> hRender = 
	//	GameObjectFactory::createComponent<RenderComponent>(params);

	//ge << hRender;

	printf(" ob %d v %d n %d t %d has w %d total %d size %d\n", objectnumber, vnumber,
		vnnumber, vtnumber, (int)w, totalvnumber, size);

	char dtexturename[256];

	size_t blocks = 0;
	fread((char*)&blocks, sizeof(size_t), 1, file);
	size_t curblock = 0;
	unsigned long pstart = ftell(file);
	printf("sizeof size_t %d\n", sizeof(size_t));

	if (groupnumber == 1) {
		printf("groupno1 : %d \n", groupnumber);
		size_t matoff = 12 * sizeof(float)+sizeof(short);
		fseek(file, matoff, SEEK_CUR);
		char namesize = 0;
		char mapType[2];
		fread(mapType, 2, 1, file);
		fread(&namesize, 1, 1, file);
		fread((char*)dtexturename, namesize, 1, file);
		dtexturename[(int)namesize] = 0;

		model->mTexs.emplace_back(dtexturename, "diffuseTexture", 0);
		//data.groups[indx].Ka

		//aqui me pide la textura
		//this->groups[0].textureIndex = loadTexture(dtexturename, false);
		//FREE_IMAGE_FORMAT f = FreeImage_GetFileType(dtexturename);
		//FIBITMAP* bm = FreeImage_Load(f, dtexturename);
		//c_handle<TextureComponent> hTex = 
		//	GameObjectFactory::createComponent<TextureComponent>(bm, 
		//	glm::vec4(1.6667, 1.6667, 1.6667, 1.6667), glm::vec4(1, 1, 1, 1), 1.0f/100.0f);
		//	
		delete[] trinum;
		return;

		//ge << hTex;
	}
	else {
		printf("groupno : %d \n", groupnumber);
		char namesize = 0;
		unsigned long toff = 0;
		unsigned long cdistance = (long)ftell(file) - pstart;
		unsigned int materialIndx = 0;


		if (ignoreGroups) {
			char namesize = 0;
			unsigned long toff = 0;
			unsigned long cdistance = (long)ftell(file) - pstart;
			int num = 0;

			while (namesize == 0 && (cdistance < blocks + groupnumber*sizeof(size_t)))
			{
				fread((char*)&curblock, sizeof(size_t), 1, file);
				unsigned long begp = ftell(file);
				size_t matoff = 12 * sizeof(float)+sizeof(short);
				fseek(file, matoff, SEEK_CUR);


				char mapType[2];
				fread(mapType, 2, 1, file);
				fread(&namesize, 1, 1, file);
				if (namesize != 0 && mapType[0] == 'k' && (mapType[1] == 'a' || mapType[1] == 'd')) // Tipo ka, o kd
				{
					fread((char*)dtexturename, (size_t)namesize, 1, file);
					dtexturename[(int)namesize] = 0;

					std::string str("diffuseTexture");
					if (num > 0) {
						str.append(std::to_string(num));
					}
					model->mTexs.emplace_back(dtexturename, str, num);

					FREE_IMAGE_FORMAT f = FreeImage_GetFileType(dtexturename);
					FIBITMAP* bm = FreeImage_Load(f, dtexturename);

					// TODO: checar en donde agregar esto
					//c_handle<TextureComponent> hTex = 
					//	GameObjectFactory::createComponent<TextureComponent>(bm, 
					//	glm::vec4(1.6667, 1.6667, 1.6667, 1.6667), 
					//	glm::vec4(1, 1, 1, 1), 1.0f/100.0f);

					delete[] trinum;
					return;
					//ge << hTex;
					//aqui me pide la textura???
					//this->groups[0].textureIndex = loadTexture(dtexturename, false);
				}
				toff = ftell(file);
				fseek(file, begp, SEEK_SET);
				fseek(file, curblock, SEEK_CUR);
				cdistance = (long)ftell(file) - pstart;
				num++;
			}
		}
		else {
			while ((cdistance < blocks + groupnumber*sizeof(size_t)))
			{
				fread((char*)&curblock, sizeof(size_t), 1, file);
				unsigned long begp = ftell(file);
				size_t matoff = 12 * sizeof(float)+sizeof(short);
				fseek(file, matoff, SEEK_CUR);

				char mapType[2];
				fread(mapType, 2, 1, file);
				fread(&namesize, 1, 1, file);
				printf("matindx %d namesize %d\n", materialIndx, namesize);
				if (namesize != 0)
				{
					fread((char*)dtexturename, (size_t)namesize, 1, file);
					dtexturename[(int)namesize] = 0;
					std::string name(dtexturename);
					//this->groups[materialIndx].textureIndex = loadTexture(dtexturename, false);
				}

				toff = ftell(file);
				fseek(file, begp, SEEK_SET);
				fseek(file, curblock, SEEK_CUR);
				cdistance = (long)ftell(file) - pstart;
				materialIndx++;
			}
		}

	}


	printf(" Texture file: \"%s\"\n", dtexturename);
	std::string troute("Textures\\");
	troute += dtexturename;

	fclose(file);


	printf(" Model Successfully loaded! ");

	delete[] trinum;
}

