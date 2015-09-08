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

#ifndef RIGID_BODY_COMPONENT
#define RIGID_BODY_COMPONENT
#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <utility>
#include <memory>
#include <btBulletDynamicsCommon.h>

#include "NKMotionState.h"

#include "GameComponent.h"
class RigidBodyComponent :
	public GameComponent
{
public:
	RigidBodyComponent(btDynamicsWorld* dynamicsWorld, btScalar mass,
		const std::shared_ptr<btCollisionShape>& collisionShape,
		const btTransform& initialTransform = btTransform(btQuaternion(0,0,0,1),btVector3(0, 0, 0)));
	RigidBodyComponent(btDynamicsWorld* dynamicsWorld, btScalar mass,
		const std::shared_ptr<btCollisionShape>& collisionShape,
		short group, short collisionMask,
		const btTransform& initialTransform = btTransform(btQuaternion(0,0,0,1),btVector3(0, 0, 0)));


	RigidBodyComponent(btDynamicsWorld* dynamicsWorld, btScalar mass,
		const std::shared_ptr<btCollisionShape>& collisionShape,
		c_handle<TransformComponent> initialTransform,
		const btTransform& physicsTransformOffset = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	~RigidBodyComponent(void);

	btMotionState* getMotionState() {
		return motionState.get();
	}

	btRigidBody* getBulletRigidBody() {
		return body.get();
	}

	std::shared_ptr<btCollisionShape> getCollisionShapeShared() {
		return collisionShape;
	}

	btCollisionShape* getCollisonShape() {
		return collisionShape.get();
	}

	virtual void recieve(const Message& msg) {};

	btDynamicsWorld* getDynamicsWorld() {
		return mDynamicsWorld;
	}

	void setUserPointerEntity(c_handle<GameEntity> gameEntity);
	
	void removeFromDynamicsWorld();

private:
	std::shared_ptr<btMotionState> motionState;
	std::shared_ptr<btCollisionShape> collisionShape;
	std::shared_ptr<btRigidBody> body;
	btDynamicsWorld* mDynamicsWorld;
	std::shared_ptr<c_handle<GameEntity>> mUserPtr;

};

#endif
