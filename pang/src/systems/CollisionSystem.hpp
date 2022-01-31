#pragma once

#include "System.hpp"

// I decided to merge together PhysicsSystem and this
// cause I don't know if I'm doing it the wrong way or
// my code is just shitty, I couldn't figure out a way to do this
// without splitting collision and updating the physics apart

class CollisionSystem : public System {

public:
	CollisionSystem(entt::registry& r);
	~CollisionSystem();

	void update(const float deltaTime, sf::Vector2f scale);
	// could do those 2 in 1 method with a boolean but too lazy for now
	void updateX(const float deltaTime, sf::Vector2f scale, entt::entity entity);
	void updateY(const float deltaTime, sf::Vector2f scale, entt::entity entity);

};
