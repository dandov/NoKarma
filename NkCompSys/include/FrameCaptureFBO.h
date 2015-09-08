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

#include <iostream>
#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <vector>
#include <random>
#include <Windows.h>
#include <FreeImage.h>
#include <glm/glm.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "utils.h"

class FrameCaptureFBO
{
public:
	FrameCaptureFBO(int resWidth = 512, int resHeight = 384);
	~FrameCaptureFBO(void);

	void saveFrame(const char* fileName);

	GLuint fbo;
	GLuint textureId;
	GLuint renderBufferId;
	std::shared_ptr<BYTE> pixels;
	//GLuint shaderProgram;

	int resWidth;
	int resHeight;

	long frameCount;
};