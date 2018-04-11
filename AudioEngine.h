#ifndef _AUDIOENGINE_INCLUDE
#define _AUDIOENGINE_INCLUDE


#include "fmod.hpp"
#include <vector>
#include <map>
using namespace std;


// AudioEngine contains all the sounds of our game.

class Music 
{
public:
	friend class AudioEngine;

	void play();
private:
	FMOD::Sound *m_music = nullptr;
};

class SoundEffect
{
public:
	friend class AudioEngine;

	void play();
private:
	FMOD::Sound *m_sound = nullptr;
};

class AudioEngine
{
	static AudioEngine* _instance;

public:
	friend class Music;
	friend class SoundEffect;
	static AudioEngine* getInstance();
	AudioEngine();
	~AudioEngine();

	void init();
	void destroy();

	FMOD::System* getSystem();

	SoundEffect loadSoundEffect(const std::string& filePath);
	Music loadMusic(const char*& filePath);

private:

private:
	enum GameState
	{
		PLAYING, MENU, PAUSE
	};

	enum LemmingSounds
	{
		BASHER, DIGGER_STATE, CLIMBER_STATE, EXPLOSION_STATE, BLOCKER_STATE, BUILDER_STATE, WALKING_LEFT_STATE,
		WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, DEAD, WIN_STATE, RESPAWN, VOLTERETA
	};

	FMOD::System *fmodSystem;
	FMOD::Sound *background;
	vector <FMOD::Sound> lemmings;
	FMOD_RESULT result;
	unsigned int version;
	bool aeInitialized;

	std::map<std::string, FMOD::Sound*> sound_effect_map;
	std::map<std::string, FMOD::Sound*> sound_music_map;
};


#endif // _AUDIOENGINE_INCLUDE