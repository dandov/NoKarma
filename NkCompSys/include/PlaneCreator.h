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

#ifndef PLANE_CREATOR_H
#define PLANE_CREATOR_H
#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include "ModelLoader.h"
#include "GameObjectFactory.h"
#include "GameComponents/TransformComponent.h"
#include "GameComponents/ShaderComponent.h"
#include "GameComponents/NodeComponent.h"
#include "GameComponents/RigidBodyComponent.h"
#include "SystemMessageSender.h"
#include "PlaneCollider.h"

#include "Device.h"

struct VertexComponents {
	short posNumber;
	short uvNumber;
	short normalNumber;
};

struct VertexData {
	float* vertices;
	unsigned int totalVertices;
	unsigned int* indices;
	unsigned int totalIndices;
	VertexComponents components;
};

/*struct Plane {
public:
	Plane(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4, const glm::vec3& normal):
	v1(v1), v2(v2), v3(v3), v4(v4), normal(normal){

	}
glm::vec3 v1;
glm::vec3 v2;
glm::vec3 v3;
glm::vec3 v4;
glm::vec3 normal;
};*/

class PlaneCreator
{
public:
	PlaneCreator(void);
	~PlaneCreator(void);
	Plane* createPlanes(VertexData vd);

	//Plane* createPlanes(const );

	void static createPlanes(const ModelData& modelData, const c_handle<ShaderComponent>& shader);
	void static createModelBtPhysics(Device* device,
		btDynamicsWorld* dynamicsWorld,
		const ModelData& modelData, 
		int modelIndex,
		const c_handle<ShaderComponent>& shader,
		std::shared_ptr<btTriangleIndexVertexArray>& outBackingMesh);
	c_handle<GameEntity> static createHeightField(const char* heightFieldFileName,
									const char* textureFileName,
									const unsigned int width,
									const unsigned int lenght,
									const unsigned short detail,
									const c_handle<ShaderComponent>& shader,
									btDynamicsWorld* dynamicsWorld, Device* device, 
									SystemMessageSender* msgSender);
	GLuint static create2DGLTexture(const Texture& texture);


private:
	glm::vec3 static getVertex(int posNumber, int vertIndex, int totalComponents, const GLfloat* vertices);
	glm::vec3 static getNormal(Component components, int vertIndex, int totalComponents, const GLfloat* vertices);

	static std::vector<std::unique_ptr<float[]>> heightMemory;
};

#endif
