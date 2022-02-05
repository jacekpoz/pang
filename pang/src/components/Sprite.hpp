#pragma once

#include <string>

struct Sprite {
	std::string path;
	bool animated = false;
	// if the sprite is animated 
	// it's probably in a spritesheet
	// so we need those for the uhhhhh ummmmm
	int width = 0, height = 0;
	int frames = 0;
	// in seconds
	float frameTime = 0;
	float timeElapsed = 0;

	bool visible = true;
};
