#include "Actor.hpp"
#include "misc.hpp"
#include "Tileset.hpp"
#include "Action.hpp"
#include "Tilemap.hpp"
#include "ActorManager.hpp"

namespace arena {
	Actor::Actor() {
		this->m_id = -1;
		this->m_graphics_id = -1;
		this->m_size = -1;
		this->m_tile_x = -1;
		this->m_tile_y = -1;
		std::memset(&this->m_stats, sizeof(this->m_stats), 0);
	}

	Actor::~Actor() {

	}

	bool Actor::init(int graphics_id, int size, Tileset* tileset) {
		return this->init(misc::generateId(), graphics_id, size, tileset);
	}

	bool Actor::init(unsigned long id, int graphics_id, int size, Tileset* tileset) {
		// actor id and graphics and size
		this->m_id = id;
		this->m_graphics_id = graphics_id;
		this->m_size = size;

		// initial tile position is not on map
		this->m_tile_x = -1;
		this->m_tile_y = -1;

		// setup the sprite
		this->m_sprite.setPosition((float)(this->m_tile_x * this->m_size), (float)(this->m_tile_x * this->m_size));
		this->m_sprite.setTexture(*(tileset->getTexture()));
		this->m_sprite.setTextureRect(*(tileset->getRect(this->m_graphics_id)));
		this->m_sprite.setScale(
			(float)size / tileset->getTilesize(),
			(float)size / tileset->getTilesize()
		);

		// setup the stats
		this->m_stats.speed = rand() % 2 + 2;
		this->m_stats.initiative = 0;

		// done
		return true;
	}

	void Actor::update(int ms, Tilemap* tilemap, ActorManager* am) {
		// update actions
		for (auto iter = this->m_actions.begin(); iter != this->m_actions.end(); ) {
			if ((*iter)->update(ms)) {
				// action done
				iter = this->m_actions.erase(iter);
			}
			else {
				++iter;
			}
		}

		// update turn actions
		for (auto iter = this->m_turn_actions.begin(); iter != this->m_turn_actions.end(); ) {
			if ((*iter)->update(ms)) {
				// action done
				iter = this->m_turn_actions.erase(iter);
			}
			else {
				++iter;
			}
		}
	}

	void Actor::draw(sf::RenderTarget* rt) {
		rt->draw(this->m_sprite);
	}

	unsigned long Actor::getId() {
		return this->m_id;
	}

	void Actor::setTilePosition(int tile_x, int tile_y, bool set_sprite_position) {
		this->m_tile_x = tile_x;
		this->m_tile_y = tile_y;
		if (set_sprite_position) {
			this->m_sprite.setPosition(static_cast<float>(this->m_tile_x * this->m_size), static_cast<float>(this->m_tile_y * this->m_size));
		}
	}

	void Actor::setSpritePosition(float x, float y) {
		this->m_sprite.setPosition(x, y);
	}
	
	int Actor::getTilePositionX() {
		return this->m_tile_x;
	}

	int Actor::getTilePositionY() {
		return this->m_tile_y;
	}

	float Actor::getWorldPositionX() {
		return this->m_sprite.getPosition().x;
	}

	float Actor::getWorldPositionY() {
		return this->m_sprite.getPosition().y;
	}

	sf::Vector2f Actor::getWorldPosition() {
		return this->m_sprite.getPosition();
	}

	void Actor::ai(Tilemap* tilemap, ActorManager* am) {
		// see if another actor around us
		for (misc::EDirection dir : misc::DIRECTIONS) {
			sf::Vector2i dir_v = misc::directionToVector2i(dir);
			Actor* other = am->getActorAtTilePosition(this->m_tile_x + dir_v.x, this->m_tile_y + dir_v.y);
			if (other != nullptr) {
				// attack
				this->m_turn_actions.push_back(
					std::make_unique<ActionMeleeAttack>(
						this,
						other,
						200,
						this->m_size
					)
				);

				// did an action
				this->m_stats.initiative -= 1000;

				// done
				return;
			}
		}

		// just move
		misc::EDirection dir = misc::randomDirection();
		sf::Vector2i vec = misc::directionToVector2i(dir);
		int new_x = this->m_tile_x + vec.x;
		int new_y = this->m_tile_y + vec.y;
		if (tilemap->isTilePositionBlocked(new_x, new_y) == false) {
			this->setTilePosition(new_x, new_y, false);
			this->m_turn_actions.push_back(
				std::make_unique<ActionMoveTo>(
					this,
					150,
					this->m_sprite.getPosition(),
					sf::Vector2f(static_cast<float>(this->m_tile_x * this->m_size), static_cast<float>(this->m_tile_y * this->m_size))
				)
			);

			// did an action
			this->m_stats.initiative -= 1000;

			// done
			return;
		}

		// didnt do an action
		this->m_stats.initiative -= 500;
	}

	ActorStats* Actor::getStats() {
		return &this->m_stats;
	}

	bool Actor::getIsPerformingTurn() {
		return this->m_turn_actions.size() != 0;
	}
}
