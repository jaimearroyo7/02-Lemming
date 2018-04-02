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
	WALKING_LEFT, WALKING_RIGHT, BLOCKER_ANIM, WIN, DIG, BASH_RIGHT, BASH_LEFT, EXPLODE, FALL_RIGHT, FALL_LEFT, BUILDING_RIGHT, BUILDING_LEFT,
	CLIMBING_LEFT, VOLTERETA_LEFT, CLIMBING_RIGHT, VOLTERETA_RIGHT
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int cooldown)
{

	prevState = WALKING_RIGHT_STATE;
	this->cooldown = cooldown;
	count = 0;
	explosionCountdown = -1;
	builderCount = 0;
	bashed = digged = climbed = false;
	right = true;
	firstStair = true;

	state = RESPAWN;
	spritesheet.loadFromFile("images/lemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.0f/16.0f, 1.0f/14.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(16);
		sprite->setAnimationSpeed(WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 16, 0.0f));
		
		sprite->setAnimationSpeed(WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(0.5f + (float(i) / 16), 0.0f));

		sprite->setAnimationSpeed(BLOCKER_ANIM, 12);
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(BLOCKER_ANIM, glm::vec2(float(i) / 16, 1.0f / 14.0f));

		sprite->setAnimationSpeed(WIN, 12);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(WIN, glm::vec2(float(i) / 16, 2.0f / 14.0f));

		sprite->setAnimationSpeed(DIG, 9);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIG, glm::vec2(0.5f +float(i) / 16, 2.0f / 14.0f));

		sprite->setAnimationSpeed(BASH_RIGHT, 9);
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(BASH_RIGHT, glm::vec2(float(i) / 16, 3.0f / 14.0f));
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(BASH_RIGHT, glm::vec2(float(i) / 16, 4.0f / 14.0f));

		sprite->setAnimationSpeed(BASH_LEFT, 9);
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(BASH_LEFT, glm::vec2(float(i) / 16, 5.0f / 14.0f));
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(BASH_LEFT, glm::vec2(float(i) / 16, 6.0f / 14.0f));
		
		sprite->setAnimationSpeed(EXPLODE, 9);
		for (int i = 0; i<15; i++)
			sprite->addKeyframe(EXPLODE, glm::vec2(float(i) / 16, 7.0f / 14.0f));

		sprite->setAnimationSpeed(FALL_RIGHT, 12);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FALL_RIGHT, glm::vec2(float(i) / 16, 8.0f / 14.0f));

		sprite->setAnimationSpeed(FALL_LEFT, 12);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FALL_LEFT, glm::vec2(0.25f + (float(i)) / 16, 8.0f / 14.0f));

		sprite->setAnimationSpeed(BUILDING_RIGHT, 12);
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(BUILDING_RIGHT, glm::vec2(float(i) / 16, 9.0f / 14.0f));

		sprite->setAnimationSpeed(BUILDING_LEFT, 12);
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(BUILDING_LEFT, glm::vec2(float(i) / 16, 10.0f / 14.0f));

		sprite->setAnimationSpeed(CLIMBING_RIGHT, 10);
		for (int i = 0; i<9; i++)
			sprite->addKeyframe(CLIMBING_RIGHT, glm::vec2(float(i) / 16, 11.0f / 14.0f));

		sprite->setAnimationSpeed(VOLTERETA_RIGHT, 10);
		for (int i = 0; i<7; i++)
			sprite->addKeyframe(VOLTERETA_RIGHT, glm::vec2(9.0f/16.0f + (float(i) / 16), 11.0f / 14.0f));

		sprite->setAnimationSpeed(CLIMBING_LEFT,  12);
		for (int i = 0; i<9; i++)
			sprite->addKeyframe(CLIMBING_LEFT, glm::vec2(float(i) / 16, 12.0f / 14.0f));

		sprite->setAnimationSpeed(VOLTERETA_LEFT, 12);
		for (int i = 0; i<7; i++)
			sprite->addKeyframe(VOLTERETA_LEFT, glm::vec2(9.0f / 16.0f + (float(i) / 16), 12.0f / 14.0f));

	sprite->changeAnimation(FALL_RIGHT);
	sprite->setPosition(initialPosition);
}

int Lemming::keepWalking(int dir) {
	int fall;
	sprite->position() += glm::vec2(dir, -1);
	if (win()) {
		state = WIN_STATE;
		sprite->changeAnimation(WIN);
		return 1;
	}
	else if ((collision(0)) && state != BASHER && state != CLIMBER_STATE)
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
		else {
			if (dir == 1) {
				prevState = state;
				state = FALLING_RIGHT_STATE;
				sprite->changeAnimation(FALL_RIGHT);
			}
			else {
				prevState = state;
				state = FALLING_LEFT_STATE;
				sprite->changeAnimation(FALL_LEFT);
			}
		}
	}
	return 0;
}

int Lemming::update(int deltaTime, int seconds)
{
	int fall;
	int dir = -1;
	if (right) dir = 1;

	if (cooldown > seconds) return 0;
	else if(!show){
		show = true;
		state = FALLING_RIGHT_STATE;
		sprite->changeAnimation(FALL_RIGHT);
	};

	if (state == DEAD) return 0;
	if (explosionCountdown != -1) { 
		if(explosionCountdown < 5100.0f)
			explosionCountdown += deltaTime;
		else if(state != EXPLOSION_STATE){
			state = EXPLOSION_STATE;
			sprite->changeAnimation(EXPLODE);
		}
	}

	if (sprite->update(deltaTime) == 0)
		return 0;

	++count;
	if (count >= 4) count = 0;

	int radius = 14;
	glm::ivec2 pos = sprite->position();
	pos += glm::vec2(120, 0);

	switch(state)
	{
		case BUILDER_STATE:
			if (builderCount == 13 || collisionHead()) {
				if (dir == 1) {
					sprite->changeAnimation(WALKING_RIGHT);
					state = WALKING_RIGHT_STATE;
					right = true;
				}
				else {
					sprite->changeAnimation(WALKING_LEFT);
					state = WALKING_LEFT_STATE;
					right = false;
				}
			}
			else if (sprite->getKeyframe() == 0 && !firstStair) {

				sprite->position() += glm::vec2(2*dir, -1);
				if (win()) {
					state = WIN_STATE;
					sprite->changeAnimation(WIN);
					return 1;
				}
				if (right) {					
					if (collision(3)) {
						state = WALKING_RIGHT_STATE;
						sprite->changeAnimation(WALKING_RIGHT);
					}
				}
				else {
					if (collision(3)) {
						state = WALKING_LEFT_STATE;
						sprite->changeAnimation(WALKING_LEFT);
					}
				}
				++builderCount;

			}
			else if (sprite->getKeyframe() == 10) {
				firstStair = false;
				if (right) {
					pos += glm::vec2(9, 15);
					for (int i = 0; i < 4; ++i) {
						mask->setPixel(pos.x + i * dir, pos.y, 255);
						color->setPixel(pos.x + i * dir, pos.y, glm::ivec4(119, 77, 0, 255));
					}
				}
				else {
					pos += glm::vec2(5, 15);
					for (int i = 0; i < 4; ++i) {
						mask->setPixel(pos.x + i * dir, pos.y, 255);
						color->setPixel(pos.x + i * dir, pos.y, glm::ivec4(120, 77, 0, 255));
					}
				}
			}

			break;
		case EXPLOSION_STATE:
			//if (explosionCountdown >= 5000.0f) {
				if (sprite->getKeyframe() == 14) {
					pos += glm::vec2(8, 8);
					for (int y = -radius; y <= radius; y++)
						for (int x = -radius; x <= radius; x++)
							if (x*x + y * y <= radius * radius) {
								mask->setPixel(pos.x + x, pos.y + y, 0);
								color->setPixel(pos.x + x, pos.y + y, glm::ivec4(0, 0, 0, 0));
							}
					state = DEAD;
					explosionCountdown = -1;
				}
				
			//}
			break;
		case WIN_STATE:
			if (sprite->getKeyframe() == 7)
				state = DEAD;
			break;

		case BLOCKER_STATE:
			fall = collisionFloor(2);
			if (fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 1)
				sprite->position() += glm::vec2(0, 1);
			break;

		case CLIMBER_STATE:
			if (win()) {
				state = WIN_STATE;
				sprite->changeAnimation(WIN);
				return 1;
			}

			if (collision(0)) {
				cout << "collision" << endl;
				if (!climbed) {
					if (right) sprite->changeAnimation(CLIMBING_RIGHT);
					else sprite->changeAnimation(CLIMBING_LEFT);
				}
				climbed = true;
				if(sprite->getKeyframe() <= 4)
				sprite->position() += glm::vec2(0, -1);
			}
			else {
				if (sprite->animation() != WALKING_RIGHT && sprite->animation() != WALKING_LEFT) {
					if (right) sprite->changeAnimation(WALKING_RIGHT);
					else sprite->changeAnimation(WALKING_LEFT);
					climbed = false;
				}
				return keepWalking(dir);
			}

			if (collisionHead()) {

				cout << "collisionHEAD" << endl;
				prevState = CLIMBER_STATE;
				climbed = false;
				sprite->position() += glm::vec2(-dir, 0);

				if (right) {
					state = FALLING_LEFT_STATE;
					sprite->changeAnimation(FALL_LEFT);
				}
				else {
					state = FALLING_RIGHT_STATE;
					sprite->changeAnimation(FALL_RIGHT);
				}
				right = !right;
			}

			break;
		case BASHER:
			if (win()) {
				state = WIN_STATE;
				sprite->changeAnimation(WIN);
				return 1;
			}
			if (collision(0)) {
				if (count % 3 == 0) {
					count = 0;

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
			if (win()) {
				state = WIN_STATE;
				sprite->changeAnimation(WIN);
				return 1;
			}
			fall = collisionFloor(2);
			if (fall == 0 && count % 3 == 0) {
				count = 0;
				if(!digged) sprite->changeAnimation(DIG);
				digged = true;
				glm::ivec2 pos = sprite->position();
				pos += glm::vec2(120, 0);
				for (int i = 0; i < 10; ++i) {
					mask->setPixel(pos.x + i + 4, pos.y + 16, 0);
					color->setPixel(pos.x + i + 4, pos.y + 16, glm::ivec4(0, 0, 0, 0));
				}
				sprite->position() += glm::vec2(0, 1);
			}
			if (fall > 0) {
				if (digged) {
					if (right) {
						prevState = WALKING_RIGHT_STATE;
						state = FALLING_RIGHT_STATE;
						sprite->changeAnimation(FALL_RIGHT);
					}
					else {
						prevState = WALKING_LEFT_STATE;
						state = FALLING_LEFT_STATE;
						sprite->changeAnimation(FALL_LEFT);
					}
					digged = false;
				}
				else
					sprite->position() += glm::vec2(0, fall);
			}
			break;

		case FALLING_LEFT_STATE:
			fall = collisionFloor(2);
			cout << fall << endl;
			if (fall > 0) {
				cout << "hola" << endl;
				sprite->position() += glm::vec2(0, fall);
			}
			else {
				state = prevState;
				sprite->changeAnimation(WALKING_LEFT);
				right = false;
			}
			break;
		case FALLING_RIGHT_STATE:
			fall = collisionFloor(2);
			if(fall > 0)
				sprite->position() += glm::vec2(0, fall);
			else {
				state = prevState;
				sprite->changeAnimation(WALKING_RIGHT);
				right = true;
			}
			break;
		case WALKING_LEFT_STATE:
			return keepWalking(dir);
			break;
		case WALKING_RIGHT_STATE:
			return keepWalking(dir);
			break;
		case RESPAWN:
			break;
	}
	return 0;
}

void Lemming::render()
{
	if(state != DEAD && state != RESPAWN) sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMask, VariableTexture *mapColor)
{
	mask = mapMask;
	color = mapColor;
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

bool Lemming::collision(int offset)
{

	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7 + offset, 15);

	//si los lemmings van en dirección opuesta a una escalera, no hay colision
	
	if(!right && (mask->pixel(posBase.x, posBase.y) == 0 || int(color->pixel(posBase.x, posBase.y)) == 119)
		&& (mask->pixel(posBase.x+1, posBase.y) == 0 || int(color->pixel(posBase.x + 1, posBase.y)) == 119))
		return false;

	if (right && (mask->pixel(posBase.x, posBase.y) == 0 || int(color->pixel(posBase.x, posBase.y)) == 120)
		&& (mask->pixel(posBase.x + 1, posBase.y) == 0 || int(color->pixel(posBase.x + 1, posBase.y)) == 120))
		return false;

	return true;
}

bool Lemming::collisionHead()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	int offset;
	if (right) offset = 6;
	else offset = 8;
	posBase += glm::ivec2(offset, 7);

	if (!right && (mask->pixel(posBase.x, posBase.y) == 0 || int(color->pixel(posBase.x, posBase.y)) == 119)
		&& (mask->pixel(posBase.x + 1, posBase.y) == 0 || int(color->pixel(posBase.x + 1, posBase.y)) == 119))
		return false;

	if (right && (mask->pixel(posBase.x, posBase.y) == 0 || int(color->pixel(posBase.x, posBase.y)) == 120)
		&& (mask->pixel(posBase.x + 1, posBase.y) == 0 || int(color->pixel(posBase.x + 1, posBase.y)) == 120))
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
	glm::ivec2 pos = sprite->position();
	pos += glm::vec2(120, 0);
	switch (stateId) {
		case BUILDER_STATE:
			state = BUILDER_STATE;
			if(right) sprite->changeAnimation(BUILDING_RIGHT);
			else sprite->changeAnimation(BUILDING_LEFT);
			break;
		case BLOCKER_STATE:
			if (state != FALLING_LEFT_STATE && state != FALLING_RIGHT_STATE){
				glm::ivec2 pos = sprite->position();
				pos += glm::vec2(120, 0);
				for (int y = max(0, pos.y + 6); y <= min(mask->height() - 1, pos.y + 6); y++)
					for (int x = max(0, pos.x); x <= min(mask->width() - 1, pos.x + 3); x++)
						color->setPixel(x, y, glm::ivec4(0, 0, 0, 0));

				for (int y = max(0, pos.y + 6); y <= min(mask->height() - 1, pos.y + 17); y++)
					for (int x = max(0, pos.x + 3); x <= min(mask->width() - 1, pos.x + 13); x++) {
						mask->setPixel(x, y, 255);
						color->setPixel(x, y, glm::ivec4(0, 0, 0, 0));
					}
				state = BLOCKER_STATE;
				sprite->changeAnimation(CLIMBING_RIGHT);
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
		case EXPLOSION_STATE:
			explosionCountdown = 0.0f;
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

int Lemming::getCountdown() {
	return explosionCountdown;
}

