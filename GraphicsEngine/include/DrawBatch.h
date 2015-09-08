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

#ifndef BATCH_DRAW_H
#define BATCH_DRAW_H

#pragma once
#include <stdint.h>
#include "BatchBuffer.h"
#include <GL/glew.h>
#include <string>
#include "Texture2D.h"
#include <functional>
#include "Uniform.h"

class BatchDraw {

public:
	enum SortMode {
		backToFront_Sort,
		frontToBack_Sort,
		immediate_Sort
	};

	enum DrawPrimitive {
		triangles_drawPrimitive,
		triangleStrip_drawPrimitive,
		triangeFan_drawPrimitive
	};

	const static GLenum sPrimitives[3];

	/**
	 * Specify the render target on which the draw calls are going to be rendered.
	*/
	BatchDraw(uint32_t renderTarget);

	/**
	* Begin method sets the state for the following method draw calls.
	* sortMode defines the order in which each draw is executed inside this batch, currently we
	* only support backToFront and we are planning to add custom orderings.
	* Since it defines a shaderId the vertices should provide the corresponding attribute
	* structure.
	*/
	void begin(SortMode sortMode, DrawPrimitive primitive, uint32_t shaderId, 
		const std::vector<Attribute> attrs, size_t numTexs, size_t numLocalUniforms, 
		std::vector<UniformBind> globalUniforms);

	/**
	* adds the
	* TODO: texture struct to hold all specs. some are in begin others are in the struct, tex
	* could be empty 
	*/
	void draw(const uint8_t* data, size_t numElements, const uint16_t* indices, size_t numIndices, 
		const Texture2D* texs = NULL, const UniformBind* uniforms = NULL);

	/**
	* Ends the batch, sets the state specified in the corresponding begin method and executes the
	* draw calls in the less number of batches possible.
	*/
	void end();

	const static size_t sMaxTextures = 4;

private:
	struct Metadata {
		
		Metadata(const uint8_t* data, size_t dataSize, const uint16_t* indices, size_t numIndices, 
			const Texture2D* texs, size_t numTexs, const UniformBind* uniforms, size_t numUniforms)
			: mData(data)
			, mIndices(indices)
			, mDataSize(dataSize)
			, mNumIndices(numIndices)
			, mTexs(nullptr)
			, mNumTexs(numTexs)
			, mUniforms() { 

			if (texs != NULL && mNumTexs > 0) {
				Texture2D* auxTexs = new Texture2D[mNumTexs];
				memcpy(auxTexs, texs, mNumTexs * sizeof(Texture2D));
				mTexs.reset(auxTexs);
			} else {
				mNumTexs = 0;
			}

			if (uniforms != NULL && numUniforms > 0) {
				for (size_t i = 0; i < numUniforms; i++) {
					this->mUniforms.push_back(uniforms[i]);
				}
			}
		}

		// sirve para el push_back del vector que guarda metadata en batchdraw
		// use una right value ref porque lo agrega con un constructor implicito
		Metadata(Metadata&& other)
			: mData(other.mData)
			, mIndices(other.mIndices)
			, mDataSize(other.mDataSize)
			, mNumIndices(other.mNumIndices)
			, mTexs(nullptr)
			, mNumTexs(other.mNumTexs)
			, mUniforms(other.mUniforms) {
			this->mTexs.reset(other.mTexs.release());
		}

		bool operator==(Metadata& other) {
			// always the same number of texs and uniforms
			//if (this->mNumUniforms != other.mNumUniforms)
			//	return false;

			// first check if they have the same number of textures
			if (this->mNumTexs != other.mNumTexs)
				return false;

			// if they do have the same number of texs then compare each one of them
			for (size_t i = 0; i < this->mNumTexs; i++) {
				if (!(this->mTexs[i] == other.mTexs[i])) {
					return false;
				}
			}

			return true;
		}

		const uint8_t* mData;
		const uint16_t* mIndices;
		size_t mDataSize;
		size_t mNumIndices;
		std::unique_ptr<Texture2D[]> mTexs;
		size_t mNumTexs;
		std::vector<UniformBind> mUniforms;
	};

	// generates VAO, VBO and IBO
	void createGPUData();

	// disables vertex attributes
	void disableAttribs();

	// binds the VAO, VBO and IBO, also creates
	// enables the vertex attributes
	// copies data to the gpu
	void bindAndSetBuffers();

	void setGlobalUniforms();

	void setLocalUniforms(Metadata& data);

	void setShader();

	void setTextures(Metadata& data);
	void disableTextures(Metadata& data);

	void draw();
	// draws the current buffer
	void draw(Metadata& data, size_t firstIndex, size_t numIndices);
	
	uint32_t mRenderTarget;
	BatchBuffer mBuffer;
	std::vector<Metadata> mMeta;
	bool mActive;
	uint32_t mShaderId;
	SortMode mSortMode;
	DrawPrimitive mDrawPrimitive;
	size_t mNumTexs, mNumLocalUniforms;
	std::vector<std::function<void(uint32_t)>> mGlobalUniforms;

	uint32_t mVAO, mVBO, mIBO;
};

#endif
