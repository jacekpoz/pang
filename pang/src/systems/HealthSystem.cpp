#include "HealthSystem.hpp"

#include <iostream>

HealthSystem::HealthSystem(entt::registry& r) 
	: System(r) {}

void HealthSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	
	const auto players = registry.view<Player, Health>();

	for (const auto player : players) {
		auto& h = players.get<Health>(player);

		if (h.damaged) {
			if (h.timeLeft <= 0.f) {
				h.timeLeft = 0.f; 
				h.damaged = false;
				registry.patch<Sprite>(player, [](auto &s) {
					s.animated = false;
					s.path = "res/textures/player.png";
				});
			} else h.timeLeft -= 1.f * deltaTime;
			continue;
		}

		if (h.collides) h.damaged = true;
		else continue;

		if (h.health > 0) {
			--h.health;
			h.timeLeft = 1000.f * deltaTime;
			h.damaged = true; 
			registry.patch<Sprite>(player, [](auto &s) {
				s.animated = true;
				s.path = "res/textures/player_damaged.png";
				s.width = 64; s.height = 128; s.frames = 2;
				s.frameTime = 0.1f;
			});
		}
	}
}
