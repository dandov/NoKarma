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

#include "GameSystems/HealthSystem.h"

void HealthSystem::update(double time, SystemMessageSender* msgSender)
{
	std::vector<c_handle<BoundingSphereComponent>> spheres = GameObjectFactory::getComponentHandlesOfType<BoundingSphereComponent>();
	for(std::vector<c_handle<BoundingSphereComponent>>::iterator it = spheres.begin();it!=spheres.end();it++)
	{
		c_handle<HealthComponent> health_handle = GameObjectFactory::getComponentBrother<HealthComponent>(*it);
		if(health_handle)
		{
			//damage done by collision
			std::deque<c_handle<GameEntity>>& entitiesCollidedTo =(*it)->entitiesCollidedTo;
			while(entitiesCollidedTo.size())
			{
				c_handle<GameEntity> entity_handle = entitiesCollidedTo.front();
				if(entity_handle){
					c_handle<DamageComponent> damage_handle = entity_handle->getComponent<DamageComponent>();
					if(health_handle->health>0 && damage_handle && damage_handle->team != health_handle->team)
					{
						health_handle->health-=damage_handle->damage;
						c_handle<InputComponent> input_handle = GameObjectFactory::getComponentBrother<InputComponent>(*it);
						if(input_handle)
						{
							//std::cout << "Damage Done!! Player Health: " << health_handle->health << std::endl;
						}
					}
				}
				entitiesCollidedTo.pop_front();
			}

			//health increment/decrement done by time
			c_handle<TimerComponent> timer_handle = GameObjectFactory::getComponentBrother<TimerComponent>(*it);
			if(timer_handle && health_handle->incrementInterval>0)
			{
				double timeElapsed = timer_handle->timeElapsed;
				double healthinterval = health_handle->incrementInterval;
				while(timer_handle->timeElapsed>=health_handle->incrementInterval)
				{
					health_handle->health+=health_handle->healthIncrement;
					timer_handle->timeElapsed-=health_handle->incrementInterval;
				}
			}
			//kill if fall into the emptiness
 			c_handle<TransformComponent> transform_handle = GameObjectFactory::getComponentBrother<TransformComponent>(*it);
 			if(transform_handle && transform_handle->position.y<-25){
 				health_handle->health=0;
 			}
		}
	}

	//borrar cosas con vida 0
	std::vector<c_handle<HealthComponent>> healths = GameObjectFactory::getComponentHandlesOfType<HealthComponent>();
	for(std::vector<c_handle<HealthComponent>>::iterator it = healths.begin();it!=healths.end();it++)
	{
		if((*it)->health<=0){
			c_handle<InputComponent> input_handle = GameObjectFactory::getComponentBrother<InputComponent>(*it);
			if(input_handle){
				//std::cout << "GAME OVER" << std::endl;
				GameStateMachine::changeState(GAMEOVERSTATE);
				break;
			}
			else
			{
				c_handle<EnemyAIComponent> ai_handle = GameObjectFactory::getComponentBrother<EnemyAIComponent>(*it);
				if(ai_handle){
					GameObjectFactory::getComponentHandlesOfType<ScoreComponent>()[0]->score+=1;
				}
				GameObjectFactory::removeEntity((*it)->getParentEntity());
			}
		}
	}

}

void HealthSystem::init(void) 
{
}

void HealthSystem::send(const Message& msg)
{
}
