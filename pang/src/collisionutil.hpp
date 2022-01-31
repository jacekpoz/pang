#pragma once

#include "components/components.hpp"
#include "util.hpp"

#include <variant>
#include <iostream>

#include <entt/entt.hpp>

// this is gonna be spaghetti so redo this idk how

// I stole the 2 following methods from SFML source cause my shit didn't work
inline bool collidesVertical(Position p1, Hitbox r1, Position p2, Hitbox r2) {

	float top1 = p1.pos.y - r1.h / 2;
	float bottom1 = p1.pos.y + r1.h / 2;

	float top2 = p2.pos.y - r2.h / 2;
	float bottom2 = p2.pos.y + r2.h / 2;

	float r1MinY = std::min(top1, bottom1);
	float r1MaxY = std::max(top1, bottom1);

	float r2MinY = std::min(top2, bottom2);
	float r2MaxY = std::max(top2, bottom2);

	float interTop = std::max(r1MinY, r2MinY);
	float interBottom = std::min(r1MaxY, r2MaxY);

	return interTop < interBottom;
}

inline bool collidesHorizontal(Position p1, Hitbox r1, Position p2, Hitbox r2) {

	float right1 = p1.pos.x + r1.w / 2;
	float left1 = p1.pos.x - r1.w / 2;
	
	float right2 = p2.pos.x + r2.w / 2;
	float left2 = p2.pos.x - r2.w / 2;

	float r1MinX = std::min(left1, right1);
	float r1MaxX = std::max(left1, right1);

	float r2MinX = std::min(left2, right2);
	float r2MaxX = std::max(left2, right2);

	float interLeft = std::max(r1MinX, r2MinX);
	float interRight = std::min(r1MaxX, r2MaxX);

	return interLeft < interRight;
}

inline bool collides(Position p1, Hitbox h1, Position p2, Hitbox h2) {
	return collidesVertical(p1, h1, p2, h2) && collidesHorizontal(p1, h1, p2, h2);
}
