#pragma once

#include "System.hpp"

/*
 * this system does generic physics shit I didn't know where to put
 * so things like changing the acceleration according to the force, 
 * change the velocity according to acceleration and so on
 * this should probably be put before CollisionSystem but after everything else
 * cause first all the systems do their own shit and then this one "cleans the rest up"
 * like it does the shit nobody else wants to and then the CollisionSystem should handle
 * collisions after all of those forces and shit have been calculated you know what I'm talking about
 */
class PhysicsSystem : public System {

public:
	PhysicsSystem();
	~PhysicsSystem();

	void update(const float deltaTime, sf::Vector2f scale);
};
