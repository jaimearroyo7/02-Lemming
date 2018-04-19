#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	
	bPlay = true;
	bLeftMouse = bRightMouse = false;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	scene.init(0);
}

bool Game::update(int deltaTime)
{
	ShowCursor(false);
	scene.update(deltaTime);
	

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::entryfuncCallback(int state)
{
	scene.entryfuncCallback(state);
}

void Game::keyPressed(int key)
{
	scene.keyPressed(key);
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	scene.specialKeyPressed(key);
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;
	scene.mouseMoved(mouseX, mouseY, false, false);
}

void Game::mousePress(int button)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		bLeftMouse = true;
		scene.mousePress(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		bRightMouse = true;
		scene.mousePress(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
}

void Game::mouseRelease(int button)
{
	if(button == GLUT_LEFT_BUTTON)
		bLeftMouse = false;
	else if(button == GLUT_RIGHT_BUTTON)
		bRightMouse = false;
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::setbPlay(bool value) {
	bPlay = value;
}


