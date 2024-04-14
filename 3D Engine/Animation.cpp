#include "Animation.h"

Animations::Sequence::Sequence()
{
}

Animations::Sequence::~Sequence()
{
}

Animations::Animation::Animation()
{
}

Animations::Animation::~Animation()
{
}

std::string Animations::Animation::GetName()
{
	return this->name;
}

void Animations::Animation::SetName(std::string _name)
{
	this->name = _name;
}
