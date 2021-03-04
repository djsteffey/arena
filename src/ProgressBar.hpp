#pragma once

#include <SFML/Graphics.hpp>

namespace arena {
	class AssetManager;

	class ProgressBar {
	public:
		ProgressBar(AssetManager* am, float width, float height, int current_value, int max_value);
		virtual ~ProgressBar();

		void update(int ms);
		void draw(sf::RenderTarget* rt);
		void setPosition(float x, float y);
		void setMaxValue(int max_value);
		void setCurrentValue(int current_value);
		float getWidth();
		float getHeight();

	protected:

	private:
		void updateShapes();

		int m_current_value;
		int m_max_value;
		bool m_dirty;
		sf::RectangleShape m_outer;
		sf::RectangleShape m_inner_filled;
		sf::RectangleShape m_inner_empty;
	};
}
