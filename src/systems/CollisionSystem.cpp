// Pang - a remake of the arcade game for the motorola science cup 2021
// Copyright (C) 2022 jacekpoz
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// 
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "CollisionSystem.hpp"

#include "../entityhelper.hpp"

#include <iostream>
#include <variant>

CollisionSystem::CollisionSystem(entt::registry& r) 
	: System(r) {}

void CollisionSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	
	const auto players = registry.view<Player, Position, Hitbox>();
	const auto balls = registry.view<Ball, Position, Hitbox>();
	const auto walls = registry.view<Wall, Position, Hitbox>();
	const auto projectiles = registry.view<Projectile, Position, Hitbox>();

	for (const auto player : players) {
//		const auto [h, a, v, p, pl] = registry.get<Health, Acceleration, Velocity, Position, Player>(player);
//		std::cout << 
//		"dt: " << deltaTime << "; " << 
//		"h: " << h.health << "; d: " << h.damaged << "; t: " << h.timeLeft << "; " <<
//		"c: " << h.collides << "; tS: " << pl.timeShooting << "; " <<
//		"aX: " << a.accel.x << "; aY: " << a.accel.y << "; " <<
//		"vX: " << v.vel.x << "; vY: " << v.vel.y << "; " <<
//		"pX: " << p.pos.x << "; pY: " << p.pos.y << "; " << 
//		"lpX: " << p.lastPos.x << "; lpY: " << p.lastPos.y << "; " <<
//		"proj: " << pl.wpn.projNum << "; \n";

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

	for (const auto projectile : projectiles) {
		const auto [pPos, pHitbox] = registry.get<Position, Hitbox>(projectile);

		for (const auto wall : walls) {
			const auto [wWall, wPos, wHitbox] = registry.get<Wall, Position, Hitbox>(wall);

			if (collides(pPos, pHitbox, wPos, wHitbox)) {
				// I can't be bothered to do this in a normal way
				for (const auto player : registry.view<Player>())
					registry.patch<Player>(player, [](auto &pl) { if (pl.wpn.projNum > 0) --pl.wpn.projNum; });
				registry.patch<Projectile>(projectile, [](auto &pr) { pr.dead = true; });
				if (wWall.type == Wall::Type::Breakable) registry.destroy(wall);
			}
		}

		for (const auto ball : balls) {
			const auto [bBall, bPos, bHitbox] = registry.get<Ball, Position, Hitbox>(ball);

			if (collides(pPos, pHitbox, bPos, bHitbox)) {
				for (const auto player : registry.view<Player>())
					registry.patch<Player>(player, [bBall](auto &pl) {
						if (pl.wpn.projNum > 0) --pl.wpn.projNum;
						pl.score += bBall.size * 100;
					});
				registry.patch<Projectile>(projectile, [](auto &pr) { pr.dead = true; });
				hitBall(registry, ball);
			}
		}
	}

}

void CollisionSystem::updateX(const float deltaTime, const sf::Vector2f scale, const entt::entity entity) {
	auto accel = registry.get<Acceleration>(entity);

	if (accel.accel.x > accel.maxAccel.x) 
		registry.patch<Acceleration>(entity, [](auto &accel) { accel.accel.x = accel.maxAccel.x; });
	if (accel.accel.x < -accel.maxAccel.x) 
		registry.patch<Acceleration>(entity, [](auto &accel) { accel.accel.x = -accel.maxAccel.x; });
	accel = registry.get<Acceleration>(entity);

	registry.patch<Velocity>(entity, [accel, deltaTime](auto &vel) {
		vel.vel.x += accel.accel.x * deltaTime;
	});
	auto vel = registry.get<Velocity>(entity);

	if (vel.vel.x > vel.maxVel.x) 
		registry.patch<Velocity>(entity, [](auto &vel) { vel.vel.x = vel.maxVel.x; });
	if (vel.vel.x < -vel.maxVel.x) 
		registry.patch<Velocity>(entity, [](auto &vel) { vel.vel.x = -vel.maxVel.x; });
	vel = registry.get<Velocity>(entity);
	
	registry.patch<Position>(entity, [vel, deltaTime](auto &pos) {
		pos.lastPos.x = pos.pos.x;
		pos.pos.x += vel.vel.x * deltaTime;
	});
}   	
    
void CollisionSystem::updateY(const float deltaTime, const sf::Vector2f scale, const entt::entity entity) {
	auto accel = registry.get<Acceleration>(entity);

	if (accel.accel.y > accel.maxAccel.y) 
		registry.patch<Acceleration>(entity, [](auto &accel) { accel.accel.y = accel.maxAccel.y; });
	if (accel.accel.y < -accel.maxAccel.y) 
		registry.patch<Acceleration>(entity, [](auto &accel) { accel.accel.y = -accel.maxAccel.y; });
	accel = registry.get<Acceleration>(entity);

	registry.patch<Velocity>(entity, [accel, deltaTime](auto &vel) {
		vel.vel.y += accel.accel.y * deltaTime;
	});
	auto vel = registry.get<Velocity>(entity);
	
	if (vel.vel.y > vel.maxVel.y) 
		registry.patch<Velocity>(entity, [](auto &vel) { vel.vel.y = vel.maxVel.y; });
	if (vel.vel.y < -vel.maxVel.y) 
		registry.patch<Velocity>(entity, [](auto &vel) { vel.vel.y = -vel.maxVel.y; });
	vel = registry.get<Velocity>(entity);

	registry.patch<Position>(entity, [vel, deltaTime](auto &pos) {
		pos.lastPos.y = pos.pos.y;
		pos.pos.y += vel.vel.y * deltaTime;
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
