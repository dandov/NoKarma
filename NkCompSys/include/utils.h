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


#ifndef SHADERUTILS_H
#define SHADERUTILS_H
#pragma once

#include <gl\glew.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <glm/glm.hpp> // Libreria de Matrices
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/component_wise.hpp>

#include <btBulletDynamicsCommon.h>


#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

namespace shader_utils
{
	GLuint createShaderFromFile(const char* filename, GLenum type);
	bool compileShader(GLuint ID, GLchar*& source);
	bool checkLinkError(const GLuint& prog);
};

namespace bullet_utils 
{
	glm::mat4 btTransform2GLM(const btTransform& transform);
	glm::quat btQuat2GLM(const btQuaternion& quat);
};

/*namespace shape_utils
{
	void createTorus(MeshData *data,
               double const & detailz, double const & detailx, 
			   double const & radius, double const & bradius);
};*/

void logGLError(const std::string &msg);

#endif