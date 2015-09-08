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

#include "Uniform.h"
#include <GL/glew.h>

/* FLOAT UNIFORMS*/

const std::function<void(std::string, uint32_t, float)> UniformsFuncs::set1f =
[](std::string str, uint32_t shaderId, float value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform1f(location, value);
};

const std::function<void(std::string, uint32_t, float, float)> UniformsFuncs::set2f =
[](std::string str, uint32_t shaderId, float value0, float value1) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform2f(location, value0, value1);
};

const std::function<void(std::string, uint32_t, float, float, float)> UniformsFuncs::set3f =
[](std::string str, uint32_t shaderId, float value0, float value1, float value2) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform3f(location, value0, value1, value2);
};

const std::function<void(std::string, uint32_t, float, float, float, float)> UniformsFuncs::set4f =
[](std::string str, uint32_t shaderId, float value0, float value1, float value2, float value3) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform4f(location, value0, value1, value2, value3);
};
const std::function<void(std::string, uint32_t, int, const float*)> UniformsFuncs::set2fv =
[](std::string str, uint32_t shaderId, int count, const float* value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform2fv(location, count, value);
};

const std::function<void(std::string, uint32_t, int, const float*)> UniformsFuncs::set3fv =
[](std::string str, uint32_t shaderId, int count, const float* value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform3fv(location, count, value);
};

const std::function<void(std::string, uint32_t, int, const float*)> UniformsFuncs::set4fv =
[](std::string str, uint32_t shaderId, int count, const float* value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform4fv(location, count, value);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/*INT UniformsFuncs*/

const std::function<void(std::string, uint32_t, int32_t)> UniformsFuncs::set1i =
[](std::string str, uint32_t shaderId, int32_t value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform1i(location, value);
};

const std::function<void(std::string, uint32_t, int32_t, int32_t)> UniformsFuncs::set2i =
[](std::string str, uint32_t shaderId, int32_t value0, int32_t value1) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform2i(location, value0, value1);
};

const std::function<void(std::string, uint32_t, int32_t, int32_t, int32_t)> UniformsFuncs::set3i =
[](std::string str, uint32_t shaderId, int32_t value0, int32_t value1, int32_t value2) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform3i(location, value0, value1, value2);
};

const std::function<void(std::string, uint32_t, int32_t, int32_t, int32_t, int32_t)>
UniformsFuncs::set4i =
[](std::string str, uint32_t shaderId, int32_t value0, int32_t value1, int32_t value2, int32_t value3) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform4i(location, value0, value1, value2, value3);
};

const std::function<void(std::string, uint32_t, int, const int32_t*)> UniformsFuncs::set2iv =
[](std::string str, uint32_t shaderId, int count, const int32_t* value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform2iv(location, count, value);
};

const std::function<void(std::string, uint32_t, int, const int32_t*)> UniformsFuncs::set3iv =
[](std::string str, uint32_t shaderId, int count, const int32_t* value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform3iv(location, count, value);
};
const std::function<void(std::string, uint32_t, int, const int32_t*)> UniformsFuncs::set4iv =
[](std::string str, uint32_t shaderId, int count, const int32_t* value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform4iv(location, count, value);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/*UNSIGNED INT UniformsFuncs*/

const std::function<void(std::string, uint32_t, uint32_t)> UniformsFuncs::set1ui =
[](std::string str, uint32_t shaderId, uint32_t value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform1ui(location, value);
};

const std::function<void(std::string, uint32_t, uint32_t, uint32_t)> UniformsFuncs::set2ui =
[](std::string str, uint32_t shaderId, uint32_t value0, uint32_t value1) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform2ui(location, value0, value1);
};

const std::function<void(std::string, uint32_t, uint32_t, uint32_t, uint32_t)> UniformsFuncs::set3ui =
[](std::string str, uint32_t shaderId, uint32_t value0, uint32_t value1, uint32_t value2) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform3ui(location, value0, value1, value2);
};

const std::function<void(std::string, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)> 
UniformsFuncs::set4ui =
[](std::string str, uint32_t shaderId, uint32_t value0, uint32_t value1, uint32_t value2, uint32_t value3) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform4ui(location, value0, value1, value2, value3);
};

const std::function<void(std::string, uint32_t, int, const uint32_t*)> UniformsFuncs::set2uiv =
[](std::string str, uint32_t shaderId, int count, const uint32_t* value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform2uiv(location, count, value);
};

const std::function<void(std::string, uint32_t, int, const uint32_t*)> UniformsFuncs::set3uiv =
[](std::string str, uint32_t shaderId, int count, const uint32_t* value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform3uiv(location, count, value);
};

const std::function<void(std::string, uint32_t, int, const uint32_t*)> UniformsFuncs::set4uiv =
[](std::string str, uint32_t shaderId, int count, const uint32_t* value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniform4uiv(location, count, value);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/*MATRIX UniformsFuncs*/

const std::function<void(std::string, uint32_t, int, bool, const float*)> UniformsFuncs::setMat2fv =
[](std::string str, uint32_t shaderId, int count, bool transpose, const float* value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniformMatrix2fv(location, count, transpose, value);
};

const std::function<void(std::string, uint32_t, int, bool, const float*)> UniformsFuncs::setMat3fv =
[](std::string str, uint32_t shaderId, int count, bool transpose, const float* value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniformMatrix3fv(location, count, transpose, value);
};

const std::function<void(std::string, uint32_t, int, bool, const float*)> UniformsFuncs::setMat4fv =
[](std::string str, uint32_t shaderId, int count, bool transpose, const float* value) {
	GLuint location = glGetUniformLocation(shaderId, str.c_str());
	glUniformMatrix4fv(location, count, transpose, value);
};
