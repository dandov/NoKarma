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

#ifndef BATCH_BUFFER_H
#define BATCH_BUFFER_H

#pragma once
#include <stdint.h>
#include <vector>
#include <GL/glew.h>
#include <memory>
#include "Attribute.h"

// need to consider if the exact number of elems fits in the size of data because some bytes may
// be left of at the end
class BatchBuffer {

public:

	// ctor that generates everything automatically
	// 1 attribute of 1 byte
	BatchBuffer();

	// ctor to specify the vertex attributes, the data is the auto generated of default size
	BatchBuffer(const std::vector<Attribute> attrs);

	// ctor to specify mem location, its size in bytes, the number of indices in elems,
	// and the vertex attributes
	BatchBuffer(size_t dataSize, size_t numIndices, const std::vector<Attribute> attrs);

	// sets the size and types of the data contained in each vertex by traversing the vertex 
	// attributes
	void setVertexType(const std::vector<Attribute> attrs);

	// clears the byte counter and the index counter
	void clear();

	// resets the object by deleting the info of the vertex attribs
	void reset(const std::vector<Attribute> attrs);

	//  same as the previous but also resizes the memory its allocated memory
	void reset(const std::vector<Attribute> attrs, size_t dataSize, size_t numIndices);

	// append data
	bool append(const uint8_t* data, size_t numElems, const uint16_t* indices, size_t numIndices);

	const std::vector<Attribute> getVertexAttrs();

	// access to the current mapped vertices if glmapbuffer is on, if not null
	const uint8_t* getVertices();

	const uint8_t* getCurrentVertex();

	// access to the current mapped indices if glmapbuffer is on, if not null
	const uint16_t* getIndices();

	const uint16_t* getCurrentIndex();

	size_t availableBytes();

	size_t avaialableIndices();

	size_t verticesSize();

	size_t indicesSize();

	size_t vertexSize();

	static const size_t sDefaultDataSize = 8192; // num elems, still within short domain
	static const size_t sDefaultIndexSize = 16384;

private:
	
	std::unique_ptr<uint8_t[]> mDataPtr;
	std::unique_ptr<uint16_t[]> mIndicesPtr;
	size_t mDataSize; // data size in bytes
	size_t mIndicesSize;
	size_t mCurrentBytes;
	size_t mCurrentIndex;
	size_t mVertexSize; // vertex size in bytes;
	std::vector<Attribute> mAttrs;
};

#endif