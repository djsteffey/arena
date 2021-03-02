#pragma once

#include <SFML/Graphics.hpp>

namespace arena {
	class AssetManager;
	class Tileset;

	class TilemapTile {
	public:
		TilemapTile(int graphics_id);
		~TilemapTile();
		int getGraphicsId();

	protected:

	private:
		int m_graphics_id;
	};

	class Tilemap {
	public:
		Tilemap();
		~Tilemap();

		bool init(AssetManager* am, int width, int height, int tilesize);
		void update(int ms);
		void draw(sf::RenderTarget* rt);
		bool isValidTilePosition(int tile_x, int tile_y);
		bool isValidTilePosition(sf::Vector2i tile);
		bool isTilePositionBlocked(int tile_x, int tile_y);
		bool isTilePositionBlocked(sf::Vector2i tile);
		int getWidthInTiles();
		int getHeightInTiles();
		int getWidthInWorld();
		int getHeightInWorld();

	protected:

	private:
		void updateVertexArray();

		int m_width;
		int m_height;
		int m_tilesize;
		Tileset* m_tileset;
		std::vector<std::vector<TilemapTile>> m_tiles;
		bool m_dirty;
		sf::VertexArray m_vertex_array;
	};
}
