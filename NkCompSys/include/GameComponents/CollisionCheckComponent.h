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

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "GameComponent.h"
#include "ControllerComponent.h"
#include "SystemMessageSender.h"

class CollisionCheckComponent :
	public GameComponent
{
public:
	CollisionCheckComponent(
		btDynamicsWorld* dynamicsWorld,
		const std::shared_ptr<btCollisionShape>& collisionShape,
		const btTransform& initialTransform, short collisionFlags, MessageType collisionMessageType);
	CollisionCheckComponent(
		btDynamicsWorld* dynamicsWorld,
		const std::shared_ptr<btCollisionShape>& collisionShape,
		const btTransform& initialTransform, short collisionFlags, short group, short collisionMask, MessageType collisionMessageType);

	btGhostObject* getGhostObject() {
		return ghost.get();
	}

	void updatePosition();

	btTransform& getWorldTransform() {
		return ghost->getWorldTransform();
	}

	void setTransform(const btTransform& transform) {
		ghost->setWorldTransform(transform);
	}

	void checkCollisions(SystemMessageSender* messageSender);

	std::shared_ptr<btCollisionShape> getCollisionShapeShared() {
		return collisionShape;
	}

	btCollisionShape* getCollisonShape() {
		return collisionShape.get();
	}

	void addToDynamicsWorld();

	void removeFromDynamicsWorld();

	void setUserPointerEntity(c_handle<GameEntity> gameEntity);

	std::vector<c_handle<GameEntity>> getCollidingEntities();

	virtual void recieve(const Message& msg) {}

	virtual ~CollisionCheckComponent(void) {}
private:
	std::shared_ptr<btCollisionShape> collisionShape;
	std::shared_ptr<btGhostObject> ghost;
	short group;
	short collisionMask;
	btDynamicsWorld* mDynamicsWorld;
	MessageType mCollisionMessageType;
	std::shared_ptr<c_handle<GameEntity>> mUserPtr;
};

