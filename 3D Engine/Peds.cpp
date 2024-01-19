#include "Peds.h"
#include "Common.h"
static std::vector<Peds::Ped*> peds;

void Peds::Insert(Ped* _ped)
{
	peds.push_back(_ped);
}


void Peds::Remove(int _serverId)
{
	for (size_t i = 0; i < peds.size(); i++)
	{
		if (peds[i]->GetServerId() == _serverId)
		{
			delete peds[i];
			peds.erase(peds.begin() + i);
			return;
		}
	}
}

std::vector<Peds::Ped*>* Peds::Get()
{
	return &peds;
}
