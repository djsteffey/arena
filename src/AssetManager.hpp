#pragma once

#include <string>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

namespace arena {
	class Tileset;

	class AssetManager {
	public:
		AssetManager();
		~AssetManager();

		bool init();
		bool loadTexture(std::string filename);
		sf::Texture* getTexture(std::string filename);
		bool loadTileset(std::string filename, int tilesize);
		Tileset* getTileset(std::string filename);

	protected:

	private:
		std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
		std::map<std::string, std::unique_ptr<Tileset>> m_tilesets;
	};
}
