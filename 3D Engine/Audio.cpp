#include <iostream>
#include <fstream>
#include <vector>
#include <OpenAl/al.h>
#include <OpenAl/alc.h>
#include "Audio.h"
#include "Wav.h"





Audio::Sound::Sound()
{
	this->initialized = false;
	this->loop = false;
	this->paused = false;
	this->finished = false;
}

Audio::Sound::~Sound()
{
}

void Audio::Sound::Initialize()
{
	std::unique_lock<std::shared_mutex> lock(this->mutex);
	alGenSources(1, &this->source);
	alGenBuffers(1, &this->buffer);
	this->initialized = true;
}

void Audio::Sound::LoadFromFile(const char* _path)
{
	std::unique_lock<std::shared_mutex> lock(this->mutex);
	LoadWavFile(_path, buffer);
}

void Audio::Sound::Erase()
{
	std::unique_lock<std::shared_mutex> lock(this->mutex);
	if (this->initialized)
	{
		alDeleteSources(1, &this->source);
		alDeleteBuffers(1, &this->buffer);
	}
}

void Audio::Sound::Loop(bool _state)
{
	std::unique_lock<std::shared_mutex> lock(this->mutex);
	this->loop = _state;
	alSourcei(this->source, AL_LOOPING, this->loop);

}

void Audio::Sound::Play()
{
	std::unique_lock<std::shared_mutex> lock(this->mutex);
	this->paused = false;
	alSourcePlay(this->source);
}

void Audio::Sound::Pause()
{
	std::unique_lock<std::shared_mutex> lock(this->mutex);
	this->paused = true;
	alSourcePause(this->source);
}

bool Audio::Sound::IsPlaying()
{
	return !this->paused;
}

bool Audio::Sound::HasFinished()
{
	return this->finished;
}

glm::vec3 Audio::Sound::GetPosition()
{
	return this->position;
}

void Audio::Sound::SetPosition(glm::vec3 _position)
{
	std::unique_lock<std::shared_mutex> lock(this->mutex);
	this->position = _position;
	if (this->initialized)
	{
		alSource3f(source, AL_POSITION, this->position.x, this->position.y, this->position.z);
	}
}

glm::vec3 Audio::Sound::GetVelocity()
{
	return this->velocity;
}

void Audio::Sound::SetVelocity(glm::vec3 _velocity)
{
	std::unique_lock<std::shared_mutex> lock(this->mutex);
	this->velocity = _velocity;
	if (this->initialized)
	{
		alSource3f(source, AL_VELOCITY, this->velocity.x, this->velocity.y, this->velocity.z);
	}
}

void Audio::Sound::Update()
{
	ALint sourceState;
    alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
	if (sourceState == AL_PLAYING)
	{
		this->finished = false;
	}
	else if (sourceState == AL_STOPPED)
	{
		this->finished = true;
	}
}







static std::shared_mutex mutex;
static std::vector<Audio::Sound*> sounds;
static std::thread thread;

void Audio::Initialize()
{
	//// Initialize OpenAL device and context
	//ALCdevice* device = alcOpenDevice(NULL);
	//if (!device)
	//{
	//	return;
	//}

	//ALCcontext* context = alcCreateContext(device, NULL);
	//if (!context) {
	//	alcCloseDevice(device);
	//	return;
	//}
	//alcMakeContextCurrent(context);

	//// Set Listener properties
	//alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f); // Position of the listener
	//ALfloat orientation[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f }; // Orientation (facing direction and up vector)
	//alListenerfv(AL_ORIENTATION, orientation);

	//// Set Distance Model (optional, depending on your needs)
	//alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

	//alcDestroyContext(context);
	//alcCloseDevice(device);
}

void Audio::Tick()
{
	mutex.lock_shared();
	for (size_t soundId = 0; soundId < sounds.size(); soundId++)
	{

	}
	mutex.unlock_shared();
}

Audio::Sound* Audio::CreateSound()
{
	std::unique_lock<std::shared_mutex> lock(mutex);
	Audio::Sound* sound = new Audio::Sound;
	sounds.push_back(sound);
	return sound;
}

void Audio::EraseSound(Sound* _sound)
{
	std::unique_lock<std::shared_mutex> lock(mutex);
	for (size_t soundId = 0; soundId < sounds.size(); soundId++)
	{
		if (sounds[soundId] == _sound)
		{
			_sound->Erase();
			delete _sound;
			sounds.erase(sounds.begin() + soundId);
			break;
		}
	}
}

