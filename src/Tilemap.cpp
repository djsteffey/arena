#include "Tilemap.hpp"
#include "AssetManager.hpp"
#include "misc.hpp"
#include "Tileset.hpp"

namespace arena {
	TilemapTile::TilemapTile(int graphics_id) {
		this->m_graphics_id = graphics_id;
	}

	TilemapTile::~TilemapTile() {

	}

	int TilemapTile::getGraphicsId() {
		return this->m_graphics_id;
	}

	//////////////////////////////////////////////////////////////////////////////

	Tilemap::Tilemap() {
		this->m_width = -1;
		this->m_height = -1;
		this->m_tilesize = -1;
		this->m_tileset = nullptr;
		this->m_dirty = false;
	}

	Tilemap::~Tilemap() {

	}

	bool Tilemap::init(AssetManager* am, int width, int height, int tilesize) {
		// save
		this->m_width = width;
		this->m_height = height;
		this->m_tilesize = tilesize;

		// load tileset
		this->m_tileset = am->getTileset("assets/tiles_24x24.png");
		if (this->m_tileset == nullptr) {
			misc::log("Tilemap::init()", "error getting tileset %s", "assets/tiles_24x24.png");
			return false;
		}

		// create the tiles
		this->m_tiles.resize(this->m_width);
		for (int x = 0; x < this->m_width; ++x) {
			for (int y = 0; y < this->m_height; ++y) {
				if (x == 0 || y == 0 || x == this->m_width - 1 || y == this->m_height - 1) {
					this->m_tiles[x].push_back(TilemapTile(0));
				}
				else {
					if (std::rand() % 1000 < 150) {
						this->m_tiles[x].push_back(TilemapTile(0));
					}
					else {
						this->m_tiles[x].push_back(TilemapTile(3));
					}
				}
			}
		}

		// mark as dirty and update the vertex array
		this->m_dirty = true;
		this->updateVertexArray();

		// done
		return true;
	}

	void Tilemap::update(int ms) {

	}

	void Tilemap::draw(sf::RenderTarget* rt) {
		// check if dirty
		if (this->m_dirty) {
			// update the vertex array
			this->updateVertexArray();
		}

		// draw the vertex array
		rt->draw(this->m_vertex_array, this->m_tileset->getTexture());
	}

	void Tilemap::updateVertexArray() {
		// check to make sure dirty
		if (this->m_dirty == false) {
			// no need
			return;
		}

		// clear dirty
		this->m_dirty = false;

		// set vertex array properties
		this->m_vertex_array.setPrimitiveType(sf::PrimitiveType::Quads);

		// make sure the vertex array is sized appropriately
		size_t size = 4;
		size *= this->m_width * this->m_height;;
		this->m_vertex_array.resize(size);
		
		// fill it up
		for (int y = 0; y < this->m_height; ++y) {
			for (int x = 0; x < this->m_width; ++x) {
				// rect of the texture in the tileset
				const sf::IntRect* rect = this->m_tileset->getRect(this->m_tiles[x][y].getGraphicsId());

				// first vert index
				size_t vert_index = (x + y * this->m_width) * 4;

				// top left
				sf::Vertex* v = &(this->m_vertex_array[vert_index + 0]);
				v->position.x = (float)(x * this->m_tilesize);
				v->position.y = (float)(y * this->m_tilesize);
				v->color = sf::Color::White;
				v->texCoords.x = (float)rect->left;
				v->texCoords.y = (float)rect->top;

				// top right
				v = &(this->m_vertex_array[vert_index + 1]);
				v->position.x = (float)((x + 1) * this->m_tilesize);
				v->position.y = (float)(y * this->m_tilesize);
				v->color = sf::Color::White;
				v->texCoords.x = (float)(rect->left + rect->width);
				v->texCoords.y = (float)rect->top;

				// bottom right
				v = &(this->m_vertex_array[vert_index + 2]);
				v->position.x = (float)((x + 1) * this->m_tilesize);
				v->position.y = (float)((y + 1) * this->m_tilesize);
				v->color = sf::Color::White;
				v->texCoords.x = (float)(rect->left + rect->width);
				v->texCoords.y = (float)(rect->top + rect->height);

				// bottom left
				v = &(this->m_vertex_array[vert_index + 3]);
				v->position.x = (float)(x * this->m_tilesize);
				v->position.y = (float)((y + 1) * this->m_tilesize);
				v->color = sf::Color::White;
				v->texCoords.x = (float)rect->left;
				v->texCoords.y = (float)(rect->top + rect->height);
			}
		}
	}

	bool Tilemap::isValidTilePosition(int tile_x, int tile_y) {
		if (tile_x < 0 || tile_y < 0 || tile_x > this->m_width - 1 || tile_y > this->m_height - 1) {
			return false;
		}
		return true;
	}

	bool Tilemap::isValidTilePosition(sf::Vector2i tile) {
		return this->isValidTilePosition(tile.x, tile.y);
	}

	bool Tilemap::isTilePositionBlocked(int tile_x, int tile_y) {
		if (this->isValidTilePosition(tile_x, tile_y) == false) {
			return true;
		}
		if (this->m_tiles[tile_x][tile_y].getGraphicsId() == 0) {
			return true;
		}
		return false;
	}

	bool Tilemap::isTilePositionBlocked(sf::Vector2i tile) {
		return this->isTilePositionBlocked(tile.x, tile.y);
	}

	int Tilemap::getWidthInTiles() {
		return this->m_width;
	}

	int Tilemap::getHeightInTiles() {
		return this->m_height;
	}

	int Tilemap::getWidthInWorld() {
		return this->m_width * this->m_tilesize;
	}

	int Tilemap::getHeightInWorld() {
		return this->m_height * this->m_tilesize;
	}
}
