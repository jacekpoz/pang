#include "CollisionSystem.hpp"
#include "../collisionutil.hpp"

#include <iostream>
#include <variant>

CollisionSystem::CollisionSystem(entt::registry& r) : 
	System(r) {}

CollisionSystem::~CollisionSystem() {}

void CollisionSystem::update(const float deltaTime, sf::Vector2f scale) {
	
	auto players = registry->view<Player, Position, Hitbox>();
	auto balls = registry->view<Ball, Position, Hitbox>();
	auto walls = registry->view<Wall, Position, Hitbox>();

	for (const auto player : players) {
		auto [a, v, p] = registry->get<Acceleration, Velocity, Position>(player);
		std::cout << 
		"aX: " << a.accel.x << "; aY: " << a.accel.y << "; " <<
		"vX: " << v.vel.x << "; vY: " << v.vel.y << "; " <<
		"pX: " << p.pos.x << "; pY: " << p.pos.y << "; " << 
		"lpX: " << p.lastPos.x << "; lpY: " << p.lastPos.y << "; \n";

		auto [pPos, pHitbox] = registry->get<Position, Hitbox>(player);

		updateX(deltaTime, scale, player);

		pPos = registry->get<Position>(player);

		for (const auto wall : walls) {
			auto [wPos, wHitbox] = registry->get<Position, Hitbox>(wall);
			
			if (collides(pPos, pHitbox, wPos, wHitbox)) {
				registry->patch<Position>(player, [](auto &pos) { pos.pos.x = pos.lastPos.x; });
				registry->patch<Velocity>(player, [](auto &vel) { vel.vel.x = 0; });
				registry->patch<Acceleration>(player, [](auto &accel) { accel.accel.x = 0; });
			}
		}

		updateY(deltaTime, scale, player);

		pPos = registry->get<Position>(player);

		for (const auto wall : walls) {
			auto [wPos, wHitbox] = registry->get<Position, Hitbox>(wall);

			if (collides(pPos, pHitbox, wPos, wHitbox)) {
				registry->patch<Position>(player, [](auto &pos) { pos.pos.y = pos.lastPos.y; });
				registry->patch<Velocity>(player, [](auto &vel) { vel.vel.y = 0; });
				registry->patch<Acceleration>(player, [](auto &accel) { accel.accel.y = 0; });
			}
		}

	}

}

void CollisionSystem::updateX(const float deltaTime, sf::Vector2f scale, entt::entity entity) {
	auto accel = registry->get<Acceleration>(entity);

	registry->patch<Velocity>(entity, [accel, deltaTime, scale](auto &vel) {
		vel.vel.x += (accel.accel.x * deltaTime)/* * scale.x*/;
	});
	auto vel = registry->get<Velocity>(entity);

	if (vel.vel.x > vel.maxVel.x) 
		registry->patch<Velocity>(entity, [deltaTime, scale](auto &vel) { vel.vel.x = vel.maxVel.x; });
	if (vel.vel.x < -vel.maxVel.x) 
		registry->patch<Velocity>(entity, [deltaTime, scale](auto &vel) { vel.vel.x = -vel.maxVel.x; });
	
	registry->patch<Position>(entity, [vel, deltaTime, scale](auto &pos) {
		pos.lastPos.x = pos.pos.x;
		pos.pos.x += (vel.vel.x * deltaTime)/* * scale.x*/;
	});
}   	
    
void CollisionSystem::updateY(const float deltaTime, sf::Vector2f scale, entt::entity entity) {
	auto accel = registry->get<Acceleration>(entity);

	registry->patch<Velocity>(entity, [accel, deltaTime, scale](auto &vel) {
		vel.vel.y += (accel.accel.y * deltaTime)/* * scale.y*/;
	});
	auto vel = registry->get<Velocity>(entity);
	
	if (vel.vel.y > vel.maxVel.y) 
		registry->patch<Velocity>(entity, [deltaTime, scale](auto &vel) { vel.vel.y = vel.maxVel.y; });
	if (vel.vel.y < -vel.maxVel.y) 
		registry->patch<Velocity>(entity, [deltaTime, scale](auto &vel) { vel.vel.y = -vel.maxVel.y; });
	vel = registry->get<Velocity>(entity);

	registry->patch<Position>(entity, [vel, deltaTime, scale](auto &pos) {
		pos.lastPos.y = pos.pos.y;
		pos.pos.y += (vel.vel.y * deltaTime)/* * scale.y*/;
	});
}
