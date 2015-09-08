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

#include "PlaneCreator.h"
#include "GameComponents/NewTextureComponent.h"
#include "GameComponents/NewRenderComponent.h"
#include "GameComponents/MessageHandlerCmp.h"

std::vector<std::unique_ptr<float[]>> PlaneCreator::heightMemory;

PlaneCreator::PlaneCreator(void) { }

PlaneCreator::~PlaneCreator(void) { }

void createGL(ModelData& modelData) {

	/*glGenVertexArrays(1, &this->vertexArray);

	glBindVertexArray(this->vertexArray);
		glGenBuffers(1, &this->vertexBuffer);
		glGenBuffers(1, &this->indexBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);

		unsigned short totalComp = params.component.vnsize+params.component.vsize+params.component.vtsize;

		glBufferData(GL_ARRAY_BUFFER, params.numVertex*sizeof(float)*(totalComp), params.vertexData, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, params.component.vsize, GL_FLOAT, GL_FALSE, sizeof(float)*totalComp, 0);  ///Atributo de Posicion

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, params.component.vtsize, GL_FLOAT, GL_FALSE, sizeof(float)*totalComp, (GLvoid*)(sizeof(float)*params.component.vsize)); //Atributo de Coordenadas de textura

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, params.component.vnsize, GL_FLOAT, GL_FALSE, sizeof(float)*totalComp, (GLvoid*)(sizeof(float)*(params.component.vsize+params.component.vtsize))); //Atributo de Coordenadas de textura
		
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, params.numIndex*sizeof(unsigned int), params.indexData, GL_STATIC_DRAW);

	glBindVertexArray(0);*/
}

GLuint PlaneCreator::create2DGLTexture(const Texture& texture) {

	GLuint textureId = 0;

	if (texture.bm != NULL) {

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
			// Cambiar el B por el R, no se si por el formato o la libreria
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_BGR, GL_UNSIGNED_BYTE, texture.data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Bilinear Filtering prueba en android trilinear en pc
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	return textureId;
}

void PlaneCreator::createModelBtPhysics(Device* device,
	btDynamicsWorld* dynamicsWorld,
	const ModelData& modelData, 
	int modelIndex, 
	const c_handle<ShaderComponent>& shader,
	std::shared_ptr<btTriangleIndexVertexArray>& outBackingMesh) {

	int planeNumber = modelData.vertexnum/4;
	Component components = modelData.components;
	int totalComponents = components.vnsize+components.vsize+components.vtsize;
	int groupIndex = 0;

	std::vector<GLuint> createdTextures;

	for (auto& it : modelData.textures) {
		createdTextures.push_back(create2DGLTexture(it));
	}

	int triangleNumber = modelData.groups[modelIndex].totalIndices / 3;

	/*for (int i = 0; i < modelData.groupNumber; i++) {
		triangleNumber =  modelData.groups[i].totalIndices / 3;
	}*/
	c_handle<GameEntity> entity = GameObjectFactory::createGameEntity();

	auto transfComp = entity->addNewComponent<TransformComponent>(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::quat(glm::vec3(0, 0, 0)));
	entity->addNewComponent<NewRenderComponent>(device, modelData, modelIndex);
	entity->addNewComponent<NewTextureComponent>(modelData, modelData.groups[modelIndex]);
	if (!shader->getParentEntity()) {
		entity->addComponent(shader);
	}
	else {
		entity->addNewCopyOfComponent(shader);
	}
	outBackingMesh = std::shared_ptr<btTriangleIndexVertexArray>(new btTriangleIndexVertexArray());
	btIndexedMesh part;

	part.m_vertexBase = (const unsigned char*)&modelData.vertexMem[0];
	part.m_vertexStride = sizeof(GLfloat) * totalComponents;
	part.m_numVertices = modelData.vertexnum;
	part.m_triangleIndexBase = (const unsigned char*)&modelData.indxMem[0];
	part.m_triangleIndexStride = sizeof(GLuint) * 3;
	part.m_numTriangles = triangleNumber;
	part.m_indexType = PHY_INTEGER;

	outBackingMesh->addIndexedMesh(part, PHY_INTEGER);

	bool	useQuantizedAabbCompression = true;

	std::shared_ptr<btCollisionShape> collisionShape = std::shared_ptr<btCollisionShape>(
		new btBvhTriangleMeshShape(outBackingMesh.get(), useQuantizedAabbCompression, true));

	btScalar mass = 0;

	entity->addNewComponent<RigidBodyComponent>(dynamicsWorld, mass, collisionShape, transfComp);//btVector3(-200.0f,-160.0f, -100.0f)));
}

void PlaneCreator::createPlanes(const ModelData& modelData, const c_handle<ShaderComponent>& shader) {
	/*int planeNumber = modelData.vertexnum/4.0f;
	Component components = modelData.components;
	int totalComponents = components.vnsize+components.vsize+components.vtsize;
	int groupIndex = 0;

	std::vector<GLuint> createdTextures;

	for (auto& it : modelData.textures) {
		createdTextures.push_back(create2DGLTexture(it));
	}

	c_handle<RenderComponent> renderComp = GameObjectFactory::createComponent<RenderComponent>(modelData);
	bool copy = false;
	for(int i = 0,j = 0;i<modelData.indxnum,j<planeNumber;i+=4,j++)
	{
		glm::vec3 v1 = getVertex(components.vsize,i,totalComponents, &modelData.vertexMem[0]);
		glm::vec3 v2 = getVertex(components.vsize,i+1,totalComponents,&modelData.vertexMem[0]);
		glm::vec3 v3 = getVertex(components.vsize,i+2,totalComponents,&modelData.vertexMem[0]);
		glm::vec3 v4 = getVertex(components.vsize,i+3,totalComponents,&modelData.vertexMem[0]);
		glm::vec3 normal = getNormal(components,i,totalComponents, &modelData.vertexMem[0]);
		c_handle<GameEntity> entity = GameObjectFactory::createGameEntity();
		entity->addNewComponent<BoundingPlaneComponent>(v1, v2, v3, v4, normal);
		entity->addNewComponent<TransformComponent>();
		entity->addNewComponent<MiniMapComponent>(glm::vec3(0.9f,0.34f,0.34f));
		//entity->addNewCopyOfComponent(renderComp);
		entity->addNewCopyOfComponent(shader);
		c_handle<TextureComponent> texture = GameObjectFactory::createComponent<TextureComponent>();
		c_handle<RenderComponent> renderCpy = renderComp;
		if (copy)  {
			renderCpy = GameObjectFactory::createCopy(renderComp);
			renderCpy->drawOffset = modelData.groups[groupIndex].indexOffset;
			renderCpy->numIndex = modelData.groups[groupIndex].totalIndices;
		}
		else {
			copy = true;
		}
		RenderComponent* rr = renderCpy.operator->();
		for (int k = 0; k < TOTAL_TEXTURE_TYPES; k++) {
			int tindex = modelData.groups[groupIndex].textureIndices[k];
			TextureComponent* tt = texture.operator->();
			texture->textureId[k] = (tindex >= 0)? createdTextures[tindex]: 0;
			//int a;
			//a = 0;
		}
		//entity->addComponent(texture);
		//entity->addComponent(renderCpy);
		groupIndex++;
	}*/
}

c_handle<GameEntity> PlaneCreator::createHeightField(const char* heightFieldFileName,
									const char* textureFileName,
									const unsigned int width,
									const unsigned int lenght,
									const unsigned short detail,
									const c_handle<ShaderComponent>& shader,
									btDynamicsWorld* dynamicsWorld, Device* device, 
									SystemMessageSender* msgSender) {
	std::string fname("Terrain\\");
	std::string texfname("Terrain\\");

	fname +=  heightFieldFileName;
	texfname += textureFileName;
	int vwidth = width/detail;
	int vheight = lenght/detail;
	std::ifstream file(fname, std::ios::ate | std::ios::binary);
	if (file)
	{
		std::ios::pos_type size = file.tellg();
		file.seekg(0);

		unsigned char* heightData = new unsigned char[width*lenght]; // Memory Leak
		//indexData = new unsigned char[];
		file.read((char*)heightData, width*lenght);
	
		ModelData modelData;
		btScalar maxHeight, minHeight;

		//unsigned char hData[] = {1, 0, 1, 1, 0, 1, 0, 0, 0};

		glm::vec3 scale(3.0f, 50.f, 3.0f);
		//memcpy(heightData, hData, sizeof(hData));



		int index = heightMemory.size();
		heightMemory.emplace_back(new float[vwidth*vheight]);
		float* hData = heightMemory[index].get();
		ModelLoader::createHeightFieldTriangles(heightData, width, lenght, detail, scale, texfname.c_str(), modelData, maxHeight, minHeight, hData);

		delete[] heightData;
		//ModelLoader::createHeightField(heightData, width, lenght, detail, texfname.c_str(), modelData, maxHeight, minHeight);

		//memset(heightData, 0, width*lenght);

		//memset(hData, 0, sizeof(float)*vwidth*vheight);

		float translateY = -scale.y/2.f;

		c_handle<GameEntity> entity = GameObjectFactory::createGameEntity();
		//entity->addNewComponent<NewRenderComponent>(device, modelData);
		
		size_t totalComp = 11; // 3pos + 2uv + 3normals + 3barycentric
		std::vector<Attribute> attrs;
		// pos
		attrs.emplace_back(0, modelData.components.vsize, Attribute::float_VA,
			sizeof(float) * totalComp, (void*)0);
		// uv
		attrs.emplace_back(1, modelData.components.vtsize, Attribute::float_VA, 
			sizeof(float)* totalComp, (void*)(sizeof(float) * 3));
		// normals
		attrs.emplace_back(2, modelData.components.vnsize, Attribute::float_VA,
			sizeof(float) * totalComp, (void*)(sizeof(float) * (3 + 2)));
		// barycentric
		attrs.emplace_back(3, 3, Attribute::float_VA, sizeof(float) * totalComp, 
			(void*)(sizeof(float) * (3 + 2 + 3)));

		// convert indices to uint16_t
		uint16_t* indices = new uint16_t[modelData.indxnum];
		for (size_t i = 0; i < modelData.indxnum; i++) {
			int value = modelData.indxMem[i];
			assert(value <= 65536);
			indices[i] = (uint16_t)(value < 0 ? 0 : value);
		}

		auto render = GameObjectFactory::createComponent<NewRenderComponent>(device,
			reinterpret_cast<uint8_t*>(modelData.vertexMem.data()), modelData.vertexnum, indices, 
			modelData.indxnum, attrs);
		render->addProperty<float>("beatIntensity", UniformsFuncs::set1f, 0.35f);

		entity->addComponent<NewRenderComponent>(render);
		delete[] indices;
		entity->addNewComponent<MessageHandlerCmp<MessageType>>();
		entity->addNewComponent<NewTextureComponent>(modelData, modelData.groups[0]);

		glm::vec3 pos = glm::vec3(0, translateY, 0);
		auto planeTransform = entity->addNewComponent<TransformComponent>(pos, 
			glm::vec3(1, 1, 1), glm::quat(glm::vec3(0, 0, 0)));

		
		//entity->addNewComponent<NodeComponent>();
		entity->addNewCopyOfComponent(shader);
		
		std::shared_ptr<btCollisionShape> collisionShape = std::shared_ptr<btCollisionShape>(
			new btHeightfieldTerrainShape(vwidth, vheight, hData, 1.0f, 0, scale.y, 1, PHY_FLOAT, 
			false));
		collisionShape->setLocalScaling(btVector3(scale.x, 1.0f, scale.z));
		btScalar mass = 0;

		btTransform initialTranf(btQuaternion(0,0,0,1), btVector3(0.0f, 2.0f, 0.0f));
		//btVector3(-200.0f,-155.0f, -100.0f));//btVector3(508.7f, -1.0f, 0.0f));

		//initialTranf.getBasis().setEulerZYX(0,-M_PI/2.0,0);
		entity->addNewComponent<RigidBodyComponent>(dynamicsWorld, mass, collisionShape, initialTranf);//btVector3(-200.0f,-160.0f, -100.0f)));
		return entity;
	}
	return c_handle<GameEntity>();
}

/*Plane* PlaneCreator::createPlanes(VertexData vd)
{
	int planeNumber = vd.totalIndices/4;
	Plane* planes = (Plane*)malloc(planeNumber*sizeof(Plane));
	VertexComponents components = vd.components;
	int totalComponents = components.normalNumber+components.posNumber+components.uvNumber;
	for(int i = 0,j = 0;i<vd.totalIndices,j<planeNumber;i+=4,j++)
	{
		planes[j].v1 = getVertex(components.posNumber,i,totalComponents,vd.vertices);
		planes[j].v2 = getVertex(components.posNumber,i+1,totalComponents,vd.vertices);
		planes[j].v3 = getVertex(components.posNumber,i+2,totalComponents,vd.vertices);
		planes[j].v4 = getVertex(components.posNumber,i+3,totalComponents,vd.vertices);
		planes[j].normal = getNormal(components,i,totalComponents,vd.vertices);
	}
	return planes;
}*/

glm::vec3 PlaneCreator::getVertex(int posNumber, int vertIndex, int totalComponents, const GLfloat* vertices)
{
	int startIndex = vertIndex*totalComponents;
	switch(posNumber){
	case 2:
		return glm::vec3(vertices[startIndex],vertices[startIndex+1],0);
	case 3:
	default:
		return glm::vec3(vertices[startIndex],vertices[startIndex+1],vertices[startIndex+2]);
	}
}

glm::vec3 PlaneCreator::getNormal(Component components, int vertIndex, int totalComponents, const GLfloat* vertices)
{
	int startIndex = vertIndex*totalComponents+components.vsize+components.vtsize;
	switch(components.vsize){
	case 2:
		return glm::vec3(vertices[startIndex],vertices[startIndex+1],0);
	case 3:
	default:
		return glm::vec3(vertices[startIndex],vertices[startIndex+1],vertices[startIndex+2]);
	}
}
