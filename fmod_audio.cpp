#include "fmod_audio.h"
#include <iostream>
#include <format>

#define MAX_CHANELS 32

fmodAudio::fmodAudio()
{
	FMOD_RESULT result = FMOD::System_Create(&fmod_system);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error: " << result;
	}
	result = fmod_system->init(MAX_CHANELS, FMOD_INIT_NORMAL, 0);
	
	fmod_system->createChannelGroup("TestChannel", &channelGroup);
}

fmodAudio::~fmodAudio()
{
}

void fmodAudio::playAudioFromAudioFilePath(std::string FilePath)
{	
	FMOD::Sound* SoundObject;
	FMOD_RESULT result = fmod_system->createSound(
		FilePath.c_str(),
		FMOD_DEFAULT,
		0,
		&SoundObject
	);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error: " << result << " When creating sound object";
	}
	
	result = fmod_system->playSound(SoundObject, channelGroup, false, &channel);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error: " << result << " When playing sound";
	}
}

