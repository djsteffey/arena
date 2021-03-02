#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace arena {
	class Tilemap;
	class ActorManager;
	class Action;
	class Tileset;

	struct ActorStats{
		int speed;
		int initiative;
	};

	class Actor {
	public:
		Actor();
		~Actor();

		bool init(int graphics_id, int size, Tileset* tileset);
		bool init(unsigned long id, int graphics_id, int size, Tileset* tileset);
		void update(int ms, Tilemap* tilemap, ActorManager* am);
		void draw(sf::RenderTarget* rt);
		unsigned long getId();
		void setTilePosition(int tile_x, int tile_y, bool set_sprite_position);
		void setSpritePosition(float x, float y);
		int getTilePositionX();
		int getTilePositionY();
		float getWorldPositionX();
		float getWorldPositionY();
		sf::Vector2f getWorldPosition();
		void ai(Tilemap* tilemap, ActorManager* am);
		ActorStats* getStats();
		bool getIsPerformingTurn();

	protected:

	private:
		unsigned long m_id;
		int m_graphics_id;
		int m_size;
		int m_tile_x;
		int m_tile_y;
		sf::Sprite m_sprite;
		ActorStats m_stats;
		std::vector<std::unique_ptr<Action>> m_actions;
		std::vector<std::unique_ptr<Action>> m_turn_actions;
	};
}
