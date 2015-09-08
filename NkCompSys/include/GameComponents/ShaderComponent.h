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

#ifndef SHADER_COMPONENT_H
#define SHADER_COMPONENT_H

#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/swizzle.hpp>
#include <Windows.h>
#include <FreeImage.h>

#include "../utils.h"
#include "../ModelLoader.h"
#include "GameSystem.h"
#include "GameObjectFactory.h"
#include "TransformComponent.h"
#include "NewRenderComponent.h"

class ShaderComponent :
	public GameComponent
{
public:
	GLuint shaderProgramId;
	GLuint vertexShaderId;
	GLuint fragmentShaderId;

	ShaderComponent(const char* vertexShaderFileName="", const char* fragmentShaderFileName="");
	~ShaderComponent(void){}
private:
	virtual void recieve(const Message& msg){}
};

#endif