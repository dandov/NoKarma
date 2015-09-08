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

#include "GameComponents/RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(btDynamicsWorld* dynamicsWorld, btScalar mass,
		const std::shared_ptr<btCollisionShape>& collisionShape,
		const btTransform& initialTransform) : 
		mDynamicsWorld(dynamicsWorld),
		motionState(new btDefaultMotionState(initialTransform)),
		collisionShape(collisionShape)
{
    btVector3 inertia(0,0,0);
	collisionShape->calculateLocalInertia(mass, inertia);
	body = std::shared_ptr<btRigidBody>(new btRigidBody(mass, motionState.get(), collisionShape.get(), inertia));
	this->mDynamicsWorld->addRigidBody(body.get());
}


RigidBodyComponent::RigidBodyComponent(btDynamicsWorld* dynamicsWorld, btScalar mass,
		const std::shared_ptr<btCollisionShape>& collisionShape,
		short group, short collisionMask,
		const btTransform& initialTransform) :
		mDynamicsWorld(dynamicsWorld),
		motionState(new btDefaultMotionState(initialTransform)),
		collisionShape(collisionShape)
{
	btVector3 inertia(0,0,0);
	collisionShape->calculateLocalInertia(mass, inertia);
	body = std::shared_ptr<btRigidBody>(new btRigidBody(mass, motionState.get(), collisionShape.get(), inertia));
	this->mDynamicsWorld->addRigidBody(body.get(), group, collisionMask);
}


RigidBodyComponent::RigidBodyComponent(btDynamicsWorld* dynamicsWorld, btScalar mass,
	const std::shared_ptr<btCollisionShape>& collisionShape,
	c_handle<TransformComponent> initialTransform,
	const btTransform& physicsTransformOffset) :
	mDynamicsWorld(dynamicsWorld),
	motionState(new NKMotionState(initialTransform, physicsTransformOffset)),
	collisionShape(collisionShape)
{
	btVector3 inertia(0, 0, 0);
	if (mass != 0.0f)
		collisionShape->calculateLocalInertia(mass, inertia);
	body = std::shared_ptr<btRigidBody>(new btRigidBody(mass, motionState.get(), collisionShape.get(), inertia));
	this->mDynamicsWorld->addRigidBody(body.get());
}

void RigidBodyComponent::removeFromDynamicsWorld() {
	this->mDynamicsWorld->removeRigidBody(body.get());
}

void RigidBodyComponent::setUserPointerEntity(c_handle<GameEntity> gameEntity){
	mUserPtr = std::shared_ptr<c_handle<GameEntity>>(new c_handle<GameEntity>(gameEntity));
	body->setUserPointer(mUserPtr.get());
}

RigidBodyComponent::~RigidBodyComponent(void)
{
}
