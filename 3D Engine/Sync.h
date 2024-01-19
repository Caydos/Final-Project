#ifndef SYNC_H
#define SYNC_H
#include "Common.h"

#define SYNC_TIME 200

namespace Synchronization
{
	void Loop(void);
	void NodeCallback(char** _args);

	void PedSyncEvents(void);
	int GetServerId();
}


#endif // !SYNC_H