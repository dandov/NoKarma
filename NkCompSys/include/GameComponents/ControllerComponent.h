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

#ifndef CONTROLLER_COMPONENT_H
#define CONTROLLER_COMPONENT_H
#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <utility>
#include <memory>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp> // Libreria de Matrices
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "GameComponent.h"
#include "RigidBodyComponent.h"
#include "utils.h"
class ControllerComponent :
	public GameComponent
{
public:

	ControllerComponent(btDynamicsWorld* dynamicsWorld, btScalar mass,
		const std::shared_ptr<btCollisionShape>& collisionShape, 
		const btTransform& capsuleTransform, const glm::vec3& maxPoints,
		const glm::vec3& minPoints,
		const btTransform& initialTransform = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	ControllerComponent(btDynamicsWorld* dynamicsWorld, btScalar mass,
		const std::shared_ptr<btCollisionShape>& collisionShape,
		const btTransform& capsuleTransform, const glm::vec3& maxPoints,
		const glm::vec3& minPoints,
		c_handle<TransformComponent> initialTransform,
		const btTransform& physicsTransformOffset = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	~ControllerComponent(void);

	btMotionState* getMotionState() {
		return motionState.get();
	}

	btRigidBody* getBulletRigidBody() {
		return body.get();
	}

	btRigidBody* getCapsuleRigidBody(){
		return capsuleBody.get();
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

	void setVelocity(const btVector3& vel);

	void setAngularVelocity(const btVector3& angVel);

	void setAngularFactor(const btVector3& angFac);

	void setAngularFactor(const btScalar& angFac);

	void removeFromDynamicsWorld();


	void rotateFromPoint(const btVector3& refPoint, const btVector3& axis, float degreesAngle) {
		btTransform mainBodyTransform;
		btTransform capsuleBodyTransform;
		this->motionState->getWorldTransform(mainBodyTransform);
		this->capsuleMotionState->getWorldTransform(capsuleBodyTransform);

		this->orientation = mainBodyTransform.getRotation();

		auto centerOfMass = mainBodyTransform.getOrigin();
		glm::vec3 pos(centerOfMass.x(), centerOfMass.y(), centerOfMass.z());
		glm::quat orient = bullet_utils::btQuat2GLM(this->orientation);
		glm::vec3 point(refPoint.x() + centerOfMass.x(), refPoint.y() + centerOfMass.y(), refPoint.z() + centerOfMass.z());

		glm::vec3 diff(point - pos);

		glm::quat newRot = glm::angleAxis(degreesAngle, glm::vec3(axis.x(), axis.y(), axis.z()));
		orient = orient * newRot;

		glm::vec3 rotated = orient * diff * glm::inverse(orient) + pos;




		mainBodyTransform.setOrigin(btVector3(rotated.x, rotated.y, rotated.z));
		//this->orientation = mainBodyTransform.getRotation();
		//btQuaternion newRot(axis, radiansAngle);
		//orientation *= newRot;

		//this->motionState->getWorldTransform(mainBodyTransform);

		//transform2.setRotation(btQuaternion(rotation1.x, rotation1.y, rotation1.z, rotation1.w));
		mainBodyTransform.setRotation(btQuaternion(orient.x, orient.y, orient.z, orient.w));


		body->setCenterOfMassTransform(mainBodyTransform);
	}

	glm::quat getMainBodyRotation() {

		btTransform mainBodyTransform;
		this->motionState->getWorldTransform(mainBodyTransform);

		auto orient = mainBodyTransform.getRotation();
		return glm::quat(orient.w(), orient.x(), orient.y(), orient.z());
	}

	glm::vec3 getMainBodyPosition() {

		btTransform mainBodyTransform;
		this->motionState->getWorldTransform(mainBodyTransform);

		auto pos = mainBodyTransform.getOrigin();
		return glm::vec3(pos.x(), pos.y(), pos.z());
	}

	void setRotation(const glm::quat& rot) {
		btTransform mainBodyTransform;
		this->motionState->getWorldTransform(mainBodyTransform);
		mainBodyTransform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
		//body->setCenterOfMassTransform(mainBodyTransform);
		motionState->setWorldTransform(mainBodyTransform);
	}
	void removeConstraintFromDynamicsWorld();

	void addConstraintToDynamicsWorld() {
		btTransform mainBodyTransform;
		this->motionState->getWorldTransform(mainBodyTransform);
		btTransform capsuleBodyTransform;
		this->capsuleMotionState->getWorldTransform(capsuleBodyTransform);
		btPoint2PointConstraint* pointConstraint = static_cast<btPoint2PointConstraint*>(constraint.get());

		btVector3 mainOrigin = mainBodyTransform.getOrigin();
		btVector3 diff = mainBodyTransform.getOrigin() - capsuleBodyTransform.getOrigin();
		pointConstraint->setPivotB(btVector3(0, -diff.y(), 0));

		capsuleBodyTransform.setOrigin(btVector3(mainOrigin.x(), capsuleBodyTransform.getOrigin().y(), mainOrigin.z()));
		mDynamicsWorld->addConstraint(constraint.get(), false);
		detached = false;
	}

	void reAttatchToConstraint() {
		btTransform mainBodyTransform;
		this->motionState->getWorldTransform(mainBodyTransform);
		btTransform capsuleBodyTransform;
		this->capsuleMotionState->getWorldTransform(capsuleBodyTransform);
		btPoint2PointConstraint* pointConstraint = static_cast<btPoint2PointConstraint*>(constraint.get());

		btVector3 mainOrigin = mainBodyTransform.getOrigin();
		//btVector3 diff = mainBodyTransform.getOrigin() - capsuleBodyTransform.getOrigin();
		//pointConstraint->setPivotB(btVector3(0, -diff.y(), 0));

		capsuleBodyTransform.setOrigin(btVector3(mainOrigin.x(), capsuleBodyTransform.getOrigin().y(), mainOrigin.z()));
		mDynamicsWorld->addConstraint(constraint.get(), false);
		detached = false;
	}

	void stopBodies() {
		body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
		body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		body->clearForces();
		capsuleBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		capsuleBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		capsuleBody->clearForces();
	}

	void rotateController(const btVector3& axis, float radiansAngle);
	void rotateWithAngularVelocity(const btVector3& angularVelocity);

	void translate(const glm::vec3& direction, float speed);

	void setUserPointerEntity(c_handle<GameEntity> gameEntity);

	glm::quat getOrientation() {
		return bullet_utils::btQuat2GLM(orientation);
	}

	bool isMoveEnabled() {
		return mEnableMove;
	}

	bool isRotationEnabled() {
		return mEnableRotate;
	}

	void setEnableMove(bool enable) {
		mEnableMove = enable;
	}

	void setEnableRotation(bool enable) {
		mEnableRotate = enable;
	}

private:
	std::shared_ptr<btMotionState> motionState;
	std::shared_ptr<btCollisionShape> collisionShape;
	std::shared_ptr<btRigidBody> body;
	std::shared_ptr<btMotionState> capsuleMotionState;
	std::shared_ptr<btCollisionShape> capsuleCollisionShape;
	std::shared_ptr<btRigidBody> capsuleBody;
	btDynamicsWorld* mDynamicsWorld;
	std::shared_ptr<btTypedConstraint> constraint;
	btScalar capsuleRadius = 0.5;
	btScalar capsuleHeight;
	btQuaternion orientation;
	std::shared_ptr<c_handle<GameEntity>> mUserPtr;
	btVector3 constraintShapePivot;
	btVector3 constraintCapsulePivot;
	bool detached;

	bool mEnableRotate;
	bool mEnableMove;
};

#endif
