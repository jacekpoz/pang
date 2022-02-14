#pragma once

#include "System.hpp"

class PlayerStateSystem : public System {

public:
	PlayerStateSystem(entt::registry& r);

	void update(const float deltaTime, const sf::Vector2f scale, const bool debug);
};
