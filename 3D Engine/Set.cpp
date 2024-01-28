#include "Set.h"

static std::vector<Sets::Set*> sets;

void Sets::UpdateVisibility()
{
	for (size_t i = 0; i < sets.size(); i++)
	{
		sets[i]->CheckVisibility();
	}
}

Sets::Set* Sets::Create()
{
	Set* newSet = new Set;
	newSet->Initialize();
	sets.push_back(newSet);
	return newSet;
}

void Sets::Erase(Set* _set)
{
	for (size_t i = 0; i < sets.size(); i++)
	{
		if (sets[i] == _set)
		{
			sets[i]->Erase();
			delete sets[i];
			sets.erase(sets.begin() + i);
			return;
		}
	}
}

std::vector<Sets::Set*>* Sets::GetAll()
{
	return &sets;
}
