#pragma once

#include <SFML/System/Vector2.hpp>

struct Wall {

	enum class Type {
		Wall, 
		None, 
	};

	Type type;

	sf::Vector2i pos;
};
