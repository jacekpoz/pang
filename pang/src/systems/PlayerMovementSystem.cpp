#include "PlayerMovementSystem.hpp"

#include <iostream>

#include <SFML/Window/Keyboard.hpp>

PlayerMovementSystem::PlayerMovementSystem(entt::registry& r) 
	: System(r) {}

void PlayerMovementSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {

	const auto players = registry.view<Player>();

	// yeah I know this isn't the best way to handle this
	// but I haven't learned event dispatchers in entt
	// or whatever they're called Idk maybe I'll do that soon
	// but it's not that high of a priority right know
	
	for (const auto player : players) {	
		if (
			sf::Keyboard::isKeyPressed(sf::Keyboard::A) 
		) {
			registry.patch<Velocity>(player, [deltaTime, scale](auto &vel) {
				vel.vel.x -= 250.f * deltaTime * scale.x;
			});
		} 
		else {
			registry.patch<Velocity>(player, [deltaTime, scale](auto &vel) {
				if (vel.vel.x < 0.f) {
					vel.vel.x += 75.f * deltaTime * scale.x;
					if (vel.vel.x > 0.f) vel.vel.x = 0.f;
				}
			});
		}
		if (
			sf::Keyboard::isKeyPressed(sf::Keyboard::D) 
		) {
			registry.patch<Velocity>(player, [deltaTime, scale](auto &vel) {
				vel.vel.x += 250.f * deltaTime * scale.x;
			});
		} 
		else {
			registry.patch<Velocity>(player, [deltaTime, scale](auto &vel) {
				if (vel.vel.x > 0.f) {
					vel.vel.x -= 75.f * deltaTime * scale.x;
					if (vel.vel.x < 0.f) vel.vel.x = 0.f;
				}
			});
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			if (!isSpacePressed) {
				isSpacePressed = true;
				auto mass = registry.get<Mass>(player);
				registry.patch<Velocity>(player, [mass, deltaTime, scale](auto &vel) {
					vel.vel.y -= 1000.f * mass.mass * deltaTime * scale.y;
				});
			}
		} else {
			isSpacePressed = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			
		}
	}

}
