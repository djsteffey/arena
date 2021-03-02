#pragma once

#include <SFML/Graphics.hpp>

namespace arena {
	class ScreenAbstract;

	class Game {
	public:
		Game();
		~Game();
		void run();

	protected:

	private:
		void processEvents();
		void update();
		void draw();

		sf::RenderWindow m_render_window;
		std::unique_ptr<ScreenAbstract> m_screen;
	};
}
