#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace arena {
	class AssetManager;

	class Tileset {
	public:
		Tileset();
		~Tileset();

		bool init(AssetManager* am, std::string filename, int tilesize);
		sf::Texture* getTexture();
		sf::IntRect* getRect(int index);
		int getTilesize();

	protected:

	private:
		int m_tilesize;
		sf::Texture* m_texture;
		std::vector<sf::IntRect> m_rects;
	};
}
