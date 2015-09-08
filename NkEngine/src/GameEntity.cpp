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

#include "GameEntity.h"

#include "GameObjectFactory.h"


GameEntity::GameEntity(const pow2::Handle& handle): handle(handle)
{
}

unsigned int GameEntity::getId() { 
	void* out;
	bool valid = HandleContainer::manager.Get(handle, out);
	return valid? (uint32)out: -1;
}


void GameEntity::clearComponents() {
	for(HandleMap::iterator it = components.begin(); it != components.end(); it++) {
		const std::type_info* t = (it->first);
		it->second->deleteData();
		//delete (it->second);
	}
	components.clear();
}


GameEntity::~GameEntity(void)
{

}
