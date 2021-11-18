#pragma once

#include <SFML/System/Clock.hpp>

// stolen from https://stackoverflow.com/questions/66787248/cant-get-my-fps-number-to-show-with-sfml

class FPS {

public:

	const unsigned int getFPS() const;
	void update();
	
private:
	unsigned int mFrame = 0;
	unsigned int mFPS = 0;
	sf::Clock mClock;
};
