#pragma once

#include "components/components.hpp"

#include <entt/entt.hpp>

#include <SFML/System/Vector2.hpp>

#include <iostream>

inline void createBall(entt::registry &r, const sf::Vector2f pos, const sf::Vector2f vel, const float mass, const int size) {

	if (size < 1) return;

	float ballSize = static_cast<float>(pow(2, size) * 10);

	auto ball = r.create(); 

	r.emplace<Ball>(ball, size);
	r.emplace<Sprite>(ball, "res/textures/ball" + std::to_string(size) + ".png");
	r.emplace<Hitbox>(ball, ballSize, ballSize);
	r.emplace<Position>(ball, pos);
	r.emplace<Mass>(ball, mass);
	r.emplace<Acceleration>(ball, sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f));
	r.emplace<Velocity>(ball, vel, sf::Vector2f(100.f, 100.f));
}

inline void hitBall(entt::registry &r, entt::entity ball) {
	if (!r.all_of<Ball>(ball)) return;

	const auto [oldPos, oldVel, oldBall, oldMass, oldHitbox] = r.get<Position, Velocity, Ball, Mass, Hitbox>(ball);

	r.destroy(ball);

	const int newSize = oldBall.size - 1;

	createBall(r, sf::Vector2f(oldPos.pos.x + oldHitbox.w, oldPos.pos.y + 1.f), oldVel.vel, oldMass.mass, newSize);
	createBall(r, sf::Vector2f(oldPos.pos.x - oldHitbox.w, oldPos.pos.y - 1.f), oldVel.vel, oldMass.mass, newSize);
}
