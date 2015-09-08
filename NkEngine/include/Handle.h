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
#ifndef pow2_datastructures_Handle_h
#define pow2_datastructures_Handle_h
#include <iostream>
#include <type_traits>

typedef unsigned int uint32;


namespace pow2
{

struct Handle
{
	Handle() : m_index(0), m_counter(0), m_type(0)
	{}

	Handle(uint32 index, uint32 counter, const std::type_info* type)
		: m_index(index), m_counter(counter), m_type(type)
	{}

	inline operator uint32() const;
	
	uint32 m_index : 20;
	uint32 m_counter : 10;
	const std::type_info* m_type;
};


Handle::operator uint32() const
{
	return m_counter << 20 | m_index;
}


}

#endif
