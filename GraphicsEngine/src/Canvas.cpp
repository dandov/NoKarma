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

#include "Canvas.h"

Canvas::Canvas()
: mDevice(NULL) { }

Canvas::Canvas(Device* device) 
: mDevice(device) { }

void Canvas::draw(uint32_t shader, Device::Primitive primitive, uint32_t VAO, size_t numIndices, 
	void* offset, const std::vector<Texture2D> texs, const std::vector<UniformBind> uniforms,
	Device::IndexType type){

	this->mDevice->setShader(shader);
	this->mDevice->setUniforms(uniforms, shader);
	this->mDevice->setTextures(texs, shader);
	this->mDevice->bindVAO(VAO);

	this->mDevice->drawElements(primitive, type, numIndices, offset);
	
	// al parecer es una perdida de recursos hacer unbind de tus cosas
	// porque el que sigue va a bindear lo que necesita y hacer overwrite 

	//this->mDevice->bindVAO(0);
	//this->mDevice->disableTextures(texs);

	// al parecer no es bueno hacer esto porque hace que no dibuje bien y lanza error
	//this->mDevice->setShader(0);
}

void Canvas::draw(uint32_t shader, Device::Primitive primitive, uint32_t VAO, size_t numIndices,
	void* offset, Device::IndexType type){

	this->mDevice->setShader(shader);
	this->mDevice->bindVAO(VAO);

	this->mDevice->drawElements(primitive, type, numIndices, offset);

	//this->mDevice->bindVAO(0);
	// al parecer no es bueno hacer esto
	//this->mDevice->setShader(0);
}