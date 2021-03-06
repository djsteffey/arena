#include "ActorManager.hpp"
#include "Tileset.hpp"
#include "Actor.hpp"
#include "AssetManager.hpp"
#include "misc.hpp"
#include "Action.hpp"

namespace arena {
	ActorManager::ActorManager() {
		this->m_asset_manager = nullptr;
		this->m_tileset = nullptr;
		this->m_turn_action = nullptr;
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
		this->m_turn_action = nullptr;

		// done
		return true;
	}

	void ActorManager::update(int ms, Tilemap* tilemap) {
		// update the actors
		for (auto& actor : this->m_actors) {
			actor->update(ms, tilemap, this);
		}

		// check turn
		if (this->m_turn_action == nullptr) {
			Actor* actor = this->getActorWithTurn();
			this->m_turn_action = actor->ai(this->m_asset_manager, tilemap, this);
		}
		if (this->m_turn_action != nullptr) {
			if (this->m_turn_action->update(ms)) {
				this->m_turn_action = nullptr;
			}
		}
	}

	void ActorManager::draw(sf::RenderTarget* rt) {
		for (auto& actor : this->m_actors) {
			actor->draw(rt);
		}
		if (this->m_turn_action != nullptr) {
			this->m_turn_action->draw(rt);
		}
	}

	void ActorManager::addActor(std::unique_ptr<Actor> actor) {
		this->m_actors.push_back(std::move(actor));
	}

	Tileset* ActorManager::getTileset() {
		return this->m_tileset;
	}

	Actor* ActorManager::getActorWithTurn() {
		// find highest >= 1000 initiative
		while (true) {
			Actor* turn = nullptr;
			for (auto& actor : this->m_actors) {
				if (actor->getIsAlive() && actor->getStats()->initiative >= 1000) {
					if (turn == nullptr || actor->getStats()->initiative > turn->getStats()->initiative) {
						turn = actor.get();
					}
				}
			}

			// check if we got one
			if (turn == nullptr) {
				// no one so update initiative
				for (auto& actor : this->m_actors) {
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
		for (auto& actor : this->m_actors) {
			if (actor->getTilePositionX() == tile_x && actor->getTilePositionY() == tile_y) {
				return actor.get();
			}
		}
		return nullptr;
	}

	int ActorManager::getActorCount() {
		return this->m_actors.size();
	}

	Actor* ActorManager::getActorAtIndex(int index) {
		return this->m_actors[index].get();
	}
}
