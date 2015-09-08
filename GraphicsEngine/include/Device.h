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

#ifndef BATCH_H
#define BATCH_H

#pragma once
#include "Attribute.h"
#include <stdint.h>
#include "Texture2D.h"
#include "Uniform.h"
#include <vector>

class Device {
public:

	struct GPUData {
		uint32_t mVAO, mVBO, mIBO;
		size_t mVertexSize, mIndexSize; // in bytes
		size_t mUsedVerts, mUsedIndices;
		std::vector<Attribute> mAttrs;

		GPUData()
			: mVAO(0)
			, mVBO(0)
			, mIBO(0)
			, mVertexSize(0)
			, mIndexSize(0)
			, mUsedVerts(0)
			, mUsedIndices(0)
			, mAttrs() { }
	};

	enum Primitive {
		Triangles,
		Triangle_Strip,
		Triangle_Fan
	};

	enum IndexType {
		Byte,
		Short,
		Int
	};

	// sizes are expected number of elements, not bytes
	virtual bool createGPUData(GPUData* data, const uint8_t* vertices, size_t numVertices,
		const uint16_t* indices, size_t numIndices, const std::vector<Attribute> attrs) = 0;

	// binds the VAO, VBO and IBO, also creates
	// enables the vertex attributes
	// copies data to the gpu
	virtual bool subGPUData(GPUData* data, const uint8_t* vertices, size_t numVertices,
		const uint16_t* indices, size_t numIndices, const std::vector<Attribute> attrs) = 0;

	// sets the attributes, expects that the buffer object is already bound
	// for the attributes we need to move the array of types out to keep with the idea of devices
	virtual void setAttributes(const std::vector<Attribute> attrs) = 0;
	virtual void disableAttributes(const std::vector<Attribute> attrs) = 0;

	// for the uniforms we need to make virtual or something the uniforms to keep with the idea 
	// of devices
	virtual void setUniforms(const std::vector<UniformBind> uniforms, uint32_t shaderId) = 0;
	virtual void setShader(uint32_t shaderId) = 0;
	virtual void setTextures(std::vector<Texture2D> texs, uint32_t shaderId) = 0;
	virtual void disableTextures(std::vector<Texture2D> texs) = 0;

	virtual void bindVAO(uint32_t VAO) = 0;
	virtual uint32_t getPrimitive(Device::Primitive primitive) = 0;
	virtual uint32_t getIndexType(Device::IndexType type) = 0;

	virtual void cleanGPUData(const GPUData* data, const std::vector<Attribute> attr) = 0;

	virtual void drawElements(Device::Primitive primitive, Device::IndexType type, size_t numIndices, void* offset) = 0;

};

class OpenGLDevice : public Device {
public:

	// generates VAO, VBO and IBO
	virtual bool createGPUData(GPUData* data, const uint8_t* vertices, size_t numVertices,
		const uint16_t* indices, size_t numIndices, const std::vector<Attribute> attrs) override;

	// binds the VAO, VBO and IBO, also creates
	// enables the vertex attributes
	// copies data to the gpu
	virtual bool subGPUData(GPUData* data, const uint8_t* vertices, size_t numVertices,
		const uint16_t* indices, size_t numIndices, const std::vector<Attribute> attrs) override;
	virtual void bindVAO(uint32_t VAO) override;
	virtual uint32_t getPrimitive(Device::Primitive primitive) override;
	virtual uint32_t getIndexType(Device::IndexType type) override;
	virtual void setAttributes(const std::vector<Attribute> attrs) override;
	virtual void disableAttributes(const std::vector<Attribute> attrs) override;
	virtual void setUniforms(const std::vector<UniformBind> uniforms, uint32_t shaderId) override;
	virtual void setShader(uint32_t shaderId) override;
	virtual void setTextures(std::vector<Texture2D> texs, uint32_t shaderId) override;
	virtual void disableTextures(std::vector<Texture2D> texs) override;
	virtual void cleanGPUData(const GPUData* data, const std::vector<Attribute> attr) override;

	virtual void drawElements(Device::Primitive primitive, Device::IndexType type, 
		size_t numIndices, void* offset) override;
private:
	const static uint32_t sPrimitives[3];
	const static uint32_t sIndexType[3];
};

#endif