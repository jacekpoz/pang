#include "CollisionSystem.hpp"

#include <iostream>
#include <variant>

CollisionSystem::CollisionSystem(entt::registry& r) 
	: System(r) {}

void CollisionSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	
	const auto players = registry.view<Player, Position, Hitbox>();
	const auto balls = registry.view<Ball, Position, Hitbox>();
	const auto walls = registry.view<Wall, Position, Hitbox>();

	for (const auto player : players) {
		const auto [h, a, v, p] = registry.get<Health, Acceleration, Velocity, Position>(player);
		std::cout << 
		"h: " << h.health << "; d: " << h.damaged << "; t: " << h.timeLeft << "; " <<
		"c: " << h.collides << "; " <<
		"aX: " << a.accel.x << "; aY: " << a.accel.y << "; " <<
		"vX: " << v.vel.x << "; vY: " << v.vel.y << "; " <<
		"pX: " << p.pos.x << "; pY: " << p.pos.y << "; " << 
		"lpX: " << p.lastPos.x << "; lpY: " << p.lastPos.y << "; \n";

		const auto [pPos, pHitbox] = registry.get<Position, Hitbox>(player);

		updateX(deltaTime, scale, player);

		pPos = registry.get<Position>(player);

		for (const auto wall : walls) {
			const auto [wPos, wHitbox] = registry.get<Position, Hitbox>(wall);
			
			if (collides(pPos, pHitbox, wPos, wHitbox)) {
				registry.patch<Position>(player, [](auto &pos) { pos.pos.x = pos.lastPos.x; });
				registry.patch<Velocity>(player, [](auto &vel) { vel.vel.x = 0; });
				registry.patch<Acceleration>(player, [](auto &accel) { accel.accel.x = 0; });
			}
		}

		updateY(deltaTime, scale, player);

		pPos = registry.get<Position>(player);

		for (const auto wall : walls) {
			const auto [wPos, wHitbox] = registry.get<Position, Hitbox>(wall);

			if (collides(pPos, pHitbox, wPos, wHitbox)) {
				registry.patch<Position>(player, [](auto &pos) { pos.pos.y = pos.lastPos.y; });
				registry.patch<Velocity>(player, [](auto &vel) { vel.vel.y = 0; });
				registry.patch<Acceleration>(player, [](auto &accel) { accel.accel.y = 0; });
			}
		}

		bool playerCollides = false;
		for (const auto ball : balls) {
			const auto [bPos, bHitbox] = registry.get<Position, Hitbox>(ball);
			if (collides(pPos, pHitbox, bPos, bHitbox)) playerCollides = true;
		}
		registry.patch<Health>(player, [playerCollides](auto &h) { h.collides = playerCollides; });

	}
	for (const auto ball : balls) {
		const auto [bPos, bHitbox] = registry.get<Position, Hitbox>(ball);

		updateX(deltaTime, scale, ball);

		bPos = registry.get<Position>(ball);

		for (const auto wall : walls) {
			const auto [wPos, wHitbox] = registry.get<Position, Hitbox>(wall);

			if (collides(bPos, bHitbox, wPos, wHitbox)) {
				registry.patch<Position>(ball, [](auto &pos) { pos.pos.x = pos.lastPos.x; });
				registry.patch<Velocity>(ball, [](auto &vel) { vel.vel.x = -vel.vel.x; });
			}
		}

		updateY(deltaTime, scale, ball);

		bPos = registry.get<Position>(ball);

		for (const auto wall : walls) {
			const auto [wPos, wHitbox] = registry.get<Position, Hitbox>(wall);

			if (collides(bPos, bHitbox, wPos, wHitbox)) {
				registry.patch<Position>(ball, [](auto &pos) { pos.pos.y = pos.lastPos.y; });
				registry.patch<Velocity>(ball, [](auto &vel) { vel.vel.y = -vel.vel.y; });
			}
		}
	}

}

void CollisionSystem::updateX(const float deltaTime, const sf::Vector2f scale, const entt::entity entity) {
	const auto accel = registry.get<Acceleration>(entity);

	registry.patch<Velocity>(entity, [accel, deltaTime, scale](auto &vel) {
		vel.vel.x += (accel.accel.x * deltaTime)/* * scale.x*/;
	});
	auto vel = registry.get<Velocity>(entity);

	if (vel.vel.x > vel.maxVel.x) 
		registry.patch<Velocity>(entity, [deltaTime, scale](auto &vel) { vel.vel.x = vel.maxVel.x; });
	if (vel.vel.x < -vel.maxVel.x) 
		registry.patch<Velocity>(entity, [deltaTime, scale](auto &vel) { vel.vel.x = -vel.maxVel.x; });
	vel = registry.get<Velocity>(entity);
	
	registry.patch<Position>(entity, [vel, deltaTime, scale](auto &pos) {
		pos.lastPos.x = pos.pos.x;
		pos.pos.x += (vel.vel.x * deltaTime)/* * scale.x*/;
	});
}   	
    
void CollisionSystem::updateY(const float deltaTime, const sf::Vector2f scale, const entt::entity entity) {
	const auto accel = registry.get<Acceleration>(entity);

	registry.patch<Velocity>(entity, [accel, deltaTime, scale](auto &vel) {
		vel.vel.y += (accel.accel.y * deltaTime)/* * scale.y*/;
	});
	auto vel = registry.get<Velocity>(entity);
	
	if (vel.vel.y > vel.maxVel.y) 
		registry.patch<Velocity>(entity, [deltaTime, scale](auto &vel) { vel.vel.y = vel.maxVel.y; });
	if (vel.vel.y < -vel.maxVel.y) 
		registry.patch<Velocity>(entity, [deltaTime, scale](auto &vel) { vel.vel.y = -vel.maxVel.y; });
	vel = registry.get<Velocity>(entity);

	registry.patch<Position>(entity, [vel, deltaTime, scale](auto &pos) {
		pos.lastPos.y = pos.pos.y;
		pos.pos.y += (vel.vel.y * deltaTime)/* * scale.y*/;
	});
}

bool CollisionSystem::collides(const Position p1, const Hitbox h1, const Position p2, const Hitbox h2) {
	
	const float top1 = p1.pos.y - h1.h / 2;
	const float bottom1 = p1.pos.y + h1.h / 2;
	const float right1 = p1.pos.x + h1.w / 2;
	const float left1 = p1.pos.x - h1.w / 2;

	const float top2 = p2.pos.y - h2.h / 2;
	const float bottom2 = p2.pos.y + h2.h / 2;
	const float right2 = p2.pos.x + h2.w / 2;
	const float left2 = p2.pos.x - h2.w / 2;

	const float h1MinY = std::min(top1, bottom1);
	const float h1MaxY = std::max(top1, bottom1);
	const float h1MinX = std::min(left1, right1);
	const float h1MaxX = std::max(left1, right1);

	const float h2MinY = std::min(top2, bottom2);
	const float h2MaxY = std::max(top2, bottom2);
	const float h2MinX = std::min(left2, right2);
	const float h2MaxX = std::max(left2, right2);

	const float interTop = std::max(h1MinY, h2MinY);
	const float interBottom = std::min(h1MaxY, h2MaxY);

	const float interLeft = std::max(h1MinX, h2MinX);
	const float interRight = std::min(h1MaxX, h2MaxX);

	return (interTop < interBottom) && (interLeft < interRight);
}
