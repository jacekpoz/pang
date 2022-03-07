
#pragma once

#include "System.hpp"

class PlayerSpriteSystem : public System {

public:
	PlayerSpriteSystem(entt::registry& r);

	void update(const float deltaTime, const sf::Vector2f scale, const bool debug);
};
