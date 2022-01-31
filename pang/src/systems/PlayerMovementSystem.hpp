#pragma once

#include "System.hpp"

class PlayerMovementSystem : public System {

public:
	PlayerMovementSystem(entt::registry& r);
	~PlayerMovementSystem();

	void update(const float deltaTime, sf::Vector2f scale);
private:
	bool isSpacePressed = false;
};
