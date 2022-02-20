#include "PlayerSpriteSystem.hpp"

#include <iostream>

#include <SFML/Window/Keyboard.hpp>

PlayerSpriteSystem::PlayerSpriteSystem(entt::registry& r) 
	: System(r) {}

void PlayerSpriteSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	const auto players = registry.view<Player>();

	for (const auto player : players) {
		const auto [pl] = players.get(player);
		

		switch (pl.st) {
			case State::WalkingLeft:
				registry.patch<Sprite>(player, [](auto &s) {
					s.animated = true;
					s.path = "res/textures/player_walking_left.png";
					s.width = 64; s.height = 128; s.frames = 10;
					s.frameTime = 0.1f;
				});
				break;
			case State::WalkingRight:
				registry.patch<Sprite>(player, [](auto &s) {
					s.animated = true;
					s.path = "res/textures/player_walking_right.png";
					s.width = 64; s.height = 128; s.frames = 10;
					s.frameTime = 0.1f;
				});
				break;
			case State::Shooting:
				registry.patch<Sprite>(player, [](auto &s) {
					s.animated = false;
					s.path = "res/textures/player_shooting.png";
				});
			case State::Standing:
			default:
				registry.patch<Sprite>(player, [](auto &s) {
					s.animated = false;
					s.path = "res/textures/player_standing.png";
				});
				break;
		}
	}
}
