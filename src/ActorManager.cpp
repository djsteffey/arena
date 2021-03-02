#include "ActorManager.hpp"
#include "Tileset.hpp"
#include "Actor.hpp"
#include "AssetManager.hpp"
#include "misc.hpp"

namespace arena {
	ActorManager::ActorManager() {
		this->m_asset_manager = nullptr;
		this->m_tileset = nullptr;
		this->m_actor_with_turn = nullptr;
	}

	ActorManager::~ActorManager() {

	}

	bool ActorManager::init(AssetManager* am) {
		// save
		this->m_asset_manager = am;

		// get tileset
		this->m_tileset = am->getTileset("assets/actors_24x24.png");
		if (this->m_tileset == nullptr) {
			misc::log("ActorManager::init()", "error getting tileset %s", "assets/actors_24x24.png");
			return false;
		}

		// turn
		this->m_actor_with_turn = nullptr;

		// done
		return true;
	}

	void ActorManager::update(int ms, Tilemap* tilemap) {
		// update the actors
		for (auto& kvp : this->m_actors) {
			kvp.second->update(ms, tilemap, this);
		}

		// check actor with turn
		if (this->m_actor_with_turn == nullptr) {
			this->m_actor_with_turn = this->getActorWithTurn();
			this->m_actor_with_turn->ai(tilemap, this);
		}
		else {
			if (this->m_actor_with_turn->getIsPerformingTurn() == false) {
				this->m_actor_with_turn = nullptr;
			}
		}
	}

	void ActorManager::draw(sf::RenderTarget* rt) {
		for (auto& kvp : this->m_actors) {
			kvp.second->draw(rt);
		}
	}

	void ActorManager::addActor(std::unique_ptr<Actor> actor) {
		this->m_actors[actor->getId()] = std::move(actor);
	}

	Tileset* ActorManager::getTileset() {
		return this->m_tileset;
	}

	Actor* ActorManager::getActorWithTurn() {
		// find highest >= 1000 initiative
		while (true) {
			Actor* turn = nullptr;
			for (auto& kvp : this->m_actors) {
				Actor* actor = kvp.second.get();
				if (actor->getStats()->initiative >= 1000) {
					if (turn == nullptr || actor->getStats()->initiative > turn->getStats()->initiative) {
						turn = actor;
					}
				}
			}

			// check if we got one
			if (turn == nullptr) {
				// no one so update initiative
				for (auto& kvp : this->m_actors) {
					Actor* actor = kvp.second.get();
					actor->getStats()->initiative += actor->getStats()->speed;
				}
			}
			else {
				// found one
				return turn;
			}
		}
	}

	Actor* ActorManager::getActorAtTilePosition(int tile_x, int tile_y) {
		for (auto& kvp : this->m_actors) {
			Actor* actor = kvp.second.get();
			if (actor->getTilePositionX() == tile_x && actor->getTilePositionY() == tile_y) {
				return actor;
			}
		}
		return nullptr;
	}
}
