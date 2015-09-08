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

#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#pragma once
#include "GameComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/component_wise.hpp>

class TransformComponent
	: public GameComponent
{
public:
	TransformComponent(
		const glm::vec3& position=glm::vec3(0,0,0), 
		const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f), 
		const glm::quat& orientation = glm::quat(1.0f, glm::vec3(0.0f, 0.f, 0.f))) 
		: GameComponent(), 
							position(position), 
							scale(scale), 
							orientation(orientation),
							needsUpdate(true) {
		this->worldModelMat = glm::translate(position) * glm::mat4_cast(orientation) * glm::scale(scale);
	}
	~TransformComponent(void){}
	virtual void recieve(const Message& msg)  {};
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat orientation;
	bool needsUpdate;
	glm::mat4 worldModelMat;
};

class OffsetComponent
	: public GameComponent
{
public:
	glm::vec3 offsetMov;
	glm::vec3 offsetRot;
	OffsetComponent(glm::vec3 offsetMov=glm::vec3(), glm::vec3 offsetRot=glm::vec3()):offsetMov(offsetMov),offsetRot(offsetRot){}
	~OffsetComponent(void){}
	virtual void recieve(const Message& msg)  {};
};

#endif