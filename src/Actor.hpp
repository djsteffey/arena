#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <functional>
#include "misc.hpp"

namespace arena {
	class Tilemap;
	class ActorManager;
	class Action;
	class Tileset;
	class Ability;
	class ProgressBar;
	class AssetManager;

	struct ActorStats{
		int speed;
		int initiative;
		int hp_max;
		int hp_current;
	};

	class Actor {
	public:
		Actor();
		~Actor();

		bool init(AssetManager* am, int graphics_id, int size, Tileset* tileset);
		bool init(unsigned long id, AssetManager* am, int graphics_id, int size, Tileset* tileset);
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
		ActorStats* getStats();
		bool getIsAlive();
		std::unique_ptr<Action> ai(Tilemap* tilemap, ActorManager* am);
		void onDamage(int amount);
		void onHeal(int amount);
		void faceDirection(misc::EDirection dir);

	protected:

	private:
		void onDeath();

		unsigned long m_id;
		int m_graphics_id;
		int m_size;
		int m_tile_x;
		int m_tile_y;
		Tileset* m_tileset;
		sf::Sprite m_sprite;
		ActorStats m_stats;
		std::vector<std::unique_ptr<Action>> m_actions;
		std::vector<std::unique_ptr<Ability>> m_abilities;
		std::unique_ptr<ProgressBar> m_hp_bar;

	};
}
