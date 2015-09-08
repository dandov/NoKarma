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

#include "SystemManager.h"

/*template class SystemManager < int > ;

auto cmp = [](const int& a, const int& b) -> bool { return a < b; };

template <typename KeyType>
SystemManager<int> SystemManager<KeyType>::systemManager(cmp);*/

/*template <typename KeyType>
SystemManager<KeyType>::SystemManager(std::function<bool(const KeyType&, const KeyType&)> comparator) :
	mSystemMap(comparator), mComparator(comparator) 
{
}


template <typename KeyType>
SystemManager<KeyType>::~SystemManager()
{
}*/

/*template <typename KeyType>
template <class T, typename ...MT >
std::shared_ptr<GameSystem> SystemManager<KeyType>::addNewSystem(const KeyType& priorityKey, MT ... listenMessageTypes)
{
	std::shared_ptr<GameSystem> system = std::shared_ptr(new T);
	this->mSystemMap.insert(system);
	this->registerSystemToMessages(priorityKey, system, listenMessageTypes...);
	return system;
}

template <typename KeyType>
template <typename MT1, typename ...MT>
void SystemManager<KeyType>::registerSystemToMessages(const KeyType& priorityKey, std::shared_ptr<GameSystem> system, MT1 messgType1, MT ... restMessages)
{
	auto msgMapIt = this->mMessageRegisterMap.find(messgType1);
	if (msgMapIt == this->mMessageRegisterMap.end()) {
		std::pair<MessageType, SystemMap> entry(messgType1, SystemMap(this->mComparator));
		auto pair = this->mMessageRegisterMap.insert(entry);
		msgMapIt = pair->first;
	}
	msgMapIt->second[messgType1] = system;
	this->registerSystemToMessages(priorityKey, system, restMessages...);
}

template <typename KeyType>
void SystemManager<KeyType>::registerSystemToMessages(const KeyType& priorityKey, std::shared_ptr<GameSystem> system)
{
}
*/

/*template <typename KeyType>
void  SystemManager<KeyType>::sendSystemMessage(const c_handle<GameEntity>& target, const MessageType message, std::shared_ptr<MessageData> messageData, const c_handle<GameEntity>& sender) {
	auto mapIt = this->mMessageRegisterMap.find(message);
	if (mapIt != this->mMessageRegisterMap.end()) {
		for (auto it = mapIt->second.begin(); it != mapIt->second.end(); it++) {
			it->second->sendMessage(target, message, messageData, sender);
		}
	}
}

template <typename KeyType>
void SystemManager<KeyType>::update(double elapsedTime)
{
	for (auto it = this->mSystemMap.begin(); it != this->mSystemMap.end(); it++) 
	{
		it->second->update(elapsedTime);
	}
}*/

//template std::shared_ptr<GameSystem> SystemManager<int>::addNewSystem<RenderSystem>(const int&, MessageType ...);