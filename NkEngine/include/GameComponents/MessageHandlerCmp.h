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

#ifndef MESSAGE_HANDLER_CMP
#define MESSAGE_HANDLER_CMP

#pragma once

#include "GameComponent.h"
#include "MessageHandler.h"

struct Callback
{
public:
	Callback() :
		function(),
		signature() {
	}

	Callback(const std::shared_ptr<void>& function, const std::type_info* signature) :
		function(function),
		signature(signature) {
	}

	std::shared_ptr<void>  function;
	const std::type_info* signature;
};

template <class SendByType>
class MessageHandlerCmp : public GameComponent {
public:

	MessageHandlerCmp() : GameComponent() {}
	MessageHandlerCmp(std::function<unsigned long(const SendByType&)> hashFunc, std::function<bool(const SendByType&, const SendByType&)> equalFunc) :
		GameComponent(),
		mCallbacks(0, hashFunc, equalFunc) {}
	~MessageHandlerCmp() {}
	virtual void recieve(const Message& msg){};

	template <typename FuncType>
	void registerCallback(const SendByType& msgType, const FuncType& callback) {
		typedef function_traits<FuncType>::function FType;
		std::shared_ptr<void> callbackPtr((void *)(new FType(callback)));
		this->mCallbacks[msgType] = Callback(callbackPtr, &typeid(FType));
		//this->mCallbacks[msgType].function = callbackPtr;
		//this->mCallbacks[msgType].signature = &typeid(FuncType);)
	}



	template <class Ret = void, class FuncType, class ...Args>
	Ret callHandler(const SendByType& msgType, Args&& ...params) {
		//typedef std::function<Ret(Args...)> FuncType;		
		auto it = this->mCallbacks.find(msgType);
		if (it != this->mCallbacks.end()) {
			Callback callback = it->second;
			const std::type_info* signature = &typeid(FuncType);
			if (*(callback.signature) != typeid(FuncType))
			{
				throw std::bad_typeid("Callback signature does not match called SendMessage parameters");
			}
			FuncType* func = static_cast<FuncType*>(callback.function.get());
			return (*func)(std::forward<Args>(params)...);
		} 
		return Ret();
	}

	template <class Ret = void, class ...Args>
	Ret callHandlerWithSelf(const c_handle<MessageHandlerCmp>& self, SendByType msgType, Args&& ...params) {
		typedef std::function<Ret(const c_handle<GameEntity>&, Args...)> FuncType;
		auto it = this->mCallbacks.find(msgType);
		if (it != this->mCallbacks.end()) {
			Callback callback = it->second;
			if (*(callback.signature) != typeid(FuncType))
			{
				throw std::bad_typeid("Callback signature does not match called SendMessage parameters");
			}
			FuncType* func = static_cast<FuncType*>(callback.function.get());
			return (*func)(self, params...);
		}
		return Ret();
	}

	bool getCallback(const SendByType& msgType, Callback& callback) {
		auto it = this->mCallbacks.find(msgType);
		if (it != this->mCallbacks.end()) {
			callback = it->second;
			return true;
		}
		return false;
	}

	

private:
	
	std::unordered_map<SendByType, Callback> mCallbacks;
};

#endif