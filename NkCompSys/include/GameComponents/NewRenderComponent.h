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

#ifndef NEWRENDER_COMPONENT_H
#define NEWRENDER_COMPONENT_H

#pragma once

#include <Device.h>
#include "GameObjectFactory.h"

#include <vector>
#include <memory>

#include "ModelLoader.h"

namespace Render
{
	struct Component {
		unsigned short vsize;
		unsigned short vtsize;
		unsigned short vnsize;
	};

	struct Model{
	public:
		Device::GPUData mGpuData;
		std::vector<Texture2D> mTexs;

		Model()
			: mGpuData()
			, mTexs() { }

		Model(const Device::GPUData data, const std::vector<Texture2D> texs)
			: mGpuData(data)
			, mTexs(texs) { }
	};

	void loadModelComponents(Model* model, const char* fileName, Device* device);
}

class NewRenderComponent :
	public GameComponent
{
public:

	NewRenderComponent();

	NewRenderComponent(Device* device, const uint8_t* data, size_t numVertices,
		const uint16_t* indices, size_t numIndices, const std::vector<Attribute> attrs,
		Device::Primitive primitive = Device::Triangles, size_t offset = 0);

	NewRenderComponent(Device::GPUData data, Device::Primitive primitive, size_t offset);

	NewRenderComponent(Device* device, const ModelData& modelData, int groupIndex = 0);
	
	~NewRenderComponent(void) { }

	template<typename... T>
	void addProperty(std::string name, std::function<void(std::string, uint32_t, T...)> function,
		T... params) {
		this->mProperties.addProperty(name, function, params...);
	}

	//template<typename... T>
	//GraphicProperty<T...>* get(std::string name) {
	//	return this->mProperties.get<T...>(name);
	//}

	//void removeProperty(std::string name) {
	//	this->mProperties.removeProperty(name);
	//}

	Device::GPUData mData;
	Device::Primitive mPrimitive;
	size_t mDrawOffset;
	GPUProperty mProperties;

private:
	virtual void recieve(const Message& msg){}	
};

#endif
