#pragma once

#include <variant>

#include "../util.hpp"

struct Hitbox {
	bool isRect;
	std::variant<Rect, Circ> rect, circ;
};
