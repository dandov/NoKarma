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

#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "GameComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "GameObjectFactory.h"

class CameraComponent
	: public GameComponent
{
public:
	CameraComponent(const glm::vec3& eye = glm::vec3(0.f, 0.f, 0.f),
					const glm::vec3& fw = glm::vec3(0.f, 0.f, -1.f),
					const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f));
	~CameraComponent(void){}
	virtual void recieve(const Message& msg) {};
	virtual glm::mat4 getViewMatrix(const c_handle<CameraComponent>& myHandle); 
	void setPerspectiveMatrix(float fieldOfView, float screenRatio, float zNear, float zFar);
	void setOrthoMatrix(float left, float right, float bottom, float top);
	glm::mat4 projectionMatrix;
	float width;
	float height;
	bool flag;

	void rotateX(float const & angle);
	void rotateY(float const & angle);

	void moveFw(const float& dist);
	void moveSide(const float& dist);

	void  rotateView(float angle, const glm::vec3& axis);

	glm::vec3 getView() { return fw; }

	glm::vec3 getEye() { return eye; }

	void setEye(const glm::vec3& nEye) {
		eye = nEye;
	}

	glm::vec3 getUp() { return up; }

private:
	glm::vec3 fw;
	glm::vec3 up;
	glm::vec3 eye;
};

#endif