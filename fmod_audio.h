
#pragma once
#include "fmod/fmod.hpp"

class fmodAudio
{
public:
	fmodAudio();
	~fmodAudio();

private:
	FMOD::System* fmod_system = nullptr;
	FMOD_RESULT createFmodSystem();

};