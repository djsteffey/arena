#include "Actor.hpp"
#include "misc.hpp"
#include "Tileset.hpp"
#include "Action.hpp"
#include "Tilemap.hpp"
#include "ActorManager.hpp"
#include "Ability.hpp"
#include "ProgressBar.hpp"

namespace arena {
	Actor::Actor() {
		this->m_id = -1;
		this->m_graphics_id = -1;
		this->m_size = -1;
		this->m_tile_x = -1;
		this->m_tile_y = -1;
		std::memset(&this->m_stats, sizeof(this->m_stats), 0);
		this->m_hp_bar = nullptr;
	}

	Actor::~Actor() {

	}

	bool Actor::init(AssetManager* am, int graphics_id, int size, Tileset* tileset) {
		return this->init(misc::generateId(), am, graphics_id, size, tileset);
	}

	bool Actor::init(unsigned long id, AssetManager* am, int graphics_id, int size, Tileset* tileset) {
		// actor id and graphics and size
		this->m_id = id;
		this->m_graphics_id = graphics_id;
		this->m_size = size;
		this->m_tileset = tileset;

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
		this->m_stats.hp_max = misc::getRandomIntInRange(10, 20);
		this->m_stats.hp_current = misc::getRandomIntInRange(5, 10);

		// abilities
		this->m_abilities.push_back(std::make_unique<AbilityMeleeAttack>(this));

		// bars
		this->m_hp_bar = std::make_unique<ProgressBar>(am, size * 0.75f, 12.0f, this->m_stats.hp_current, this->m_stats.hp_max);
		this->m_hp_bar->setPosition(
			this->getWorldPositionX() + (this->m_size - this->m_hp_bar->getWidth()) / 2,
			this->getWorldPositionY()
		);

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
	}

	void Actor::draw(sf::RenderTarget* rt) {
		if (this->getIsAlive()) {
			this->m_hp_bar->draw(rt);
		}
		rt->draw(this->m_sprite);
	}

	unsigned long Actor::getId() {
		return this->m_id;
	}

	void Actor::setTilePosition(int tile_x, int tile_y, bool set_sprite_position) {
		this->m_tile_x = tile_x;
		this->m_tile_y = tile_y;
		if (set_sprite_position) {
			this->setSpritePosition(static_cast<float>(this->m_tile_x * this->m_size), static_cast<float>(this->m_tile_y * this->m_size));
		}
	}

	void Actor::setSpritePosition(float x, float y) {
		this->m_sprite.setPosition(x, y);
		this->m_hp_bar->setPosition(
			this->getWorldPositionX() + (this->m_size - this->m_hp_bar->getWidth()) / 2,
			this->getWorldPositionY()
		);
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

	std::unique_ptr<Action> Actor::ai(Tilemap* tilemap, ActorManager* am) {
		// gather all possible abilities
		std::vector<AbilityTargetSet> ats;

		// check all abilities
		for (auto& ability : this->m_abilities) {
			ability->findTargets(ats, am, tilemap);
		}

		// if we got abilities then choose one of them
		if (ats.empty() == false) {
			this->m_stats.initiative -= 1000;
			AbilityTargetSet a = ats[misc::getRandomIntInRange(0, ats.size() - 1)];
			return a.ability->generateAction(a.target);
		}

		// got here so lets just move
		misc::EDirection dir = misc::randomDirection();
		sf::Vector2i vec = misc::directionToVector2i(dir);
		int new_x = this->m_tile_x + vec.x;
		int new_y = this->m_tile_y + vec.y;
		if (tilemap->isTilePositionBlocked(new_x, new_y) == false) {
			// doing an action
			this->m_stats.initiative -= 1000;
			this->setTilePosition(new_x, new_y, false);
			return std::make_unique<ActionMoveTo>(
				this,
				150,
				this->getWorldPosition(),
				sf::Vector2f(static_cast<float>(this->m_tile_x * this->m_size), static_cast<float>(this->m_tile_y * this->m_size))
			);
		}

		// didnt do an action
		this->m_stats.initiative -= 500;
		return nullptr;
	}

	ActorStats* Actor::getStats() {
		return &this->m_stats;
	}

	bool Actor::getIsAlive() {
		return this->m_stats.hp_current > 0;
	}

	void Actor::onDamage(int amount) {
		this->m_stats.hp_current -= amount;
		if (this->m_stats.hp_current <= 0) {
			this->m_stats.hp_current = 0;
			this->onDeath();
		}
		this->m_hp_bar->setCurrentValue(this->m_stats.hp_current);
	}

	void Actor::onHeal(int amount) {
		this->m_stats.hp_current += amount;
		if (this->m_stats.hp_current > this->m_stats.hp_max) {
			this->m_stats.hp_current = this->m_stats.hp_max;
		}
		this->m_hp_bar->setCurrentValue(this->m_stats.hp_current);
	}

	void Actor::onDeath() {
		this->m_sprite.setTextureRect(*(this->m_tileset->getRect(20 * 26 + 18)));
	}

	void Actor::faceDirection(misc::EDirection dir) {
		switch (dir) {
			case misc::EDirection::UP: {
				// nothing
			} break;
			case misc::EDirection::DOWN: {
				// nothing
			} break;
			case misc::EDirection::LEFT: {
				this->m_sprite.setScale(
					static_cast<float>(this->m_size) / this->m_tileset->getTilesize(),
					static_cast<float>(this->m_size) / this->m_tileset->getTilesize()
				);
			} break;
			case misc::EDirection::RIGHT: {
				this->m_sprite.setScale(
					static_cast<float>(-this->m_size) / this->m_tileset->getTilesize(),
					static_cast<float>(this->m_size) / this->m_tileset->getTilesize()
				);
			} break;
		}
	}
}
