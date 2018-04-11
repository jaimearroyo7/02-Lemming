#include <iostream>
#include <cmath>
#include <algorithm>
#include "AudioEngine.h"
#include <string>
#include <fmod.hpp>
using namespace std;

AudioEngine* AudioEngine::_instance;
AudioEngine* AudioEngine::getInstance()
{
	if (_instance = NULL)
		_instance = new AudioEngine();
	return _instance;
}

AudioEngine::AudioEngine()
{
	fmodSystem = NULL;
	init();
}

AudioEngine::~AudioEngine()
{
	if (fmodSystem != NULL)
		delete fmodSystem;
}

void AudioEngine::init()
{
	result = FMOD::System_Create(&fmodSystem); // Create the Studio System object.
	if (result != FMOD_OK) {
		cout << "error at systemCreate" << endl;
	}
	result = fmodSystem->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) {
		cout << "error at system init" << endl;
	}
	aeInitialized = true;
}

void AudioEngine::destroy()
{
	if (aeInitialized) {
		aeInitialized = false;
		delete fmodSystem;
	}
}

FMOD::System* AudioEngine::getSystem()
{
	return fmodSystem;
}

SoundEffect AudioEngine::loadSoundEffect(const std::string & filePath)
{

	SoundEffect effect;

	// Intentem buscar l'effecte a la cache
	auto it = sound_effect_map.find(filePath);

	if (it == sound_effect_map.end()) {
		// La cerca ha fallat -> Carreguem el fitxer
		FMOD::Sound* sound_effect = nullptr;
		std::vector<char> writable(filePath.begin(), filePath.end());
		result = fmodSystem->createSound(&*writable.begin(), FMOD_DEFAULT, 0, &sound_effect);
		if (result != FMOD_OK) {
			cout << "error creating the shound " << filePath << endl;
		}
		sound_effect_map[filePath] = sound_effect;
		effect.m_sound = sound_effect;
	}
	else {
		effect.m_sound = it->second;
	}

	return effect;
}

void SoundEffect::play() {
	AudioEngine *engine = AudioEngine::getInstance();
	engine->getSystem()->playSound(m_sound, 0, false, 0);
}

Music AudioEngine::loadMusic(const char* & filePath)
{
	Music music;

	// Intentem buscar la musica a la cache
	auto it = sound_music_map.find(filePath);

	if (it == sound_music_map.end()) {
		// La cerca ha fallat -> Carreguem el fitxer
		FMOD::Sound* music_sound = nullptr;
		cout << "error creating the stream " << filePath << endl;
		//std::vector<char> writable(filePath.begin(), filePath.end());
		result = fmodSystem->createStream(&*filePath, FMOD_LOOP_NORMAL, 0, &music_sound);
		if (result != FMOD_OK) {
			cout << "error creating the shound " << filePath << endl;
		}
		std::string str(filePath);
		sound_music_map[str] = music_sound;
		music.m_music = music_sound;
	}
	else {
		music.m_music = it->second;
	}
	cout << "music loaded" << endl;
	return music;
}

void Music::play() {
	cout << "play" << endl;
	AudioEngine *engine = AudioEngine::getInstance();
	engine->getSystem()->playSound(m_music, 0, false, 0);
	cout << "Play sound" << endl;
}