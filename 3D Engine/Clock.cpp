#include "Clock.h"
#include <algorithm>


Clock::Clock() : isPaused(false), pausedTime(0)
{
    clock = std::chrono::high_resolution_clock::now();
    isPaused = false;
    timeSave = 0;
}

void Clock::Restart(void)
{
    auto end = std::chrono::high_resolution_clock::now();
    this->clock = end;
    timeSave = 0;
}

float Clock::GetElapsedTime()
{
    float elapsedTime;
    if (isPaused)
    {
        return timeSave;
    }
    else
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - this->clock);
        elapsedTime = duration.count() + timeSave;
    }


    return elapsedTime;
}

void Clock::SetElapsedTime(float _time)
{
    if (isPaused)
    {
        //Si l'horloge est en pause ajustez timeSave.
        timeSave = _time;
    }
    else
    {
        //Calculer un nouveau point de d�part
        auto now = std::chrono::high_resolution_clock::now();
        auto adjusted_time = now - std::chrono::milliseconds(int(_time));
        clock = adjusted_time;

        //R�initialiser timeSave
        timeSave = 0;
    }
}

void Clock::Pause(void)
{
    timeSave = this->GetElapsedTime();
    if (!isPaused)
    {
        auto now = std::chrono::high_resolution_clock::now();
        pausedTime += now - clock;
        isPaused = true;
    }
}

void Clock::Play(void)
{
    if (isPaused)
    {
        clock = std::chrono::high_resolution_clock::now();
        isPaused = false;
    }
}
