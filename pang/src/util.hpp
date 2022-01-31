#pragma once

#include <cmath>

inline float distanceSquared(float x1, float y1, float x2, float y2) {
	float distX = x2 - x1;
	float distY = y2 - y1;
	return std::sqrt(distX * distX + distY * distY);
}   
