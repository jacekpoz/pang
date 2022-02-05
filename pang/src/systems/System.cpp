#include "System.hpp"

System::System(entt::registry& r) 
	: isPaused(false), registry(r) {}

void System::pause() {
	isPaused = true;
}

void System::resume() {
	isPaused = false;
}

void System::togglePause() {
	if (!isPaused) pause();
	else if (isPaused) resume();
}
