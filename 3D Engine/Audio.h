#ifndef AUDIO_H
#define AUDIO_H
#include "Common.h"

namespace Audio
{

	class Sound
	{
	public:
		Sound();
		~Sound();

		std::string GetName();
		void SetName(std::string _name);

		void LoadFromFile(const char* _path);
		void Erase();

		void Loop(bool _state);
		void Play();
		void Pause();
		bool IsPlaying();

		bool HasFinished();


		float GetReferenceDistance();
		void SetReferenceDistance(float _referenceDistance);

		float GetMaxDistance();
		void SetMaxDistance(float _maxDistance);

		float GetRollOffFactor();
		void SetRollOffFactor(float _rollOffFactor);

		glm::vec3 GetPosition();
		void SetPosition(glm::vec3 _position);
		glm::vec3 GetVelocity();
		void SetVelocity(glm::vec3 _velocity);

		float GetVolume();
		void SetVolume(float _volume);

		void Update();

	private:
		std::shared_mutex mutex;
		std::string name;
		bool initialized;
		bool loop;
		bool paused;
		bool finished;
		unsigned int buffer;
		unsigned int source;
		int channelCount;

		//Attenuation params
		float referenceDistance;
		float maxDistance;
		float rollOffFactor;

		glm::vec3 position;
		glm::vec3 velocity;

		float volume;
	};
	void Initialize();
	void Tick(glm::vec3 _position, glm::vec3 _frontVector, glm::vec3 _upVector);
	void Destroy();

	Sound* CreateSound();
	void EraseSound(Sound* _sound);

}

#endif // !AUDIO_H