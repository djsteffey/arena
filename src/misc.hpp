#pragma once

#include <SFML/System.hpp>
#include <vector>
#include <functional>

namespace arena {
	namespace misc {
		enum class EDirection {
			UP, DOWN, LEFT, RIGHT
		};
		const std::vector<EDirection> DIRECTIONS{ EDirection::UP, EDirection::DOWN, EDirection::LEFT, EDirection::RIGHT };
		EDirection randomDirection();
		sf::Vector2i directionToVector2i(EDirection dir);

		void log(std::string tag, std::string format, ...);

		unsigned long generateId();

		int getRandomIntInRange(int min, int max);

		float getTileDistance(int tile_x0, int tile_y0, int tile_x1, int tile_y1);
		int getTileDistanceManhattan(int tile_x0, int tile_y0, int tile_x1, int tile_y1);

		bool bresenham(int x1, int y1, int x2, int y2, std::function<bool(int x, int y)> callback);
	}
}
