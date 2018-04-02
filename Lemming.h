#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"
#include <cmath>
#include <algorithm>

// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int cooldown);
	int update(int deltaTime, int seconds);
	void render();
	
	void setMapMask(VariableTexture *mapMask, VariableTexture *mapColor);

	void setState(int stateID);
	int getState();
	int getCountdown();

	Sprite *getSprite();
	
private:
	int collisionFloor(int maxFall);
	int cooldown; 
	int count;

	bool digged, bashed, climbed, firstStair;
	int builderCount;
	float explosionCountdown;

	bool show = false;
	bool right;
	bool collision(int offset);
	bool collisionHead();
	bool win();

	int keepWalking(int dir);

private:
	enum LemmingState
	{
		BASHER, DIGGER_STATE, CLIMBER_STATE, EXPLOSION_STATE, BLOCKER_STATE, BUILDER_STATE, WALKING_LEFT_STATE,
		WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, DEAD, WIN_STATE, RESPAWN, VOLTERETA
	};

	LemmingState state;
	LemmingState prevState;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	VariableTexture *color;

};


#endif // _LEMMING_INCLUDE


