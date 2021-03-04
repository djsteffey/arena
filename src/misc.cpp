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
			return std::sqrt(dx * dx + dy * dy);
		}

		int getTileDistanceManhattan(int tile_x0, int tile_y0, int tile_x1, int tile_y1) {
			int dx = tile_x1 - tile_x0;
			int dy = tile_y1 - tile_y0;
			return std::abs(dx) + std::abs(dy);
		}

		bool bresenham(int x1, int y1, int x2, int y2, std::function<bool(int x, int y)> callback) {
			int m_new = 2 * (y2 - y1);
			int slope_error_new = m_new - (x2 - x1);
			for (int x = x1, y = y1; x <= x2; x++)
			{
				// callback on this point
				if (callback(x, y) == false) {
					// dont want to continue
					return false;
				}

				// Add slope to increment angle formed 
				slope_error_new += m_new;

				// Slope error reached limit, time to 
				// increment y and update slope error. 
				if (slope_error_new >= 0)
				{
					y++;
					slope_error_new -= 2 * (x2 - x1);
				}
			}

			// complete
			return true;
		}
	}
}