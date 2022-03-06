#pragma once

#include "components/components.hpp"

#include <entt/entt.hpp>

#include <SFML/System/Vector2.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>

inline void createPlayer(entt::registry &r, const int health, const sf::Vector2f pos, const float mass) {

	auto player = r.create();

	r.emplace<Sprite>(player, "res/textures/player_standing.png");
	r.emplace<Hitbox>(player, 64.f, 128.f);
	r.emplace<Health>(player, 5);
	r.emplace<Position>(player, pos);
	r.emplace<Mass>(player, mass);
	r.emplace<Acceleration>(player, sf::Vector2f(0.f, 0.f), sf::Vector2f(50.f, 50.f));
	r.emplace<Velocity>(player, sf::Vector2f(0.f, 0.f), sf::Vector2f(500.f, 500.f));
	Weapon wpn{Weapon::Type::Hook};
	r.emplace<Player>(player, wpn);
}

inline void createWall(entt::registry &r, const sf::Vector2f pos, const bool breakable) {

	auto wall = r.create();

	Wall w;
	w.type = breakable ? Wall::Type::Breakable : Wall::Type::Wall;
	r.emplace<Wall>(wall, w);
	r.emplace<Sprite>(wall, w.type == Wall::Type::Wall ? "res/textures/wall.png" : "res/textures/breakable_wall.png");
	r.emplace<Hitbox>(wall, 16.f, 16.f);
	r.emplace<Position>(wall, pos);
}

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

inline void parseLevel(entt::registry &r, const std::string path, const sf::Vector2f offset) {

	std::ifstream ifs(path);
	const std::string file(std::istreambuf_iterator<char>{ifs}, {});

	int col = 0, row = 0;
	for (const char c : file) {

		sf::Vector2f pos{col * 16.f + offset.x, row * 16.f + offset.y};

		switch (c) {
			case '\n': 
				++row;
				col = 0;
				continue;
			case '#': createWall(r, pos, false); break;
			case '~': createWall(r, pos, true); break;
			case 'P': createPlayer(r, 5, pos, 50.f); break;
			case '1': createBall(r, pos, sf::Vector2f{200.f, 100.f}, 50.f, 1); break;
			case '2': createBall(r, pos, sf::Vector2f{200.f, 100.f}, 50.f, 2); break;
			case '3': createBall(r, pos, sf::Vector2f{200.f, 100.f}, 50.f, 3); break;
			case '4': createBall(r, pos, sf::Vector2f{200.f, 100.f}, 50.f, 4); break;
			case ' ':
			default:
				break;
		}
		++col;
	}
}
