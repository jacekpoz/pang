#pragma once

#include "components/components.hpp"

#include <string>
#include <fstream>
#include <iterator>
#include <vector>

inline std::vector<Wall> parseLevel(std::string path) {

	std::ifstream ifs(path);
	std::string file(std::istreambuf_iterator<char>{ifs}, {});

	std::vector<Wall> ret;
	int col = 0, row = 0;
	Wall::Type type;
	for (char& c : file) {
		switch (c) {
			case '\n': 
				++row;
				col = 0;
				continue;
			case '#':
				type = Wall::Type::Wall;
				break;
			default:
				type = Wall::Type::None;
				break;
		}
		Wall w;
		w.type = type;
		w.pos = sf::Vector2i(col, row);
		ret.push_back(w);

		++col;
	}
	return ret;
}
