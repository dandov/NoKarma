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

#ifndef CANVAS_H
#define CANVAS_H

#pragma once
#include <stdint.h>
#include "Attribute.h"
#include "Device.h"
#include "Texture2D.h"
#include "Uniform.h"
#include <vector>
#include "Device.h"

class Canvas {
public:
	Canvas();
	Canvas(Device* device);

	void draw(uint32_t shader, Device::Primitive primitive, uint32_t VAO, size_t numIndices, 
		void* offset, const std::vector<Texture2D> texs, const std::vector<UniformBind> uniforms,
		Device::IndexType type = Device::Short);

	void draw(uint32_t shader, Device::Primitive primitive, uint32_t VAO, size_t numIndices,
		void* offset, Device::IndexType type = Device::Short);

	Device* getDevice() {
		return this->mDevice;
	}
private:
	Device* mDevice;
};

#endif
