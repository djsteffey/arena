#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>

namespace arena {
	class Actor;
	class AssetManager;
	class Tileset;

	class UiWidget {
	public:
		UiWidget(float x, float y, float width, float height);
		virtual ~UiWidget();

		virtual void update(int ms);
		virtual void draw(sf::RenderTarget* rt);
		virtual void setPosition(float x, float y);
		virtual void move(float dx, float dy);
		virtual void setWidth(float width);
		virtual void setHeight(float height);
		float getWidth();
		float getHeight();
		float getPositionX();
		float getPositionY();
		void addChild(std::unique_ptr<UiWidget> child);

	protected:
		virtual void updateGraphics();

		bool m_dirty;
		sf::RectangleShape m_frame;

	private:
		std::list<std::unique_ptr<UiWidget>> m_children;
	};

	class UiImage : public UiWidget {
	public:
		UiImage(sf::Texture* texture, sf::IntRect* texture_rect, float x, float y, float width, float height);
		~UiImage();
		void draw(sf::RenderTarget* rt) override;
		void setTextureRect(sf::IntRect* rect);

	protected:

	private:
		sf::Sprite m_sprite;
	};

	class UiProgressBar : public UiWidget {
	public:
		UiProgressBar(float x, float y, float width, float height, int current_value, int max_value);
		virtual ~UiProgressBar();

		void draw(sf::RenderTarget* rt) override;
		void setMaxValue(int max_value);
		void setCurrentValue(int current_value);

	protected:

	private:
		void updateGraphics() override;

		int m_current_value;
		int m_max_value;
		sf::RectangleShape m_inner_filled;
		sf::RectangleShape m_inner_empty;
	};

	class UiBattleActorStatus : public UiWidget {
	public:
		UiBattleActorStatus(AssetManager* am, float x, float y, Actor* actor);
		~UiBattleActorStatus();

		void draw(sf::RenderTarget* rt) override;

	protected:

	private:
		static Tileset* s_tileset;
		Actor* m_actor;
		UiProgressBar* m_hp_bar;
		UiImage* m_image;
	};
}
