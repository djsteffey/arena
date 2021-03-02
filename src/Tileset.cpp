#include "Tileset.hpp"
#include "AssetManager.hpp"
#include "misc.hpp"

namespace arena {
	Tileset::Tileset() {
		this->m_tilesize = -1;
		this->m_texture = nullptr;
	}

	Tileset::~Tileset() {

	}

	bool Tileset::init(AssetManager* am, std::string filename, int tilesize) {
		// size
		this->m_tilesize = tilesize;

		// load the texture
		this->m_texture = am->getTexture(filename);
		if (this->m_texture == nullptr) {
			misc::log("Tileset::Tileset()", "error getting texture %s", filename.c_str());
			return false;
		}

		// create the rects
		for (unsigned int y = 0; y < this->m_texture->getSize().y; y += this->m_tilesize) {
			for (unsigned int x = 0; x < this->m_texture->getSize().x; x += this->m_tilesize) {
				this->m_rects.push_back(sf::IntRect(x, y, this->m_tilesize, this->m_tilesize));
			}
		}

		// done
		return true;
	}

	sf::Texture* Tileset::getTexture() {
		return this->m_texture;
	}

	sf::IntRect* Tileset::getRect(int index) {
		return &(this->m_rects[index]);
	}

	int Tileset::getTilesize() {
		return this->m_tilesize;
	}
}
