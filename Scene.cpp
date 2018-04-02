#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
using namespace std;


Scene::Scene()
{
	gamestate = MENU;
	map = NULL;
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
	cursor = Sprite::createSprite(glm::ivec2(15, 15), glm::vec2(0.5f, 1.0f), &spritesheetCursor, &simpleTexProgram);


	/*	Animation 0: Cuadrado
		Animation 1: Flecha		*/

	cursor->setNumberAnimations(2);
	cursor->setAnimationSpeed(0, 1);
	cursor->setAnimationSpeed(1, 1);
	
	cursor->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	cursor->addKeyframe(1, glm::vec2(0.5f, 0.0f));

	cursor->changeAnimation(1);
	cursor->setPosition(glm::vec2(posX, posY));
}

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

void Scene::initOpenDoor() {
	spritesheetDoor.loadFromFile("images/openDoor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetDoor.setMinFilter(GL_NEAREST);
	spritesheetDoor.setMagFilter(GL_NEAREST);
	openDoor = Sprite::createSprite(glm::ivec2(41, 21), glm::vec2(1, 1.0f / 10.0f), &spritesheetDoor, &simpleTexProgram);
	openDoor->setNumberAnimations(1);
	openDoor->setAnimationSpeed(0, 10);
	for (int i = 0; i<10; i++)
		openDoor->addKeyframe(0, glm::vec2(0.0f, float(i) / 10));

	openDoor->changeAnimation(0);

	openDoor->setPosition(glm::vec2(77, 30));
}

void Scene::initFinishDoor() {
	spritesheetfinishDoor.loadFromFile("images/finishDoor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetfinishDoor.setMinFilter(GL_NEAREST);
	spritesheetfinishDoor.setMagFilter(GL_NEAREST);
	finishDoor = Sprite::createSprite(glm::ivec2(33, 26), glm::vec2(1.0f / 3.0f, 1), &spritesheetfinishDoor, &simpleTexProgram);
	finishDoor->setNumberAnimations(1);
	finishDoor->setAnimationSpeed(0, 6);
	for (int i = 0; i<3; i++)
		finishDoor->addKeyframe(0, glm::vec2(float(i) / 3, 0.0f));

	finishDoor->changeAnimation(0);
	finishDoor->setPosition(glm::vec2(225, 107));
}

void Scene::init()
{
	// Select which font you want to use
	//if (!levelInfo.init("fonts/OpenSans-Regular.ttf"))
		if(!levelInfo.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		std::cout << "Could not load font!!!" << endl;

	if(!countdownText.init("fonts/OpenSans-Regular.ttf"))
		std::cout << "Could not load font!!!" << endl;

	//factor
	float scale = 0.827586f;

	//scale = 1.0f;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	glm::vec2 texCoords[2] = { glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, (160.f / 256.0f)) };
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	initShaders();
	initCursor();
	switch (gamestate) {
		case PLAYING:
			//init some vars
			currentTime = 0.0f;
			id = -1;
			score = in = out = 0;
			totalLemmings = 1;
			lemmings.clear();
			lemmings.resize(totalLemmings);
			posX = posY = 100;
			levelTime = 300000;
			finish = allOut = false;
			stateSelected = false;
			pause = x2speed = exploding = false;
			renderSeleccionPause = renderSeleccionExplosion = renderSeleccionLemming = false;


			for (int i = 0; i < 6; ++i) {
				numLemmings[i] = 10;
			}

			geom[1] = glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)*scale);
		

			//create Mao
			map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
			colorTexture.loadFromFile("images/fun1.png", TEXTURE_PIXEL_FORMAT_RGBA);
			colorTexture.setMinFilter(GL_NEAREST);
			colorTexture.setMagFilter(GL_NEAREST);
			maskTexture.loadFromFile("images/fun1_mask.png", TEXTURE_PIXEL_FORMAT_L);
			maskTexture.setMinFilter(GL_NEAREST);
			maskTexture.setMagFilter(GL_NEAREST);


			//create UI
			geom[0] = glm::vec2(0.0f, (float(CAMERA_HEIGHT)*scale)-1);
			geom[1] = glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT));
			texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.0f, 1.0f);
			UI = TexturedQuad::createTexturedQuad(geom, texCoords, imagesProgram);
			UITexture.loadFromFile("images/interface.png", TEXTURE_PIXEL_FORMAT_RGBA);

			//create Lemmings
			spritesheetLemmings.loadFromFile("images/lemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
			spritesheetLemmings.setMinFilter(GL_NEAREST);
			spritesheetLemmings.setMagFilter(GL_NEAREST);
			for (int i = 0; i < totalLemmings; ++i) {
				lemmings[i].init(glm::vec2(90, 30), simpleTexProgram, spritesheetLemmings, 2 + i * 1);
				lemmings[i].setMapMask(&maskTexture, &colorTexture);
			}


			//openDoor	
			initOpenDoor();

			//finishDoor
			initFinishDoor();

			//initRejillas de boton seleccionado
			initSeleccion();

			break;

		case MENU:
			texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.0f, 1.0f);
			menu = TexturedQuad::createTexturedQuad(geom, texCoords, imagesProgram);

			menuTexture.loadFromFile("images/portada.png", TEXTURE_PIXEL_FORMAT_RGB);
			break;
		default:
			break;
	}
	
}

unsigned int x = 0;

void Scene::update(int deltaTime)
{
	
	if (x2speed)
		deltaTime *= 2;
		
	if (renderSeleccionPause) 
		deltaTime *= 0;

	currentTime += deltaTime;
	bool found = false;
	switch (gamestate) {
		case PLAYING:
			
			//buscar si estamos seleccionando a algun Lemming
			
			for (int i = lemmings.size() - 1; i >= 0 && !found; --i) {
				glm::ivec2 pos = lemmings[i].getSprite()->position();
				pos += glm::vec2(120, 0);

				if (pos.x <= posX && posX <= (pos.x + 16) && pos.y <= posY && posY <= (pos.y + 16) && lemmings[i].getState() != WIN_STATE && lemmings[i].getState() != DEAD && lemmings[i].getState() != RESPAWN) {
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

			if (id == -1) {
				cursor->changeAnimation(1);
			}
			else {
				cursor->changeAnimation(0);
			}
			cursor->update(deltaTime);


			if (openDoor->getKeyframe() != 9) openDoor->update(deltaTime);

			finishDoor->update(deltaTime);

			for (int i = 0; i < totalLemmings; ++i) {
				score += lemmings[i].update(deltaTime, int(currentTime / 1000));
			}

			finish = true;
			allOut = true;
			for (int i = 0; i < lemmings.size(); ++i) {
				if (lemmings[i].getState() != DEAD) finish = false;
				if (lemmings[i].getState() == RESPAWN) allOut = false;
				if (lemmings[i].getState() != DEAD && lemmings[i].getState() != RESPAWN) ++out;
			}

			if (finish || currentTime > levelTime) {
				//Si has ganado, hacer lo que quieras.
				std::cout << score << endl;
				gamestate = MENU;
				init();
			}
			break;
		case MENU:
			cursor->setPosition(glm::vec2(posX - 120 - 8, posY - 8));
			cursor->update(deltaTime);
			break;
	}
}

void Scene::render()
{
	glm::mat4 modelview = glm::mat4(1.0f);

	switch (gamestate) {
		case PLAYING:

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			finishDoor->render();
			openDoor->render();

			//se renderiza el mapa encima de esos dos sprites para poder ver la escalera del builder si pasa sobre alguna puerta
			maskedTexProgram.use();
			maskedTexProgram.setUniformMatrix4f("projection", projection);
			maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

			maskedTexProgram.setUniformMatrix4f("modelview", modelview);
			map->render(maskedTexProgram, colorTexture, maskTexture);


			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			for (int i = 0; i < totalLemmings; ++i) {
				lemmings[i].render();
			}
			cursor->render();

			textProgram.use();
			textProgram.setUniformMatrix4f("projection", projection);
			textProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			textProgram.setUniformMatrix4f("modelview", modelview);
			for (int i = 0; i < totalLemmings; ++i) {
				if (lemmings[i].getCountdown() != -1 && lemmings[i].getState() != EXPLOSION_STATE) {
					int time = 5100 - lemmings[i].getCountdown();
					glm::vec2 position = lemmings[i].getSprite()->position();
					position *= glm::vec2(3, 3);
					position += glm::vec2(18, 15);
					levelInfo.render(to_string(1 + (time / 1000)), position, 21, glm::vec4(1));
				}
			}

			imagesProgram.use();
			imagesProgram.setUniformMatrix4f("projection", projection);
			imagesProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			imagesProgram.setUniformMatrix4f("modelview", modelview);
			UI->render(UITexture);

			textProgram.use();
			textProgram.setUniformMatrix4f("projection", projection);
			textProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			textProgram.setUniformMatrix4f("modelview", modelview);

			if (true) {
				int timeLeft = (300000 - currentTime) / 1000;
				string aux = to_string(timeLeft % 60);
				if (timeLeft % 60 < 10) aux = "0" + aux;
				string timeleft = "Time  " + to_string(timeLeft / 60) + "-" + aux;
				levelInfo.render(timeleft, glm::vec2(780, 172 * 3), 32, glm::vec4(0, 1, 0, 1));

				string lemmingsout = "Out: " + to_string(out);
				levelInfo.render(lemmingsout, glm::vec2(600, 185 * 3), 32, glm::vec4(0, 1, 0, 1));
				out = 0;

				string lemmingsin = "In: " + to_string((score * 100) / totalLemmings) + "%";
				levelInfo.render(lemmingsin, glm::vec2(780, 185 * 3), 32, glm::vec4(0, 1, 0, 1));

				levelInfo.render(lemmingHover, glm::vec2(600, 172 * 3), 32, glm::vec4(0, 1, 0, 1));
			}

			if (numLemmings[BUILDER_STATE] != 0) {
				string num = to_string(numLemmings[BUILDER_STATE]);
				if (numLemmings[BUILDER_STATE] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(200, 172 * 3), 32, glm::vec4(1));
			}
			if (numLemmings[BLOCKER_STATE] != 0) {
				string num = to_string(numLemmings[BLOCKER_STATE]);
				if (numLemmings[BLOCKER_STATE] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(138, 172 * 3), 32, glm::vec4(1));
			}
			if (numLemmings[EXPLOSION_STATE] != 0) {
				string num = to_string(numLemmings[EXPLOSION_STATE]);
				if (numLemmings[EXPLOSION_STATE] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(78, 172 * 3), 32, glm::vec4(1));
			}
			if (numLemmings[BASHER] != 0) {
				string num = to_string(numLemmings[BASHER]);
				if (numLemmings[BASHER] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(261, 172 * 3), 32, glm::vec4(1));
			}

			if (numLemmings[CLIMBER_STATE] != 0) {
				string num = to_string(numLemmings[CLIMBER_STATE]);
				if (numLemmings[CLIMBER_STATE] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(17, 172 * 3), 32, glm::vec4(1));
			}
			if (numLemmings[DIGGER_STATE] != 0) {
				string num = to_string(numLemmings[DIGGER_STATE]);
				if (numLemmings[DIGGER_STATE] < 10) num = "0" + num;
				levelInfo.render(num, glm::vec2(322, 172 * 3), 32, glm::vec4(1));
			}
			

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			
			seleccionLemming->setPosition(lemmingSelected);
			if (renderSeleccionLemming)
				seleccionLemming->render();

			if (renderSeleccionPause) {
				seleccionLemming->setPosition(glm::vec2(124, 160));
				seleccionLemming->render();
			}

			if (renderSeleccionExplosion) {
				seleccionLemming->setPosition(glm::vec2(145, 160));
				seleccionLemming->render();
			}

			if (x2speed) {
				seleccionLemming->setPosition(glm::vec2(165, 160));
				seleccionLemming->render();
			}

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			cursor->render();

			break;

		case MENU:
			imagesProgram.use();
			imagesProgram.setUniformMatrix4f("projection", projection);
			imagesProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			imagesProgram.setUniformMatrix4f("modelview", modelview);
			menu->render(menuTexture);

			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);

			cursor->render();
			
			break;
	}

}


void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + 120;
	posY = mouseY / 3;

	switch (gamestate) {
		case PLAYING:
			if (bLeftButton && posY < 160) {				
				/*std::cout << id << endl;
				//convertir un Lemming a Blocker
				std::cout << mouseX << " " << mouseY << endl;
				std::cout << posX << " " << posY << endl;*/
				if (id != -1 && stateSelected) {
					if (numLemmings[lemmingsState] > 0) {
						--numLemmings[lemmingsState];
						lemmings[id].setState(lemmingsState);
					}
				}
				eraseMask(mouseX, mouseY);
			}
			else if (bRightButton && posY < 160) {
				if (id != -1) {

					lemmings[id].setState(BLOCKER_STATE);
				}
				applyMask(mouseX, mouseY);
			}
			//interfaz seleccionable
			else if (bLeftButton && posY >= 160) {
				std::cout << posX << " " << posY << endl;
				int num = (posX - 4 - 120) / 20;
				switch (num) {
					case 0:
						lemmingsState = CLIMBER_STATE;
						renderSeleccionLemming = true;
						lemmingSelected = glm::vec2(2, 160);
						seleccionLemming->setPosition(lemmingSelected);
						
						std::cout << "climber" << endl;
						stateSelected = true;
						break;
					case 1:
						lemmingsState = EXPLOSION_STATE;
						renderSeleccionLemming = true;
						lemmingSelected = glm::vec2(22, 160);
						seleccionLemming->setPosition(lemmingSelected);
						std::cout << "Explosion " << endl;
						stateSelected = true;
						break;
					case 2:
						lemmingsState = BLOCKER_STATE;
						renderSeleccionLemming = true;
						lemmingSelected = glm::vec2(43, 160);
						seleccionLemming->setPosition(lemmingSelected);
						std::cout << "blocker" << endl;
						stateSelected = true;
						break;
					case 3:
						lemmingsState = BUILDER_STATE;
						renderSeleccionLemming = true;
						lemmingSelected = glm::vec2(63, 160);
						seleccionLemming->setPosition(lemmingSelected);
						std::cout << "Builder" << endl;
						stateSelected = true;
						break;
					case 4:
						lemmingsState = BASHER;
						renderSeleccionLemming = true;
						lemmingSelected = glm::vec2(83, 160);
						seleccionLemming->setPosition(lemmingSelected);
						std::cout << "basher" << endl;
						stateSelected = true;
						break;
					case 5:
						lemmingsState = DIGGER_STATE;
						renderSeleccionLemming = true;
						lemmingSelected = glm::vec2(104, 160);
						seleccionLemming->setPosition(lemmingSelected);
						std::cout << "digger" << endl;
						stateSelected = true;
						break;
					case 6:
						//PAUSE
						renderSeleccionPause = !renderSeleccionPause;
						if (renderSeleccionPause) x2speed = false;
						std::cout << "pasue" << endl;
						break;
					case 7:
						//FULL EXPLOSION
						
						if (!exploding && allOut) {
							renderSeleccionExplosion = true;
							exploding = true;
							for (int i = 0; i < lemmings.size(); ++i) {
								lemmings[i].setState(EXPLOSION_STATE);
							}
						}
						std::cout << "Full Explosion" << endl;
						break;
					case 8:
						//x2 Speed
						x2speed = !x2speed;
						if (x2speed) renderSeleccionPause = false;
						std::cout << "x2Speed" << endl;
						break;

				}
			}
			break;

		case MENU:
			if (bLeftButton) {
				gamestate = PLAYING;
				init();
			}
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
		for(int x=max(0, posX-3); x<=min(maskTexture.width()-1, posX+3); x++)
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

