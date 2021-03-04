#include "ProgressBar.hpp"

namespace arena {
	ProgressBar::ProgressBar(AssetManager* am, float width, float height, int current_value, int max_value) {
		// value
		this->m_current_value = current_value;
		this->m_max_value = max_value;

		// shapes
		this->m_outer.setSize(sf::Vector2f(width, height));
		this->m_outer.setPosition(0.0f, 0.0f);
		this->m_outer.setOutlineThickness(-2.0f);
		this->m_outer.setOutlineColor(sf::Color::White);
		this->m_dirty = true;
	}

	ProgressBar::~ProgressBar() {

	}

	void ProgressBar::update(int ms) {

	}

	void ProgressBar::draw(sf::RenderTarget* rt) {
		if (this->m_dirty) {
			this->updateShapes();
		}
		rt->draw(this->m_outer);
		rt->draw(this->m_inner_filled);
		rt->draw(this->m_inner_empty);
	}

	void ProgressBar::setPosition(float x, float y) {
		this->m_outer.setPosition(x, y);
		this->m_dirty = true;
	}

	void ProgressBar::setMaxValue(int max_value) {
		this->m_max_value = max_value;
		this->m_dirty = true;
	}

	void ProgressBar::setCurrentValue(int current_value) {
		this->m_current_value = current_value;
		this->m_dirty = true;
	}

	float ProgressBar::getWidth() {
		return this->m_outer.getSize().x;
	}

	float ProgressBar::getHeight() {
		return this->m_outer.getSize().y;
	}

	void ProgressBar::updateShapes() {
		// clear dirty
		this->m_dirty = false;

		// total dimensions for inner
		float inner_width = this->m_outer.getSize().x - (-this->m_outer.getOutlineThickness() * 2);
		float inner_height = this->m_outer.getSize().y - (-this->m_outer.getOutlineThickness() * 2);

		// percent
		float percent = static_cast<float>(this->m_current_value) / this->m_max_value;

		// individual widths
		float filled_width = inner_width * percent;
		float empty_width = inner_width - filled_width;

		// setup the shapes
		this->m_inner_filled.setPosition(
			this->m_outer.getPosition().x + -this->m_outer.getOutlineThickness(),
			this->m_outer.getPosition().y + -this->m_outer.getOutlineThickness()
		);
		this->m_inner_filled.setSize(sf::Vector2f(filled_width, inner_height));
		this->m_inner_filled.setOutlineThickness(0.0f);
		this->m_inner_filled.setFillColor(sf::Color::Green);

		this->m_inner_empty.setPosition(this->m_inner_filled.getPosition().x + this->m_inner_filled.getSize().x, this->m_inner_filled.getPosition().y);
		this->m_inner_empty.setSize(sf::Vector2f(empty_width, inner_height));
		this->m_inner_empty.setOutlineThickness(0.0f);
		this->m_inner_empty.setFillColor(sf::Color::Red);
	}
}