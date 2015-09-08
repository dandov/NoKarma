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

#include "GameComponents/CollisionCheckComponent.h"



CollisionCheckComponent::CollisionCheckComponent(
		btDynamicsWorld* dynamicsWorld,
		const std::shared_ptr<btCollisionShape>& collisionShape,
		const btTransform& initialTransform, short collisionFlags, MessageType collisionMessageType) :
		mDynamicsWorld(dynamicsWorld),
		collisionShape(collisionShape), group(btBroadphaseProxy::DefaultFilter), 
		collisionMask(btBroadphaseProxy::AllFilter), mCollisionMessageType(collisionMessageType)
{
	ghost = std::shared_ptr<btGhostObject>(new btPairCachingGhostObject());
	ghost->setCollisionShape(collisionShape.get());
	

	ghost->setCollisionFlags(ghost->getCollisionFlags() | collisionFlags);
	//ghost->setCollisionFlags(ghost->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);

	//ghost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	ghost->setWorldTransform(initialTransform);

	//RigidBodyComponent::dynamicsWorld->addRigidBody(ghost.get());
}

CollisionCheckComponent::CollisionCheckComponent(
		btDynamicsWorld* dynamicsWorld,
		const std::shared_ptr<btCollisionShape>& collisionShape,	
		const btTransform& initialTransform,
		short collisionFlags,
		short group, short collisionMask, MessageType collisionMessageType) :
		mDynamicsWorld(dynamicsWorld),
		collisionShape(collisionShape), group(group), collisionMask(collisionMask), 
		mCollisionMessageType(collisionMessageType)
{
	ghost = std::shared_ptr<btGhostObject>(new btPairCachingGhostObject());
	ghost->setCollisionShape(collisionShape.get());
	ghost->setCollisionFlags(collisionFlags);
	ghost->setWorldTransform(initialTransform);
}

void CollisionCheckComponent::updatePosition() {
	c_handle<ControllerComponent> ccHandle = parent->getComponent<ControllerComponent>();

	if (ccHandle) {
		btRigidBody* rigidBody = ccHandle->getBulletRigidBody();
		btTransform& transform = rigidBody->getWorldTransform();
		ghost->setWorldTransform(transform);
	}
}

void CollisionCheckComponent::addToDynamicsWorld(){
	this->mDynamicsWorld->addCollisionObject(ghost.get(), group, collisionMask);
}

void CollisionCheckComponent::removeFromDynamicsWorld(){
	this->mDynamicsWorld->removeCollisionObject(ghost.get());
}

void CollisionCheckComponent::checkCollisions(SystemMessageSender* messageSender) {
	btAlignedObjectArray<btCollisionObject*>& overlapping = ghost->getOverlappingPairs();
	const int total = overlapping.size();
	for (int i = 0; i < total; i++) {
		btCollisionObject* collided = overlapping[i];
		c_handle<GameEntity>* collidedEntity = static_cast<c_handle<GameEntity>*>(collided->getUserPointer());
		if (collidedEntity && *collidedEntity){
			messageSender->sendMessageTo(mCollisionMessageType, *collidedEntity, this->getParentEntity(), *collidedEntity);
		}
	}
}

std::vector<c_handle<GameEntity>> CollisionCheckComponent::getCollidingEntities(){
	btAlignedObjectArray<btCollisionObject*>& overlapping = ghost->getOverlappingPairs();
	const int total = overlapping.size();
	auto collidingEntities = std::vector<c_handle<GameEntity>>();
	for (int i = 0; i < total; i++) {
		btCollisionObject* collided = overlapping[i];
		c_handle<GameEntity>* collidedEntity = static_cast<c_handle<GameEntity>*>(collided->getUserPointer());
		if (collidedEntity && *collidedEntity){
			collidingEntities.push_back(*collidedEntity);
		}
	}
	return collidingEntities;
}


void CollisionCheckComponent::setUserPointerEntity(c_handle<GameEntity> gameEntity){
	mUserPtr = std::shared_ptr<c_handle<GameEntity>>(new c_handle<GameEntity>(gameEntity));
	ghost->setUserPointer(mUserPtr.get());
}