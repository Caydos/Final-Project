#ifndef WAV_H
#define WAV_H
#include <iostream>
#include <fstream>
#include <vector>
#include <OpenAl/al.h>
#include <OpenAl/alc.h>

void LoadWavFile(const std::string& _filename, ALuint& _buffer)
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

    bool fmtFound = false, dataFound = false;
    unsigned int dataSize = 0;
    unsigned short audioFormat = 0, numChannels = 0, bitsPerSample = 0;
    unsigned int sampleRate = 0;
    std::vector<char> data;

    while (file.read(buffer, 4)) {
        unsigned int chunkSize;
        file.read(reinterpret_cast<char*>(&chunkSize), sizeof(chunkSize));

        if (strncmp(buffer, "fmt ", 4) == 0 && !fmtFound) {
            fmtFound = true;
            file.read(reinterpret_cast<char*>(&audioFormat), sizeof(audioFormat));
            file.read(reinterpret_cast<char*>(&numChannels), sizeof(numChannels));
            file.read(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
            file.ignore(6); // Ignore byte rate and block align
            file.read(reinterpret_cast<char*>(&bitsPerSample), sizeof(bitsPerSample));
            file.ignore(chunkSize - 16); // Skip the rest of the fmt chunk if any
        }
        else if (strncmp(buffer, "data", 4) == 0 && !dataFound) {
            dataFound = true;
            dataSize = chunkSize;
            data.resize(dataSize);
            if (!file.read(data.data(), dataSize)) {
                std::cerr << "Error reading WAV data" << std::endl;
                return;
            }
        }
        else {
            // Skip unrecognized or non-essential chunks
            file.ignore(chunkSize);
        }

        if (fmtFound && dataFound) break; // Found both necessary chunks
    }

    if (!fmtFound || !dataFound) {
        std::cerr << "Invalid WAV file (missing fmt or data subchunk)" << std::endl;
        return;
    }

    ALenum format;
    if (numChannels == 1) {
        format = (bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    }
    else {
        format = (bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    }

    alGenBuffers(1, &_buffer);
    alBufferData(_buffer, format, data.data(), dataSize, sampleRate);
}

#endif // !WAV_H