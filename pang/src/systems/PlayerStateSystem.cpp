#include "PlayerStateSystem.hpp"

#include <iostream>

#include <SFML/Window/Keyboard.hpp>

PlayerStateSystem::PlayerStateSystem(entt::registry& r) 
	: System(r) {}

void PlayerStateSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	const auto players = registry.view<Player>();

	for (const auto player : players) {
		const auto vel = registry.get<Velocity>(player);

		if (vel.vel.x == 0.f) 
			registry.patch<Player>(player, [](auto& pl) { pl.st = State::Standing; });

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
			registry.patch<Player>(player, [](auto& pl) { pl.st = State::WalkingLeft; });
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
			registry.patch<Player>(player, [](auto& pl) { pl.st = State::WalkingRight; });

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
			registry.patch<Velocity>(player, [deltaTime, scale](auto& vel) {
				if (vel.vel.x > 0.f) {
					vel.vel.x -= 75.f * deltaTime * scale.x;
					if (vel.vel.x < 0.f) vel.vel.x = 0.f;
				}
			});
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
			registry.patch<Velocity>(player, [deltaTime, scale](auto& vel) {
				if (vel.vel.x < 0.f) {
					vel.vel.x += 75.f * deltaTime * scale.x;
					if (vel.vel.x > 0.f) vel.vel.x = 0.f;
				}
			});

		if (vel.vel.y > 0.f) 
			registry.patch<Player>(player, [](auto& pl) { pl.st = State::Falling; });

		if (vel.vel.y < 0.f) 
			registry.patch<Player>(player, [](auto& pl) { pl.st = State::Climbing; });
	}
}
