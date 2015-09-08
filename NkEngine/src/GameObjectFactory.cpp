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

#include "GameObjectFactory.h"

int GameObjectFactory::componentTypeCount = 0;
pow2::HandleManager HandleContainer::manager; 
//GameObjectFactory::HandlesMap GameObjectFactory::handlesMap;
//pow2::HandleManager GameObjectFactory::manager;

GameObjectFactory::GameObjectFactory(void)
{
}


GameObjectFactory::~GameObjectFactory(void)
{
}


c_handle<GameEntity>& GameObjectFactory::createGameEntity() {
	std::vector<GameEntity>& entities = GameObjectFactory::ComponentContainer<GameEntity>::getComponentsOfType();
	std::vector<c_handle<GameEntity>>& handles = GameObjectFactory::ComponentContainer<GameEntity>::getHandlesOfType();
	int index = entities.size();
	pow2::Handle handle = HandleContainer::manager.Add((void*)index, &typeid(GameEntity));
	entities.emplace_back(handle);
	//c_handle<GameEntity> wraper(handle);
	handles.emplace_back(handle);
	const int end = handles.size() -1;
	return handles[end];
}

std::vector<c_handle<GameEntity>>& GameObjectFactory::getAllEntities() {
	return GameObjectFactory::ComponentContainer<GameEntity>::getHandlesOfType();
}

void GameObjectFactory::removeEntity(c_handle<GameEntity>& handle) {
	handle->clearComponents();
	handle.deleteData();
}
