#include <iostream>
#include <cmath>
#include <algorithm>
#include "AudioEngine.h"
#include <string>
#include <fmod.hpp>
using namespace std;

AudioEngine::~AudioEngine()
{
	/*if (fmodSystem != NULL)
		delete fmodSystem;*/
}

void AudioEngine::init()
{
	result = FMOD::System_Create(&fmodSystem); // Create the Studio System object.
	if (result != FMOD_OK) {
		cout << "error at systemCreate" << endl;
	}
	else {
		iniciat = "systema iniciat!";
		cout << iniciat << " " << result << endl;
	}
	result = fmodSystem->init(512, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) {
		cout << "error at system init" << endl;
	}
	else cout << "system initiated correctly" << endl;
	/*string fp = "sounds/DOOR.wav";
	//loadMusic(fp).play();
	loadSoundEffect(fp).play();
	fmodSystem->createSound(fp.c_str(), FMOD_2D, 0, &background);
	fmodSystem->playSound(loadSoundEffect(fp).m_sound, 0, false, 0);*/
}

void AudioEngine::update() {
	fmodSystem->update();
}

void AudioEngine::playLoop(const std::string & songPath) {
	cout << endl << endl << "PLAY LOOP" << endl;
	cout << iniciat << endl;
	channelBackground->stop();
	FMOD_RESULT result = fmodSystem->playSound(loadMusic(songPath).m_music, 0, false, &channelBackground);
	if (result != FMOD_OK) cout << "error playing loop" << result << endl;
	else cout << "playing loop " << result << endl;
	cout << endl;
}

void AudioEngine::play(const std::string & songPath) {
	cout << endl << endl << "PLAY LOOP" << endl;
	cout << iniciat << endl;
	FMOD_RESULT result = fmodSystem->playSound(loadSoundEffect(songPath).m_sound, 0, false, &channelSoundEffect);
	if (result != FMOD_OK) cout << "error playing loop" << result << endl;
	else cout << "playing loop " << result << endl;
	cout << endl;
}

void AudioEngine::destroy()
{
	delete fmodSystem;
}

SoundEffect AudioEngine::loadSoundEffect(const std::string & filePath)
{

	SoundEffect effect;

	// Intentem buscar l'effecte a la cache
	auto it = sound_effect_map.find(filePath);

	if (it == sound_effect_map.end()) {
		// La cerca ha fallat -> Carreguem el fitxer
		FMOD::Sound* sound_effect;
		result = fmodSystem->createSound(filePath.c_str(), FMOD_2D, 0, &sound_effect);
		if (result != FMOD_OK) {
			cout << "error creating the shound " << filePath << endl;
		}
		else cout << "soundEffect created" << endl;
		sound_effect_map[filePath] = sound_effect;
		effect.m_sound = sound_effect;
		//fmodSystem->playSound(effect.m_sound, 0, false, 0);
	}
	else {
		effect.m_sound = it->second;
	}

	return effect;
}

Music AudioEngine::loadMusic(const string & filePath)
{
	Music music;

	// Intentem buscar la musica a la cache
	auto it = sound_music_map.find(filePath);

	if (it == sound_music_map.end()) {
		// La cerca ha fallat -> Carreguem el fitxer
		FMOD::Sound* music_sound = nullptr;
		//std::vector<char> writable(filePath.begin(), filePath.end());
		result = fmodSystem->createStream(filePath.c_str(), FMOD_LOOP_NORMAL, 0, &music_sound);
		if (result != FMOD_OK) {
			cout << "error creating the shound " << filePath << endl;
		}
		else cout << "file loaded correctly: " << filePath << endl;
		sound_music_map[filePath] = music_sound;
		music.m_music = music_sound;
	}
	else {
		music.m_music = it->second;
	}
	cout << "music loaded" << endl;
	unsigned int *length = 0;
	music.m_music->getLength(length, FMOD_TIMEUNIT_MS);
	cout << "Length :" << length << endl;
	return music;
}