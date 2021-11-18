#include "FPS.hpp"

const unsigned int FPS::getFPS() const { return mFPS; }

void FPS::update() {
	if (mClock.getElapsedTime().asSeconds() >= 1.f) {
		mFPS = mFrame;
		mFrame = 0;
		mClock.restart();
	}
	++mFrame;
}
