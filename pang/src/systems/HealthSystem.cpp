#include "HealthSystem.hpp"

HealthSystem::HealthSystem(entt::registry& r) 
	: System(r) {}

void HealthSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	
	auto players = registry.view<Player, Health>();

	for (auto player : players) {
		auto& h = players.get<Health>(player);

		if (h.damaged) {
			if (h.timeLeft <= 0.f) {
				h.timeLeft = 0.f; 
				h.damaged = false;
			} else h.timeLeft -= 0.01f;
			continue;
		}

		if (h.collides) h.damaged = true;

		if (h.health > 0) {
			--h.health;
			h.timeLeft = 1000.f * deltaTime;
			h.damaged = true; 
			registry.patch<Sprite>(player, [](auto &s) {
				s.animated = true;
				s.path = "res/textures/player_damaged.png";
				s.width = 64; s.height = 128; s.frames = 2;
				s.frameTime = 50.f;
			});
		}
	}
}
