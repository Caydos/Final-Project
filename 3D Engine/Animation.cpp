#include "Animation.h"




Animations::Movement::Movement()
{
	this->object = nullptr;
}

Animations::Movement::~Movement()
{
}

glm::vec3 Animations::Movement::GetOffset()
{
	return this->offset;
}

void Animations::Movement::SetOffset(glm::vec3 _offset)
{
	this->offset = _offset;
}

glm::vec3 Animations::Movement::GetRotation()
{
	return this->rotation;
}

void Animations::Movement::SetRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;
}

Sets::Set* Animations::Movement::GetObject()
{
	return this->object;
}

void Animations::Movement::SetObject(Sets::Set* _object)
{
	this->object = _object;
}

void Animations::Movement::Update(float _elapsedTime, float _duration)
{
	if (_elapsedTime >= _duration)
	{
        // For debug purpose only
	}
    else
    {
        std::cout << _elapsedTime << std::endl;
    }
}


Animations::Sequence::Sequence()
{
}

Animations::Sequence::~Sequence()
{
}

std::string Animations::Sequence::GetName()
{
	return this->name;
}

void Animations::Sequence::SetName(std::string _name)
{
	this->name = _name;
}

float Animations::Sequence::GetDuration()
{
	return this->duration;
}

void Animations::Sequence::SetDuration(float _duration)
{
	this->duration = _duration;
}

std::vector<Animations::Movement>* Animations::Sequence::GetMovements()
{
	return &this->movements;
}

Animations::Animation::Animation()
{
}

Animations::Animation::~Animation()
{
}

Clock* Animations::Animation::AccessClock()
{
	return &this->clock;
}

std::string Animations::Animation::GetName()
{
	return this->name;
}

void Animations::Animation::SetName(std::string _name)
{
	this->name = _name;
}

std::vector<Animations::Sequence>* Animations::Animation::GetSequences()
{
	return &this->sequences;
}

float Animations::Animation::GetDuration()
{
	return this->duration;
}

void Animations::Animation::CalculateDuration()
{
	this->duration = 0.0f;
	for (size_t sequenceId = 0; sequenceId < this->sequences.size(); sequenceId++)
	{
		this->duration += this->sequences[sequenceId].GetDuration();
	}
}

void Animations::Animation::Update()
{
    // Get the elapsed time in milliseconds from the clock
    float elapsedTime = clock.GetElapsedTime() / 1000.0f; // Convert to seconds

    // If there are no sequences, return
    if (sequences.empty())
    {
        return;
    }

    float accumulatedTime = 0.0f;
    Sequence* currentSequence = nullptr;

    for (Sequence& sequence : sequences)
    {
        float sequenceEnd = accumulatedTime + sequence.GetDuration();

        if (elapsedTime >= accumulatedTime && elapsedTime < sequenceEnd)
        {
            currentSequence = &sequence;
            break;
        }

        accumulatedTime = sequenceEnd;
    }

    // Animation end
    if (currentSequence == nullptr)
    {
        return;
    }

    std::vector<Movement>* movements = currentSequence->GetMovements();
    if (movements) {
        float sequenceStartTime = accumulatedTime - currentSequence->GetDuration(); // Start of current sequence

        // Calculate time within the current sequence
        float timeInSequence = elapsedTime - sequenceStartTime;

        for (Movement& movement : *movements) {
            // You can add a parameter to indicate the current time in the sequence
            movement.Update(timeInSequence, currentSequence->GetDuration());
        }
    }
}

