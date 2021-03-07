#pragma once

#include <SFML/Graphics.hpp>

namespace arena {
	class Effect {
	public:
		Effect();
		virtual ~Effect();
		virtual bool update(int ms) = 0;
		virtual void draw(sf::RenderTarget* rt) = 0;

	protected:

	private:
	};

	class EffectProjectile : public Effect {
	public:
		EffectProjectile(float x, float y, float destination_x, float destination_y, int duration);
		~EffectProjectile();
		bool update(int ms) override;
		void draw(sf::RenderTarget* rt) override;

	protected:

	private:
		sf::CircleShape m_shape;
		float m_start_x;
		float m_start_y;
		float m_destination_x;
		float m_destination_y;
		int m_duration;
		int m_duration_current;
	};
}
