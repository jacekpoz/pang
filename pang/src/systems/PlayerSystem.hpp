#pragma once

#include "System.hpp"

// this system handles the player's position and sprite and shit depending on the state
// it should be run after PlayerStateSystem

class PlayerSystem : public System {
	
public:
	PlayerSystem(entt::registry& r);

	void update(const float deltaTime, const sf::Vector2f scale, const bool debug);
};
