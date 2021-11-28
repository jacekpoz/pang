#pragma once

#include "System.hpp"

class MovementSystem : public System {

public:
	MovementSystem();
	~MovementSystem();

	void update(const float deltaTime, sf::Vector2f scale);
};
