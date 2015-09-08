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

#include "GameComponents/CameraComponent.h"


CameraComponent::CameraComponent(const glm::vec3& eye,
								const glm::vec3& fw,
								const glm::vec3& up) : GameComponent(), eye(eye), fw(fw), up(up),
	width(800), height(600), flag(false) 
{

}

glm::mat4 CameraComponent::getViewMatrix(const c_handle<CameraComponent>& myHandle){
	c_handle<TransformComponent> transform_handle = GameObjectFactory::getComponentBrother<TransformComponent>(myHandle);
	c_handle<RigidBodyComponent> rb_handle = GameObjectFactory::getComponentBrother<RigidBodyComponent>(myHandle);
	if(transform_handle){
		
		/*glm::quat q = transform_handle->orientation;
		glm::vec3 rotEuler = glm::eulerAngles(transform_handle->orientation);

		//rotEuler = -rotEuler;
		//rotEuler.y = glm::radians(rotEuler.y);

		btTransform physicsTranform;
		btRigidBody* physicsBody = NULL;
		glm::vec3 position = transform_handle->position;

		if (rb_handle) {
			rb_handle->getMotionState()->getWorldTransform(physicsTranform);
			physicsBody = rb_handle->getBulletRigidBody();
			btVector3& physPos = physicsTranform.getOrigin();
			position += glm::vec3(physPos.x(), physPos.y(), physPos.z());
		}

		float c1 = sin(rotEuler.y);
		float c2 = cos(rotEuler.y);
		return glm::lookAt(position, 
			glm::vec3(position.x+(sin(rotEuler.y)),position.y,position.z+(cos(rotEuler.y))),
			glm::vec3(0.0f,1.0f,0.0f));*/

		btTransform physicsTranform;
		btRigidBody* physicsBody = NULL;
		glm::vec3 position = transform_handle->position;
		glm::vec3 physPosition;

		if (rb_handle) {
			rb_handle->getMotionState()->getWorldTransform(physicsTranform);
			physicsBody = rb_handle->getBulletRigidBody();
			btVector3& physPos = physicsTranform.getOrigin();
			physPosition = glm::vec3(physPos.x(), physPos.y(), physPos.z());
		}

		//eye = position + physPosition;

	
		glm::mat4 viewMat =  glm::transpose(glm::mat4_cast(transform_handle->orientation)) * glm::translate(-eye);

		//viewMat[1] = glm::vec4(0, 1, 0, 1);
		return viewMat;

	}
	else{
		return glm::mat4();
	}

}

void  CameraComponent::rotateView(float angle, const glm::vec3& axis) {

	glm::quat qrot = glm::angleAxis(angle, axis);
	fw = qrot * fw;
}


void CameraComponent::rotateX(float const & angle) {
	glm::vec3 axis = glm::normalize(glm::cross(fw, up));
	glm::quat qrot = glm::angleAxis(angle, axis);

	up = qrot * up;
	fw = qrot * fw;
}

void CameraComponent::rotateY(float const & angle) {
	glm::quat qrot = glm::angleAxis(angle, up);
	fw = qrot * fw;
}


void CameraComponent::moveFw(const float& dist) {
	eye += dist*fw;
}
	
void CameraComponent::moveSide(const float& dist) {
	fw = glm::normalize(fw);
	up = glm::normalize(up);
	glm::vec3 side =  glm::cross(fw, up);
	eye += dist*side;
}
	
void CameraComponent::setPerspectiveMatrix(float fieldOfView, float screenRatio, float zNear, float zFar)
{
	projectionMatrix = glm::perspective(fieldOfView,screenRatio, zNear, zFar);		
		
}

void CameraComponent::setOrthoMatrix(float left, float right, float bottom, float top)
{
	projectionMatrix = glm::ortho(left,right,bottom,top);
}

