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

#ifndef MESSAGE_HANDLER_SYS_H
#define MESSAGE_HANDLER_SYS_H
#pragma once

#include <unordered_set>
#include "GameSystem.h"
#include "GameComponents/MessageHandlerCmp.h"

template <class SendByType>
class MessageHandlerSys
	: public GameSystem
{
public:
	typedef c_handle<MessageHandlerCmp<SendByType>> HandlerCmp;

	typedef std::unordered_set < HandlerCmp,
		std::function<unsigned long(const HandlerCmp&) >> ComponentSet;

	MessageHandlerSys() : GameSystem() {
		this->mEntityHashFun = [](const HandlerCmp& msgHandler) {
			return std::hash<decltype(msgHandler.handle.m_index)>()(msgHandler.handle.m_index);
		};
		//this->mEntityHashFun = std::function<unsigned long(const HandlerCmp&)>(func);
	}

	virtual ~MessageHandlerSys() { }

	MessageHandlerSys(std::function<unsigned long(const SendByType&)> hashFunc,
		std::function<bool(const SendByType&, const SendByType&)> equalFunc) : 
		GameSystem(),
		mComponents(0, hashFunc, equalFunc) {
		this->mEntityHashFun = [](const HandlerCmp& msgHandler) {
			return std::hash<decltype(msgHandler.handle.m_index)>()(msgHandler.handle.m_index);
		};
	}
	virtual void update(double time, SystemMessageSender* msgSender) { }
	virtual void init(void) { }

	void registerComponent(const SendByType& msgType, const HandlerCmp& msgHandlerCmp) {
		auto it = this->mComponents.find(msgType);
		if (it == this->mComponents.end()) {
			ComponentSet componentSet(0, mEntityHashFun);
			auto result = this->mComponents.insert(std::make_pair(msgType, componentSet));
			it = result.first;
		}
		it->second.insert(msgHandlerCmp);
	}

	template <typename FuncType>
	void registerCallback(const SendByType& msgType, const HandlerCmp& msgHandlerCmp, const FuncType& callback) {
		msgHandlerCmp->registerCallback(msgType, callback);
		auto it = this->mComponents.find(msgType);
		if (it == this->mComponents.end()) {
			ComponentSet componentSet(0, mEntityHashFun);
			auto result = this->mComponents.insert(std::make_pair(msgType, componentSet));
			it = result.first;
		}
		it->second.insert(msgHandlerCmp);
	}

	template <class Ret, class ...Args>
	std::vector<Ret> onHandleMessageReturn(const SendByType& msgType, Args&& ...params) {
		std::vector<Ret> result;
		const ComponentSet& callBackVector = this->mComponents[msgType];
		for (const auto& it: callBackVector) {
			result.push_back(it->callHandler(msgType, params...));
		}
		return result;
	}

	template <class FuncType, class ...Args>
	void onHandleMessage(const SendByType& msgType, Args&& ...params) {
		//typedef std::function<void(Args...)> FuncType;
		const ComponentSet& callBackVector = this->mComponents[msgType];
		for (const auto& it: callBackVector) {
			it->callHandler<void, FuncType>(msgType, std::forward<Args>(params)...);
		}
	}

	template <class ...Args>
	void onHandleMessageSelf(const SendByType& msgType, Args&& ...params) {
		typedef std::function<void(Args...)> FuncType;
		const ComponentSet& callBackVector = this->mComponents[msgType];
		for (const auto& it: callBackVector) {
			it->callHandlerWithSelf((*it), msgType, params...);
		}
	}

	ComponentSet& getCallbackVector(const SendByType& msgType) {
		return this->mComponents[msgType];
	}

private:
	
	typedef std::unordered_map<SendByType, ComponentSet> MsgComponentsMap;
	std::function<unsigned long(const HandlerCmp&)> mEntityHashFun;
	MsgComponentsMap mComponents;
};

#endif