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


#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#pragma once

#include <iostream>
#include <type_traits>
#include "GameEntity.h"
#include "Handle.h"
#include "c_handle.h"

//#include "GameObjectFactory.h"

//class GameObjectFactory;

struct Message
{
	int id;
	void *data;
};

struct InputMsg : public Message
{

};


/*struct c_handle {
public:
	c_handle(pow2::Handle& handle = pow2::Handle()) : handle(handle) { }
	pow2::Handle handle;
	template <typename T>
	T& get() { return GameObjectFactory::getComponent<T>(*this);}
	template <typename T>
	T& get() const{ return GameObjectFactory::getComponent<T>(*this);}
	bool operator!() const{
		return !(HandleContainer::manager.Get(handle));
	}
	explicit operator bool() const {
		return HandleContainer::manager.Get(handle);
	}
	const std::type_info* getType() const{ return handle.m_type; }
	const std::type_info* getType() { return handle.m_type; }
	const unsigned int getIndex() const{ return handle.m_index; }
	const unsigned int getIndex() { return handle.m_index; }
	
};*/


class GameComponent
{
public:
	GameComponent() {}
	virtual ~GameComponent(void) {}


	c_handle<GameEntity> getParentEntity() {return parent; }
	c_handle<GameEntity> getParentEntity() const {return parent; }
	void setParentEntity(c_handle<GameEntity>& parent) { this->parent = parent; }
protected:
	c_handle<GameEntity> parent;
};


#endif