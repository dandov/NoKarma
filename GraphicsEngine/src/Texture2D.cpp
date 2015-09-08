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

#include "Texture2D.h"

Texture2D::Texture2D() 
: mTexId(0)
, mName("")
, mTexNumber(0)
, mWidth(0)
, mHeight(0)
, mBitsPerPixel(0)
, mTexels(NULL) { }

// TODO: Texture manager que controle el texnumber y la uniform location

Texture2D::Texture2D(std::string filename, std::string name, uint32_t texNumber)
: mTexId(0)
, mName(name)
, mTexNumber(texNumber)
, mWidth(0)
, mHeight(0)
, mBitsPerPixel(0)
, mTexels(NULL) { 

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str());
	this->mTexels = FreeImage_Load(format, filename.c_str());

	if (mTexels != NULL) {
		//Para asegurar que este en RGB de 8 bits cada color
		mTexels = FreeImage_ConvertTo24Bits(mTexels);
		this->mWidth = FreeImage_GetWidth(mTexels);
		this->mHeight = FreeImage_GetHeight(mTexels);
		uint8_t * pixels = reinterpret_cast<uint8_t*>(FreeImage_GetBits(mTexels));

		glGenTextures(1, &this->mTexId);
		glBindTexture(GL_TEXTURE_2D, this->mTexId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->mWidth, this->mHeight, 0, GL_BGR, 
			GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

Texture2D::Texture2D(FIBITMAP* mTexels, std::string name, uint32_t texNumber, 
int channels)
: mTexId(0)
, mName(name)
, mTexNumber(texNumber)
, mWidth(0)
, mHeight(0)
, mBitsPerPixel(0)
, mTexels(mTexels) {

	if (mTexels != NULL) {

		this->mBitsPerPixel = FreeImage_GetBPP(this->mTexels);
		this->mWidth = FreeImage_GetWidth(this->mTexels);
		this->mHeight = FreeImage_GetHeight(this->mTexels);
		uint8_t * pixels = reinterpret_cast<uint8_t*>(FreeImage_GetBits(this->mTexels));

		glGenTextures(1, &this->mTexId);
		glBindTexture(GL_TEXTURE_2D, this->mTexId);

		GLenum internalFormat = GL_RGB;
		GLenum format = GL_BGR;

		if (channels == 32) {
			internalFormat = GL_RGBA;
			format = GL_BGRA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, this->mWidth, this->mHeight, 0, format,
			GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

bool Texture2D::operator==(Texture2D& other) {
	if (this->mTexId != other.mTexId)
		return false;

	if (this->mName != other.mName)
		return false;

	if (this->mTexNumber != other.mTexNumber)
		return false;

	if (this->mWidth != other.mWidth)
		return false;

	if (this->mHeight != other.mHeight)
		return false;

	if (this->mBitsPerPixel != other.mBitsPerPixel)
		return false;

	// return false because if either one of them is NULL, then we can't compare their memory and 
	// we can't make sure they are the same
	if (this->mTexels == NULL || other.mTexels == NULL)
		return false;

	// compare the texels of both textures
	if (memcmp(FreeImage_GetBits(this->mTexels), FreeImage_GetBits(other.mTexels),
		this->mBitsPerPixel * this->mWidth * this->mHeight) != 0)
		return false;

	return true;
}

void Texture2D::releaseTexels() {
	if (this->mTexels != NULL) {
		//FreeImage_Unload(mTexels);
		//this->mTexels = NULL;
	}
}

Texture2D::~Texture2D() {
	//this->releaseTexels();
}
