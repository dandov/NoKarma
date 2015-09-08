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
#ifndef pow2_datastructures_HandleManager_h
#define pow2_datastructures_HandleManager_h
#include <iostream>
#include <type_traits>

#include "Handle.h"

namespace pow2
{

class HandleManager
{
public:
	enum { MaxEntries = 65536 }; // 2^16

	HandleManager();

	void Reset();	
	Handle Add(void* p, const std::type_info* type);
	void Update(Handle handle, void* p);
	void Remove(Handle handle);
	
	void* Get(Handle handle) const;
	bool Get(Handle handle, void*& out) const;
	template< typename T > bool GetAs(Handle handle, T& out) const;

	int GetCount() const;

private:
	HandleManager(const HandleManager&);
	HandleManager& operator=(const HandleManager&);

	struct HandleEntry
	{
		HandleEntry();
		explicit HandleEntry(uint32 nextFreeIndex);
		
		uint32 m_nextFreeIndex : 20;
		uint32 m_counter : 10;
		uint32 m_active : 1;
		uint32 m_endOfList : 1;
		void* m_entry;
	};

	HandleEntry m_entries[MaxEntries];

	int m_activeEntryCount;
	uint32 m_firstFreeEntry;
};


template< typename T >
inline bool HandleManager::GetAs(Handle handle, T& out) const
{
	void* outAsVoid;
	const bool rv = Get(handle, outAsVoid);
	out = union_cast< T >(outAsVoid);

	return rv;
}


}

#endif
