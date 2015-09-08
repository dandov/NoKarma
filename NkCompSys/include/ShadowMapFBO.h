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

#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <vector>
#include <random>
#include <Windows.h>
#include <FreeImage.h>
#include <glm/glm.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.h"
#include "GameComponents/NewRenderComponent.h"
#include "Canvas.h"

class ShadowMapFBO
{
public:
	ShadowMapFBO(int resWidth = 1024, int resHeight = 1024);
	void renderShadowPass(Canvas* canvas, const std::vector<c_handle<NewRenderComponent>>& components, const glm::vec3& lightPositon);
	~ShadowMapFBO(void);

	GLuint fbo;
	GLuint textureId;
	GLuint shaderProgram;

	int resWidth;
	int resHeight;

	glm::mat4 depthBiasMVP;

private:
	GLuint depthMatLoc;
	int ShadowMapFBO::createShader(const char* vShader, const char* fShader);
};

