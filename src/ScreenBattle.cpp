#include "ScreenBattle.hpp"
#include "AssetManager.hpp"
#include "Tilemap.hpp"
#include "ActorManager.hpp"
#include "misc.hpp"
#include "Actor.hpp"
#include "UiManager.hpp"
#include "UiWidget.hpp"

namespace arena {
	ScreenBattle::ScreenBattle() {
		this->m_asset_manager = nullptr;
		this->m_tilemap = nullptr;
		this->m_actor_manager = nullptr;
		this->m_ui_manager = nullptr;
	}

	ScreenBattle::~ScreenBattle() {

	}

	bool ScreenBattle::init(sf::RenderWindow* rw) {
		// assets
		this->m_asset_manager = std::make_unique<AssetManager>();
		if (this->m_asset_manager->init() == false) {
			misc::log("ScreenBattle::init()", "error init AssetManager");
			return false;
		}
		if (this->m_asset_manager->loadTexture("assets/actors_24x24.png") == false) {
			misc::log("ScreenBattle::init()", "error loading texture %s", "assets/actors_24x24.png");
			return false;
		}
		if (this->m_asset_manager->loadTexture("assets/tiles_24x24.png") == false) {
			misc::log("ScreenBattle::init()", "error loading texture %s", "assets/tiles_24x24.png");
			return false;
		}
		if (this->m_asset_manager->loadTileset("assets/tiles_24x24.png", 24) == false) {
			misc::log("ScreenBattle::init()", "error loading tileset %s", "assets/tiles_24x24.png");
			return false;
		}
		if (this->m_asset_manager->loadTileset("assets/actors_24x24.png", 24) == false) {
			misc::log("ScreenBattle::init()", "error loading tileset %s", "assets/actors_24x24.png");
			return false;
		}

		// tilemap
		this->m_tilemap = std::make_unique<Tilemap>();
		if (this->m_tilemap->init(this->m_asset_manager.get(), 10, 13, 48) == false) {
			misc::log("ScreenBattle::init()", "error init tilemap");
			return false;
		}

		// actors
		this->m_actor_manager = std::make_unique<ActorManager>();
		if (this->m_actor_manager->init(this->m_asset_manager.get()) == false) {
			misc::log("ScreenBattle::init()", "error init actor manager");
			return false;
		}
		for (int i = 0; i < 2; ++i) {
			std::unique_ptr<Actor> actor = std::make_unique<Actor>();
			if (actor->init(this->m_asset_manager.get(), 44, 48, this->m_actor_manager->getTileset()) == false) {
				misc::log("ScreenBattle::init()", "error creating Actor");
				return false;
			}
			actor->setTilePosition(
				misc::getRandomIntInRange(1, this->m_tilemap->getWidthInTiles() - 2),
				misc::getRandomIntInRange(1, this->m_tilemap->getHeightInTiles() - 2),
				true
			);
			this->m_actor_manager->addActor(std::move(actor));
		}

		// ui
		this->m_ui_manager = std::make_unique<UiManager>();
		for (int i = 0; i < 3; ++i) {
			if (i < this->m_actor_manager->getActorCount()) {
				std::unique_ptr<UiWidget> widget = std::make_unique<UiBattleActorStatus>(
					this->m_asset_manager.get(),
					0.0f,
					0.0f,
					this->m_actor_manager->getActorAtIndex(i)
				);
				widget->setPosition(4.0f, 4.0f + (widget->getHeight() + 4.0f) * i);
				this->m_ui_manager->add(std::move(widget));
			}
		}
		for (int i = 0; i < 3; ++i) {
			if (i + 3 < this->m_actor_manager->getActorCount()) {
				std::unique_ptr<UiWidget> widget = std::make_unique<UiBattleActorStatus>(
					this->m_asset_manager.get(),
					0.0f,
					0.0f,
					this->m_actor_manager->getActorAtIndex(i + 3)
				);
				widget->setPosition(rw->getSize().x - 4.0f - widget->getWidth(), 4.0f + (widget->getHeight() + 4.0f) * i);
				this->m_ui_manager->add(std::move(widget));
			}
		}

		// view
		this->m_view = rw->getDefaultView();
		this->m_view.setCenter(this->m_tilemap->getWidthInWorld() * 0.50f, this->m_tilemap->getHeightInWorld() * 0.50f);
		this->m_ui_view = rw->getDefaultView();


		// done
		return true;
	}

	void ScreenBattle::update(int ms){
		this->m_tilemap->update(ms);
		this->m_actor_manager->update(ms, this->m_tilemap.get());
		this->m_ui_manager->update(ms);
	}

	void ScreenBattle::draw(sf::RenderWindow* rw) {
		rw->setView(this->m_view);
		this->m_tilemap->draw(rw);
		this->m_actor_manager->draw(rw);

		rw->setView(this->m_ui_view);
		this->m_ui_manager->draw(rw);
	}
}
