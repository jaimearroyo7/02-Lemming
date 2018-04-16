#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "TexturedQuad.h"
#include "Text.h"
#include "Level.h"
#include <fmod.hpp>

// Scene contains all the entities of our game.
// It is responsible for updating and render them.



class Scene
{

public:
	Scene();

	static Scene &instance() {
		static Scene S;

		return S;
	}

	~Scene();

	void init(int level);
	void initLevel(const Level &l);
	void update(int deltaTime);
	void render();

	FMOD::System* Scene::getSoundSystem();
	
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	void specialKeyPressed(int key);
	void entryfuncCallback(int state);

	int getgameState();

private:
	void initShaders();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);


	void initOpenDoor(const Level &l);
	void initFinishDoor(const Level &l);
	void initCursor();
	void initSeleccion();

	void freeScene();

	void loadSounds();
	void setBackgroundMusic(int level);

private:
	enum GameState
	{
		PLAYING, MENU, PAUSE, SELECT_LEVEL, LEVEL_INFO, WIN, LOSE
	};

	GameState gamestate;

	VariableTexture colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram, imagesProgram, textProgram;
	float currentTime;
	float winTime;
	glm::mat4 projection;
	
	bool load = false;
	bool free = false;
	TexturedQuad *menu;
	Texture menuTexture;

	Texture selectLevel;
	bool levelSelectClick;
	float alpha;
	int transitionTime;

	Texture level1Info, level2Info, level3Info, level4Info, level5Info;

	Texture background;

	TexturedQuad *UI;
	Texture UITexture;

	bool onePlayer;


	int numLevel;
	int score;
	int needToWin;
	int levelTime;

	int posX, posY;

	enum LemmingState
	{
		BASHER, DIGGER_STATE, CLIMBER_STATE, EXPLOSION_STATE, BLOCKER_STATE, BUILDER_STATE, WALKING_LEFT_STATE,
		WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, DEAD, WIN_STATE, RESPAWN, VOLTERETA
	};

	int id;
	LemmingState lemmingsState;
	bool stateSelected;
	float scroll;

	bool pause, x2speed;

	bool finish, allOut;

	vector < Lemming > lemmings;
	Texture spritesheetLemmings;

	//start Door
	Sprite *openDoor;
	Texture spritesheetDoor;

	//finish Door
	Sprite *finishDoor;
	Texture spritesheetfinishDoor;

	Sprite *cursor;
	Texture spritesheetCursor;

	glm::vec2 lemmingSelected;
	Sprite *seleccionLemming;
	Texture spritesheetSeleccionLemming;
	bool renderSeleccionLemming;

	bool renderSeleccionPause;

	bool renderSeleccionExplosion;
	bool exploding = false;

	string lemmingHover;
	int totalLemmings;
	int out, in;
	int numLemmings[6];
	
	Text levelInfo;
	Text countdownText;


	Level L1;
	Level L2;
	Level L3;
	Level L4;

	//Sound
	FMOD::System *system;
	FMOD::Sound *menuLoop;
	FMOD::Sound *lvl1Loop;
	FMOD::Sound *lvl2Loop;
	FMOD::Sound *lvl3Loop;
	FMOD::Sound *lvl4Loop;
	FMOD::Sound *doorOpen;
	FMOD::Channel *channel0 = 0;
	FMOD::Channel *channel1 = 0;
	FMOD::Channel *channel2 = 0;
	FMOD::Channel *channel3 = 0;
	FMOD::Channel *channel4 = 0;
	FMOD::Channel *channelDoor = 0;
	int currentChannel;


};

#endif // _SCENE_INCLUDE

