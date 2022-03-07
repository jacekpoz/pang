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

#include "InfoSystem.hpp"

#include <string>

#include <SFML/System/Vector2.hpp>

InfoSystem::InfoSystem(entt::registry& r, sf::RenderWindow& w) : RenderingSystem(r, w) {
	font.loadFromFile("res/fonts/PublicPixel.ttf");
	health.setFont(font);
	health.setFillColor(sf::Color::White);
	health.setCharacterSize(24);
	score.setFont(font);
	score.setFillColor(sf::Color::White);
	score.setCharacterSize(24);
}

void InfoSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	const auto players = registry.view<Player>();

	for (const auto player : players) {
		const auto [h, pl] = registry.get<Health, Player>(player);

		health.setString("Lives: " + std::to_string(h.health));
		health.setPosition(sf::Vector2f{10 * scale.x, 10 * scale.y});
		health.setScale(scale);
		score.setString("Score: " + std::to_string(pl.score));
		score.setPosition(sf::Vector2f{200 * scale.x, 10 * scale.y});
		score.setScale(scale);

		window.draw(health);
		window.draw(score);
	}
}
