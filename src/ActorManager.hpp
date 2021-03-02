#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

namespace arena {
	class AssetManager;
	class Tilemap;
	class Actor;
	class Tileset;

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

	protected:

	private:
		Actor* getActorWithTurn();

		AssetManager* m_asset_manager;
		Tileset* m_tileset;
		std::map<unsigned long, std::unique_ptr<Actor>> m_actors;
		Actor* m_actor_with_turn;
	};
}
