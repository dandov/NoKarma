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
#include <GL/glew.h>
#include <vector>
#define GLM_SWIZZLE GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/component_wise.hpp>
#include <Windows.h>
#include <FreeImage.h>
#define _USE_MATH_DEFINES
#include <cmath>

#include "../utils.h"
#include "GameSystem.h"
#include "GameObjectFactory.h"
#include "GameComponents/TransformComponent.h"
#include "GameComponents/CameraComponent.h"
#include "SkyBox.h"
#include "ShadowMapFBO.h"
#include "FrameCaptureFBO.h"
#include "Canvas.h"

const int TOTAL_LIGHTS = 1;

class RenderSystem :
	public GameSystem
{
public:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	GLuint textureId;
	SkyBox skyBox;
	GLuint shaderProgram;
	//ShadowMapFBO shadowFBO;
	FrameCaptureFBO frameFBO;

	glm::vec3 lightPos[TOTAL_LIGHTS];

	RenderSystem();
	virtual ~RenderSystem(void);
	virtual void update(double time, SystemMessageSender* msgSender);
	virtual void init(void);
	virtual void send(const Message& msg);
	void setCanvas(Canvas canvas);

private:
	Canvas mCanvas;
};