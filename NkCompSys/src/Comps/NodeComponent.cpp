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

#pragma once
#include "GameComponents/NodeComponent.h"

std::hash<int> GameEntityHasher::hasher = std::hash<int>();

NodeComponent::NodeComponent():GameComponent()
{
	children = std::unordered_set<c_handle<GameEntity>,GameEntityHasher,GameEntityComparator>();
	parent = c_handle<GameEntity>();
}

bool NodeComponent::insertChild(c_handle<GameEntity>& child)
{
	c_handle<NodeComponent> childNode = child->getComponent<NodeComponent>();
	if(childNode){
		childNode->parent = getParentEntity();
		children.insert(child);
		return true;
	}
	else
	{
		c_handle<RootComponent> rootChild = child->getComponent<RootComponent>();
		c_handle<GameEntity> myself = getParentEntity();
		if(rootChild){
			c_handle<NodeComponent> nodeComp = GameObjectFactory::createComponent<NodeComponent>();
			nodeComp->children = rootChild->children;
			nodeComp->parent = myself;
			child->addComponent<NodeComponent>(nodeComp);
			c_handle<GameEntity> papa = nodeComp->getParentEntity();
			child->removeComponent<RootComponent>();
			children.insert(child);
			return true;
		}
	}
	return false;
}

std::size_t NodeComponent::eraseChild(c_handle<GameEntity>& child)
{
	return children.erase(child);
}

void NodeComponent::eraseMyself()
{
	c_handle<GameEntity> myself = getParentEntity();
	if(myself)
	{
		c_handle<NodeComponent> parentChildren = parent->getComponent<NodeComponent>();
		if(parentChildren)
		{
			parentChildren->eraseChild(myself);
			parent = c_handle<GameEntity>();

			std::queue<c_handle<GameEntity>> queue = std::queue<c_handle<GameEntity>>();

			for(auto& child : children)
			{
				queue.push(child);
			}
			GameObjectFactory::removeEntity(myself);

			while(!queue.empty())
			{
				c_handle<GameEntity> current = queue.front();
				queue.pop();
				c_handle<NodeComponent> node = current->getComponent<NodeComponent>();
				if(node)
				{
					for(auto& child : node->children)
					{
						queue.push(child);
					}
				}
				GameObjectFactory::removeEntity(current);
			}
		}
		else
		{
			c_handle<RootComponent> rootParent = parent->getComponent<RootComponent>();
			if(rootParent)
			{
				rootParent->eraseChild(myself);
				parent = c_handle<GameEntity>();

				std::queue<c_handle<GameEntity>> queue = std::queue<c_handle<GameEntity>>();

				for(auto& child : children)
				{
					queue.push(child);
				}
				GameObjectFactory::removeEntity(myself);

				while(!queue.empty())
				{
					c_handle<GameEntity> current = queue.front();
					queue.pop();
					c_handle<NodeComponent> node = current->getComponent<NodeComponent>();
					if(node)
					{
						for(auto& child : node->children)
						{
							queue.push(child);
						}
					}
					GameObjectFactory::removeEntity(current);
				}
			}
		}
	}
	
}

void NodeComponent::disconnectFromParent()
{
	c_handle<GameEntity> myself = getParentEntity();
	c_handle<NodeComponent> parentChildren = parent->getComponent<NodeComponent>();
	if(myself)
	{
		if(parentChildren)
		{
			parentChildren->eraseChild(myself);
			parent = c_handle<GameEntity>();
		}
		else
		{
			c_handle<RootComponent> rootParent = parent->getComponent<RootComponent>();
			if(rootParent)
			{
				rootParent->eraseChild(myself);
				parent = c_handle<GameEntity>();
			}
		}
		c_handle<RootComponent> rootComp = myself->addNewComponent<RootComponent>();
		rootComp->children = children;
		//el papa se settea invalido en el constructor de nodecomponent
		myself->removeComponent<NodeComponent>();
	}
}

void NodeComponent::disconnectFromChildren()
{
	for(auto& child : children)
	{
		c_handle<NodeComponent> node = child->getComponent<NodeComponent>();
		if(node)
		{
			node->parent=c_handle<GameEntity>();
		}
	}
	// se llaman sus destructores ¿funciona?
	children.clear();
}

void NodeComponent::disconnect()
{
	disconnectFromParent();
	disconnectFromChildren();
}