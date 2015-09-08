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

#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <functional>
#include <cstdarg>
#include "GameSystem.h"
#include "Message.h" 
#include "SystemMessageSender.h"
#include "GameSystems/MessageHandlerSys.h"
#include "GameLoop.h"

template <typename KeyType>
class SystemManager : public SystemMessageSender
{
public:
	SystemManager() : 
		SystemMessageSender()
	{
	}

	virtual ~SystemManager() {

	}

	void addNewGameLoop(int priorityKey, GameLoop<KeyType>&& gameLoop) {
		gameLoop.setPriority(priorityKey);
		this->mGameLoopMap[priorityKey] = std::move(gameLoop);
	}

	void removeGameLoop(int priorityKey) {
		this->mGameLoopMap.erase(priorityKey);
	}

	void clearGameLoops() {
		this->mGameLoopMap.clear();
	}

	virtual void update(double elapsedTime) {
		for (auto it = this->mGameLoopMap.begin(); it != this->mGameLoopMap.end(); it++)
		{
			it->second.run(elapsedTime, this);
		}
	}

	virtual void sendSystemMessage(const c_handle<GameEntity>& target, const c_handle<GameEntity>& sender)  
	{

	}

private:
	typedef std::map < int, GameLoop<KeyType> >  GameLoopMap;
	GameLoopMap mGameLoopMap;
};

#endif
