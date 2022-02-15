#include "PlayerSystem.hpp"

PlayerSystem::PlayerSystem(entt::registry& r) 
	: System(r) {}

void PlayerSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {

	const auto players = registry.view<Player>();

	for (const auto player : players) {	
		const auto [pl] = players.get(player);

		switch (pl.st) {
			case State::WalkingRight:
				registry.patch<Velocity>(player, [deltaTime, scale](auto& vel) {
					vel.vel.x += 500.f * deltaTime * scale.x;
				});
				break;
			case State::WalkingLeft:
				registry.patch<Velocity>(player, [deltaTime, scale](auto& vel) {
					vel.vel.x -= 500.f * deltaTime * scale.x;
				});
				break;
			default:
				break;
		}

		if (pl.st == pl.lastSt) continue;

		switch (pl.st) {
			case State::WalkingRight:
				registry.patch<Sprite>(player, [](auto &s) {
					s.animated = true;
					s.path = "res/textures/player_walking_right.png";
					s.width = 64; s.height = 128; s.frames = 10;
					s.frameTime = 0.1f;
				});
				break;
			case State::WalkingLeft:
				registry.patch<Sprite>(player, [](auto &s) {
					s.animated = true;
					s.path = "res/textures/player_walking_left.png";
					s.width = 64; s.height = 128; s.frames = 10;
					s.frameTime = 0.1f;
				});
				break;
			case State::Standing:
				registry.patch<Sprite>(player, [](auto& s) {
					s.animated = false;
					s.path = "res/textures/player.png";
				});
				break;
			default:
				break;
		}
	}
}
