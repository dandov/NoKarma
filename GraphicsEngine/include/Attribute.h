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

#ifndef ATTR_H
#define ATTR_H

#pragma once
#include <GL/glew.h>

//loc, size, tipoglfloat, normalizetf, stride, innerpos
struct Attribute {

public:

	enum Type { // translate to glenums, decouple from opengl
		float_VA,
		byte_VA,
		short_VA,
		int_VA
	};

	const static int sTypeSize[4];
	const static GLenum sTypesGL[4];

	Attribute(size_t location, size_t size, Type type, size_t stride, void* pointer)
		: mLocation(location)
		, mSize(size)
		, mType(type)
		, mNormalize(false)
		, mStride(stride)
		, mPointer(pointer) { }

	size_t mLocation, mSize;
	Type mType;
	bool mNormalize;
	size_t mStride;
	void* mPointer;
};

#endif
