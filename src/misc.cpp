#include "misc.hpp"
#include <cstdlib>
#include <cstdarg>
#include <cstdio>

namespace arena {
	namespace misc {
		EDirection randomDirection() {
			return (EDirection)(std::rand() % 4);
		}

		sf::Vector2i directionToVector2i(EDirection dir) {
			switch (dir) {
			case EDirection::UP: { return sf::Vector2i(0, -1); } break;
			case EDirection::DOWN: { return sf::Vector2i(0, 1); } break;
			case EDirection::LEFT: { return sf::Vector2i(-1, 0); } break;
			case EDirection::RIGHT: { return sf::Vector2i(1, 0); } break;
			}
			return sf::Vector2i(0, 0);
		}

		void log(std::string tag, std::string format, ...) {
			// just forward on to printf
			printf("%s: ", tag.c_str());

			va_list args;
			va_start(args, format);
			vprintf(format.c_str(), args);
			printf("\n");
			va_end(args);
		}

		unsigned long generateId() {
			static unsigned long next_id = 0;
			return ++next_id;
		}

		int getRandomIntInRange(int min, int max) {
			int range = max - min + 1;
			return (std::rand() % range) + min;

		}

		float getTileDistance(int tile_x0, int tile_y0, int tile_x1, int tile_y1) {
			int dx = tile_x1 - tile_x0;
			int dy = tile_y1 - tile_y0;
			return static_cast<float>(std::sqrt(dx * dx + dy * dy));
		}

		int getTileDistanceManhattan(int tile_x0, int tile_y0, int tile_x1, int tile_y1) {
			int dx = tile_x1 - tile_x0;
			int dy = tile_y1 - tile_y0;
			return std::abs(dx) + std::abs(dy);
		}

		// http://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm
		bool bresenham(int x1, int y1, int const x2, int const y2, std::function<bool(int x, int y)> callback) {
			int delta_x(x2 - x1);
			// if x1 == x2, then it does not matter what we set here
			signed char const ix((delta_x > 0) - (delta_x < 0));
			delta_x = std::abs(delta_x) << 1;

			int delta_y(y2 - y1);
			// if y1 == y2, then it does not matter what we set here
			signed char const iy((delta_y > 0) - (delta_y < 0));
			delta_y = std::abs(delta_y) << 1;

			if (callback(x1, y1) == false) {
				// dont want to play anymore
				return false;
			}

			if (delta_x >= delta_y)
			{
				// error may go below zero
				int error(delta_y - (delta_x >> 1));

				while (x1 != x2)
				{
					// reduce error, while taking into account the corner case of error == 0
					if ((error > 0) || (!error && (ix > 0)))
					{
						error -= delta_x;
						y1 += iy;
					}
					// else do nothing

					error += delta_y;
					x1 += ix;

					if (callback(x1, y1) == false) {
						// dont want to play anymore
						return false;
					}
				}
			}
			else
			{
				// error may go below zero
				int error(delta_x - (delta_y >> 1));

				while (y1 != y2)
				{
					// reduce error, while taking into account the corner case of error == 0
					if ((error > 0) || (!error && (iy > 0)))
					{
						error -= delta_y;
						x1 += ix;
					}
					// else do nothing

					error += delta_x;
					y1 += iy;

					if (callback(x1, y1) == false) {
						// dont want to play anymore
						return false;
					}
				}
			}

			// finished
			return true;
		}
	}
}