#ifndef WAV_H
#define WAV_H
#include <iostream>
#include <fstream>
#include <vector>
#include <OpenAl/al.h>
#include <OpenAl/alc.h>

void LoadWavFile(const std::string& _filename, ALuint _buffer)
{
    std::ifstream file(_filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open WAV file: " << _filename << std::endl;
        return;
    }

    char buffer[4];
    if (!file.read(buffer, 4)) {
        std::cerr << "Error reading WAV file (1)" << std::endl;
        return;
    }
    if (strncmp(buffer, "RIFF", 4) != 0) {
        std::cerr << "File is not a valid WAV file (does not start with RIFF)" << std::endl;
        return;
    }
    file.ignore(4); // Ignore chunk size
    if (!file.read(buffer, 4) || strncmp(buffer, "WAVE", 4) != 0) {
        std::cerr << "File is not a valid WAV file (does not contain WAVE)" << std::endl;
        return;
    }

    // Read format subchunk
    if (!file.read(buffer, 4) || strncmp(buffer, "fmt ", 4) != 0) {
        std::cerr << "Invalid WAV file (no fmt )" << std::endl;
        return;
    }

    // Read format chunk size
    unsigned int subchunk1Size;
    file.read(reinterpret_cast<char*>(&subchunk1Size), sizeof(subchunk1Size));

    unsigned short audioFormat, numChannels;
    file.read(reinterpret_cast<char*>(&audioFormat), sizeof(audioFormat));
    file.read(reinterpret_cast<char*>(&numChannels), sizeof(numChannels));

    unsigned int sampleRate;
    file.read(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));

    file.ignore(6); // Ignore byte rate and block align

    unsigned short bitsPerSample;
    file.read(reinterpret_cast<char*>(&bitsPerSample), sizeof(bitsPerSample));

    // Read data subchunk
    if (!file.read(buffer, 4) || strncmp(buffer, "data", 4) != 0) {
        std::cerr << "Invalid WAV file (no data subchunk)" << std::endl;
        return;
    }

    unsigned int dataSize;
    file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));

    std::vector<char> data(dataSize);
    if (!file.read(data.data(), dataSize)) {
        std::cerr << "Error reading WAV data" << std::endl;
        return;
    }

    ALenum format;
    if (numChannels == 1) {
        format = (bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    }
    else {
        format = (bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    }

    alBufferData(_buffer, format, data.data(), dataSize, sampleRate);
}

#endif // !WAV_H