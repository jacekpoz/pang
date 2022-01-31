#pragma once

#include "System.hpp"

class BallMovementSystem : public System {

public:
	BallMovementSystem(entt::registry& r);
	~BallMovementSystem();

	void update(const float deltaTime, sf::Vector2f scale);
};
