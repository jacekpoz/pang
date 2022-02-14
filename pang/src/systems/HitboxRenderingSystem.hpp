#pragma once

#include "RenderingSystem.hpp"

class HitboxRenderingSystem : public RenderingSystem {

public:
	HitboxRenderingSystem(entt::registry& r, sf::RenderWindow& w);

	void update(const float deltaTime, const sf::Vector2f scale, const bool debug);
};
