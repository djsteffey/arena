#include "Game.hpp"
#include "ScreenBattle.hpp"
#include "misc.hpp"
#include <ctime>

namespace arena {
	Game::Game() {
		this->m_screen = nullptr;
		std::srand(static_cast<unsigned int>(std::time(0)));
	}

	Game::~Game() {

	}

	void Game::run() {
		// create the render window
		this->m_render_window.create(
			sf::VideoMode(1280, 720),
			"Arena",
			sf::Style::Close | sf::Style::Titlebar
		);
		this->m_render_window.setPosition(sf::Vector2i(200, 200));

		// init
		this->m_screen = std::make_unique<ScreenBattle>();
		if (this->m_screen->init(&this->m_render_window) == false) {
			misc::log("Game::init()", "error init screen");
			return;
		}

		// loop while the window is open
		while (this->m_render_window.isOpen()){
			// game loop
			this->processEvents();
			this->update();
			this->draw();
		}
	}

	void Game::processEvents() {
		sf::Event event;
		while (this->m_render_window.pollEvent(event)) {
			// handle the event
			switch (event.type) {
				case sf::Event::Closed:{
					this->m_render_window.close();
				} break;
				case sf::Event::KeyPressed: {
					if (event.key.code == sf::Keyboard::Escape) {
						this->m_render_window.close();
					}
				} break;
			}
		}
	}

	void Game::update() {
		static sf::Clock clock;
		int ms = clock.restart().asMilliseconds();
		this->m_screen->update(ms);
	}

	void Game::draw() {
		// clear
		this->m_render_window.clear();

		// draw stuff
		this->m_screen->draw(&(this->m_render_window));

		// present
		this->m_render_window.display();
	}
}