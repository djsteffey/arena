#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace arena {
	class AssetManager;
	class Tilemap;
	class Actor;
	class Tileset;
	class Action;

	class ActorManager {
	public:
		ActorManager();
		~ActorManager();

		bool init(AssetManager* am);
		void update(int ms, Tilemap* tilemap);
		void draw(sf::RenderTarget* rt);
		void addActor(std::unique_ptr<Actor> actor);
		Tileset* getTileset();
		Actor* getActorAtTilePosition(int tile_x, int tile_y);
		int getActorCount();
		Actor* getActorAtIndex(int index);

	protected:

	private:
		Actor* getActorWithTurn();

		AssetManager* m_asset_manager;
		Tileset* m_tileset;
		std::vector<std::unique_ptr<Actor>> m_actors;
		std::unique_ptr<Action> m_turn_action;
	};
}
