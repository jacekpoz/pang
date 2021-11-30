#pragma once

#include "System.hpp"

/*
 * this system does generic physics shit I didn't know where to put
 * so things like changing the acceleration according to the force, 
 * change the velocity according to acceleration and so on
 * this should probably be the last system that gets updated due to
 * it resetting the forces of all entities and various other shit 
 * I don't care enough to explain
 */
class PhysicsSystem : public System {

public:
	PhysicsSystem();
	~PhysicsSystem();

	void update(const float deltaTime, sf::Vector2f scale);
};
