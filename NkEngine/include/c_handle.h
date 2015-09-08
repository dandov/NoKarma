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

#ifndef C_HANDLE_H
#define C_HANDLE_H

#pragma once

#include <iostream>
#include <unordered_map>
#include <type_traits>
#include <vector>
#include <string>
#include <stdarg.h>
#include <queue>

#include "GameObjectFactory.h"

#include "HandleManager.h"

class HandleContainer {
	public:
		static pow2::HandleManager manager;

};

struct factory_handle {
public:
	factory_handle(pow2::Handle& handle = pow2::Handle()) : handle(handle) {}
	virtual ~factory_handle() {};
	virtual void deleteData() = 0;
	pow2::Handle handle;
	const unsigned int getIndex() const{ return handle.m_index; }
	const unsigned int getIndex() { return handle.m_index; }
	const std::type_info* getType() const{ return handle.m_type; }
	const std::type_info* getType() { return handle.m_type; }

	bool operator!() const{
		void* out;
		return !HandleContainer::manager.Get(handle, out);
	}
	explicit operator bool() const {
		void* out;
		return HandleContainer::manager.Get(handle, out);
	}

	bool operator()(const factory_handle& k) {
		return (handle.m_index < k.handle.m_index);
	}
};

template <class T>
struct c_handle : public factory_handle{
public:
	c_handle(pow2::Handle& handle = pow2::Handle()) : factory_handle(handle) {}
	T& get() { return GameObjectFactory::getComponent<T>(*this);}
	T& get() const{ return GameObjectFactory::getComponent<T>(*this);}
	
	virtual void deleteData() {
		using namespace pow2;
		std::vector<T>& components = GameObjectFactory::getComponentsOfType<T>();
		std::vector<c_handle<T>>& handles = GameObjectFactory::getComponentHandlesOfType<T>();
		void* out;
		bool valid = HandleContainer::manager.Get(handle, out);
		const uint32 index = (uint32)out;
		assert(valid && index < components.size() && index < handles.size());
		int end = components.size()-1;

		if (index != end) { // Si no esta al final pasar al final y borrar, realmente pasa la ultima a la posicion de la que se borra y se hace pop_back del final
			components[index] = components[end];
			handles[index] = handles[end];
			c_handle<T>& ch = handles[index];
			Handle locHandle = ch.handle;  /// Handle que tiene la posicion en el vector del elemento que cambio
			HandleContainer::manager.Update(locHandle, (void*)index);
		}

		
		components.pop_back();

		HandleContainer::manager.Remove(handle); //quitar del handlemanager

		handles.pop_back();

			
	}

	bool operator==(const c_handle<T>& rhs) { 
		return handle.m_counter == rhs.handle.m_counter && handle.m_index == rhs.handle.m_index && handle.m_type == rhs.handle.m_type;
	} 

	bool operator==(const c_handle<T>& rhs) const { 
		return handle.m_counter == rhs.handle.m_counter && handle.m_index == rhs.handle.m_index && handle.m_type == rhs.handle.m_type;
	} 

	bool operator< (const c_handle<T>& rhs) {
		void* out = HandleContainer::manager.Get(handle);
		const uint32 index = (uint32)out;
		out = HandleContainer::manager.Get(rhs.handle);
		const uint32 indexR = (uint32)out;
		return index < indexR;
	 }

	  bool operator< (const c_handle<T>& rhs) const{
		void* out = HandleContainer::manager.Get(handle);
		const uint32 index = (uint32)out;
		out = HandleContainer::manager.Get(rhs.handle);
		const uint32 indexR = (uint32)out;
		return index < indexR;
	 }
	
	T* operator->() { return &GameObjectFactory::getComponent<T>(*this); }
	T* operator->() const { return &GameObjectFactory::getComponent<T>(*this); }
};

#endif