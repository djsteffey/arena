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
	}
}