#include "Clock.h"
#include <algorithm>


Clock::Clock() : isPaused(false), pausedTime(0) 
{
    clock = std::chrono::high_resolution_clock::now();
}
 
void Clock::Restart(void)
{
	auto end = std::chrono::high_resolution_clock::now();
	this->clock = end;
    timeSave = 0;
}

float Clock::GetElapsedTime()
{
    if (isPaused) 
    {
        return timeSave;
    }
    else
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - this->clock);
        return duration.count() + timeSave;
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