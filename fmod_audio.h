
#pragma once
#include "fmod/fmod.hpp"
#include <string>

class fmodAudio
{
public:
	fmodAudio();
	~fmodAudio();

private:
	FMOD::System* fmod_system;
public:
	void playAudioFromAudioFilePath(std::string FilePath);
	FMOD::Channel* channel;
	FMOD::ChannelGroup* channelGroup;
	FMOD::Sound* sound; //sound object, contains things related to current sound being played
};