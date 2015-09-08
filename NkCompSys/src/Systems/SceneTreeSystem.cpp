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
#include "GameSystems/SceneTreeSystem.h"


SceneTreeSystem::SceneTreeSystem(void){}
SceneTreeSystem::~SceneTreeSystem(void){}

void SceneTreeSystem::update(double time, SystemMessageSender* msgSender)
{
	std::vector<c_handle<NodeComponent>> nodes = GameObjectFactory::getComponentHandlesOfType<NodeComponent>();
	for(c_handle<NodeComponent>& node : nodes)
	{
		//no tiene papa? == es root?
		c_handle<GameEntity> entity = node->getParentEntity();
		if(!node->parent && entity)
		{
			c_handle<TransformComponent> transformHandle = GameObjectFactory::getComponentBrother<TransformComponent>(node);
			if(transformHandle)
			{
				TransformComponent& transform=transformHandle.get();


				//transform.worldModelMat *= glm::scale(transform.scale);

				/*c_handle<RigidBodyComponent> rigidBHandle = entity->getComponent<RigidBodyComponent>();
				c_handle<ControllerComponent> controllerCHandle = entity->getComponent<ControllerComponent>();
				c_handle<CollisionCheckComponent> collisionHandle = entity->getComponent<CollisionCheckComponent>();*/
				
				//if (!rigidBHandle && !controllerCHandle && !collisionHandle) {
				//transform.worldModelMat *= glm::scale(transform.scale);

				if (transform.needsUpdate) {
					transform.worldModelMat = glm::translate(transform.position) * glm::mat4_cast(transform.orientation) * glm::scale(transform.scale);
				}

				/*	transform.worldModelMat = 
						glm::translate(transform.position) * glm::mat4_cast(transform.orientation) * glm::scale(transform.scale);*/
				/*}
				else{
					btTransform worldTransform;
					if (rigidBHandle) {
						rigidBHandle->getMotionState()->getWorldTransform(worldTransform);
					}
					else if (controllerCHandle) {
						controllerCHandle->getMotionState()->getWorldTransform(worldTransform);
					}
					else if (collisionHandle){
						worldTransform = collisionHandle->getWorldTransform();
					}
					glm::quat physQuat = bullet_utils::btQuat2GLM(worldTransform.getRotation());
					btVector3& vecT = worldTransform.getOrigin();
					glm::vec3 physTranslate(vecT.x(), vecT.y(), vecT.z());
					transform.worldModelMat = glm::translate(transform.position + physTranslate) *
						glm::mat4_cast(transform.orientation * physQuat)*
						glm::scale(transform.scale);
				}	*/			

				std::queue<c_handle<GameEntity>> queue = std::queue<c_handle<GameEntity>>();
				for(auto& child : node->children)
				{
					queue.push(child);
				}

				while(!queue.empty())
				{
					c_handle<GameEntity> current = queue.front();
					queue.pop();
					c_handle<NodeComponent> node = current->getComponent<NodeComponent>();
					if(node)
					{
						c_handle<GameEntity> parentEntity = node->parent;
						c_handle<TransformComponent> parentTransform = parentEntity->getComponent<TransformComponent>();
						if(parentTransform)
						{
							c_handle<TransformComponent> myTHandle = current->getComponent<TransformComponent>();
							c_handle<RigidBodyComponent> myRBHandle = current->getComponent<RigidBodyComponent>();

							if(myTHandle){
								TransformComponent& currentTransform=myTHandle.get();

								//if (!myRBHandle) {
								if (currentTransform.needsUpdate) {
									currentTransform.worldModelMat = glm::translate(currentTransform.position) * glm::mat4_cast(currentTransform.orientation) * glm::scale(currentTransform.scale);
								}


								currentTransform.worldModelMat = parentTransform->worldModelMat * currentTransform.worldModelMat;



								//currentTransform.worldModelMat *= glm::scale(currentTransform.scale);
								/*}
								else {
									btTransform rbTransform;
									myRBHandle->getMotionState()->getWorldTransform(rbTransform);
									glm::quat physQuat = bullet_utils::btQuat2GLM(rbTransform.getRotation());
									btVector3& vecT = rbTransform.getOrigin();
									glm::vec3 physTranslate(vecT.x(), vecT.y(), vecT.z());

									currentTransform.worldModelMat = parentTransform->worldModelMat * 
																	glm::translate(currentTransform.position+physTranslate) *
																	glm::mat4_cast(physQuat*currentTransform.orientation) *
																	glm::scale(currentTransform.scale);
								}*/

								//como el actual si tiene render entonces le puedo calcular las matrices a sus hijos
								for(auto& child : node->children)
								{
									queue.push(child);
								}
							}
						}
					}
				}
			}
		}
	}
}

void SceneTreeSystem::init(void)
{

}

void SceneTreeSystem::send(const Message& msg)
{

}