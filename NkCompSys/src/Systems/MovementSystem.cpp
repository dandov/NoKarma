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

#include "GameSystems/MovementSystem.h"

void MovementSystem::update(double time, SystemMessageSender* msgSender)
{
	std::vector<c_handle<VelocityComponent>>& speeds = GameObjectFactory::getComponentHandlesOfType<VelocityComponent>();
	for(std::vector<c_handle<VelocityComponent>>::iterator it = speeds.begin(); it!=speeds.end();it++){
		VelocityComponent& speed = it->get();
		c_handle<TransformComponent> transform_handle = GameObjectFactory::getComponentBrother<TransformComponent>(*it);
		if(transform_handle)
		{
			glm::vec3 tempspeed = speed.velocity;
			//tempspeed*=time;
			if(speed.applyGravity){
				tempspeed.y += -0.19;
			}
			transform_handle->position+=tempspeed;
		}
		
	}
}

void MovementSystem::init(void) 
{
}

void MovementSystem::send(const Message& msg)
{
}