#pragma once

#include "ScreenAbstract.hpp"
#include <memory>

namespace arena {
	class AssetManager;
	class Tilemap;
	class ActorManager;

	class ScreenBattle: public ScreenAbstract{
	public:
		ScreenBattle();
		~ScreenBattle();

		bool init(sf::RenderWindow* rw) override;
		void update(int ms) override;
		void draw(sf::RenderWindow* rw) override;

	protected:

	private:
		std::unique_ptr<AssetManager> m_asset_manager;
		std::unique_ptr<Tilemap> m_tilemap;
		std::unique_ptr<ActorManager> m_actor_manager;
		sf::View m_view;
	};
}
