#include "AssetManager.hpp"
#include "misc.hpp"
#include "Tileset.hpp"

namespace arena {
	AssetManager::AssetManager() {

	}

	AssetManager::~AssetManager() {

	}

	bool AssetManager::init() {
		return true;
	}

	bool AssetManager::loadTexture(std::string filename) {
		// see if already loaded
		auto find = this->m_textures.find(filename);
		if (find == this->m_textures.end()) {
			// not loaded; load it
			auto ptr = std::make_unique<sf::Texture>();
			if (ptr->loadFromFile(filename) == false) {
				// failed
				misc::log("AssetManager::load_texture", "error loading texture %s", filename.c_str());
				return false;
			}

			// add it to the map
			this->m_textures[filename] = std::move(ptr);
		}

		// success
		return true;
	}

	sf::Texture* AssetManager::getTexture(std::string filename) {
		// find it
		auto find = this->m_textures.find(filename);

		// check it
		if (find != this->m_textures.end()) {
			// found it
			return find->second.get();
		}

		// not loaded
		misc::log("AssetManager::get_texture", "error getting texture %s", filename.c_str());
		return nullptr;
	}

	bool AssetManager::loadTileset(std::string filename, int tilesize) {
		// see if already loaded
		auto find = this->m_tilesets.find(filename);
		if (find == this->m_tilesets.end()) {
			// not loaded; load it
			auto ptr = std::make_unique<Tileset>();
			if (ptr->init(this, filename, tilesize) == false) {
				// failed
				return false;
			}

			// add it to the map
			this->m_tilesets[filename] = std::move(ptr);
		}

		// success
		return true;
	}

	Tileset* AssetManager::getTileset(std::string filename) {
		// find it
		auto find = this->m_tilesets.find(filename);

		// check it
		if (find != this->m_tilesets.end()) {
			// found it
			return find->second.get();
		}

		// not loaded
		misc::log("AssetManager::get_tileset", "error getting tileset %s", filename.c_str());
		return nullptr;
	}
}
