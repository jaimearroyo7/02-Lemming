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
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture &spritesheet, float cooldown);
	int update(int deltaTime, float seconds);
	void render(float scroll);
	
	void setMapMask(VariableTexture *mapMask, VariableTexture *mapColor);

	int setState(int stateID);
	int getState();
	int getCountdown();
	void setCountdown(float value);

	Sprite *getSprite();
	
private:
	void createSprite(ShaderProgram &shaderProgram, Texture &spritesheet);

	int collisionFloor(int maxFall);
	float cooldown; 
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
		WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, DEAD, WIN_STATE, RESPAWN, VOLTERETA, BUILDER_OK
	};

	LemmingState state;
	LemmingState prevState;
	Sprite *sprite;
	VariableTexture *mask;
	VariableTexture *color;

};


#endif // _LEMMING_INCLUDE


