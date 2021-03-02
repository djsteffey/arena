#pragma once

#include <SFML/Graphics.hpp>

namespace arena {
	class ScreenAbstract {
	public:
		ScreenAbstract();
		virtual ~ScreenAbstract();

		virtual bool init(sf::RenderWindow* rw) = 0;
		virtual void update(int ms) = 0;
		virtual void draw(sf::RenderWindow* rw) = 0;

	protected:

	private:
	};
}
