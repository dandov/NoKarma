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


#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#pragma once

#include <iostream>
#include <unordered_map>
#include <type_traits>
#include <string>
#include <memory>


#include <assert.h>


#include "GameComponent.h"
#include "Handle.h"

struct factory_handle;

template <class T>
struct c_handle;

class GameEntity
{
public:
	GameEntity(const pow2::Handle& handle = pow2::Handle());
	//GameEntity(const GameEntity& ge);
	//GameEntity& operator=(const GameEntity& ge);
	~GameEntity(void);

	unsigned int getId();

	template <typename T>
	GameEntity& addComponent(const c_handle<T>& component) {
		components[component.getType()] = std::shared_ptr<factory_handle>(new c_handle<T>(component));
		c_handle<GameEntity> wrapper(handle);
		//T* ptr = component.operator->();
		component->setParentEntity(wrapper);
		return (*this);
	}

	template <typename T>
	void removeComponent() {
		const type_info* componentType = &typeid(T);
		if (components.count(componentType) != 0) {
			//components[componentType]->deleteData();
			//delete components[componentType];
			components.erase(componentType);
		}
	}

	template <typename T>
	void removeAndDeleteComponent() {
		const type_info* componentType = &typeid(T);
		if (components.count(componentType) != 0) {
			components[componentType]->deleteData();
			//comp->deleteData();
			//delete comp;
			components.erase(componentType);
		}
	}

	void clearComponents();

	template <typename T>
	c_handle<T> getComponent() {
		const std::type_info* componentType = &typeid(T);
		if (components.count(componentType) != 0) {
			c_handle<T>* ptr = dynamic_cast<c_handle<T>*>(components[componentType].get());
			return *ptr;
		}
		else {
			return c_handle<T>();
		}
	}

	template <typename T, typename... Params>
	c_handle<T> addNewComponent(const Params & ... params) {
		c_handle<GameEntity> wraper(handle);
		c_handle<T>& component = GameObjectFactory::createComponent<T>(params...);
		c_handle<GameEntity> wrapper(handle);
		component->setParentEntity(wrapper);
		components[&typeid(T)] = std::shared_ptr<factory_handle>(new c_handle<T>(component));;
		return component;
	}

	template <typename T, typename... Params>
	c_handle<T> addNewComponent(Params&& ... params) {
		c_handle<GameEntity> wraper(handle);
		c_handle<T>& component = GameObjectFactory::createComponent<T>(std::forward<Params>(params)...);
		c_handle<GameEntity> wrapper(handle);
		component->setParentEntity(wrapper);
		components[&typeid(T)] = std::shared_ptr<factory_handle>(new c_handle<T>(component));
		return component;
	}

	template <typename T>
	c_handle<T> addNewComponent() {
		c_handle<GameEntity> wraper(handle);
		c_handle<T>& component = GameObjectFactory::createComponent<T>();
		c_handle<GameEntity> wrapper(handle);
		component->setParentEntity(wrapper);
		components[&typeid(T)] = std::shared_ptr<factory_handle>(new c_handle<T>(component));
		return component;
	}

	
	template <typename T>
	c_handle<T> addNewCopyOfComponent(const c_handle<T>& comp) {
		c_handle<GameEntity> wraper(handle);
		c_handle<T>& component = GameObjectFactory::createCopy(comp);
		c_handle<GameEntity> wrapper(handle);
		component->setParentEntity(wrapper);
		components[&typeid(T)] = std::shared_ptr<factory_handle>(new c_handle<T>(component));
		return component;
	}

	template <typename T>
	GameEntity& operator<<(const c_handle<T> component) {
		components[component.getType()] = std::shared_ptr<factory_handle>(new c_handle<T>(component));
		c_handle<GameEntity> wrapper(handle);
		component->setParentEntity(wrapper);
		return (*this);
	}

private:
	pow2::Handle handle;
	typedef std::unordered_map<const std::type_info* , std::shared_ptr<factory_handle>> HandleMap ;
	HandleMap components;
};

#endif