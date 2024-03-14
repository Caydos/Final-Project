#ifndef CLOCK_H
#define CLOCK_H
#include <chrono>

class Clock
{
public:
	Clock();
	void Restart(void);
	float GetElapsedTime();
	void Pause(void);
	void Play(void);
	void SetElapsedTime(float _time);
private:
	std::chrono::high_resolution_clock::time_point clock;

	std::chrono::duration<float> pausedTime;
	bool isPaused = false;
	float timeSave;
};

#endif // !CLOCK_H
