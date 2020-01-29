//Dustin Gehm

#include "SoundManager.h"

#include "ThreadManager.h"

SoundManager::SoundManager(){
	Mix_ChannelFinished(outerChannelDone);
}

SoundManager::~SoundManager(){
	for (auto iter = chunks.begin(); iter != chunks.end(); iter++){
		Mix_FreeChunk(iter->second);
	}

	if (music){
		Mix_FreeMusic(music);
		music = NULL;
	}
}

void SoundManager::setSoundVolume(float vol){
	soundVolume = (int)(vol * 128);

	Mix_Volume(-1, soundVolume);
}

void SoundManager::addSound(string name, string sndPath){
	if (chunks.find(name) == chunks.end()){
		ATOMIC_LOCK(ThreadManager::fileLock);

		Mix_Chunk* tempSound = Mix_LoadWAV(sndPath.c_str());

		ATOMIC_UNLOCK(ThreadManager::fileLock);

		if (!tempSound){
			Error("Could not load file: " + sndPath, Mix_GetError());
		}

		chunks[name] = tempSound;
	}
}

void SoundManager::removeSound(string name){
	if (chunks.find(name) != chunks.end()){
		stopSound(name);

		Mix_FreeChunk(chunks[name]);

		chunks.erase(name);
	}
}

bool SoundManager::playSound(string name){
	if (isSoundLoaded(name)){
		if (isSoundNotPlaying(name)){
			int channel = Mix_PlayChannel(-1, chunks[name], 0);
			Mix_Volume(channel, soundVolume);

			sounds[name] = channel;

			return true;
		}
	}

	return false;
}

bool SoundManager::playSoundFor(string name, uint seconds){
	if (isSoundLoaded(name)){
		if (isSoundNotPlaying(name)){
			int channel = Mix_PlayChannelTimed(-1, chunks[name], -1, seconds * 1000);
			Mix_Volume(channel, soundVolume);

			sounds[name] = channel;

			return true;
		}
	}

	return false;
}

void SoundManager::pauseSound(string name){
	if (isSoundLoaded(name) && isSoundPlaying(name)){
		Mix_Pause(sounds[name]);
	}
}

void SoundManager::resumeSound(string name){
	if (isSoundLoaded(name) && isSoundPlaying(name)){
		Mix_Resume(sounds[name]);
	}
}

void SoundManager::stopSound(string name){
	if (isSoundLoaded(name) && isSoundPlaying(name)){
		Mix_HaltChannel(sounds[name]);
	}
}

void SoundManager::setMusic(string musicPath){
	if (music){
		Mix_FreeMusic(music);
		music = NULL;
	}

	ATOMIC_LOCK(ThreadManager::fileLock);

	music = Mix_LoadMUS(musicPath.c_str());

	ATOMIC_UNLOCK(ThreadManager::fileLock);

	if (!music){
		Error("Could not load file: " + musicPath, Mix_GetError());
	}
}

void SoundManager::setMusicVolume(float vol){
	musicVolume = (int)(vol * 128);

	Mix_VolumeMusic(musicVolume);
}

void SoundManager::playMusic(){
	Mix_PlayMusic(music, -1);
}

void SoundManager::pauseMusic(){
	Mix_PauseMusic();
}

void SoundManager::resumeMusic(){
	Mix_ResumeMusic();
}

void SoundManager::stopMusic(){
	Mix_HaltMusic();
}

bool SoundManager::isMusicStarted(){
	return Mix_PlayingMusic() == 1;
}

bool SoundManager::isMusicPaused(){
	return Mix_PausedMusic() == 1;
}

void SoundManager::channelDone(int channel){
	for (auto iter = sounds.begin(); iter != sounds.end(); iter++){
		if (iter->second == channel){
			sounds.erase(iter);
			break;
		}
	}
}

bool SoundManager::isSoundLoaded(string name){
	return chunks.find(name) != chunks.end();
}

bool SoundManager::isSoundPlaying(string name){
	return sounds.find(name) != sounds.end();
}

bool SoundManager::isSoundNotPlaying(string name){
	return sounds.find(name) == sounds.end();
}

uint SoundManager::getSoundLength(string name){
	static bool specsLoaded = false;
	static int freq;
	static Uint16 format;
	static uint byterate;
	static int channels;

	if (!specsLoaded){
		if (!Mix_QuerySpec(&freq, &format, &channels)){
			Warn("Failed to query mixer specifications", Mix_GetError());
		}

		switch (format){
		case AUDIO_U8:
		case AUDIO_S8:
			byterate = 8 / 8;
			break;
		case AUDIO_U16LSB:
		case AUDIO_S16LSB:
		case AUDIO_U16MSB:
		case AUDIO_S16MSB:
			byterate = 16 / 8;
			break;
		default:
			byterate = 0;
		}

		specsLoaded = true;
	}

	if (chunks.find(name) != chunks.end()){
		if (specsLoaded){
			//std::cout << "alen " << chunks[name]->alen << std::endl;
			//std::cout << "freq " << freq << std::endl;
			//std::cout << "byterate " << byterate << std::endl;
			//std::cout << "channels " << channels << std::endl;
			return (chunks[name]->alen) / ((freq * byterate * channels) / 1000);
		}
	}

	return 0;
}

static void outerChannelDone(int channel){
	SoundManager::getSingleton().channelDone(channel);
}