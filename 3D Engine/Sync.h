#ifndef SYNC_H
#define SYNC_H
#include "Common.h"

#define SYNC_TIME 2000

namespace Synchronization
{
	void Loop(void);

	void PedSyncEvents(void);
	int GetServerId();
}


#endif // !SYNC_H