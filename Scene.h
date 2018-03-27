#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "TexturedQuad.h"


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

private:
	enum GameState
	{
		PLAYING, MENU, PAUSE
	};

	GameState gamestate;

	Texture colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram, imagesProgram;
	float currentTime;
	glm::mat4 projection;
	
	TexturedQuad *menu;
	Texture menuTexture;

	int score;

	int posX, posY;

	int id;
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
};


#endif // _SCENE_INCLUDE

