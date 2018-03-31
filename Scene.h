#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "TexturedQuad.h"
#include "Text.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	int getgameState();

private:
	void initShaders();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);


	void initOpenDoor();
	void initFinishDoor();
	void initCursor();
	void initSeleccion();

private:
	enum GameState
	{
		PLAYING, MENU, PAUSE
	};

	GameState gamestate;

	VariableTexture colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram, imagesProgram, textProgram;
	float currentTime;
	glm::mat4 projection;
	
	TexturedQuad *menu;
	Texture menuTexture;

	TexturedQuad *UI;
	Texture UITexture;

	int score;

	int posX, posY;

	enum LemmingState
	{
		BASHER, DIGGER_STATE, CLIMBER_STATE, EXPLOSION_STATE, BLOCKER_STATE, BUILDER_STATE, WALKING_LEFT_STATE,
		WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, DEAD, WIN_STATE, RESPAWN
	};

	int id;
	LemmingState lemmingsState;
	bool stateSelected;


	bool pause, x2speed;

	bool finish;

	vector < Lemming > lemmings;

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
	bool renderSeleccionLemming = false;

	bool renderSeleccionPause = false;

	bool renderSeleccionExplosion = false;
	bool exploding = false;

	int numLemmings[6];
	Text numLemmingsText;

	Text text;
};


#endif // _SCENE_INCLUDE

