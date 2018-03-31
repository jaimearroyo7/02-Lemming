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

	void setState(int animId);
	int getState();

	Sprite *getSprite();
	
private:
	int collisionFloor(int maxFall);
	int cooldown; 
	int count;

	bool digged, bashed, climbed, firstStair;
	int builderCount;

	bool show = false;
	bool right;
	bool collision(int offset);
	bool collisionHead();
	bool win();

	int keepWalking(int dir);

private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, DEAD, BLOCKER_STATE, 
		BASHER, DIGGER_STATE, CLIMBER_STATE, EXPLOSION_STATE, WIN_STATE, RESPAWN, BUILDER_STATE
	};

	LemmingState state;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	VariableTexture *color;

};


#endif // _LEMMING_INCLUDE


