//Dustin Gehm
//A manager to handle playing music and sounds

#pragma once
#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H

#include <map>
#include <string>

#include <SDL_mixer.h>

#include "Singleton.h"

using std::map;
using std::string;

class SoundManager : public Singleton<SoundManager> {
public:
	SoundManager();
	~SoundManager();

	/*set the volume for all sounds, vol is between 0 and 1*/
	void setSoundVolume(float vol);
	inline float getSoundVolume() const {
		return soundVolume / 128.0f;
	}

	void addSound(string name, string sndPath);
	void removeSound(string name);
	bool playSound(string name);
	bool playSoundFor(string name, uint seconds);
	void pauseSound(string name);
	void resumeSound(string name);
	void stopSound(string name);

	void setMusic(string musicPath);

	void setMusicVolume(float vol);
	inline float getMusicVolume() const {
		return musicVolume / 128.0f;
	}

	void playMusic();
	void pauseMusic();
	void resumeMusic();
	void stopMusic();
	bool isMusicStarted();
	bool isMusicPaused();

	bool isSoundLoaded(string name);
	bool isSoundPlaying(string name);
	bool isSoundNotPlaying(string name);

	//return the length of specified sound in milliseconds
	uint getSoundLength(string name);

private:
	void channelDone(int channel);

	map<string, string> soundNames;
	map<string, Mix_Chunk*> soundChunks;
	map<string, int> activeSounds;
	Mix_Music* music = NULL;
	int soundVolume = 128;
	int musicVolume = 128;

	friend void outerChannelDone(int channel);
};

static void outerChannelDone(int channel);

#endif