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

#ifndef NK_MOTION_STATE_H
#define NK_MOTION_STATE_H

#pragma once

#include <btBulletDynamicsCommon.h>

#include "GameComponents/TransformComponent.h"
#include "utils.h"

ATTRIBUTE_ALIGNED16(struct) NKMotionState : public btMotionState
{
public:

	c_handle<TransformComponent> mTransformCmp;
	btTransform mCenterOfMassOffset;

	BT_DECLARE_ALIGNED_ALLOCATOR();

	NKMotionState(c_handle<TransformComponent> transformCmp, const btTransform &centerOfMassOffset = btTransform::getIdentity())
		: btMotionState(),
		  mTransformCmp(transformCmp),
		  mCenterOfMassOffset(centerOfMassOffset)
	{
		if (mTransformCmp) {
			mTransformCmp->needsUpdate = false;
		}
	}

	virtual ~NKMotionState()
	{
	}

	void setTransformCmp(c_handle<TransformComponent> transformCmp)
	{
		this->mTransformCmp = transformCmp;
		mTransformCmp->needsUpdate = false;
	}

	virtual void getWorldTransform(btTransform &worldTransform) const
	{
		if (this->mTransformCmp) {
			TransformComponent& transform = this->mTransformCmp.get();
			worldTransform.setOrigin(btVector3(transform.position.x, transform.position.y, transform.position.z));
			worldTransform.setRotation(btQuaternion(transform.orientation.x, transform.orientation.y, transform.orientation.z, transform.orientation.w));
		}
	}

	virtual void setWorldTransform(const btTransform &worldTransform)
	{
		if (this->mTransformCmp) {
			TransformComponent& transform = this->mTransformCmp.get();
			btQuaternion btQuat = worldTransform.getRotation();
			transform.orientation = glm::quat(btQuat.w(), glm::vec3(btQuat.x(), btQuat.y(), btQuat.z()));
			const btVector3& origin = worldTransform.getOrigin();
			transform.position = glm::vec3(origin.x(), origin.y(), origin.z());
			const btTransform& finalTransform = worldTransform * mCenterOfMassOffset;
			transform.worldModelMat = bullet_utils::btTransform2GLM(finalTransform) * glm::scale(transform.scale);
		}
	}

};

#endif
