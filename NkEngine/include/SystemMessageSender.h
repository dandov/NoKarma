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

#ifndef MESSAGE_SENDER_H
#define MESSAGE_SENDER_H

#pragma once

#include <unordered_map>
#include <map>

#include "GameSystem.h"
#include "GameSystems\MessageHandlerSys.h"

class SystemMessageSender
{
public:
	SystemMessageSender() {

	}

	virtual ~SystemMessageSender() {

	}

	virtual void sendSystemMessage(const c_handle<GameEntity>& target, const c_handle<GameEntity>& sender) = 0;

	template <class T>
	std::shared_ptr<T> getSystem() {
		const std::type_info* type = &typeid(T);
		auto it = this->mSystemByType.find(type);
		if (it != this->mSystemByType.end()) {
			return std::static_pointer_cast<T>(it->second);
		}
		return std::shared_ptr<T>();
	}

	// En esta version todos los argumentos que sean variables se pasan como referencia, entonces tiene que corresponder con la
	// firma de la llamada original, los valores inmediatos por ejemplo 42 o un regreso de una funcion int foo() mandados
	// asi: int x; sendMessageRef(message, foo(), x); foo() se manda por copia como rvalue y x por referencia entonces firma
	// de la callback debe de ser (int, int&)
	template <typename SendByType, typename ...Args>
	void sendMessageRef(const SendByType& message, Args&& ...params) {
		typedef std::function<void(Args...)> FuncType;
		std::shared_ptr<MessageHandlerSys<SendByType>> messageHandler = this->getSystem<MessageHandlerSys<SendByType>>();
		if (messageHandler) {
			messageHandler->onHandleMessage<FuncType>(message, std::forward<Args>(params)...);
		}
	}

	// Aqui se reciben los argumentos solo dependiendo de su tipo, si se quiere enviar por referencia o por referencia const
	// se debe de llamar a la funcion especificando los tipos templeteados explicitamente por ejemplo, si int x; funcion int foo();:
	// sendMessage<MessageType, int, int&>(message, foo(), x);
	template <typename SendByType, typename ...Args>
	void sendMessage(const SendByType& message, Args ...params) {
		typedef std::function<void(Args...)> FuncType;
		std::shared_ptr<MessageHandlerSys<SendByType>> messageHandler = this->getSystem<MessageHandlerSys<SendByType>>();

		if (messageHandler) {
			MessageHandlerSys<SendByType>::ComponentSet& callBackVector = messageHandler->getCallbackVector(message);
			for (const auto& it : callBackVector) {
				Callback callback;
				if (it->getCallback(message, callback)) {
					auto signature = &typeid(FuncType);
					if (*(callback.signature) != typeid(FuncType)){
						throw std::bad_typeid("Callback signature does not match called SendMessage parameters");
					}
					FuncType* func = static_cast<FuncType*>(callback.function.get());
					(*func)(std::forward<Args>(params)...);
				}
			}
		}
	}

	template <typename SendByType, typename ...Args>
	void sendMessageTo(const SendByType& message, c_handle<GameEntity> entity, Args ...params) {
		typedef std::function<void(Args...)> FuncType;

		c_handle<MessageHandlerCmp<SendByType>> msgHandlerCmp = entity->getComponent<MessageHandlerCmp<SendByType>>();
		if (msgHandlerCmp) {
			Callback callback;
			if (msgHandlerCmp->getCallback(message, callback)) {
				auto signature = &typeid(FuncType);
				if (*(callback.signature) != typeid(FuncType)){
					throw std::bad_typeid("Callback signature does not match called SendMessage parameters");
				}
				FuncType* func = static_cast<FuncType*>(callback.function.get());
				(*func)(std::forward<Args>(params)...);
			}
		}
	}

	template <typename SendByType, typename ...Args>
	void sendMessageToRef(const SendByType& message, c_handle<GameEntity> entity, Args&& ...params) {
		typedef std::function<void(Args...)> FuncType;

		c_handle<MessageHandlerCmp<SendByType>> msgHandlerCmp = entity->getComponent<MessageHandlerCmp<SendByType>>();
		if (msgHandlerCmp) {
			msgHandlerCmp->callHandler<void, FuncType>(std::forward<Args>(params)...);
		}
	}

	template <typename SendByType, typename FuncType>
	void registerCallback(const SendByType& msgType, const c_handle<MessageHandlerCmp<SendByType>>& msgHandlerCmp, const FuncType& callback) {
		std::shared_ptr<MessageHandlerSys<SendByType>> messageHandler = this->getSystem<MessageHandlerSys<SendByType>>();
		if (messageHandler) {
			messageHandler->registerCallback(msgType, msgHandlerCmp, callback);
		}
	}

	void addSystem(const std::type_info* type, std::shared_ptr<GameSystem> system) {
		auto mapIt = this->mSystemByType.find(type);
		if (mapIt != this->mSystemByType.end()) {
			throw std::runtime_error("Adding system of type that already exists");
		}
		this->mSystemByType[type] = system;
	}

	void removeSystem(const std::type_info* type) {
		this->mSystemByType.erase(type);
	}

protected:
	typedef std::unordered_map<const std::type_info*, std::shared_ptr<GameSystem> > SystemUnorderedMap;
	SystemUnorderedMap mSystemByType;
};

#endif