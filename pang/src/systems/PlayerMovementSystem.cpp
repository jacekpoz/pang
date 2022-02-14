#include "PlayerMovementSystem.hpp"

#include <iostream>

#include <SFML/Window/Keyboard.hpp>

PlayerMovementSystem::PlayerMovementSystem(entt::registry& r) 
	: System(r) {}

void PlayerMovementSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {

	const auto players = registry.view<Player>();

	for (const auto player : players) {	
		const auto [pl] = players.get(player);
		if (pl.st == State::WalkingLeft) std::cout << "DUPA !!!!";

		switch (pl.st) {
			case State::WalkingRight:
				registry.patch<Velocity>(player, [deltaTime, scale](auto& vel) {
					std::cout << "+dupa";
					vel.vel.x += 500.f * deltaTime * scale.x;
				});
				break;
			case State::WalkingLeft:
				registry.patch<Velocity>(player, [deltaTime, scale](auto& vel) {
					std::cout << "-dupa";
					vel.vel.x -= 500.f * deltaTime * scale.x;
				});
				break;
			default:
				break;
		}
	}
}
