#pragma once

#include <SFML/System.hpp>
#include <vector>

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
	}
}
