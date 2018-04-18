#ifndef _LEVEL
#define _LEVEL

#include <string>
#include <glm/glm.hpp>

using namespace std;


struct Level {
	int levelLemmings;
	int needToWin;
	float mapLength;
	string colortexture;
	string masktexture;
	glm::vec2 openDoorPos;
	string openDoortexture;
	glm::vec2 finishDoorPos;
	glm::vec2 finishDoorSize;
	string finishDoortexture;
	int levelTime;
	float ratio;
	string levelSong;

	// 0:BASHER, 1:DIGGER, 2:CLIMBER, 3:EXPLOSION, 4:BLOCKER, 5:BUILDER
	int numLemmings[6];
};


#endif // _LEVEL