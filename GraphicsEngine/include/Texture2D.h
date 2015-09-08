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

#ifndef TEX2D_H
#define TEX2D_H

#pragma once
#include <stdint.h>
#include <GL/glew.h>
#include <string>
#include <Windows.h>
#include <FreeImage.h>

class Texture2D {

public:
	Texture2D();

	Texture2D(std::string filename, std::string name, uint32_t texNumber);

	Texture2D(FIBITMAP* mTexels, std::string name, uint32_t texNumber, int channels);

	bool operator==(Texture2D& other);

	bool isCPUBacked();

	void releaseTexels();

	~Texture2D();

	uint32_t mTexId;
	std::string mName;
	uint32_t mTexNumber;
	size_t mWidth, mHeight, mBitsPerPixel;
	//TODO: cambiar por std::unique_ptr e investigar custom destructor para usar freeimage_unload
	FIBITMAP* mTexels;
};


#endif