#pragma once

#include "System.hpp"

class HealthSystem : public System {

public:
	HealthSystem(entt::registry& r);

	void update(const float deltaTime, const sf::Vector2f scale, const bool debug);
};
