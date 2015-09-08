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
#include "GameComponents/ShaderComponent.h"

ShaderComponent::ShaderComponent(const char* vertexShaderFileName, const char* fragmentShaderFileName):
	GameComponent()
{
	//Carga los shaders de un archivo y los compila, y crea un identificador
	// Regresa -1 si no encuentra el archivo o no se puede compilar el Shader
	vertexShaderId = shader_utils::createShaderFromFile(vertexShaderFileName, GL_VERTEX_SHADER);
	fragmentShaderId = shader_utils::createShaderFromFile(fragmentShaderFileName, GL_FRAGMENT_SHADER);

	if (vertexShaderId == -1 || fragmentShaderId == -1) {
		//return false;
		return;
	}

	//Crear programa que se compone de dos o más shaders, (basicos son vertex y fragment shader)
	//Variable global shaderProgramId
	shaderProgramId = glCreateProgram();

	glAttachShader(shaderProgramId, vertexShaderId);
	glAttachShader(shaderProgramId, fragmentShaderId);

	glLinkProgram(shaderProgramId);

	if(!shader_utils::checkLinkError(shaderProgramId)) {
		//return false;
		return;
	}
	//return true;
}
