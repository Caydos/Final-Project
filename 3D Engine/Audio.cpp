#include <iostream>
#include <fstream>
#include <vector>
#include <OpenAl/al.h>
#include <OpenAl/alc.h>
#include "Audio.h"
#include "Wav.h"


void Audio::Initialize()
{
    // Initialize OpenAL device and context
    ALCdevice* device = alcOpenDevice(NULL);
    if (!device) {
        return;
    }

    ALCcontext* context = alcCreateContext(device, NULL);
    if (!context) {
        alcCloseDevice(device);
        return;
    }
    alcMakeContextCurrent(context);

    // Load sound data into a buffer
    ALuint buffer = loadWavFile("../Sounds/Footsteps.wav");
    std::cout << buffer << std::endl;
    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSource3f(source, AL_POSITION, 10.0f, 0.0f, 0.0f); // Position of the source
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f); // Velocity of the source

    // Set Listener properties
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f); // Position of the listener
    ALfloat orientation[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f }; // Orientation (facing direction and up vector)
    alListenerfv(AL_ORIENTATION, orientation);

    // Set Distance Model (optional, depending on your needs)
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

    // Play the sound
    alSourcePlay(source);

    // Wait for the sound to finish playing
    ALint sourceState;
    do {
        alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
    } while (sourceState == AL_PLAYING);

    // Clean up
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void Audio::Tick()
{
}
