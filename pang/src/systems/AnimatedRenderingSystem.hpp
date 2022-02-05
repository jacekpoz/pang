#pragma once

#include "RenderingSystem.hpp"

#include <string>
#include <unordered_map>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

#include <entt/entt.hpp>

class AnimatedRenderingSystem : public RenderingSystem {
	
public:
	AnimatedRenderingSystem(entt::registry& r, sf::RenderWindow& w);

	void update(const float deltaTime, const sf::Vector2f scale, const bool debug);

private:
	sf::Clock animClock;
};
