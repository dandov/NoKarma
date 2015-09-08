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

#include "Device.h"

const uint32_t OpenGLDevice::sPrimitives[3] = {GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN};
const uint32_t OpenGLDevice::sIndexType[3] = { GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT };

bool OpenGLDevice::createGPUData(GPUData* data, const uint8_t* vertices, size_t numVertices,
	const uint16_t* indices, size_t numIndices, const std::vector<Attribute> attrs) {

	size_t vertexSize = 0;
	for (Attribute attr : attrs) {
		vertexSize += attr.mSize * Attribute::sTypeSize[attr.mType];
	}
	size_t sizeVertices = numVertices * vertexSize;
	size_t sizeIndices = numIndices * sizeof(uint16_t);

	if (data == NULL || sizeVertices <= 0 || sizeIndices <= 0 || attrs.size() == 0) {
		return false;
	}

	//podriamos necesitar un objeto de contexto para tener contextos en bloques y mantener lo viejo
	// Crear VAO
	glGenVertexArrays(1, &data->mVAO);
	glBindVertexArray(data->mVAO);

	// Crear VBO y NULL en datos porque no se van a copiar datos.
	glGenBuffers(1, &data->mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, data->mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeVertices, (GLvoid*)vertices, GL_STATIC_DRAW);
	data->mVertexSize = sizeVertices;
	data->mUsedVerts = vertices != NULL ? sizeVertices : 0;

	// set the vertex attributes
	this->setAttributes(attrs);
	// deep copy
	data->mAttrs = attrs;

	// Crear IBO y null porque no se van a copiar datos.
	glGenBuffers(1, &data->mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, (GLvoid*)indices, GL_DYNAMIC_DRAW);
	data->mIndexSize = sizeIndices;
	data->mUsedIndices = sizeIndices;

	return true;
}

bool OpenGLDevice::subGPUData(GPUData* data, const uint8_t* vertices, size_t numVertices,
	const uint16_t* indices, size_t numIndices, const std::vector<Attribute> attrs) {

	size_t vertexSize = 0;
	for (Attribute attr : attrs) {
		vertexSize += attr.mSize * Attribute::sTypeSize[attr.mType];
	}
	size_t sizeVertices = numVertices * vertexSize;
	size_t sizeIndices = numIndices * sizeof(uint16_t);

	// for the moment we do not set to 0 or NULL, could change if needed.
	if (data == NULL || sizeVertices > data->mVertexSize || sizeIndices > data->mIndexSize || 
		sizeVertices <=0 || sizeIndices <= 0 || attrs.size() == 0 || vertices == NULL || 
		indices == NULL) {
		return false;
	}

	glBindVertexArray(data->mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, data->mVBO);
	// hint for the driver to invalidate previous data
	glBufferData(GL_ARRAY_BUFFER, data->mVertexSize, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeVertices, (GLvoid*)vertices);
	data->mUsedVerts = sizeVertices;

	// clear previous attrs
	this->disableAttributes(data->mAttrs);
	// set the new vertex attributes
	this->setAttributes(attrs);
	data->mAttrs = attrs;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->mIBO);
	// hint to invalidate previous data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->mIndexSize, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeIndices, (GLvoid*)indices);
	data->mUsedIndices = sizeIndices;

	return true;
}

void OpenGLDevice::bindVAO(uint32_t VAO) {
	glBindVertexArray(VAO);
}

uint32_t OpenGLDevice::getPrimitive(Device::Primitive primitive) {
	return this->sPrimitives[primitive];
}

void OpenGLDevice::cleanGPUData(const GPUData* data, const std::vector<Attribute> attrs) {
	glBindVertexArray(data->mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, data->mVBO);
	this->disableAttributes(attrs);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &data->mVBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(data->mIndexSize, &data->mIBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &data->mVAO);
}

void OpenGLDevice::setAttributes(const std::vector<Attribute> attrs) {
	// calculate vertex size
	size_t vertexSize = 0;
	for (Attribute attr : attrs) {
		vertexSize += attr.mSize * Attribute::sTypeSize[attr.mType];
	}
	// set the vertex attributes
	int currentSize = 0;
	for (Attribute attr : attrs) {
		glVertexAttribPointer(attr.mLocation, attr.mSize, Attribute::sTypesGL[attr.mType], GL_FALSE, 
			vertexSize, (GLvoid*)currentSize);
		currentSize += attr.mSize * Attribute::sTypeSize[attr.mType];
		glEnableVertexAttribArray(attr.mLocation);
	}
}

void OpenGLDevice::disableAttributes(const std::vector<Attribute> attrs) {
	for (Attribute attr : attrs) {
		glDisableVertexAttribArray(attr.mLocation);
	}
}

void OpenGLDevice::setUniforms(const std::vector<UniformBind> uniforms, uint32_t shaderId) {
	// execute functions that set uniforms
	for (UniformBind uni : uniforms) {
		uni(shaderId);
	}
}

void OpenGLDevice::setShader(uint32_t shaderId) {
	glUseProgram(shaderId);
}

void OpenGLDevice::setTextures(std::vector<Texture2D> texs, uint32_t shaderId) {
	for (Texture2D tex : texs) {
		// activate a texture unit
		glActiveTexture(GL_TEXTURE0 + tex.mTexNumber);
		// get the location of the sampler
		GLuint textureLoc = glGetUniformLocation(shaderId, tex.mName.c_str());
		// make the sampler point to a texture unit
		glUniform1i(textureLoc, tex.mTexNumber);
		// bind a texture object to the tex unit specified before as a 2D tex
		glBindTexture(GL_TEXTURE_2D, tex.mTexId);
	}
}

void OpenGLDevice::disableTextures(std::vector<Texture2D> texs) {
	for (Texture2D tex : texs) {
		glActiveTexture(GL_TEXTURE0 + tex.mTexNumber);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

uint32_t OpenGLDevice::getIndexType(Device::IndexType type) {
	return this->sIndexType[type];
}

void OpenGLDevice::drawElements(Device::Primitive primitive, Device::IndexType type,
	size_t numIndices, void* offset) {
	glDrawElements(this->getPrimitive(primitive), numIndices, 
		this->getIndexType(type), offset);
}