#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	WALKING_LEFT, WALKING_RIGHT, BLOCKER_ANIM, WIN, DIG, BASH_RIGHT, BASH_LEFT
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int cooldown)
{


	this->cooldown = cooldown;
	count = 0;
	bashed = digged = climbed = false;
	right = true;

	state = FALLING_RIGHT_STATE;
	spritesheet.loadFromFile("images/lemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.0f/16.0f, 1.0f/7.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(7);
		sprite->setAnimationSpeed(WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 16, 0.0f));
		
		sprite->setAnimationSpeed(WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(0.5f + (float(i) / 16), 0.0f));

		sprite->setAnimationSpeed(BLOCKER_ANIM, 12);
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(BLOCKER_ANIM, glm::vec2(float(i) / 16, 1.0f / 7.0f));

		sprite->setAnimationSpeed(WIN, 12);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(WIN, glm::vec2(float(i) / 16, 2.0f / 7.0f));

		sprite->setAnimationSpeed(DIG, 9);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIG, glm::vec2(0.5f +float(i) / 16, 2.0f / 7.0f));

		sprite->setAnimationSpeed(BASH_RIGHT, 9);
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(BASH_RIGHT, glm::vec2(float(i) / 16, 3.0f / 7.0f));
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(BASH_RIGHT, glm::vec2(float(i) / 16, 4.0f / 7.0f));

		sprite->setAnimationSpeed(BASH_LEFT, 9);
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(BASH_LEFT, glm::vec2(float(i) / 16, 5.0f / 7.0f));
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(BASH_LEFT, glm::vec2(float(i) / 16, 6.0f / 7.0f));
		
	sprite->changeAnimation(WALKING_RIGHT);
	sprite->setPosition(initialPosition);
}


int Lemming::keepWalking(int dir) {
	int fall;
	sprite->position() += glm::vec2(dir, -1);
	if (win()) {
		state = DEAD;
		sprite->changeAnimation(WIN);
		return 1;
	}
	else if (collision() && state != BASHER && state != CLIMBER_STATE)
	{
		sprite->position() -= glm::vec2(dir, -1);
		if (dir == 1) {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
			right = false;
		}
		else {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
			right = true;

		}
	}
	else
	{
		fall = collisionFloor(3);
		if (fall < 3)
			sprite->position() += glm::vec2(0, fall);
		else
			state = FALLING_RIGHT_STATE;
	}
	return 0;
}

int Lemming::update(int deltaTime, int seconds)
{
	int fall;
	int dir = -1;
	if (right) dir = 1;

	if (cooldown > seconds) return 0;
	else show = true;

	if (state == DEAD && sprite->getKeyframe() == 7) return 0;

	if(sprite->update(deltaTime) == 0)
		return 0;

	++count;
	if (count >= 4) count = 0;

	switch(state)
	{
		case BLOCKER_STATE:
			fall = collisionFloor(2);
			if (fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 1)
				sprite->position() += glm::vec2(0, 1);
			break;

		case CLIMBER_STATE:
			if (collision()) {
				cout << "Hola" << endl;
				climbed = true;
				glm::ivec2 pos = sprite->position();
				pos += glm::vec2(120, 0);
				sprite->position() += glm::vec2(0, -1);
			}
			else {
				if (climbed) {
					if (right) {
						state = WALKING_RIGHT_STATE;
						sprite->changeAnimation(WALKING_RIGHT);
					}
					else {
						state = WALKING_LEFT_STATE;
						sprite->changeAnimation(WALKING_LEFT);
					}
					climbed = false;
				}
				else {
					return keepWalking(dir);
				}
			}

			break;
		case BASHER:
			if (collision()) {
				if (count % 4 == 0) {
					count = 0;
					glm::ivec2 pos = sprite->position();
					pos += glm::vec2(120, 0);

					if (right) {
						if (!bashed) {
							sprite->changeAnimation(BASH_RIGHT);
							bashed = true;
						}
						for (int i = 0; i < 12; ++i)
							mask->setPixel(pos.x + 8, pos.y + i + 4, 0);
					}
					else {
						if (!bashed) {
							sprite->changeAnimation(BASH_LEFT);
							bashed = true;
						}
						for (int i = 0; i < 12; ++i)
							mask->setPixel(pos.x + 7, pos.y + i + 4, 0);

					}
					sprite->position() += glm::vec2(dir, 0);
				}
			}
			else {
				if (bashed) {
					if (right) {
						state = WALKING_RIGHT_STATE;
						sprite->changeAnimation(WALKING_RIGHT);
					}
					else {
						state = WALKING_LEFT_STATE;
						sprite->changeAnimation(WALKING_LEFT);
					}
					bashed = false;
				}
				else {
					return keepWalking(dir);
				}
			}
		
		
			break;

		case DIGGER_STATE:
		
			fall = collisionFloor(2);
			if (fall == 0 && count % 3 == 0) {
				count = 0;
				if(!digged) sprite->changeAnimation(DIG);
				digged = true;
				glm::ivec2 pos = sprite->position();
				pos += glm::vec2(120, 0);
				for(int i = 0; i < 10; ++i)
					mask->setPixel(pos.x + i + 4, pos.y + 16, 0);

				sprite->position() += glm::vec2(0, 1);
			}
			if (fall > 0) {
				if (digged) {
					if (right) {
						state = FALLING_RIGHT_STATE;
						sprite->changeAnimation(WALKING_RIGHT);
					}
					else {
						state = FALLING_LEFT_STATE;
						sprite->changeAnimation(WALKING_LEFT);
					}
					digged = false;
				}
				else
					sprite->position() += glm::vec2(0, fall);
			}
			break;

		case FALLING_LEFT_STATE:
			fall = collisionFloor(2);
			if(fall > 0)
				sprite->position() += glm::vec2(0, fall);
			else {
				state = WALKING_LEFT_STATE;
				right = false;
			}
			break;
		case FALLING_RIGHT_STATE:
			fall = collisionFloor(2);
			if(fall > 0)
				sprite->position() += glm::vec2(0, fall);
			else {
				state = WALKING_RIGHT_STATE;
				right = true;
			}
			break;
		case WALKING_LEFT_STATE:
			return keepWalking(dir);
			break;
		case WALKING_RIGHT_STATE:
			return keepWalking(dir);
			break;
	}
	return 0;
}

void Lemming::render()
{
	if(show && (state != DEAD || sprite->getKeyframe() != 7)) sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) == 0) && (mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	//cout << int(mask->pixel(posBase.x+1, posBase.y)) << endl;
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;
	return true;
}

bool Lemming::win()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement

	posBase += glm::ivec2(7, 15);
	if ((mask->pixel(posBase.x, posBase.y) == 100) || (mask->pixel(posBase.x + 1, posBase.y) == 100))
		return true;

	return false;
}


void Lemming::setState(int stateId) {

	switch (stateId) {
	case BLOCKER_STATE:
		if (state != FALLING_LEFT_STATE && state != FALLING_RIGHT_STATE){
			glm::ivec2 pos = sprite->position();
			pos += glm::vec2(120, 0);
			for (int y = max(0, pos.y + 6); y <= min(mask->height() - 1, pos.y + 17); y++)
				for (int x = max(0, pos.x + 3); x <= min(mask->width() - 1, pos.x + 13); x++)
					mask->setPixel(x, y, 255);
			state = BLOCKER_STATE;
			sprite->changeAnimation(BLOCKER_ANIM);
		}
		break;
		case DIGGER_STATE:
			state = DIGGER_STATE;
			break;

		case CLIMBER_STATE:
			state = CLIMBER_STATE;
			break;

		case BASHER:
			state = BASHER;
			break;
		default:
			break;
	}
	
}

Sprite *Lemming::getSprite() {
	return sprite;
}

int Lemming::getState() {
	return state;
}


