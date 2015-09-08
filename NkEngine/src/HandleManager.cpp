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

#include "HandleManager.h"
#include "Pow2Assert.h"
#include <cstddef>


namespace pow2
{



HandleManager::HandleEntry::HandleEntry() 
	: m_nextFreeIndex(0)
	, m_counter(1)
	, m_active(0)
	, m_endOfList(0)
	, m_entry(NULL)
{}

HandleManager::HandleEntry::HandleEntry(uint32 nextFreeIndex)
	: m_nextFreeIndex(nextFreeIndex)
	, m_counter(1)
	, m_active(0)
	, m_endOfList(0)
	, m_entry(NULL)
{}



HandleManager::HandleManager()
{
	Reset();
}


void HandleManager::Reset()
{
	m_activeEntryCount = 0;
	m_firstFreeEntry = 0;

	for (int i = 0; i < MaxEntries - 1; ++i)
		m_entries[i] = HandleEntry(i + 1);
	m_entries[MaxEntries - 1] = HandleEntry();
	m_entries[MaxEntries - 1].m_endOfList = true;
}


Handle HandleManager::Add(void* p, const std::type_info* type)
{
	POW2_ASSERT(m_activeEntryCount < MaxEntries - 1);
	//POW2_ASSERT(type >= 0 && type <= 31);

	const int newIndex = m_firstFreeEntry;
	POW2_ASSERT(newIndex < MaxEntries);
	POW2_ASSERT(m_entries[newIndex].m_active == false);
	POW2_ASSERT(!m_entries[newIndex].m_endOfList);

	m_firstFreeEntry = m_entries[newIndex].m_nextFreeIndex;
	m_entries[newIndex].m_nextFreeIndex = 0;
	m_entries[newIndex].m_counter = m_entries[newIndex].m_counter + 1;
	if (m_entries[newIndex].m_counter == 0)
		m_entries[newIndex].m_counter = 1;
	m_entries[newIndex].m_active = true;
	m_entries[newIndex].m_entry = p;

	++m_activeEntryCount;

	return Handle (newIndex, m_entries[newIndex].m_counter, type);
}


void HandleManager::Update(Handle handle, void* p)
{
	const int index = handle.m_index;
	POW2_ASSERT(m_entries[index].m_counter == handle.m_counter);
	POW2_ASSERT(m_entries[index].m_active == true);

	m_entries[index].m_entry = p;
}


void HandleManager::Remove(const Handle handle)
{
	const uint32 index = handle.m_index;
	POW2_ASSERT(m_entries[index].m_counter == handle.m_counter);
	POW2_ASSERT(m_entries[index].m_active == true);

	m_entries[index].m_nextFreeIndex = m_firstFreeEntry;
	m_entries[index].m_active = 0;
	m_firstFreeEntry = index;

	--m_activeEntryCount;
}


void* HandleManager::Get(Handle handle) const
{
	void* p = NULL;
	if (!Get(handle, p))
		return NULL;
	return p;
}


bool HandleManager::Get(const Handle handle, void*& out) const
{
	const int index = handle.m_index;
	if (m_entries[index].m_counter != handle.m_counter ||
	    m_entries[index].m_active == false)
		return false;

	out = m_entries[index].m_entry;
	return true;
}


int HandleManager::GetCount() const
{
	return m_activeEntryCount;
}


}
