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

#ifndef NODE_COMPONENT_H
#define NODE_COMPONENT_H

#pragma once
#include <unordered_set>
#include <queue>
#include "GameObjectFactory.h"


struct GameEntityHasher
{
public:
	static std::hash<int> hasher;
	std::size_t operator()(const c_handle<GameEntity>& entity) const
	{
		return hasher(entity.getIndex());
	}
};

struct GameEntityComparator
{
public:
	bool operator()(const c_handle<GameEntity>& entity1, const c_handle<GameEntity>& entity2) const
	{ 
		return (entity1.getIndex() >= entity2.getIndex());
	}
};

class NodeComponent : public GameComponent
{
public:
	std::unordered_set<c_handle<GameEntity>,GameEntityHasher,GameEntityComparator> children;
	c_handle<GameEntity> parent;

	NodeComponent();

	~NodeComponent(void){}

	bool insertChild(c_handle<GameEntity>& child);
	std::size_t eraseChild(c_handle<GameEntity>& child);

	void eraseMyself();
	void disconnectFromParent();
	void disconnectFromChildren();
	void disconnect();

	virtual void recieve(const Message& msg){}
};

class RootComponent : public NodeComponent
{
public:
	RootComponent() : NodeComponent(){}
	~RootComponent(void){}
};

#endif