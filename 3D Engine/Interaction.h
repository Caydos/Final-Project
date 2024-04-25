#ifndef INTERACTION_H
#define INTERACTION_H
#include "Common.h"


namespace Interactions
{
	const char END_CHARACTER = '§';
	const char SEPARATOR_CHARACTER = 'µ';
	const unsigned int ARRAY_SIZE = 1024;
	const unsigned int BUFFER_OVERSIZE = 30000;
	class Interaction
	{
	public:
		char** argStack = nullptr;
		int argCount = 0;
		const char* name;
		void (*function)(char** _args);
		bool processing = false;

		void AddArg(const char* array);
		void Call(void);
	};

	void Listener(int _socketfd);
	void Unserialize(char* _buffer);
	void Register(const char* _name, void (*_function)(char** _args));
	void CreateEvents(void);

	void* AddWaiting(const char* _array);
	void RemoveWaiting(unsigned int _index);
	void Thread(bool _running);



	void Register(const char* _name, void (*_function)(char** _args));

	class WInteraction
	{
	public:
		char* buffer = nullptr;
		unsigned int bufferSize = 0;

		template<typename Arg>
		void SerializeArg(Arg _arg)
		{
			std::stringstream valueString;
			valueString << _arg;
			std::string argStr = valueString.str();

			int argLength = argStr.length();

			int oldBufferSize = bufferSize - 1;
			bufferSize += (argLength + 1); // +1 for the null-terminator

			char* newBuffer = new char[bufferSize + 1];

			if (buffer != nullptr)
			{
				std::memcpy(newBuffer, buffer, oldBufferSize);
				delete[] buffer;
			}

			buffer = newBuffer;

			for (int i = 0; i < argLength; i++)
			{
				buffer[oldBufferSize + i] = argStr[i];
			}
			buffer[bufferSize - 2] = SEPARATOR_CHARACTER;
			buffer[bufferSize - 1] = '\0';
		}
	};
	template<class...Arg>
	void Trigger(const char* _name, Arg..._args)
	{
		WInteraction interaction;
		int nameLength = strlen(_name);

		char* targetString = new char[nameLength];
		sprintf(targetString, "%s", _name);
		interaction.bufferSize = strlen(targetString) + 2;
		interaction.buffer = new char[interaction.bufferSize];
		strcpy(interaction.buffer, targetString);
		interaction.buffer[interaction.bufferSize - 2] = SEPARATOR_CHARACTER;

		((interaction.SerializeArg(_args)), ...);
		interaction.buffer[interaction.bufferSize - 2] = END_CHARACTER;
		interaction.buffer[interaction.bufferSize - 1] = '\0';
		Interaction::AddWaiting(interaction.buffer);
		delete[] targetString;
		delete[] interaction.buffer;
		interaction.buffer = nullptr;
		interaction.bufferSize = 0;
	}
}

#endif // !INTERACTION_H