#pragma once

#include <string>
#include <functional>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

struct Button {
	sf::Text text;
	sf::Color baseColor;
	sf::Color hoverColor;
	bool isHover = false;
	sf::FloatRect bounds;

	std::string levelPath;
};
