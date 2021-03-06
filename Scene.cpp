#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <time.h>
#include <fmod.hpp>
using namespace std;



Scene::Scene()
{
	posX = posY = 100;
	gamestate = MENU;
	map = NULL;

	//Level 1
	L1.levelLemmings = 10;
	L1.needToWin = 1;
	L1.mapLength = 512.0;
	L1.colortexture = "images/fun1.png";
	L1.masktexture = "images/fun1_mask.png";
	L1.openDoortexture = "images/openDoor.png";
	L1.openDoorPos = glm::vec2(77, 30);
	L1.finishDoortexture = "images/finishDoor.png";
	L1.finishDoorPos = glm::vec2(225, 107);
	L1.finishDoorSize = glm::ivec2(33, 26);
	L1.levelTime = 300000;
	L1.ratio = 1;
	L1.numLemmings[0] = 10;
	L1.numLemmings[1] = 10;
	L1.numLemmings[2] = 0;
	L1.numLemmings[3] = 5;
	L1.numLemmings[4] = 0;
	L1.numLemmings[5] = 0;
	L1.numLemmings[6] = 3;
	L1.bounds = glm::vec4(-100, 360, 160, 2);
	L1.levelSong = "sounds/lemmings.mid";

	//Level 2
	L2.levelLemmings = 50;
	L2.needToWin = 5;
	L2.mapLength = 848.0;
	L2.colortexture = "images/fun5.png";
	L2.masktexture = "images/fun5_mask.png";
	L2.openDoortexture = "images/openDoorGrey.png";
	L2.openDoorPos = glm::vec2(25, 20);
	L2.finishDoortexture = "images/finishDoorBlue.png";
	L2.finishDoorSize = glm::ivec2(38, 22);
	L2.finishDoorPos = glm::vec2(485, 109);
	L2.levelTime = 300000;
	L2.ratio = 1;
	L2.numLemmings[0] = 10;
	L2.numLemmings[1] = 2;
	L2.numLemmings[2] = 10;
	L2.numLemmings[3] = 20;
	L2.numLemmings[4] = 10;
	L2.numLemmings[5] = 0;
	L2.numLemmings[6] = 15;
	L2.bounds = glm::vec4(-100, 848-120-30, 160, 2);
	L2.levelSong = "sounds/lvl2.mp3";

	//Level 3
	L3.levelLemmings = 5;
	L3.needToWin = 4;
	L3.mapLength = 1211.0;
	L3.colortexture = "images/tricky6.png";
	L3.masktexture = "images/tricky6_mask.png";
	L3.openDoortexture = "images/openDoorGrey.png";
	L3.openDoorPos = glm::vec2(305, 30);
	L3.finishDoortexture = "images/finishDoorFire.png";
	L3.finishDoorPos = glm::vec2(630, 85);
	L3.finishDoorSize = glm::ivec2(52, 52);
	L3.levelTime = 300000;
	L3.numLemmings[0] = 20;
	L3.numLemmings[1] = 0;
	L3.numLemmings[2] = 20;
	L3.numLemmings[3] = 20;
	L3.numLemmings[4] = 20;
	L3.numLemmings[5] = 20;
	L3.numLemmings[6] = 20;
	L3.ratio = 2;
	L3.bounds = glm::vec4(-100, 1211 - 120 - 30, 160, 2);
	L3.levelSong = "sounds/lvl3.mp3";

	//Level 4
	L4.levelLemmings = 80;
	L4.needToWin = 65;
	L4.mapLength = 1100.0;
	L4.colortexture = "images/mayhem1.png";
	L4.masktexture = "images/mayhem1_mask.png";
	L4.openDoortexture = "images/openDoor.png";
	L4.openDoorPos = glm::vec2(77, 3);
	L4.finishDoortexture = "images/finishDoor.png";
	L4.finishDoorPos = glm::vec2(710, 0);
	L4.finishDoorSize = glm::ivec2(33, 26);
	L4.levelTime = 540000;
	L4.numLemmings[0] = 0;
	L4.numLemmings[1] = 5;
	L4.numLemmings[2] = 2;
	L4.numLemmings[3] = 20;
	L4.numLemmings[4] = 10;
	L4.numLemmings[5] = 40;
	L4.numLemmings[6] = 7;
	L4.ratio = 0.8;
	L4.bounds = glm::vec4(-100, 1100 - 120 - 30, 160, 2);
	L4.levelSong = "sounds/lvl4.mp3";
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}

void Scene::initCursor() {
	spritesheetCursor.loadFromFile("images/cursor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetCursor.setMinFilter(GL_NEAREST);
	spritesheetCursor.setMagFilter(GL_NEAREST);
	cursor = Sprite::createSprite(glm::ivec2(15, 15), glm::vec2(1.0f/3.0f, 1.0f), &spritesheetCursor, &simpleTexProgram);


	/*	Animation 0: Cuadrado
		Animation 1: Flecha
		Animation 2: Shooter apuntando  */

	cursor->setNumberAnimations(3);
	cursor->setAnimationSpeed(0, 1);
	cursor->setAnimationSpeed(1, 1);
	cursor->setAnimationSpeed(2, 1);
	
	cursor->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	cursor->addKeyframe(1, glm::vec2(1.0f / 3.0f, 0.0f));
	cursor->addKeyframe(2, glm::vec2(2.0f / 3.0f, 0.0f));

	cursor->changeAnimation(1);
	cursor->setPosition(glm::vec2(posX, posY));
}

//init sprite de seleccionar casilla de interfaz

void Scene::initSeleccion() {
	renderSeleccionLemming = false;
	spritesheetSeleccionLemming.loadFromFile("images/seleccion.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetSeleccionLemming.setMinFilter(GL_NEAREST);
	spritesheetSeleccionLemming.setMagFilter(GL_NEAREST);
	seleccionLemming = Sprite::createSprite(glm::ivec2(20, 32), glm::vec2(1.0f, 1.0f), &spritesheetSeleccionLemming, &simpleTexProgram);


	seleccionLemming->setNumberAnimations(1);
	seleccionLemming->setAnimationSpeed(0, 1);

	seleccionLemming->addKeyframe(0, glm::vec2(0.0f, 0.0f));

	seleccionLemming->changeAnimation(0);
}

void Scene::initOpenDoor(const Level &l) {
	spritesheetDoor.loadFromFile(l.openDoortexture, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetDoor.setMinFilter(GL_NEAREST);
	spritesheetDoor.setMagFilter(GL_NEAREST);
	openDoor = Sprite::createSprite(glm::ivec2(41, 21), glm::vec2(1, 1.0f / 10.0f), &spritesheetDoor, &simpleTexProgram);
	openDoor->setNumberAnimations(1);
	openDoor->setAnimationSpeed(0, 10);
	for (int i = 0; i<10; i++)
		openDoor->addKeyframe(0, glm::vec2(0.0f, float(i) / 10));

	openDoor->changeAnimation(0);

	openDoor->setPosition(l.openDoorPos);
}

//sprite que contiene todos los digitos, se utiliza para sacar el resultado en la pantalla de WIN/LOSE
void Scene::initDigits() {
	spritesheetDigits.loadFromFile("images/digits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetDigits.setMinFilter(GL_NEAREST);
	spritesheetDigits.setMagFilter(GL_NEAREST);
	digits = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(1.0f / 10.0f, 1), &spritesheetDigits, &simpleTexProgram);
	digits->setNumberAnimations(10);
	for (int i = 0; i < 10; i++) {
		digits->setAnimationSpeed(0, 1);
		digits->addKeyframe(i, glm::vec2(float(i) / 10.0f, 0.0f));
	}
	digits->changeAnimation(0);

	//openDoor->setPosition(l.openDoorPos);
}

void Scene::initFinishDoor(const Level &l) {
	spritesheetfinishDoor.loadFromFile(l.finishDoortexture, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetfinishDoor.setMinFilter(GL_NEAREST);
	spritesheetfinishDoor.setMagFilter(GL_NEAREST);
	finishDoor = Sprite::createSprite(l.finishDoorSize, glm::vec2(1.0f / 6.0f, 1), &spritesheetfinishDoor, &simpleTexProgram);
	finishDoor->setNumberAnimations(1);
	finishDoor->setAnimationSpeed(0, 6);
	for (int i = 0; i<6; i++)
		finishDoor->addKeyframe(0, glm::vec2(float(i) / 6, 0.0f));

	finishDoor->changeAnimation(0);
	finishDoor->setPosition(l.finishDoorPos);
}

void Scene::initFire() {
	spritesheetFire.loadFromFile("images/fire.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetFire.setMinFilter(GL_NEAREST);
	spritesheetFire.setMagFilter(GL_NEAREST);
	fire = Sprite::createSprite(glm::vec2(78, 11), glm::vec2(0.5f, 0.1f), &spritesheetFire, &simpleTexProgram);
	fire->setNumberAnimations(1);
	fire->setAnimationSpeed(0, 12);
	for (int i = 0; i<10; i++)
		fire->addKeyframe(0, glm::vec2(0.0f, float(i) / 10.0f));
	fire->changeAnimation(0);
	fire->setPosition(glm::vec2(271, 101));

	fire2 = Sprite::createSprite(glm::vec2(78, 11), glm::vec2(0.5f, 0.1f), &spritesheetFire, &simpleTexProgram);
	fire2->setNumberAnimations(1);
	fire2->setAnimationSpeed(0, 12);
	for (int i = 0; i<10; i++)
		fire2->addKeyframe(0, glm::vec2(0.5f, float(i) / 10.0f));
	fire2->changeAnimation(0);
	fire2->setPosition(glm::vec2(628, 53));
}

//init de los sprites de la pistola y de la bala del lemming shooter
void Scene::initPistolBala() {
	spritesheetPistol.loadFromFile("images/pistol.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetPistol.setMinFilter(GL_NEAREST);
	spritesheetPistol.setMagFilter(GL_NEAREST);
	pistol = Sprite::createSprite(glm::vec2(15,7), glm::vec2(0.5f, 1), &spritesheetPistol, &simpleTexProgram);
	pistol->setNumberAnimations(2);
	pistol->setAnimationSpeed(0, 1);
	pistol->setAnimationSpeed(1, 1);
	pistol->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	pistol->addKeyframe(1, glm::vec2(0.5f, 0.0f));
	pistol->changeAnimation(0);
	pistol->setPosition(glm::vec2(100,100));

	spritesheetBala.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetBala.setMinFilter(GL_NEAREST);
	spritesheetBala.setMagFilter(GL_NEAREST);
	bala = Sprite::createSprite(glm::vec2(16,16), glm::vec2(1.0f/10.0f, 1), &spritesheetBala, &simpleTexProgram);
	bala->setNumberAnimations(2);
	bala->setAnimationSpeed(0, 1);
	bala->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	bala->setAnimationSpeed(1, 15);
	for(int i = 1; i < 10; ++i)
		bala->addKeyframe(1, glm::vec2(float(i) / 10.0f, 0.0f));

	bala->changeAnimation(0);
	bala->setPosition(glm::vec2(100, 100));
}

void Scene::initLevel(const Level &l) {
	float scale = 0.827586f;

	//scale = 1.0f;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	glm::vec2 texCoords[2] = { glm::vec2(120.f / l.mapLength, 0.f), glm::vec2((120.f + 320.f) / l.mapLength, (160.f / 256.0f)) };
	//init some vars
	
	id = -1;
	score = in = out = 0;
	totalLemmings = l.levelLemmings;
	lemmings.clear();
	lemmings.resize(totalLemmings);
	levelTime = l.levelTime;
	needToWin = l.needToWin;
	bounds = l.bounds;
	shooting = false;
	finish = allOut = false;
	stateSelected = false;
	pause = x2speed = exploding = false;
	renderSeleccionPause = renderSeleccionExplosion = renderSeleccionLemming = false;
	scroll = 0.0f;

	for (int i = 0; i < 7; ++i) {
		numLemmings[i] = l.numLemmings[i];
	}
	texCoords[0] = glm::vec2(0.0f, 0.0f);
	texCoords[1] = glm::vec2(1.0f, (160.f / 256.0f));
	geom[1] = glm::vec2(l.mapLength, float(CAMERA_HEIGHT)*scale);


	//create Map
	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile(l.colortexture, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	if(numLevel == 3)
		maskTexture.loadFromFile(l.masktexture, TEXTURE_PIXEL_FORMAT_RGBA);
	else
		maskTexture.loadFromFile(l.masktexture, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);


	//create UI
	geom[0] = glm::vec2(0.0f, (float(CAMERA_HEIGHT)*scale) - 1);
	geom[1] = glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT));
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.0f, 1.0f);
	UI = TexturedQuad::createTexturedQuad(geom, texCoords, imagesProgram);
	UITexture.loadFromFile("images/interface.png", TEXTURE_PIXEL_FORMAT_RGBA);

	free = true;

	//create Lemmings
	spritesheetLemmings.loadFromFile("images/lemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetLemmings.setMinFilter(GL_NEAREST);
	spritesheetLemmings.setMagFilter(GL_NEAREST);
	glm::vec2 initpos = l.openDoorPos + glm::vec2(13, 0);
	for (int i = 0; i < totalLemmings; ++i) {
		lemmings[i].init(initpos, simpleTexProgram, spritesheetLemmings, 2000.0 + 1000.0*(i * l.ratio), &aEngine);
		lemmings[i].setMapMask(&maskTexture, &colorTexture);
	}


	//openDoor	
	initOpenDoor(l);

	//finishDoor
	initFinishDoor(l);

	//initRejillas de boton seleccionado
	initSeleccion();

	//initSound
	aEngine.playLoop(l.levelSong);
}

void Scene::init(int level)
{
	// Select which font you want to use
	//if (!levelInfo.init("fonts/OpenSans-Regular.ttf"))
	endGameSound = true;
	//Variacion del background en funci�n de la hora
	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);
	int hour = aTime->tm_hour;
	if(level == 3)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	else if(hour > 7 && hour < 21)
		glClearColor(0.0f, 142.f/255.f, 204 / 255.f, 1.0f);
	else if(hour >= 21)
		glClearColor(0.0f, 0.0f, 100 / 255.f, 1.0f);
	else
		glClearColor(0.0f, 0.0f, 30 / 255.f, 1.0f);
	//factor
	float scale = 0.827586f;
	currentTime = 0.0f;
	//scale = 1.0f;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	glm::vec2 texCoords[2] = { glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, (160.f / 256.0f)) };
	projection = glm::ortho(0.0f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	winTime = 0;
	scroll = 0;
	gocredits = instruct = false;
	levelSelectClick = false;


	//load files
	if (!load) {
		if (!levelInfo.init("fonts/OpenSans-Bold.ttf"))
			//if(!text.init("fonts/DroidSerif.ttf"))
			std::cout << "Could not load font!!!" << endl;

		if (!countdownText.init("fonts/OpenSans-Regular.ttf"))
			std::cout << "Could not load font!!!" << endl;
		initShaders();
		initCursor();
		initFire();
		initDigits();
		initPistolBala();
		load = true;
		texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.0f, 1.0f);
		menu = TexturedQuad::createTexturedQuad(geom, texCoords, imagesProgram);

		menuTexture.loadFromFile("images/menuPrincipal.png", TEXTURE_PIXEL_FORMAT_RGBA);
		selectLevel.loadFromFile("images/menuLevels.png", TEXTURE_PIXEL_FORMAT_RGBA);
		level1Info.loadFromFile("images/infoLevel1.png", TEXTURE_PIXEL_FORMAT_RGBA);
		level2Info.loadFromFile("images/infoLevel2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		level3Info.loadFromFile("images/infoLevel3.png", TEXTURE_PIXEL_FORMAT_RGBA);
		level4Info.loadFromFile("images/infoLevel4.png", TEXTURE_PIXEL_FORMAT_RGBA);
		instr.loadFromFile("images/howtoplay1.png", TEXTURE_PIXEL_FORMAT_RGBA);
		instr2.loadFromFile("images/howtoplay2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		instr3.loadFromFile("images/howtoplay3.png", TEXTURE_PIXEL_FORMAT_RGBA);
		credits.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
		winTex.loadFromFile("images/win.png", TEXTURE_PIXEL_FORMAT_RGBA);
		loseTex.loadFromFile("images/lose.png", TEXTURE_PIXEL_FORMAT_RGBA);
		aEngine = AudioEngine::AudioEngine();
		aEngine.init();
	}
	cursor->changeAnimation(1);
	switch (gamestate) {
		case CREDITS:
			alpha = 1.0f;
			transitionTime = 0;
			inMenu = false;
			aEngine.playLoop("sounds/crowd.mp3");
		case PLAYING:
			inMenu = false;
			if (level == 1) {
				initLevel(L1);
				numLevel = 1;
			}
			if (level == 2) {
				initLevel(L2);
				scroll = -30;
				numLevel = 2;
			}
			if (level == 3) {
				initLevel(L3);
				scroll = 200;
				numLevel = 3;
			}
			if (level == 4) {
				initLevel(L4);
				numLevel = 4;
			}
			break;

		case MENU:
			alpha = 1.0f;
			transitionTime = 0;
			if (!inMenu)
				aEngine.playLoop("sounds/mainmenu.mp3");
			inMenu = true;
			break;
		case SELECT_LEVEL:
			alpha = 1.0f;
			transitionTime = 0;
		case LEVEL_INFO:
			alpha = 1.0f;
			transitionTime = 0;
		default:
			alpha = 1.0f;
			transitionTime = 0;
			break;
	}

}

unsigned int x = 0;

void Scene::freeScene(){
	for (int i = 0; i < totalLemmings; ++i) {
		lemmings[i].getSprite()->free();
	}
	levelInfo.destroy();
	countdownText.destroy();
	map->free();
	UI->free();
	openDoor->free();
	fire->free();
	pistol->free();
	bala->free();
	finishDoor->free();
}

void Scene::update(int deltaTime)
{
	
	if (x2speed)
		deltaTime *= 2;
		
	if (renderSeleccionPause) 
		deltaTime *= 0;
	aEngine.update();
	currentTime += deltaTime;
	bool found = false;
	switch (gamestate) {
		case PLAYING:

			if (numLevel == 3) {
				if (fire->getKeyframe() == 2) {
					fireSound = true;
				}
				if (fire->getKeyframe() == 1 && fireSound) {
					aEngine.play("sounds/FIRE.wav");
					fireSound = false;
				}
			}

			if (posX < 140) 
				scroll -= 1;
			if (posX < 130)
				scroll -= 1;
		
			if (posX > 420)
				scroll += 1;
			if (posX > 430)
				scroll += 1;

			//buscamos si algun lemmings se ha salido de los bounds del mapa y ponemos su estado a DEAD
			for (int i = 0; i < lemmings.size(); ++i) {
				glm::ivec2 pos = lemmings[i].getSprite()->position();
				if (pos.x < bounds[0] || pos.x > bounds[1] || pos.y > bounds[2] || pos.y < bounds[3]) {
					lemmings[i].setState(DEAD);
					lemmings[i].setCountdown(-1);
				}
			}

			//buscar si estamos seleccionando a algun Lemming		
			for (int i = lemmings.size() - 1; i >= 0 && !found; --i) {
				glm::ivec2 pos = lemmings[i].getSprite()->position();
				pos += glm::vec2(120, 0);

				if (pos.x <= posX + int(scroll) && posX + int(scroll) <= (pos.x + 16) && pos.y <= posY && posY <= (pos.y + 16) && lemmings[i].getState() != WIN_STATE && lemmings[i].getState() != DEAD && lemmings[i].getState() != RESPAWN) {
					id = i;
					found = true;
				}
			}
			if (!found) {
				id = -1;
				lemmingHover = "";
			}
			else {
				//hover lemmings
				switch (lemmings[id].getState()) {
					case VOLTERETA:
						lemmingHover = "Climber";
						break;
					case BASHER:
						lemmingHover = "Basher";
						break;
					case DIGGER_STATE:
						lemmingHover = "Digger";
						break;
					case CLIMBER_STATE:
						lemmingHover = "Climber";
						break;
					case BLOCKER_STATE:
						lemmingHover = "Blocker";
						break;
					case BUILDER_STATE:
						lemmingHover = "Builder";
						break;
					case WALKING_LEFT_STATE:
						lemmingHover = "Walker";
						break;
					case WALKING_RIGHT_STATE:
						lemmingHover = "Walker";
						break;
					case FALLING_RIGHT_STATE:
						lemmingHover = "Faller";
						break;
					case FALLING_LEFT_STATE:
						lemmingHover = "Faller";
						break;
					default: 
						lemmingHover = "";
						break;
				}
			}

			//actualizar el cursor
			cursor->setPosition(glm::vec2(posX - 120 - 8, posY - 8));

			if (shooting) {
				cursor->changeAnimation(2);
			}
			else if (id == -1) {
				cursor->changeAnimation(1);
			}
			else {
				cursor->changeAnimation(0);
			}
			cursor->update(deltaTime);

			///////Bala update
			if (balamoving) {
				bala->update(deltaTime);
				if(!balaexploding)
					bala->setPosition(glm::vec2(3 * cos(baladegree), 3 * sin(baladegree)) + bala->position());

				glm::ivec2 intpos = bala->position()+glm::vec2(120+8,8);
				if (intpos.x < bounds[0] || intpos.x > bounds[1] || intpos.y > bounds[2] || intpos.y < bounds[3]) {
					balaexploding = false;
					balamoving = false;
					bala->changeAnimation(0);
				}

				if (int(maskTexture.pixel(intpos.x, intpos.y)) != 0 && int(maskTexture.pixel(intpos.x, intpos.y)) != 100 && !balaexploding) {
					balaexploding = true;
					bala->changeAnimation(1);
				}
				if (balaexploding && bala->getKeyframe() == 6) {
					aEngine.play("sounds/balaexplode.wav");
				}
				if (balaexploding && bala->getKeyframe() == 8) {
					int radius = 14;
					for (int y = -radius; y <= radius; y++)
						for (int x = -radius; x <= radius; x++)
							if (x*x + y * y <= radius * radius) {
								if (maskTexture.pixel(intpos.x + x, intpos.y + y) != 100) 
									maskTexture.setPixel(intpos.x + x, intpos.y + y, 0);
							}
					balaexploding = false;
					balamoving = false;
					bala->changeAnimation(0);
				}
			}



			if (openDoor->getKeyframe() != 9) openDoor->update(deltaTime);
			if (openDoor->getKeyframe() == 2) aEngine.play("sounds/DOOR.wav");

			finishDoor->update(deltaTime);
			if (numLevel == 3) {
				fire->update(deltaTime);
				fire2->update(deltaTime);
			}
			//update lemmings
			for (int i = 0; i < totalLemmings; ++i) {
				score += lemmings[i].update(deltaTime, currentTime);
			}

			finish = true;
			allOut = true;

			//recuento de como vamos...
			out = 0;
			for (int i = 0; i < lemmings.size(); ++i) {
				if (lemmings[i].getState() != DEAD) finish = false;
				if (lemmings[i].getState() == RESPAWN) allOut = false;
				if (lemmings[i].getState() != DEAD && lemmings[i].getState() != RESPAWN) ++out;
			}

			if (finish || currentTime > levelTime) {
				aEngine.stopLoop();
				
				pause = false;
				x2speed = false;
				if (winTime == 0)
					winTime = currentTime;
				//final de la partida
				if (currentTime - winTime > 1000.0f) {
					gamestate = WIN;
					scroll = 0.0f;
					if (score >= needToWin) {
						if (endGameSound) {
							aEngine.play("sounds/gewonnen.mp3");
							endGameSound = false;
						}
						if (onePlayer) {
							if (numLevel == 4)
								onePlayer = false;
							else
								++numLevel;
						}

					}
					else
						aEngine.play("sounds/negative_beeps.mp3");
					freeScene();
					init(0);
				}
			}
			break;
		case MENU:
			cursor->setPosition(glm::vec2(posX - 120 - 8, posY - 8));
			cursor->update(deltaTime);

			//transition
			if (onePlayer || levelSelectClick || quit || instruct || gocredits) {
				if (transitionTime == 0) {
					transitionTime = currentTime;
					aEngine.play("sounds/BEEPSELECT.wav");
				}

				alpha = 1 - (currentTime - transitionTime) / 800.0f;
				if (currentTime - transitionTime > 800) {
					if (onePlayer) {
						gamestate = LEVEL_INFO;
						numLevel = 1;
					}
					else if (levelSelectClick)
						gamestate = SELECT_LEVEL;
					else if (instruct)
						gamestate = INSTR;
					else if (gocredits)
						gamestate = CREDITS;
					else
						exit(0); //el usuario ha apretado al boton exit del men�, despu�s de la transici�n se cierra el juego
						

					transitionTime = 0;
					if (free) {
						freeScene();
					}
					init(0);
				}
			}

			break;

		case SELECT_LEVEL:
			cursor->setPosition(glm::vec2(posX - 120 - 8, posY - 8));
			cursor->update(deltaTime);

			//transition
			if (levelSelectClick) {			
				if (transitionTime == 0) {
					transitionTime = currentTime;
					aEngine.play("sounds/BEEPSELECT.wav");
				}

				alpha = 1 - (currentTime - transitionTime) / 800.0f;
				if (currentTime - transitionTime > 800) {		
					onePlayer = false;
					if (numLevel != 0)
						gamestate = LEVEL_INFO;
					else gamestate = MENU;

					if (free)
						freeScene();
					init(0);
					levelSelectClick = false;
					transitionTime = 0;
				}
			}
			break;
		case LEVEL_INFO:
			cursor->setPosition(glm::vec2(posX - 120 - 8, posY - 8));
			cursor->update(deltaTime);
			if (levelSelectClick) {
				if (transitionTime == 0) {
					aEngine.play("sounds/BEEPSELECT.wav");
					transitionTime = currentTime;
				}

				alpha = 1 - (currentTime - transitionTime) / 800.0f;
				if (currentTime - transitionTime > 800) {
					gamestate = PLAYING;
					if (free) {
						freeScene();
					}
					init(numLevel);
					levelSelectClick = false;
					transitionTime = 0;

				}
			}
			break;
		case WIN:
			cursor->setPosition(glm::vec2(posX - 120 - 8, posY - 8));
			cursor->update(deltaTime);
			if (levelSelectClick || goToMenu) {
				if (transitionTime == 0) {
					transitionTime = currentTime;
				}

				alpha = 1 - (currentTime - transitionTime) / 800.0f;
				if (currentTime - transitionTime > 800) {
					if (free) {
						freeScene();
					}
					if (goToMenu) {
						gamestate = MENU;
						goToMenu = false;
						levelSelectClick = false;
						onePlayer = false;
						init(0);
					}
					else {
						if (!onePlayer) {
							if (score < needToWin)
								gamestate = LEVEL_INFO;
							else
								gamestate = MENU;

							init(0);
						}
						else {
							gamestate = LEVEL_INFO;
							init(numLevel);
						}
						levelSelectClick = false;
						transitionTime = 0;
					}
				}
			}
			break;
		case INSTR:
			cursor->setPosition(glm::vec2(posX - 120 - 8, posY - 8));
			cursor->update(deltaTime);

			if (levelSelectClick) {
				if (transitionTime == 0) {
					aEngine.play("sounds/BEEPSELECT.wav");
					transitionTime = currentTime;
				}

				alpha = 1 - (currentTime - transitionTime) / 800.0f;
				if (currentTime - transitionTime > 800) {
					gamestate = MENU;
					if (free) {
						freeScene();
					}
					init(numLevel);
					levelSelectClick = false;
					transitionTime = 0;
				}
			}
			break;
		case CREDITS:
			cursor->setPosition(glm::vec2(posX - 120 - 8, posY - 8));
			cursor->update(deltaTime);

			if (levelSelectClick) {
				if (transitionTime == 0) {
					aEngine.play("sounds/BEEPSELECT.wav");
					transitionTime = currentTime;
				}

				alpha = 1 - (currentTime - transitionTime) / 800.0f;
				if (currentTime - transitionTime > 800) {
					gamestate = MENU;
					if (free) {
						freeScene();
					}
					init(numLevel);
					levelSelectClick = false;
					transitionTime = 0;
				}
			}
			break;

	}
}

void Scene::render()
{
	glm::mat4 modelview = glm::mat4(1.0f);
	glm::mat4 projection2 = glm::ortho(scroll, scroll + float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	int need;
	int rescued;
	switch (gamestate) {
		case PLAYING:
			
			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection2);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			finishDoor->render(0,0);
			openDoor->render(0,0);
			if (numLevel == 3) {
				fire->render(0,0);
				fire2->render(0,0);
			}



			if (true) {
				//se renderiza el mapa encima de esos dos sprites para poder ver la escalera del builder si pasa sobre alguna puerta
				maskedTexProgram.use();
				maskedTexProgram.setUniformMatrix4f("projection", projection2);
				maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
				glm::mat4 modelviewmap = glm::translate(modelview, glm::vec3(-120.f, 0.0f, 0.f));
				maskedTexProgram.setUniformMatrix4f("modelview", modelviewmap);
				map->render(maskedTexProgram, colorTexture, maskTexture);
			}

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection2);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			for (int i = 0; i < totalLemmings; ++i) {
				lemmings[i].render(0);
			}
			cursor->render(scroll,0);

			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			
			if (shooting) {
				if (posX >= shootpos.x - scroll) {
					pistol->changeAnimation(0);
					pistol->setPosition(glm::vec2(shootpos.x - 120, shootpos.y));
					pistol->render(0, degree);
				}
				else {
					pistol->changeAnimation(1);
					pistol->setPosition(glm::vec2(shootpos.x - 120, shootpos.y+7));
					pistol->render(0, degree);
				}
				
			}
			//bala->setPosition(glm::vec2(100, 100));
			
			if (balamoving) {
				bala->render(0, 0);
			}



			textProgram.use();
			textProgram.setUniformMatrix4f("projection", projection2);
			textProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			textProgram.setUniformMatrix4f("modelview", modelview);
			for (int i = 0; i < totalLemmings; ++i) {
				if (lemmings[i].getCountdown() != -1 && lemmings[i].getState() != EXPLOSION_STATE) {
					int time = 5100 - lemmings[i].getCountdown();
					glm::vec2 position = lemmings[i].getSprite()->position();
					position *= glm::vec2(3, 3);
					position += glm::vec2(18-2*scroll, 15);
					levelInfo.render(to_string(1 + (time / 1000)), position, 21, glm::vec4(1), scroll);
				}
			}

			imagesProgram.use();
			imagesProgram.setUniformMatrix4f("projection", projection);
			imagesProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			imagesProgram.setTextureUnit("alpha", 1.0f);
			imagesProgram.setUniformMatrix4f("modelview", modelview);
			UI->render(UITexture);

			textProgram.use();
			textProgram.setUniformMatrix4f("projection", projection);
			textProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			textProgram.setUniformMatrix4f("modelview", modelview);
			//datos del juego
			if (true) {
				int timeLeft = (levelTime - currentTime) / 1000;
				string aux = to_string(timeLeft % 60);
				if (timeLeft % 60 < 10) aux = "0" + aux;
				string timeleft = "Time  " + to_string(timeLeft / 60) + "-" + aux;
				levelInfo.render(timeleft, glm::vec2(780, 172 * 3), 32, glm::vec4(0, 1, 0, 1), 0);

				string lemmingsout = "Out: " + to_string(out);
				levelInfo.render(lemmingsout, glm::vec2(625, 185 * 3), 32, glm::vec4(0, 1, 0, 1), 0);

				string lemmingsin = "In: " + to_string((score * 100) / totalLemmings) + "%";
				levelInfo.render(lemmingsin, glm::vec2(780, 185 * 3), 32, glm::vec4(0, 1, 0, 1), 0);

				levelInfo.render(lemmingHover, glm::vec2(625, 172 * 3), 32, glm::vec4(0, 1, 0, 1), 0);
			}

			if (numLemmings[BUILDER_STATE] != 0) {
				string num = to_string(numLemmings[BUILDER_STATE]);
				if (numLemmings[BUILDER_STATE] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(200, 172 * 3), 32, glm::vec4(1),0);
			}
			if (numLemmings[BLOCKER_STATE] != 0) {
				string num = to_string(numLemmings[BLOCKER_STATE]);
				if (numLemmings[BLOCKER_STATE] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(138, 172 * 3), 32, glm::vec4(1),0);
			}
			if (numLemmings[EXPLOSION_STATE] != 0) {
				string num = to_string(numLemmings[EXPLOSION_STATE]);
				if (numLemmings[EXPLOSION_STATE] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(78, 172 * 3), 32, glm::vec4(1),0);
			}
			if (numLemmings[BASHER] != 0) {
				string num = to_string(numLemmings[BASHER]);
				if (numLemmings[BASHER] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(261, 172 * 3), 32, glm::vec4(1),0);
			}

			if (numLemmings[CLIMBER_STATE] != 0) {
				string num = to_string(numLemmings[CLIMBER_STATE]);
				if (numLemmings[CLIMBER_STATE] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(17, 172 * 3), 32, glm::vec4(1),0);
			}
			if (numLemmings[DIGGER_STATE] != 0) {
				string num = to_string(numLemmings[DIGGER_STATE]);
				if (numLemmings[DIGGER_STATE] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(322, 172 * 3), 32, glm::vec4(1),0);
			}

			if (numLemmings[SHOOTER] != 0) {
				string num = to_string(numLemmings[SHOOTER]);
				if (numLemmings[SHOOTER] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(384, 172 * 3), 32, glm::vec4(1), 0);
			}
			

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			
			seleccionLemming->setPosition(lemmingSelected);
			if (renderSeleccionLemming)
				seleccionLemming->render(0,0);

			if (renderSeleccionPause) {
				seleccionLemming->setPosition(glm::vec2(145, 160));
				seleccionLemming->render(0,0);
			}

			if (renderSeleccionExplosion) {
				seleccionLemming->setPosition(glm::vec2(165, 160));
				seleccionLemming->render(0,0);
			}

			if (x2speed) {
				seleccionLemming->setPosition(glm::vec2(185, 160));
				seleccionLemming->render(0,0);
			}

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection2);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			cursor->render(scroll,0);

			break;

		case MENU:
			imagesProgram.use();
			imagesProgram.setUniformMatrix4f("projection", projection);
			imagesProgram.setUniform4f("color", alpha, alpha, alpha, 1.0f);
			imagesProgram.setTextureUnit("alpha", 0.1f);
			imagesProgram.setUniformMatrix4f("modelview", modelview);
			menu->render(menuTexture);

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);

			cursor->render(0,0);
			
			break;

		case SELECT_LEVEL:
			imagesProgram.use();
			imagesProgram.setUniformMatrix4f("projection", projection);
			imagesProgram.setUniform4f("color", alpha, alpha, alpha, 1.0f);
			imagesProgram.setTextureUnit("alpha", 1.0f);
			imagesProgram.setUniformMatrix4f("modelview", modelview);
			menu->render(selectLevel);

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);

			cursor->render(0,0);
			break;

		case LEVEL_INFO:
			imagesProgram.use();
			imagesProgram.setUniformMatrix4f("projection", projection);
			imagesProgram.setUniform4f("color", alpha, alpha, alpha, 1.0f);
			imagesProgram.setTextureUnit("alpha", 1.0f);
			imagesProgram.setUniformMatrix4f("modelview", modelview);
			if(numLevel == 1)
				menu->render(level1Info);
			if (numLevel == 2)
				menu->render(level2Info);
			if (numLevel == 3)
				menu->render(level3Info);
			if (numLevel == 4)
				menu->render(level4Info);

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);

			cursor->render(0,0);
			break;
		case WIN:
			imagesProgram.use();
			imagesProgram.setUniformMatrix4f("projection", projection);
			imagesProgram.setUniform4f("color", alpha, alpha, alpha, 1.0f);
			imagesProgram.setTextureUnit("alpha", 1.0f);
			imagesProgram.setUniformMatrix4f("modelview", modelview);
			if (needToWin <= score)
				menu->render(winTex);
			else
				menu->render(loseTex);

			/////////////
			//printing results
			/////////////

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", alpha, alpha, alpha, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);


			need = (100 * needToWin) / totalLemmings;
			rescued = (100 * score) / totalLemmings;
			digits->setPosition(glm::vec2(179+5, 37));
			if (need >= 100) {
				digits->changeAnimation(1);
				digits->render(0,0);
			}
			digits->setPosition(glm::vec2(188+5, 37));
			digits->changeAnimation((need / 10) % 10);
			if ((need / 10) % 10 != 0 || need == 100)
				digits->render(0,0);

			digits->setPosition(glm::vec2(197+5, 37));
			digits->changeAnimation(need%10);
			digits->render(0,0);

			digits->setPosition(glm::vec2(179+7, 27));
			if (rescued >= 100) {
				digits->changeAnimation(1);
				digits->render(0,0);
			}
			digits->setPosition(glm::vec2(188+7, 27));
			digits->changeAnimation((rescued / 10) % 10);
			if((rescued / 10) % 10 != 0 || rescued == 100)
				digits->render(0,0);

			digits->setPosition(glm::vec2(197+7, 27));
			digits->changeAnimation(rescued % 10);
			digits->render(0,0);

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);

			cursor->render(0,0);
			break;

		case INSTR:
			imagesProgram.use();
			imagesProgram.setUniformMatrix4f("projection", projection);
			imagesProgram.setUniform4f("color", alpha, alpha, alpha, 1.0f);
			imagesProgram.setTextureUnit("alpha", 1.0f);
			imagesProgram.setUniformMatrix4f("modelview", modelview);
			if(instrnum == 0)
				menu->render(instr);
			else if(instrnum == 1)
				menu->render(instr2);
			else
				menu->render(instr3);
			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);

			cursor->render(0,0);

			break;
		case CREDITS:
			imagesProgram.use();
			imagesProgram.setUniformMatrix4f("projection", projection);
			imagesProgram.setUniform4f("color", alpha, alpha, alpha, 1.0f);
			imagesProgram.setTextureUnit("alpha", 1.0f);
			imagesProgram.setUniformMatrix4f("modelview", modelview);
			menu->render(credits);

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);

			cursor->render(0,0);


			break;
	}

}

void Scene::mousePress(int mouseX, int mouseY, bool bLeftButton, bool bRightButton) {
	int res = 0;
	glm::vec2 lemmingidpos;
	glm::vec2 lemmingnormalpos;
	switch (gamestate) {
	case PLAYING:
		if (!shooting) {
			if (bLeftButton && posY < 160) {
				if (id != -1 && stateSelected) {
					aEngine.play("sounds/ingameClick.wav");
					if (numLemmings[lemmingsState] > 0) {
						res = lemmings[id].setState(lemmingsState);
						numLemmings[lemmingsState] -= res;
						////////////////////////
						////calcular lemmings inmunes a los blockers, para evitar que queden atrapados
						////////////////////////
						if (res == 1 && lemmingsState == BLOCKER_STATE) {
							lemmingidpos = lemmings[id].getSprite()->position() + glm::vec2(120, 0);
							for (int i = 0; i < lemmings.size(); ++i) {
								lemmingnormalpos = lemmings[i].getSprite()->position() + glm::vec2(127, 14);
								if (id != i && lemmingnormalpos.y <= lemmingidpos.y + 15 && lemmingnormalpos.y >= lemmingidpos.y + 6)
									if ((lemmingnormalpos.x <= lemmingidpos.x + 13 && lemmingnormalpos.x >= lemmingidpos.x + 3) ||
										(lemmingnormalpos.x + 1 <= lemmingidpos.x + 13 && lemmingnormalpos.x + 1 >= lemmingidpos.x + 3))
										lemmings[i].setInmune();
							}
						}
						//set SHOOTER
						if (res == 1 && lemmingsState == SHOOTER && !balamoving) {
							shootpos = lemmings[id].getSprite()->position() + glm::vec2(127, 7);
							bala->setPosition(shootpos - glm::vec2(120, 0));
							idshooter = id;
							cursor->changeAnimation(2);
							shooting = true;
							
						}
					}
				}
			}
			//interfaz seleccionable
			else if (bLeftButton && posY >= 160) {
				int num = (posX - 4 - 120) / 20;
				switch (num) {
				case 0:
					aEngine.play("sounds/BEEP.wav");
					lemmingsState = CLIMBER_STATE;
					renderSeleccionLemming = true;
					lemmingSelected = glm::vec2(2, 160);
					seleccionLemming->setPosition(lemmingSelected);

					stateSelected = true;
					break;
				case 1:
					aEngine.play("sounds/BEEP.wav");
					lemmingsState = EXPLOSION_STATE;
					renderSeleccionLemming = true;
					lemmingSelected = glm::vec2(22, 160);
					seleccionLemming->setPosition(lemmingSelected);
					stateSelected = true;
					break;
				case 2:
					aEngine.play("sounds/BEEP.wav");
					lemmingsState = BLOCKER_STATE;
					renderSeleccionLemming = true;
					lemmingSelected = glm::vec2(43, 160);
					seleccionLemming->setPosition(lemmingSelected);
					stateSelected = true;
					break;
				case 3:
					aEngine.play("sounds/BEEP.wav");
					lemmingsState = BUILDER_STATE;
					renderSeleccionLemming = true;
					lemmingSelected = glm::vec2(63, 160);
					seleccionLemming->setPosition(lemmingSelected);
					stateSelected = true;
					break;
				case 4:
					aEngine.play("sounds/BEEP.wav");
					lemmingsState = BASHER;
					renderSeleccionLemming = true;
					lemmingSelected = glm::vec2(83, 160);
					seleccionLemming->setPosition(lemmingSelected);
					stateSelected = true;
					break;
				case 5:
					aEngine.play("sounds/BEEP.wav");
					lemmingsState = DIGGER_STATE;
					renderSeleccionLemming = true;
					lemmingSelected = glm::vec2(104, 160);
					seleccionLemming->setPosition(lemmingSelected);
					stateSelected = true;
					break;
				case 6:
					aEngine.play("sounds/BEEP.wav");
					lemmingsState = SHOOTER;
					renderSeleccionLemming = true;
					lemmingSelected = glm::vec2(124, 160);
					seleccionLemming->setPosition(lemmingSelected);
					stateSelected = true;
					break;
				case 7:
					//PAUSE
					aEngine.play("sounds/BEEP.wav");
					renderSeleccionPause = !renderSeleccionPause;
					if (renderSeleccionPause) x2speed = false;
					break;
				case 8:
					//FULL EXPLOSION
					aEngine.play("sounds/BEEP.wav");
					if (!exploding && allOut) {
						renderSeleccionExplosion = true;
						exploding = true;
						for (int i = 0; i < lemmings.size(); ++i) {
							lemmings[i].setState(EXPLOSION_STATE);
						}
					}
					break;
				case 9:
					//x2 Speed
					aEngine.play("sounds/BEEP.wav");
					x2speed = !x2speed;
					if (x2speed) renderSeleccionPause = false;
					break;
				}
			}
		}
		else {
			aEngine.play("sounds/shot.wav");
			//Codigo de obtener posicion de disparo y calculos varios
			objpos = glm::vec2(float(posX)+scroll, float(posY)-3);
			lemmings[idshooter].setState(WALKING_RIGHT_STATE);
			degree = atan2(float(objpos.y - shootpos.y), float(objpos.x - (shootpos.x)));
			baladegree = degree;
			balamoving = true;
			balaexploding = false;
			shooting = false;
			cursor->changeAnimation(2);

		}
		break;

	case MENU:
		if (bLeftButton) {
			if (posX >= 120 && posX < 180 && posY > 106 && posY < 167) {
				onePlayer = true;
			}
			if (posX >= 194 && posX < 254 && posY > 106 && posY < 167) {
				levelSelectClick = true;
			}
			if (posX >= 381 && posX < 434 && posY > 106 && posY < 167) {
				quit = true;
			}
			if (posX >= 256 && posX < 308 && posY > 106 && posY < 167) {
				instruct = true;
				instrnum = 0;
			}
			if (posX >= 318 && posX < 372 && posY > 106 && posY < 167) {
				gocredits = true;
			}
		}
		break;
	case SELECT_LEVEL:
		if (bLeftButton) {
			if (posX >= 128 && posX < 180 && posY > 106 && posY < 167) {
				numLevel = 1;
				levelSelectClick = true;
			}
			if (posX >= 194 && posX < 247 && posY > 106 && posY < 167) {
				numLevel = 2;
				levelSelectClick = true;
			}
			if (posX >= 256 && posX < 308 && posY > 106 && posY < 167) {
				numLevel = 3;
				levelSelectClick = true;
			}
			if (posX >= 318 && posX < 372 && posY > 106 && posY < 167) {
				numLevel = 4;
				levelSelectClick = true;
			}
			if (posX >= 381 && posX < 432 && posY > 106 && posY < 167) {
				numLevel = 0;
				levelSelectClick = true;
			}
		}
		break;
	case LEVEL_INFO:
		if (bLeftButton)
			levelSelectClick = true;
		break;
	case WIN:
		if (bLeftButton)
			levelSelectClick = true;
		if (bRightButton)
			goToMenu = true;
		break;
	case INSTR:
		if (bLeftButton) {
			if (instrnum != 2) {
				++instrnum;
				aEngine.play("sounds/BEEPSELECT.wav");
			}
			else
				levelSelectClick = true;
		}
		if (bRightButton) {
			if (instrnum != 0) {
				--instrnum;
				aEngine.play("sounds/BEEPSELECT.wav");
			} 
			else
				levelSelectClick = true;
		}
		break;
	case CREDITS:
		if (bLeftButton && posX > 400 && posX < 423 && posY > 147 && posY < 170)
			levelSelectClick = true;
		break;
	}
}
void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + 120;
	posY = mouseY / 3;
	//cout << posX << " " << posY << endl;
	objpos = glm::vec2(float(posX) + scroll, float(posY) - 3);
	degree = atan2(float(objpos.y - shootpos.y), float(objpos.x - (shootpos.x)));
}

void Scene::specialKeyPressed(int key) {
	switch (key) {
		case 100:
			
			scroll -= 3;
			break;
		case 102:
			scroll += 3;
			break;
	}
}

void Scene::eraseMask(int mouseX, int mouseY)
{

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX/3 + 120;
	posY = mouseY/3;

	std::cout << posX << " " << posY << endl;

	for(int y=max(0, posY-3); y<=min(maskTexture.height()-1, posY+3); y++)
		for(int x=max(0, posX-3+int(scroll)); x<=min(maskTexture.width()-1, posX+int(scroll)+3); x++)
			maskTexture.setPixel(x, y, 0);
}

void Scene::applyMask(int mouseX, int mouseY)
{

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX/3 + 120;
	posY = mouseY/3;
	for (int y = max(0, posY - 3); y <= min(colorTexture.height() - 1, posY + 3); y++)
		for (int x = max(0, posX - 3); x <= min(colorTexture.width() - 1, posX + 3); x++)
			maskTexture.setPixel(x, y, 255);
			//colorTexture.setPixel(x, y, glm::ivec4(150, 150, 150, 255));
}


void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		std::cout << "Vertex Shader Error" << endl;
		std::cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		std::cout << "Fragment Shader Error" << endl;
		std::cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if(!simpleTexProgram.isLinked())
	{
		std::cout << "Shader Linking Error" << endl;
		std::cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if(!vShader.isCompiled())
	{
		std::cout << "Vertex Shader Error" << endl;
		std::cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if(!fShader.isCompiled())
	{
		std::cout << "Fragment Shader Error" << endl;
		std::cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if(!maskedTexProgram.isLinked())
	{
		std::cout << "Shader Linking Error" << endl;
		std::cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();


	vShader.initFromFile(VERTEX_SHADER, "shaders/images.vert");
	if (!vShader.isCompiled())
	{
		std::cout << "Vertex Shader Error" << endl;
		std::cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/images.frag");
	if (!fShader.isCompiled())
	{
		std::cout << "Fragment Shader Error" << endl;
		std::cout << "" << fShader.log() << endl << endl;
	}
	imagesProgram.init();
	imagesProgram.addShader(vShader);
	imagesProgram.addShader(fShader);
	imagesProgram.link();
	if (!imagesProgram.isLinked())
	{
		std::cout << "Shader Linking Error" << endl;
		std::cout << "" << imagesProgram.log() << endl << endl;
	}
	imagesProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();



	vShader.initFromFile(VERTEX_SHADER, "shaders/text.vert");
	if (!vShader.isCompiled())
	{
		std::cout << "Vertex Shader Error" << endl;
		std::cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/text.frag");
	if (!fShader.isCompiled())
	{
		std::cout << "Fragment Shader Error" << endl;
		std::cout << "" << fShader.log() << endl << endl;
	}
	textProgram.init();
	textProgram.addShader(vShader);
	textProgram.addShader(fShader);
	textProgram.link();
	if (!textProgram.isLinked())
	{
		std::cout << "Shader Linking Error" << endl;
		std::cout << "" << textProgram.log() << endl << endl;
	}
	textProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

int Scene::getgameState() {
	return gamestate;
}

void Scene::entryfuncCallback(int state)
{
	if (state) {
		if(posX < 150)
			glutWarpPointer(5, posY * 3);
		//glutWarpPointer(posX*3 - 120, posY*3);
		else if (posX > 400){
			glutWarpPointer(956, posY * 3);
		}
	}
}

void Scene::keyPressed(int key)
{
	if (key == 27) {
		if (gamestate == PLAYING || LEVEL_INFO || WIN || LOSE) {
			gamestate = MENU;
			onePlayer = false;
			levelSelectClick = false;
			x2speed = false;
			pause = false;
			init(0);

		}
	}
	if (key == 109 | key == 77) { // key: m
		if (!aEngine.mute()) {
			if (gamestate == PLAYING) {
				switch (numLevel) {
				case 1:
					aEngine.playLoop(L1.levelSong);
					break;
				case 2:
					aEngine.playLoop(L2.levelSong);
					break;
				case 3:
					aEngine.playLoop(L3.levelSong);
					break;
				case 4:
					aEngine.playLoop(L4.levelSong);
					break;
				}
			}
			else if (gamestate == MENU || SELECT_LEVEL)
				aEngine.playLoop("sounds/mainmenu.mp3");
		}
		
	}
}