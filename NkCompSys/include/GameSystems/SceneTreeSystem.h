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
#include <GL/glew.h>
#include <vector>
#define GLM_SWIZZLE GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/component_wise.hpp>

#define _USE_MATH_DEFINES
#include <cmath>

#include "../utils.h"
#include "GameSystem.h"
#include "GameObjectFactory.h"
#include "GameComponents/NodeComponent.h"
#include "GameComponents/TransformComponent.h"
#include "GameComponents/RigidBodyComponent.h"
#include "GameComponents/ControllerComponent.h"
#include "GameComponents/CollisionCheckComponent.h"

class SceneTreeSystem :
	public GameSystem
{
public:

	SceneTreeSystem(void);
	virtual ~SceneTreeSystem(void);
	virtual void update(double time, SystemMessageSender* msgSender);
	virtual void init(void);
	virtual void send(const Message& msg);
	
};