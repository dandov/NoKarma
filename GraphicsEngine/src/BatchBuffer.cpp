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

#include "BatchBuffer.h"

/* VERTEXATTRIBUTE DEFINITIONS */

const int Attribute::sTypeSize[4] = {
	sizeof(float), sizeof(uint8_t), sizeof(short), sizeof(uint32_t)
};

const GLenum Attribute::sTypesGL[4] = {
	GL_FLOAT, GL_BYTE, GL_SHORT, GL_INT
};

///////////////////////////////////////////////////////////////////////////////////////////////////

/*VERTEXBUFFER DEFINITIONS*/

BatchBuffer::BatchBuffer()
: mDataPtr(new uint8_t[BatchBuffer::sDefaultDataSize])
, mIndicesPtr(new uint16_t[BatchBuffer::sDefaultIndexSize])
, mDataSize(BatchBuffer::sDefaultDataSize) //can set directly because it will be 1 byte
, mIndicesSize(BatchBuffer::sDefaultIndexSize)
, mCurrentBytes(0)
, mCurrentIndex(0)
, mVertexSize(0)
, mAttrs() {
	// create a default 1 byte attribute
	std::vector<Attribute> attrs;
	attrs.emplace_back(0, 1, Attribute::byte_VA, 1, (void*)0);
	this->setVertexType(attrs);
	//this->setData();
}

BatchBuffer::BatchBuffer(const std::vector<Attribute> attrs)
: mDataPtr(new uint8_t[BatchBuffer::sDefaultDataSize])
, mIndicesPtr(new uint16_t[BatchBuffer::sDefaultIndexSize])
, mDataSize(BatchBuffer::sDefaultDataSize)
, mIndicesSize(BatchBuffer::sDefaultIndexSize)
, mCurrentBytes(0)
, mCurrentIndex(0)
, mVertexSize(0)
, mAttrs() {
	this->setVertexType(attrs);
	// set the data buffer based on the vertex size
	//this->setData();
}

BatchBuffer::BatchBuffer(size_t dataSize, size_t numIndices, const std::vector<Attribute> attrs)
: mDataPtr(new uint8_t[dataSize < BatchBuffer::sDefaultDataSize ? 
						BatchBuffer::sDefaultDataSize : dataSize])
, mIndicesPtr(new uint16_t[numIndices < BatchBuffer::sDefaultIndexSize ? 
							BatchBuffer::sDefaultIndexSize : numIndices])
, mDataSize(dataSize < BatchBuffer::sDefaultDataSize ? BatchBuffer::sDefaultDataSize : dataSize)
, mIndicesSize(numIndices < BatchBuffer::sDefaultIndexSize ? 
				BatchBuffer::sDefaultIndexSize : numIndices)
, mCurrentBytes(0)
, mCurrentIndex(0)
, mVertexSize(0)
, mAttrs() {
	this->setVertexType(attrs);
	///this->setData();
}

void BatchBuffer::setVertexType(const std::vector<Attribute> attrs) {
	// maybe throw an exception?
	_ASSERT(attrs.size() > 0);

	this->mAttrs.clear();
	this->mVertexSize = 0;
	for (Attribute attr : attrs) {
		this->mAttrs.push_back(attr);
		this->mVertexSize += attr.mSize * Attribute::sTypeSize[attr.mType];
	}
}

void BatchBuffer::clear() {
	this->mCurrentBytes = 0;
	this->mCurrentIndex = 0;
}

void BatchBuffer::reset(const std::vector<Attribute> attrs)  {
	// reset the vertex attributes and vertex size
	this->setVertexType(attrs);

	// reset the byte and index counter
	this->clear();
}

void BatchBuffer::reset(const std::vector<Attribute> attrs, size_t dataSize, size_t numIndices)  {

	this->reset(attrs);

	// Validate and update mDataSize
	// if either dataSize or data are not valid then we use the default options
	this->mDataSize = dataSize < BatchBuffer::sDefaultDataSize ?
		BatchBuffer::sDefaultDataSize : dataSize;
	// reset indices
	this->mIndicesSize = numIndices < BatchBuffer::sDefaultIndexSize ?
		BatchBuffer::sDefaultIndexSize : numIndices;

	this->mDataPtr.reset(new uint8_t[this->mDataSize]);
	this->mIndicesPtr.reset(new uint16_t[this->mIndicesSize]);
}

bool BatchBuffer::append(const uint8_t* data, size_t numElems, const uint16_t* indices, 
	size_t numIndices) {

	int bytes = numElems * this->mVertexSize;
	// check if there is enough space free for the new data, if not return false
	if (this->mCurrentBytes + bytes > this->mDataSize ||
		this->mCurrentIndex + numIndices > mIndicesSize) {
		return false;
	}

	// copy the new data taking into account the current byte offset and update the byte counter
	memcpy(this->mDataPtr.get() + this->mCurrentBytes, data, bytes);
	int offset = this->mCurrentBytes / this->mVertexSize;
	this->mCurrentBytes += bytes;
	_ASSERT(this->mCurrentBytes % this->mVertexSize == 0);

	//copy indices adjusting offset of the initial vertex
	uint16_t* indexPtr = this->mIndicesPtr.get();
	for (size_t i = 0; i < numIndices; i++, this->mCurrentIndex++) {
		indexPtr[this->mCurrentIndex] = indices[i] + offset;
	}
	//fixe assert, store previous currentindex
	//_ASSERT(this->mCurrentIndex == this->mCurrentIndex + numIndices);

	return true;
}

const std::vector<Attribute> BatchBuffer::getVertexAttrs() {
	return this->mAttrs;
}

const uint8_t* BatchBuffer::getVertices() {
	return const_cast<const uint8_t*>(this->mDataPtr.get());
}

const uint8_t* BatchBuffer::getCurrentVertex() {
	return const_cast<const uint8_t*>(this->mDataPtr.get() + this->mCurrentBytes);
}

const uint16_t* BatchBuffer::getIndices() {
	return const_cast<const uint16_t*>(this->mIndicesPtr.get());
}

const uint16_t* BatchBuffer::getCurrentIndex() {
	return const_cast<const uint16_t*>(this->mIndicesPtr.get() + this->mCurrentIndex);
}

size_t BatchBuffer::availableBytes() {
	return this->mDataSize;
}

size_t BatchBuffer::avaialableIndices() {
	return this->mIndicesSize;
}

size_t BatchBuffer::verticesSize() {
	return this->mCurrentBytes / this->mVertexSize;
}

size_t BatchBuffer::indicesSize() {
	return this->mCurrentIndex;
}

size_t BatchBuffer::vertexSize() {
	return this->mVertexSize;
}
