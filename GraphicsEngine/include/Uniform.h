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

#ifndef UNIFORMS_H
#define UNIFORMS_H

#pragma once

#include <functional>
#include <memory>
#include <stdint.h>
#include <unordered_map>
#include <iostream>

typedef std::function<void(uint32_t)> UniformBind;

namespace UniformsFuncs {
	//const static std::function<std::tuple<std::string, float>(std::string, float, uint32_t)> set1f;
	const extern std::function<void(std::string, uint32_t, float)> set1f;
	const extern std::function<void(std::string, uint32_t, float, float)> set2f;
	const extern std::function<void(std::string, uint32_t, float, float, float)> set3f;
	const extern std::function<void(std::string, uint32_t, float, float, float, float)> set4f;
	const extern std::function<void(std::string, uint32_t, int, const float*)> set2fv;
	const extern std::function<void(std::string, uint32_t, int, const float*)> set3fv;
	const extern std::function<void(std::string, uint32_t, int, const float*)> set4fv;

	const extern std::function<void(std::string, uint32_t, int32_t)> set1i;
	const extern std::function<void(std::string, uint32_t, int32_t, int32_t)> set2i;
	const extern std::function<void(std::string, uint32_t, int32_t, int32_t, int32_t)> set3i;
	const extern
		std::function<void(std::string, uint32_t, int32_t, int32_t, int32_t, int32_t)> set4i;
	const extern std::function<void(std::string, uint32_t, int, const int32_t*)> set2iv;
	const extern std::function<void(std::string, uint32_t, int, const int32_t*)> set3iv;
	const extern std::function<void(std::string, uint32_t, int, const int32_t*)> set4iv;

	const extern std::function<void(std::string, uint32_t, uint32_t)> set1ui;
	const extern std::function<void(std::string, uint32_t, uint32_t, uint32_t)> set2ui;
	const extern std::function<void(std::string, uint32_t, uint32_t, uint32_t, uint32_t)> set3ui;
	const extern
		std::function<void(std::string, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)> set4ui;
	const extern std::function<void(std::string, uint32_t, int, const uint32_t*)> set2uiv;
	const extern std::function<void(std::string, uint32_t, int, const uint32_t*)> set3uiv;
	const extern std::function<void(std::string, uint32_t, int, const uint32_t*)> set4uiv;

	const extern std::function<void(std::string, uint32_t, int, bool, const float*)> setMat2fv;
	const extern std::function<void(std::string, uint32_t, int, bool, const float*)> setMat3fv;
	const extern std::function<void(std::string, uint32_t, int, bool, const float*)> setMat4fv;
	
	/* Faltan las matrices que no son cuadradas*/

	template<typename... Params> 
	UniformBind createUniform(std::function<void(std::string, uint32_t, Params...)> uniform,
		std::string name, Params... params) {
		return std::bind(uniform, name, std::placeholders::_1, params...);
	}

	template<typename... Params>
	void createUniform(UniformBind& ret, 
		std::function<void(std::string, uint32_t, Params...)> uniform, std::string name,
		Params... params) {
		ret = std::bind(uniform, name, std::placeholders::_1, params...);
		//ret = [uniform, name, params...](uint32_t shaderId) {
		//	auto mytuple = std::make_tuple(params...);
		//	uniform(name, shaderId, params...);
		//};
	}
};

// implementation details, users never invoke these directly
// NO ENTIENDO PORQUE NECESITA LA && RVALUE REF
namespace detail
{
	template <typename R, typename F, typename Tuple, bool Done, int Total, int... N>
	struct call_impl
	{
		static R call(F f, Tuple && t)
		{
			// lo que hace aqui es crear una secuencia de ints y despues unpack la tupla y llamar 
			// con eso la funcion
			return call_impl<R, F, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(f,
				std::forward<Tuple>(t));
		}
	};

	template <typename R, typename F, typename Tuple, int Total, int... N>
	struct call_impl<R, F, Tuple, true, Total, N...>
	{
		static R call(F f, Tuple && t)
		{
			return f(std::get<N>(std::forward<Tuple>(t))...);
		}
	};
}

// user invokes this
// F es el tipo de la funcion
// Tuple es el tipo de la tuple, que llega como referencia de rvalue. 
template <typename R, typename F, typename Tuple>
R callTuple(F f, Tuple && t)
{
	//ttype es el tipo de la tuple convertido a rvalue?
	typedef typename std::decay<Tuple>::type ttype;
	return detail::call_impl<R, F, Tuple, 0 == std::tuple_size<ttype>::value, std::tuple_size<ttype>::value>::call(f, std::forward<Tuple>(t));
}

/** 
 * Uniforms a las que se les puede asociar un valor para que despues sean bindeadas por el system.
 * Se necesita una clase padre para que podamos guardarlas en un mapa. 
 */

class GP {
public:
	/**
	 * la unica funcion que necesitamos que conozcan los apuntadores papa para que la puedan 
	 * bindear
	 */
	virtual UniformBind bind(std::string name) = 0;
	virtual void call(std::string name, uint32_t shaderId) = 0;
};

template<typename... T>
class GraphicProperty : public GP {

public:
	/**
	 * recibe la uniform que le corresponde
	 * y los parametro de esa funcion, que se guardan en una std::tuple
	 */
	GraphicProperty(std::function<void(std::string, uint32_t, T...)> function, T... params)
		: mFunction(function)
		, mParams(params...) { }

	/**
	 * hace bind de los valores y regresa la funcion que sube la uniform al gpu
	 * ahora solo espera por el id del shader
	 */
	UniformBind bind(std::string name) override {

		auto tuple = std::make_tuple(this->mFunction, name);
		auto tupleCat = std::tuple_cat(tuple, this->mParams);
			
		auto func = [](std::function<void(std::string, uint32_t, T...)> function, std::string name, 
			T... params) {
			return std::bind(function, name, std::placeholders::_1, params...);
		};

		return callTuple<UniformBind>(func, tupleCat);
	}

	/**
	 * sube la uniform al gpu, ejecutando la funcion con los miembros guardados
	 * y los parametros de esta funcion
	 */

	void call(std::string name, uint32_t shaderId) override {

		auto tuple = std::make_tuple(name, shaderId);
		auto tupleCat = std::tuple_cat(tuple, this->mParams);

		callTuple<void>(this->mFunction, tupleCat);
	}

	template<int N, typename type>
	type getParam() {
		return std::get<N>(this->mParams);
	}

	const std::tuple<T...> getTuple() {
		return this->mParams;
	}

private:
	// el tipo de uniform que se va a bindear
	std::function<void(std::string, uint32_t, T...)> mFunction;

	// los parametros variables de esa uniform
	std::tuple<T...> mParams;
};

class GPUProperty {

public:

	typedef std::unordered_map<std::string, std::shared_ptr<GP>> Map;
	typedef std::unordered_map<std::string, std::shared_ptr<GP>>::iterator iterator;
	typedef std::unordered_map<std::string, std::shared_ptr<GP>>::const_iterator const_iterator;

	GPUProperty()
	: mProperties() { }

	template<typename... T>
	void addProperty(std::string name, std::function<void(std::string, uint32_t, T...)> function,
		T... params) {
		this->mProperties[name] = std::make_shared<GraphicProperty<T...>>(function, params...);
	}

	template<typename... T>
	GraphicProperty<T...>* get(std::string name) {
		return reinterpret_cast<GraphicProperty<T...>*>(this->mProperties[name]);
	}

	void removeProperty(std::string name) {
		this->mProperties.erase(name);
	}

	void setProperties(uint32_t shaderId) {
		for (auto it = this->mProperties.begin(); it != this->mProperties.end(); it++) {
			it->second->call(it->first, shaderId);
		}
	}

	/**
	 * Para iterar el mapa
	 */
	iterator begin() {
		return this->mProperties.begin();
	}

	iterator end() {
		return this->mProperties.end();
	}

	const_iterator begin() const {
		return this->mProperties.begin();
	}

	const_iterator end() const {
		return this->mProperties.end();
	}

private:
	std::unordered_map<std::string, std::shared_ptr<GP>> mProperties;
};

#endif
