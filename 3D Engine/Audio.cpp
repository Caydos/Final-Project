#include <iostream>
#include <fstream>
#include <vector>
#include <OpenAl/al.h>
#include <OpenAl/alc.h>
#include "Audio.h"
#include "Wav.h"


static std::shared_mutex mutex;
static bool audioDeviceActivated = false;
static std::vector<Audio::Sound*> sounds;
static std::thread thread;
static ALCdevice* device;
static ALCcontext* context;



Audio::Sound::Sound()
{
	this->initialized = false;
	this->loop = false;
	this->paused = false;
	this->finished = false;
	this->buffer = 0;
	this->source = 0;
	this->channelCount = 0;

	this->referenceDistance = 0.0;
	this->maxDistance = 0.0;
	this->rollOffFactor = 0.0;

	this->position = glm::vec3(0);
	this->velocity = glm::vec3(0);
	this->volume = 1.0f;
}

Audio::Sound::~Sound()
{
}

std::string Audio::Sound::GetName()
{
	return this->name;
}

void Audio::Sound::SetName(std::string _name)
{
	this->name = _name;
}

void Audio::Sound::LoadFromFile(const char* _path)
{
	if (!this->source) { alGenSources(1, &this->source); }
	LoadWavFile(_path, buffer);
	{
		ALenum error;
		if ((error = alGetError()) != AL_NO_ERROR && audioDeviceActivated)
		{
			Logger::Write("[LoadWavFile] - OpenAL error: ", alGetString(error));
		}
	}
	ALint channels;
	alGetBufferi(this->buffer, AL_CHANNELS, &channels);
	this->channelCount = channels;

	alSourcei(this->source, AL_BUFFER, buffer);

	alGetSourcef(this->source, AL_REFERENCE_DISTANCE, &this->referenceDistance);
	alGetSourcef(this->source, AL_MAX_DISTANCE, &this->maxDistance);
	alGetSourcef(this->source, AL_ROLLOFF_FACTOR, &this->rollOffFactor);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR && audioDeviceActivated)
	{
		Logger::Write("[LoadFromFile] - OpenAL error: ", alGetString(error));
	}
}

void Audio::Sound::Erase()
{
	if (this->source)
	{
		alDeleteSources(1, &this->source);
	}
	if (this->buffer)
	{
		alDeleteBuffers(1, &this->buffer);
	}
}

void Audio::Sound::Loop(bool _state)
{
	if (!this->source) { alGenSources(1, &this->source); }
	this->loop = _state;
	alSourcei(this->source, AL_LOOPING, this->loop);
}

void Audio::Sound::Play()
{
	if (!this->source) { alGenSources(1, &this->source); }
	this->paused = false;
	alSourcePlay(this->source);
}

void Audio::Sound::Pause()
{
	if (!this->source) { alGenSources(1, &this->source); }
	this->paused = true;
	alSourcePause(this->source);
}

bool Audio::Sound::IsPlaying()
{
	ALint sourceState;
	alGetSourcei(this->source, AL_SOURCE_STATE, &sourceState);
	if (sourceState == AL_PLAYING)
	{
		return true;
	}
	return false;
}

bool Audio::Sound::HasFinished()
{
	return this->finished;
}

float Audio::Sound::GetReferenceDistance()
{
	return this->referenceDistance;
}

void Audio::Sound::SetReferenceDistance(float _referenceDistance)
{
	if (!this->source) { alGenSources(1, &this->source); }
	this->referenceDistance = _referenceDistance;
	alSourcef(this->source, AL_REFERENCE_DISTANCE, this->referenceDistance);
}

float Audio::Sound::GetMaxDistance()
{
	return this->maxDistance;
}

void Audio::Sound::SetMaxDistance(float _maxDistance)
{
	if (!this->source) { alGenSources(1, &this->source); }
	this->maxDistance = _maxDistance;
	alSourcef(this->source, AL_MAX_DISTANCE, this->maxDistance);
}

float Audio::Sound::GetRollOffFactor()
{
	return this->rollOffFactor;
}

void Audio::Sound::SetRollOffFactor(float _rollOffFactor)
{
	if (!this->source) { alGenSources(1, &this->source); }
	this->rollOffFactor = _rollOffFactor;
	alSourcef(this->source, AL_ROLLOFF_FACTOR, this->rollOffFactor);
}

glm::vec3 Audio::Sound::GetPosition()
{
	return this->position;
}

void Audio::Sound::SetPosition(glm::vec3 _position)
{
	if (!this->source) { alGenSources(1, &this->source); }
	if (this->channelCount != 1)
	{
		Logger::Write("[SetPosition] - Unable to set a position to a stereo source");
		return;
	}
	this->position = _position;

	alSource3f(this->source, AL_POSITION, this->position.x, this->position.y, this->position.z);
}

glm::vec3 Audio::Sound::GetVelocity()
{
	return this->velocity;
}

void Audio::Sound::SetVelocity(glm::vec3 _velocity)
{
	if (!this->source) { alGenSources(1, &this->source); }
	this->velocity = _velocity;
	alSource3f(this->source, AL_VELOCITY, this->velocity.x, this->velocity.y, this->velocity.z);
}

float Audio::Sound::GetVolume()
{
	return this->volume;
}

void Audio::Sound::SetVolume(float _volume)
{
	this->volume = _volume;
	alSourcef(this->source, AL_GAIN, this->volume);
}

void Audio::Sound::Update()
{
	if (!this->source) { alGenSources(1, &this->source); }
	ALint sourceState;
    alGetSourcei(this->source, AL_SOURCE_STATE, &sourceState);
	if (sourceState == AL_PLAYING)
	{
		this->finished = false;
	}
	else if (sourceState == AL_STOPPED)
	{
		this->finished = true;
	}
}








void Audio::Initialize()
{
	// Initialize OpenAL device and context
	audioDeviceActivated = true;
	device = alcOpenDevice(NULL);
	if (!device)
	{
		audioDeviceActivated = false;
		//return 0;
	}

	context = alcCreateContext(device, NULL);
	if (!context)
	{
		audioDeviceActivated = false;
		alcCloseDevice(device);
		//return 0;
	}
	alcMakeContextCurrent(context);

	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	ALfloat orientation[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	alListenerfv(AL_ORIENTATION, orientation);

	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR && audioDeviceActivated)
	{
		Logger::Write("[Initialize] - OpenAL error: ", alGetString(error));
	}
}

void Audio::Tick(glm::vec3 _position, glm::vec3 _frontVector, glm::vec3 _upVector)
{
	alListener3f(AL_POSITION, _position.x, _position.y, _position.z);

	float orientation[] = { _frontVector.x, _frontVector.y, _frontVector.z, _upVector.x, _upVector.y, _upVector.z };
	alListenerfv(AL_ORIENTATION, orientation);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR && audioDeviceActivated)
	{
		Logger::Write("[Tick] - OpenAL error: ", alGetString(error));
	}
}

void Audio::Destroy()
{
	alcDestroyContext(context);
	alcCloseDevice(device);
}

Audio::Sound* Audio::CreateSound()
{
	Audio::Sound* sound = new Audio::Sound;
	sounds.push_back(sound);
	return sound;
}

void Audio::EraseSound(Sound* _sound)
{
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

