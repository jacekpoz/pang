#include "PlayerMovementSystem.hpp"

#include <iostream>

#include <SFML/Window/Keyboard.hpp>

PlayerMovementSystem::PlayerMovementSystem(entt::registry &r) : System(r) {}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::update(const float deltaTime, sf::Vector2f scale) {

	auto players = registry->view<Player>();

	// yeah I know this isn't the best way to handle this
	// but I haven't learned event dispatchers in entt
	// or whatever they're called Idk maybe I'll do that soon
	// but it's not that high of a priority right know
	if (
		sf::Keyboard::isKeyPressed(sf::Keyboard::A) || 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
	) {
		for (auto entity : players) {
			registry->patch<Velocity>(entity, [deltaTime, scale](auto &vel) {
				vel.vel.x -= 250.f * deltaTime * scale.x;
			});
//			registry->patch<Acceleration>(entity, [deltaTime, scale](auto &accel) {
//				accel.accel.x -= 10.f * deltaTime * scale.x;
//			});
		} 
	}

	if (
		sf::Keyboard::isKeyPressed(sf::Keyboard::D) || 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
	) {
		for (auto entity : players) {
			registry->patch<Velocity>(entity, [deltaTime, scale](auto &vel) {
				vel.vel.x += 250.f * deltaTime * scale.x;
			});
//			registry->patch<Acceleration>(entity, [deltaTime, scale](auto &accel) {
//				accel.accel.x += 10.f * deltaTime * scale.x;
//			});
		} 
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (!isSpacePressed) {
			isSpacePressed = true;
			for (auto entity : players) {
				auto mass = registry->get<Mass>(entity);
//				registry->patch<Acceleration>(entity, [mass, deltaTime, scale](auto &accel) {
//					accel.accel.y -= 10.f * mass.mass * deltaTime * scale.y;
//				});
				registry->patch<Velocity>(entity, [mass, deltaTime, scale](auto &vel) {
					vel.vel.y -= 1000.f * mass.mass * deltaTime * scale.y;
				});
			}
		}
	} else {
		isSpacePressed = false;
	}
}
