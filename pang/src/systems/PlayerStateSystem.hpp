#pragma once

#include "System.hpp"

// this system will change the state of the player depending on certain conditions
// it will NOT handle the player's movement and shit depending on the state
// for that see PlayerSystem

class PlayerStateSystem : public System {

public:
	PlayerStateSystem(entt::registry& r);

	void update(const float deltaTime, const sf::Vector2f scale, const bool debug);
};
