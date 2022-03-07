#pragma once

#include "System.hpp"

class PlayerMovementSystem : public System {

public:
	PlayerMovementSystem(entt::registry& r);

	void update(const float deltaTime, const sf::Vector2f scale, const bool debug);
};
