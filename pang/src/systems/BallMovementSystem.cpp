#include "BallMovementSystem.hpp"

BallMovementSystem::BallMovementSystem(entt::registry& r) : System(r) {}

BallMovementSystem::~BallMovementSystem() {}

void BallMovementSystem::update(const float deltaTime, sf::Vector2f scale) {

	auto balls = registry->view<Ball>();

	for (auto ball : balls) {
		
	}
}
