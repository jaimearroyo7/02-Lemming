#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <iostream>


void Game::init()
{
	
	bPlay = true;
	bLeftMouse = bRightMouse = false;
	lemingAction = -1;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
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

void Game::keyPressed(int key)
{
	switch (key)
	{
	case 27: // Escape code
		bPlay = false;
		break;
	case 49: // 1 code
		lemingAction = 1;
		break;
	case 50: // 2 code
		lemingAction = 2;
		break;
	case 51: // 3 code
		lemingAction = 3;
		break;
	case 52: // 4 code
		lemingAction = 4;
		break;
	case 114: // R code
		scene.init();
		break;
	default: 
		lemingAction = -1;
	}
	keys[key] = true;
	cout << key;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
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
	scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse, 0);
}

void Game::mousePress(int button)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		bLeftMouse = true;
		scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse, lemingAction);
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		bRightMouse = true;
		scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse, lemingAction);
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





