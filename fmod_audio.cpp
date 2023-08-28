#include "fmod_audio.h"
#include <stdio.h>

fmodAudio::fmodAudio()
{
}

fmodAudio::~fmodAudio()
{
}

FMOD_RESULT fmodAudio::createFmodSystem()
{
	FMOD_RESULT result = FMOD::System_Create(&fmod_system);
	if (result != FMOD_OK)
	{

	}
}
