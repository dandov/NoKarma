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

#include <iostream>
#ifndef GAMESYSTEM_H
#define GAMESYSTEM_H

#pragma once

#include "GameComponent.h"
#include "Message.h"

class SystemMessageSender;

class GameSystem
{
public:
	GameSystem(void) {}
	virtual ~GameSystem(void) {}
	virtual void update(double time, SystemMessageSender* msgSender) = 0;
	virtual void init(void) = 0;
	virtual void sendMessage(const c_handle<GameEntity>& target, const MessageType message, const c_handle<GameEntity>& sender) {
		std::cout << " sent message of type " << int(message) << std::endl;
	}
};

#endif