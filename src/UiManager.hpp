#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <list>

namespace arena {
	class UiWidget;

	class UiManager {
	public:
		UiManager();
		~UiManager();

		void update(int ms);
		void draw(sf::RenderTarget* rt);
		void add(std::unique_ptr<UiWidget> widget);

	protected:

	private:
		std::list<std::unique_ptr<UiWidget>> m_widgets;
	};
}
