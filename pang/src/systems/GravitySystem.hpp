#pragma once

#include "System.hpp"

/*
 * yes the sole purpose of this system is to handle gravity
 * it's literally gonna be like 3 lines of actual code that gets ran but I don't care
 * if you have a problem with it go cry about it
 */
class GravitySystem : public System {

public:
	GravitySystem(entt::registry& r);

	void update(const float deltaTime, const sf::Vector2f scale, const bool debug);

private:
	// gravitational constant
	const float g = 9.81f;
};
