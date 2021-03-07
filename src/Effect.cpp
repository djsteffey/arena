#include "Effect.hpp"

namespace arena {
	Effect::Effect() {

	}

	Effect::~Effect() {

	}

	//////////////////////////////////////////////////////////////////////////////

	EffectProjectile::EffectProjectile(float x, float y, float destination_x, float destination_y, int duration) : Effect() {
		this->m_shape.setFillColor(sf::Color::Red);
		this->m_shape.setRadius(6.0f);
		this->m_shape.setOrigin(this->m_shape.getRadius(), this->m_shape.getRadius());
		this->m_shape.setPosition(x, y);

		this->m_start_x = x;
		this->m_start_y = y;
		this->m_destination_x = destination_x;
		this->m_destination_y = destination_y;
		this->m_duration = duration;
		this->m_duration_current = 0;

	}

	EffectProjectile::~EffectProjectile() {

	}

	bool EffectProjectile::update(int ms) {
		// update duration
		this->m_duration_current += ms;
		if (this->m_duration_current > this->m_duration) {
			this->m_duration_current = this->m_duration;
		}

		// calculate position
		float percent = static_cast<float>(this->m_duration_current) / this->m_duration;
		float x = (this->m_destination_x - this->m_start_x) * percent + this->m_start_x;
		float y = (this->m_destination_y - this->m_start_y) * percent + this->m_start_y;
		this->m_shape.setPosition(x, y);

		// check if done
		if (this->m_duration_current >= this->m_duration) {
			return true;
		}
		return false;
	}

	void EffectProjectile::draw(sf::RenderTarget* rt) {
		rt->draw(this->m_shape);
	}
}
