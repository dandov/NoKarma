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

#include "DrawBatch.h"
#include <GL/glew.h>

/*DRAW BATCH DEFINITIONS*/

const GLenum BatchDraw::sPrimitives[3] = { GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN };

BatchDraw::BatchDraw(uint32_t renderTarget)
: mRenderTarget(renderTarget)
, mBuffer()
, mMeta()
, mActive(false)
, mShaderId(0)
, mSortMode(BatchDraw::backToFront_Sort)
, mDrawPrimitive(BatchDraw::triangles_drawPrimitive)
, mVAO(0) 
, mVBO(0)
, mIBO(0)
, mNumTexs(0)
, mNumLocalUniforms(0)
, mGlobalUniforms() {
	this->createGPUData();
}

void BatchDraw::createGPUData() {
	//podriamos necesitar un objeto de contexto para tener contextos en bloques y mantener lo viejo
	// Crear VAO
	glGenVertexArrays(1, &this->mVAO);
	glBindVertexArray(this->mVAO);

	// Crear VBO y NULL en datos porque no se van a copiar datos.
	glGenBuffers(1, &this->mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->mVBO);
	glBufferData(GL_ARRAY_BUFFER, this->mBuffer.availableBytes(), NULL, GL_DYNAMIC_DRAW);

	// Crear IBO y null porque no se van a copiar datos. 
	glGenBuffers(1, &this->mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mBuffer.avaialableIndices() * sizeof(uint16_t),
		NULL, GL_DYNAMIC_DRAW);
}

void BatchDraw::disableAttribs() {
	size_t numAttribs = this->mBuffer.getVertexAttrs().size();
	for (size_t i = 0; i < numAttribs; i++) {
		glDisableVertexAttribArray(i);
	}
}

void BatchDraw::bindAndSetBuffers() {
	glBindVertexArray(this->mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->mVBO);
	// hint for the driver to invalidate previous data
	glBufferData(GL_ARRAY_BUFFER, this->mBuffer.availableBytes(), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->mBuffer.verticesSize() * this->mBuffer.vertexSize(),
		(GLvoid*)this->mBuffer.getVertices());

	// set the vertex attributes
	int index = 0;
	int currentSize = 0;
	for (Attribute attr : this->mBuffer.getVertexAttrs()) {
		glVertexAttribPointer(index, attr.mSize, Attribute::sTypesGL[attr.mType], GL_FALSE,
			this->mBuffer.vertexSize(), (GLvoid*)currentSize);
		currentSize += attr.mSize * Attribute::sTypeSize[attr.mType];
		glEnableVertexAttribArray(index);
		index++;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mIBO);
	// hint to invalidate previous data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mBuffer.avaialableIndices() * sizeof(uint16_t),
		NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, this->mBuffer.indicesSize() * sizeof(uint16_t), 
		this->mBuffer.getIndices());
}

void BatchDraw::setShader() {
	glUseProgram(this->mShaderId);
}

void BatchDraw::begin(SortMode sortMode, DrawPrimitive primitive, uint32_t shaderId,
	const std::vector<Attribute> attrs, size_t numTexs, size_t numLocalUniforms,
	std::vector<std::function<void(uint32_t)>> globalUniforms) {
	// In case begin has already being called and no end call has been executed we do not modify
	// the state of the current draw call. Also check for other bad cases
	if (this->mActive || attrs.size() == 0 || shaderId <= 0 || numTexs < 0 || 
		numTexs > BatchDraw::sMaxTextures) {
		return;
	}

	this->mActive = true;
	this->mSortMode = sortMode;
	this->mDrawPrimitive = primitive;
	this->mShaderId = shaderId;
	this->mNumTexs = numTexs;
	this->mNumLocalUniforms = numLocalUniforms;

	// clear previous attribs and set the new ones in the batch buffer
	// TODO: como mover esta a end porque supone que el buffer esta activado
	this->disableAttribs();
	this->mBuffer.reset(attrs); // only changes the attributes and no reallocation
	this->mMeta.clear();

	// reset uniforms
	this->mGlobalUniforms.clear();
	for (auto uni : globalUniforms) {
		this->mGlobalUniforms.push_back(uni);
	}
}

void BatchDraw::draw(const uint8_t* data, size_t numElements, const uint16_t* indices, 
	size_t numIndices, const Texture2D* texs, const UniformBind* uniforms) {

	// exit in case no begin call
	if (!this->mActive) {
		return;
	}

	if (this->mSortMode == BatchDraw::immediate_Sort) {
		if (this->mBuffer.append(data, numElements, indices, numIndices)) {
			this->draw();
			this->mBuffer.clear();
			return;
		}
	}
	
	// save previous pointer locations to add to metada in case it succeeds.
	const uint8_t* dataPtr = this->mBuffer.getCurrentVertex();
	const uint16_t* indicesPtr = this->mBuffer.getCurrentIndex();

	if (!this->mBuffer.append(data, numElements, indices, numIndices)) {
		// MUST FLUSH HERE
		this->draw();
		
		// once empty append the data, it should succeed now
		this->mBuffer.clear();
		
		// can ask for the original ptr because it is now at the beginning since it was cleared
		dataPtr = this->mBuffer.getVertices();
		indicesPtr = this->mBuffer.getIndices();
		this->mBuffer.append(data, numElements, indices, numIndices);
	}

	// build metadata and add to queue
	// must send as right value reference constructor
	this->mMeta.emplace_back(dataPtr, numElements * this->mBuffer.vertexSize(), indicesPtr,
		numIndices, texs, this->mNumTexs, uniforms, this->mNumLocalUniforms);
}

void BatchDraw::end() {
	// just return in case begin has not been called. 
	if (!this->mActive || this->mMeta.size() == 0) {
		return;
	}

	this->mActive = false;
	//this->mBuffer.end();
	//this->mBuffer.use();
	//glDrawElements(BatchDraw::sPrimitives[this->mDrawPrimitive], this->mBuffer.indicesSize(),
	//	GL_UNSIGNED_SHORT, (GLvoid*)0);
	this->draw();
}

void BatchDraw::draw() {
	
	this->setShader();
	this->bindAndSetBuffers();
	this->setGlobalUniforms(); ///DEFINE METHOD AND ADD PARAMETERS TO BEGIN

	size_t numIndices = this->mMeta[0].mNumIndices;
	size_t firstIndex = 0;
	Metadata* begin = &this->mMeta[0];
	for (size_t i = 1; i < this->mMeta.size(); i++) {
		Metadata& current = this->mMeta[i];
		// they are different if the don't have the same texs or if they have individual uniforms
		// optimization here if theyy share the same uniform values
		if (current == *begin && this->mNumLocalUniforms == 0) {
			numIndices += current.mNumIndices;
		}
		// batch breaker
		else {
			this->draw(*begin, firstIndex, numIndices);
			begin = &this->mMeta[i];
			firstIndex = numIndices;
			numIndices = begin->mNumIndices;
		}
	}
	this->draw(*begin, firstIndex, numIndices);
}

void BatchDraw::draw(Metadata& data, size_t firstIndex, size_t numIndices) {
	// TODO: set individual uniforms

	this->setLocalUniforms(data);
	this->setTextures(data);

	glDrawElements(BatchDraw::sPrimitives[this->mDrawPrimitive], numIndices,
		GL_UNSIGNED_SHORT, (GLvoid*)(firstIndex * sizeof(uint16_t)));
	
	this->disableTextures(data);
}

void BatchDraw::setGlobalUniforms() {
	// execute functions that set uniforms
	for (auto uni : this->mGlobalUniforms) {
		uni(this->mShaderId);
	}
}

void BatchDraw::setLocalUniforms(Metadata& data) {
	// execute functions that set uniforms
	for (size_t i = 0; i < this->mNumLocalUniforms; i++) {
		data.mUniforms[i](this->mShaderId);
	}
}

void BatchDraw::setTextures(Metadata& data) {
	// set textures
	char buffer[15];
	GLenum currentTex = GL_TEXTURE0;
	for (size_t i = 0; i < this->mNumTexs; i++, currentTex++) {
		sprintf(buffer, "sampler%d", i);
		GLuint textureLoc = glGetUniformLocation(this->mShaderId, buffer);
		glActiveTexture(currentTex);
		glUniform1i(textureLoc, i);
		glBindTexture(GL_TEXTURE_2D, data.mTexs[i].mTexId);
	}
}

void BatchDraw::disableTextures(Metadata& data) {
	for (size_t i = 0; i < data.mNumTexs; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
