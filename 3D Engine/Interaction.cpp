#include "Interaction.h"


static bool initialized;

static Interactions::Interaction* interactions = nullptr;
static unsigned int count = 0;


void Interactions::Interaction::AddArg(const char* _array)
{
	char** temp = new char* [this->argCount + 1];

	for (int i = 0; i < this->argCount; i++)
	{
		temp[i] = this->argStack[i];
	}

	temp[this->argCount] = new char[strlen(_array) + 1];
	strcpy(temp[this->argCount], _array);

	delete[] this->argStack;

	this->argStack = temp;
	this->argCount++;
}
void Interactions::Interaction::Call(void)
{
	this->function(this->argStack);
	// Reset
	if (this->argStack != nullptr)
	{
		for (unsigned int i = 0; i < this->argCount; i++)
		{
			if (this->argStack[i] != nullptr)
			{
				delete[] this->argStack[i];
			}
		}
		if (this->argStack != nullptr)
		{
			delete[] this->argStack;
			this->argStack = nullptr;
		}
	}
	this->argCount = 0;
	this->processing = false;
}

void Interactions::Listener(int socketfd)
{
	//char msgBuffer[ARRAY_SIZE];
	//std::string eventBuffer;
	//while (true)
	//{
	//	int bytesRead = recv(socketfd, msgBuffer, ARRAY_SIZE, 0);
	//	if (bytesRead <= 0)
	//	{
	//		std::cout << "Error reading data." << std::endl;
	//		Connection::Close("51.178.46.32", 55301);
	//		eventBuffer.clear();
	//		break;
	//	}
	//	eventBuffer += msgBuffer;

	//	//std::cout << "recv : " << msgBuffer << std::endl;
	//	const char* result = static_cast<const char*>(std::memchr(msgBuffer, END_CHARACTER, ARRAY_SIZE));
	//	if (result != nullptr)
	//	{
	//		const char* position = result;
	//		int index = position - msgBuffer;
	//		eventBuffer.erase(index);

	//		/* Do somthing with the event */

	//		//std::cout << "Event is : " << eventBuffer << std::endl << std::endl;
	//		Interactions::AddWaiting(eventBuffer.c_str());

	//		/* */
	//		eventBuffer.clear();
	//		char* leftData = &msgBuffer[index + 1];
	//		eventBuffer += leftData;
	//		//std::cout << eventBuffer << std::endl;
	//	}
	//	if (eventBuffer.size() > BUFFER_OVERSIZE)
	//	{
	//		eventBuffer.clear();
	//		std::cout << "Server sent too much informations" << std::endl;
	//		return;
	//	}
	//	bzero(msgBuffer, sizeof(msgBuffer));
	//}
}

void Interactions::Unserialize(char* _buffer)
{
	void* result = std::memchr(_buffer, SEPARATOR_CHARACTER, strlen(_buffer));
	if (result != nullptr)
	{//Found
		char* position = (char*)result;
		int index = static_cast<int>(position - _buffer);
		char* name = new char[index + 1];
		std::memcpy(name, _buffer, index);
		name[index] = '\0';

		//printf("Event name : %s\n", name);
		int eventId = -1;
		for (int i = 0; i < count; i++)
		{
			if (strcmp(interactions[i].name, name) == 0)
			{
				//std::cout << "Found event : " << events[i].name << std::endl;
				eventId = i;
				break;
			}
		}
		if (eventId < 0)
		{
			std::cout << "Event not found : " << name << std::endl;
			return;
		}

		// Lock
		while (interactions[eventId].processing == true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		interactions[eventId].processing = true;

		int lastIndex = index + 1;
		while (true)
		{
			char* nBuffer = _buffer + lastIndex;
			void* marker = std::memchr(nBuffer, SEPARATOR_CHARACTER, strlen(nBuffer));
			if (marker != nullptr)
			{
				char* markerPosition = (char*)marker;
				int markerIndex = static_cast<int>(markerPosition - nBuffer);
				char* newArg = new char[markerIndex + 1];
				std::memcpy(newArg, nBuffer, markerIndex);
				newArg[markerIndex] = '\0';
				//std::cout << "Arg : " << newArg << std::endl;
				interactions[eventId].AddArg(newArg);
				delete[] newArg;
				lastIndex += (markerIndex + 1);
			}
			else
			{
				interactions[eventId].AddArg(nBuffer);
				break;
			}
		}
		interactions[eventId].Call();
		delete[] name;
	}
	else
	{
		std::cout << "Unserializing Error : " << _buffer << std::endl;
	}
}

static std::shared_mutex mutex;
void Interactions::Register(const char* _name, void(*function)(char** _args))
{
	std::unique_lock<std::shared_mutex> lock(mutex);
	Interaction* newInteractions = new Interaction[count + 1];
	if (count > 0)
	{
		std::memcpy(newInteractions, interactions, count * sizeof(Interaction));
		delete[] interactions;
	}
	newInteractions[count].name = _name;
	newInteractions[count].function = function;
	newInteractions[count].processing = false;
	newInteractions[count].argStack = nullptr;
	newInteractions[count].argCount = 0;
	interactions = newInteractions;

	count++;
}

void Interactions::CreateEvents(void)
{
	if (initialized)
	{
		std::cout << "Event has already been initialized" << std::endl;
		return;
	}
	initialized = true;
	//RegisterInteractions();
}



char** waitingInteractions = nullptr;
unsigned int waitingInteractionsCount = 0;

void* Interactions::AddWaiting(const char* array)
{
	std::unique_lock<std::shared_mutex> lock(mutex);

	char** temp = new char* [waitingInteractionsCount + 1];

	for (unsigned int i = 0; i < waitingInteractionsCount; i++)
	{
		temp[i] = waitingInteractions[i];
	}

	temp[waitingInteractionsCount] = new char[strlen(array) + 1];
	strcpy(temp[waitingInteractionsCount], array);

	delete[] waitingInteractions;

	waitingInteractions = temp;
	waitingInteractionsCount++;
	return &waitingInteractions[waitingInteractionsCount - 1];
}

void Interactions::RemoveWaiting(unsigned int index)
{
	std::unique_lock<std::shared_mutex> lock(mutex);
	if (index >= waitingInteractionsCount)
		return;

	delete[] waitingInteractions[index];

	for (unsigned int i = index; i < waitingInteractionsCount - 1; i++)
	{
		waitingInteractions[i] = waitingInteractions[i + 1];
	}

	waitingInteractionsCount--;
}

void Interactions::Thread(bool _running)
{
	while (_running)
	{
		if (waitingInteractionsCount > 0)
		{
			Interactions::Unserialize(waitingInteractions[0]);
			RemoveWaiting(0);
		}
	}
}

