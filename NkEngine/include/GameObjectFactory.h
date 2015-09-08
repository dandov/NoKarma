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

#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#pragma once

#include <iostream>
#include <unordered_map>
#include <type_traits>
#include <vector>
#include <string>
#include <stdarg.h>
#include <queue>


#include "GameComponent.h"
//#include "GameEntity.h"
#include "HandleManager.h"
//#include "c_handle.h"

#define INITIAL_SIZE 5

class HandleContainer;


/*
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
		return !(HandleContainer::manager.Get(handle));
	}
	explicit operator bool() const {
		return HandleContainer::manager.Get(handle);
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
		assert(valid && index < components.size());
		int end = components.size()-1;

		components[index] = components[end];
		components.pop_back();

		assert(index < handles.size());
		end = handles.size()-1;

		HandleContainer::manager.Remove(handles[index].handle); //quitar del handlemanager

		handles[index] = handles[end];
		c_handle<T>& ch = handles[index];
		Handle locHandle = ch.handle;  /// Handle que tiene la posicion en el vector del elemento que cambio
		handles.pop_back();

		HandleContainer::manager.Update(locHandle, (void*)index);
	}

	bool operator==(c_handle<T>& rhs) { 
		return handle.m_counter == rhs.handle.m_counter && handle.m_index && rhs.handle.m_index && handle.m_type && rhs.handle.m_type;
	} 

	bool operator==(c_handle<T>& rhs) const { 
		return handle.m_counter == rhs.handle.m_counter && handle.m_index && rhs.handle.m_index && handle.m_type && rhs.handle.m_type;
	} 
	
	T* operator->() { &GameObjectFactory::getComponent<T>(*this); }
	T* operator->() const { &GameObjectFactory::getComponent<T>(*this); }
};*/

struct factory_handle;

template <class T>
struct c_handle;

class GameEntity;

class GameObjectFactory
{
public:
	GameObjectFactory(void);
	~GameObjectFactory(void);

	static c_handle<GameEntity>& createGameEntity();
	static std::vector<c_handle<GameEntity>>& getAllEntities();

	static int componentTypeCount;

	template <typename T>
	static c_handle<T> createCopy(const c_handle<T>&  component) {
		std::vector<T>& components = ComponentContainer<T>::getComponentsOfType();
		std::vector<c_handle<T>>& handles = ComponentContainer<T>::getHandlesOfType();

		void* out;
		bool valid = HandleContainer::manager.Get(component.handle, out);
		const uint32 index = (uint32)out;
		assert(valid && index < components.size());

		T& comp = components[index];
		components.push_back(comp);
		const std::type_info* t = &typeid(T);
		pow2::Handle handle = HandleContainer::manager.Add((void*)(components.size()-1), t); 
		handles.emplace_back(handle);
		const int end = handles.size() -1;

		return handles[end];
	}

	template <typename T, typename... Params>
	static c_handle<T> createComponent(const Params & ... params) {
		std::vector<T>& components = ComponentContainer<T>::getComponentsOfType();
		std::vector<c_handle<T>>& handles = ComponentContainer<T>::getHandlesOfType();
		//std::queue<c_handle>& queue = ComponentContainer<T>::getHandleQueue();

		components.emplace_back(params...);
		const std::type_info* t = &typeid(T);
		pow2::Handle handle = HandleContainer::manager.Add((void*)(components.size()-1), t); 
		handles.emplace_back(handle);
		const int end = handles.size() -1;

		return handles[end];
	}

	template <typename T, typename... Params>
	static c_handle<T> createComponent(Params&& ... params) {
		std::vector<T>& components = ComponentContainer<T>::getComponentsOfType();
		std::vector<c_handle<T>>& handles = ComponentContainer<T>::getHandlesOfType();
		//std::queue<c_handle>& queue = ComponentContainer<T>::getHandleQueue();

		components.emplace_back(std::forward<Params>(params)...);
		const std::type_info* t = &typeid(T);
		pow2::Handle handle = HandleContainer::manager.Add((void*)(components.size() - 1), t);
		handles.emplace_back(handle);
		const int end = handles.size() - 1;

		return handles[end];
	}

	template <typename T>
	static c_handle<T> createComponent() {
		std::vector<T>& components = ComponentContainer<T>::getComponentsOfType();
		std::vector<c_handle<T>>& handles = ComponentContainer<T>::getHandlesOfType();
		//std::queue<c_handle>& queue = ComponentContainer<T>::getHandleQueue();

		components.emplace_back();
		const std::type_info* t = &typeid(T);
		pow2::Handle handle = HandleContainer::manager.Add((void*)(components.size() - 1), t);
		handles.emplace_back(handle);
		const int end = handles.size() - 1;

		return handles[end];
	}

	template <typename T>
	static std::vector<T>& getComponentsOfType() {
		return ComponentContainer<T>::getComponentsOfType();
	}
	
	template <typename T>
	static T& getComponent(const c_handle<T>& h) {
		void* out;
		bool valid = HandleContainer::manager.Get(h.handle, out);
		_ASSERT(valid);
		const uint32 index = (uint32)out;
		std::vector<T>& components = ComponentContainer<T>::getComponentsOfType();
		_ASSERT(index < components.size());
		
		return components[index];
	}



	template <typename N, typename T>
	static c_handle<N> getComponentBrother(const c_handle<T>& h) {
		void* out;
		std::vector<T>& components = ComponentContainer<T>::getComponentsOfType();
		std::vector<GameEntity>& entities = ComponentContainer<GameEntity>::getComponentsOfType();
		bool valid = HandleContainer::manager.Get(h.handle, out);
		assert(valid);
		const uint32 index = (uint32)out;
		assert(index < components.size());


		c_handle<GameEntity> parent = components[index].getParentEntity();

		valid = HandleContainer::manager.Get(parent.handle, out);

		const uint32 eIndex = (uint32)out;

		c_handle<N> brotherH;

		if (valid) {
			brotherH = entities[eIndex].getComponent<N>();
		}

		 

		return brotherH;
	}

	template <typename T>
	static void removeComponent(c_handle<T>& h) {
		//c_handle<GameEntity> entity = h->getParentEntity();
		//entity->removeComponent<T>();
		h.deleteData();
	}


	template <typename T>
	static std::vector<c_handle<T>>& getComponentHandlesOfType() {
		return ComponentContainer<T>::getHandlesOfType();
	}


	static void removeEntity(c_handle<GameEntity>& handle);

private:
	//static std::vector<GameEntity> entities;
	static std::queue<unsigned int> freeIndices;
	//static HandlesMap handlesMap;
	template <typename CT>
	class ComponentContainer {

	public:
		ComponentContainer() {
			Init();
		}
		static void Init() {
			getComponentsOfType();
		}

		static std::vector<CT>& getComponentsOfType() {
			static std::vector<CT> components;
			static const std::type_info* type = &typeid(CT);
			return components;
		}

		static std::queue<c_handle<CT>>& getHandleQueue() {
			static std::queue<c_handle<CT>> hQueue;
			return hQueue;
		}

		static std::vector<c_handle<CT>>& getHandlesOfType() {
			static std::vector<c_handle<CT>> handles;
			return handles;
		}

	};
};

#endif