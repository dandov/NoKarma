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

#include "GameComponents/ControllerComponent.h"

ControllerComponent::ControllerComponent(btDynamicsWorld* dynamicsWorld, btScalar mass,
	const std::shared_ptr<btCollisionShape>& collisionShape,
	const btTransform& capsuleTransform, const glm::vec3& maxPoints,
	const glm::vec3& minPoints,
	const btTransform& initialTransform) :
	mDynamicsWorld(dynamicsWorld),
	motionState(new btDefaultMotionState(initialTransform)),
	capsuleMotionState(new btDefaultMotionState(capsuleTransform)),
	collisionShape(collisionShape),
	orientation(0, 0, 0, 1),
	mEnableMove(true),
	mEnableRotate(true)
{
	btVector3 inertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, inertia);
	body = std::shared_ptr<btRigidBody>(new btRigidBody(mass, motionState.get(), collisionShape.get(), inertia));

	capsuleHeight = 0.3f + maxPoints.x - minPoints.x;
	capsuleCollisionShape = std::shared_ptr<btCollisionShape>(new btCapsuleShape(capsuleRadius, capsuleHeight));
	capsuleCollisionShape->calculateLocalInertia(1, inertia);
	capsuleBody = std::shared_ptr<btRigidBody>(new btRigidBody(1, capsuleMotionState.get(), capsuleCollisionShape.get(), inertia));
	capsuleBody->setAngularFactor(0.0);
	capsuleBody->setSleepingThresholds(0.0, 0.0);
	capsuleBody->setFriction(2.0f);
	//capsuleBody->setWorldTransform(capsuleTransform);

	this->mDynamicsWorld->addRigidBody(capsuleBody.get());

	constraintShapePivot = btVector3((maxPoints.x - fabs(minPoints.x)) / 2, (maxPoints.y - fabs(minPoints.y)) / 2, (maxPoints.z - fabs(minPoints.z)) / 2);
	//btVector3 pivotShape((maxPoints.x - fabs(minPoints.x)) / 2, 0.0f, (maxPoints.z - fabs(minPoints.z)) / 2);
	//btVector3 pivotCapsule(capsuleRadius, 0, 0);
	constraintCapsulePivot = btVector3(0, 0, 0);
	

	constraint = std::shared_ptr<btTypedConstraint>(new btPoint2PointConstraint(*body, *capsuleBody, constraintShapePivot, constraintCapsulePivot));
	dynamicsWorld->addConstraint(constraint.get(), false);

	body->setAngularFactor(0.0);
	body->setSleepingThresholds(0.0, 0.0);
	body->setFriction(2.0f);

	this->mDynamicsWorld->addRigidBody(body.get());
}

ControllerComponent::ControllerComponent(btDynamicsWorld* dynamicsWorld, btScalar mass,
	const std::shared_ptr<btCollisionShape>& collisionShape,
	const btTransform& capsuleTransform, const glm::vec3& maxPoints,
	const glm::vec3& minPoints,
	c_handle<TransformComponent> initialTransform,
	const btTransform& physicsTransformOffset ) :
	mDynamicsWorld(dynamicsWorld),
	motionState(new NKMotionState(initialTransform, physicsTransformOffset)),
	capsuleMotionState(new btDefaultMotionState(capsuleTransform)),
	collisionShape(collisionShape),
	orientation(0, 0, 0, 1),
	mEnableMove(true),
	mEnableRotate(true)
{
	btVector3 inertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, inertia);
	body = std::shared_ptr<btRigidBody>(new btRigidBody(mass, motionState.get(), collisionShape.get(), inertia));

	capsuleHeight = 0.3f + maxPoints.x - minPoints.x;
	capsuleCollisionShape = std::shared_ptr<btCollisionShape>(new btCapsuleShape(capsuleRadius, capsuleHeight));
	capsuleCollisionShape->calculateLocalInertia(1, inertia);
	capsuleBody = std::shared_ptr<btRigidBody>(new btRigidBody(1, capsuleMotionState.get(), capsuleCollisionShape.get(), inertia));
	capsuleBody->setAngularFactor(0.0);
	capsuleBody->setSleepingThresholds(0.0, 0.0);
	capsuleBody->setFriction(2.0f);
	//capsuleBody->setWorldTransform(capsuleTransform);

	this->mDynamicsWorld->addRigidBody(capsuleBody.get());

	constraintShapePivot = btVector3((maxPoints.x - fabs(minPoints.x)) / 2, (maxPoints.y - fabs(minPoints.y)) / 2, (maxPoints.z - fabs(minPoints.z)) / 2);
	//btVector3 pivotShape((maxPoints.x - fabs(minPoints.x)) / 2, 0.0f, (maxPoints.z - fabs(minPoints.z)) / 2);
	//btVector3 pivotCapsule(capsuleRadius, 0, 0);
	constraintCapsulePivot = btVector3(0, 0, 0);


	constraint = std::shared_ptr<btTypedConstraint>(new btPoint2PointConstraint(*body, *capsuleBody, constraintShapePivot, constraintCapsulePivot));
	dynamicsWorld->addConstraint(constraint.get(), false);

	body->setAngularFactor(0.0);
	body->setSleepingThresholds(0.0, 0.0);
	body->setFriction(2.0f);

	this->mDynamicsWorld->addRigidBody(body.get());
}


void ControllerComponent::setVelocity(const btVector3& vel){
	if (mEnableRotate) {
		body->setLinearVelocity(vel);
	}
}

void ControllerComponent::setAngularVelocity(const btVector3& angVel){
	if (mEnableRotate) {
		body->setAngularVelocity(angVel);
	}
}

void ControllerComponent::setAngularFactor(const btVector3& angFac){
	body->setAngularFactor(angFac);
}

void ControllerComponent::setAngularFactor(const btScalar& angFac){
	body->setAngularFactor(angFac);
}

void ControllerComponent::removeConstraintFromDynamicsWorld() {
	detached = true;
	this->mDynamicsWorld->removeConstraint(constraint.get());
}


void ControllerComponent::removeFromDynamicsWorld() {
	detached = true;
	this->mDynamicsWorld->removeConstraint(constraint.get());
	this->mDynamicsWorld->removeRigidBody(body.get());
	this->mDynamicsWorld->removeRigidBody(capsuleBody.get());
}

ControllerComponent::~ControllerComponent(void)
{
}

void ControllerComponent::rotateWithAngularVelocity(const btVector3& angularVelocity) {
	if (mEnableRotate) {
		btTransform mainBodyTransform;
		this->motionState->getWorldTransform(mainBodyTransform);

		this->orientation = mainBodyTransform.getRotation();

		body->setAngularVelocity(angularVelocity);
		//capsuleBody->setAngularVelocity(angularVelocity);
	}
}

// Recieves direction vector in local coordinate system
void ControllerComponent::translate(const glm::vec3& direction, float speed) {
	if (mEnableMove) {
		const btVector3& currentLinVel = capsuleBody->getLinearVelocity();

		glm::quat orient = bullet_utils::btQuat2GLM(orientation);

		glm::vec3 dir = orient * direction;
		glm::vec3 addVelocity;
		float length = glm::length(dir);
		if (length > 0) {
			addVelocity = (dir / length) * speed;
		}

		capsuleBody->setLinearVelocity(btVector3(addVelocity.x, currentLinVel.y() + addVelocity.y, addVelocity.z));
	}
}

void ControllerComponent::rotateController(const btVector3& axis, float radiansAngle) {
		btTransform mainBodyTransform;
		btTransform capsuleBodyTransform;
		this->motionState->getWorldTransform(mainBodyTransform);
		this->capsuleMotionState->getWorldTransform(capsuleBodyTransform);

		this->orientation = mainBodyTransform.getRotation();
		btQuaternion newRot(axis, radiansAngle);
		orientation *= newRot;

		//this->motionState->getWorldTransform(mainBodyTransform);

		//transform2.setRotation(btQuaternion(rotation1.x, rotation1.y, rotation1.z, rotation1.w));
		mainBodyTransform.setRotation(orientation);
		//capsuleBodyTransform.setRotation(orientation);

		body->setCenterOfMassTransform(mainBodyTransform);

		//capsuleBody->setCenterOfMassTransform(mainBodyTransform);
		//capsuleBody->setAngularVelocity(btVector3);
		//triangleController->getMotionState()->setWorldTransform(transform2);
}

void ControllerComponent::setUserPointerEntity(c_handle<GameEntity> gameEntity){
	mUserPtr = std::shared_ptr<c_handle<GameEntity>>(new c_handle<GameEntity>(gameEntity));
	body->setUserPointer(mUserPtr.get());
}
