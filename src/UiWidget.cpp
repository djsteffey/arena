#include "UiWidget.hpp"
#include "Actor.hpp"
#include "AssetManager.hpp"
#include "Tileset.hpp"

namespace arena {
	UiWidget::UiWidget(float x, float y, float width, float height) {
		// border and color
		this->m_frame.setOutlineColor(sf::Color::White);
		this->m_frame.setOutlineThickness(-2.0f);
		this->m_frame.setFillColor(sf::Color::Black);

		// position and size
		this->setPosition(x, y);
		this->setWidth(width);
		this->setHeight(height);
	}

	UiWidget::~UiWidget() {

	}

	void UiWidget::update(int ms) {
		for (auto& child : this->m_children) {
			child->update(ms);
		}
	}

	void UiWidget::draw(sf::RenderTarget* rt) {
		if (this->m_dirty) {
			this->updateGraphics();
		}
		rt->draw(this->m_frame);
		for (auto& child : this->m_children) {
			child->draw(rt);
		}
	}

	void UiWidget::setPosition(float x, float y) {
		// get the delta
		float dx = x - this->getPositionX();
		float dy = y - this->getPositionY();

		// move
		this->move(dx, dy);
	}

	void UiWidget::move(float dx, float dy) {
		// set the new position on the frame
		this->m_frame.setPosition(this->getPositionX() + dx, this->getPositionY() + dy);
		this->m_dirty = true;

		// move the children
		for (auto& child : this->m_children) {
			child->move(dx, dy);
		}
	}

	void UiWidget::setWidth(float width) {
		this->m_frame.setSize(sf::Vector2f(width, this->m_frame.getSize().y));
		this->m_dirty = true;
	}

	void UiWidget::setHeight(float height) {
		this->m_frame.setSize(sf::Vector2f(this->m_frame.getSize().x, height));
		this->m_dirty = true;
	}

	float UiWidget::getWidth() {
		return this->m_frame.getSize().x;
	}

	float UiWidget::getHeight() {
		return this->m_frame.getSize().y;
	}

	float UiWidget::getPositionX() {
		return this->m_frame.getPosition().x;
	}

	float UiWidget::getPositionY() {
		return this->m_frame.getPosition().y;
	}

	void UiWidget::updateGraphics() {
		// not dirty anymore
		this->m_dirty = false;
	}

	void UiWidget::addChild(std::unique_ptr<UiWidget> child) {
		child->move(this->getPositionX(), this->getPositionY());
		this->m_children.push_back(std::move(child));
	}

	//////////////////////////////////////////////////////////////////////////////

	UiImage::UiImage(sf::Texture* texture, sf::IntRect* texture_rect, float x, float y, float width, float height) : UiWidget(x, y, width, height) {
		this->m_sprite.setTexture(*texture);
		if (texture_rect != nullptr) {
			this->m_sprite.setTextureRect(*texture_rect);
			this->m_sprite.setScale(
				width / texture_rect->width,
				height / texture_rect->height
			);
		}
		else {
			this->m_sprite.setScale(
				width / texture->getSize().x,
				height / texture->getSize().y
			);
		}
	}

	UiImage::~UiImage() {

	}

	void UiImage::draw(sf::RenderTarget* rt) {
		this->m_sprite.setPosition(this->getPositionX(), this->getPositionY());
		rt->draw(this->m_sprite);
	}

	void UiImage::setTextureRect(sf::IntRect* rect) {
		if (rect != nullptr) {
			this->m_sprite.setTextureRect(*rect);
		}
		else {
			sf::IntRect rect = sf::IntRect(
				0.0f,
				0.0f,
				this->m_sprite.getTexture()->getSize().x,
				this->m_sprite.getTexture()->getSize().y
			);
			this->m_sprite.setTextureRect(rect);
		}
		this->m_sprite.setScale(
			this->getWidth() / rect->width,
			this->getHeight() / rect->height
		);
	}

	//////////////////////////////////////////////////////////////////////////////

	UiProgressBar::UiProgressBar(float x, float y, float width, float height, int current_value, int max_value) : UiWidget(x, y, width, height) {
		// value
		this->m_current_value = current_value;
		this->m_max_value = max_value;
		this->m_dirty = true;
	}

	UiProgressBar::~UiProgressBar() {

	}

	void UiProgressBar::draw(sf::RenderTarget* rt) {
		UiWidget::draw(rt);
		rt->draw(this->m_inner_filled);
		rt->draw(this->m_inner_empty);
	}

	void UiProgressBar::setMaxValue(int max_value) {
		this->m_max_value = max_value;
		this->m_dirty = true;
	}

	void UiProgressBar::setCurrentValue(int current_value) {
		this->m_current_value = current_value;
		this->m_dirty = true;
	}

	void UiProgressBar::updateGraphics() {
		UiWidget::updateGraphics();

		// total dimensions for inner
		float inner_width = this->m_frame.getSize().x - (-this->m_frame.getOutlineThickness() * 2);
		float inner_height = this->m_frame.getSize().y - (-this->m_frame.getOutlineThickness() * 2);

		// percent
		float percent = static_cast<float>(this->m_current_value) / this->m_max_value;

		// individual widths
		float filled_width = inner_width * percent;
		float empty_width = inner_width - filled_width;

		// setup the shapes
		this->m_inner_filled.setPosition(
			this->m_frame.getPosition().x + -this->m_frame.getOutlineThickness(),
			this->m_frame.getPosition().y + -this->m_frame.getOutlineThickness()
		);
		this->m_inner_filled.setSize(sf::Vector2f(filled_width, inner_height));
		this->m_inner_filled.setOutlineThickness(0.0f);
		this->m_inner_filled.setFillColor(sf::Color::Green);

		this->m_inner_empty.setPosition(this->m_inner_filled.getPosition().x + this->m_inner_filled.getSize().x, this->m_inner_filled.getPosition().y);
		this->m_inner_empty.setSize(sf::Vector2f(empty_width, inner_height));
		this->m_inner_empty.setOutlineThickness(0.0f);
		this->m_inner_empty.setFillColor(sf::Color::Red);
	}

	//////////////////////////////////////////////////////////////////////////////

	Tileset* UiBattleActorStatus::s_tileset = nullptr;

	UiBattleActorStatus::UiBattleActorStatus(AssetManager* am, float x, float y, Actor* actor) : UiWidget(x, y, 384.0f, 196.0f) {
		// handle to tileset
		if (UiBattleActorStatus::s_tileset == nullptr) {
			UiBattleActorStatus::s_tileset = am->getTileset("assets/actors_24x24.png");
		}

		// save actor
		this->m_actor = actor;

		// image
		std::unique_ptr<UiWidget> ptr = std::make_unique<UiImage>(
			UiBattleActorStatus::s_tileset->getTexture(),
			UiBattleActorStatus::s_tileset->getRect(this->m_actor->getGraphicsId()),
			4.0f,
			4.0f,
			96.0f,
			96.0f
		);
		this->m_image = static_cast<UiImage*>(ptr.get());
		this->addChild(std::move(ptr));

		// health bar
		ptr = std::make_unique<UiProgressBar>(
			4.0f + 96.0f + 4.0f,
			4.0f,
			this->getWidth() * 0.70f,
			32.0f,
			this->m_actor->getStats()->hp_current,
			this->m_actor->getStats()->hp_max
		);
		this->m_hp_bar = static_cast<UiProgressBar*>(ptr.get());
		this->addChild(std::move(ptr));
	}

	UiBattleActorStatus::~UiBattleActorStatus() {

	}

	void UiBattleActorStatus::draw(sf::RenderTarget* rt) {
		// update image
		if (this->m_actor->getIsAlive()) {
			this->m_image->setTextureRect(UiBattleActorStatus::s_tileset->getRect(this->m_actor->getGraphicsId()));
		}
		else {
			this->m_image->setTextureRect(UiBattleActorStatus::s_tileset->getRect(20 * 26 + 18));
		}

		// update hp bar
		this->m_hp_bar->setCurrentValue(this->m_actor->getStats()->hp_current);
		this->m_hp_bar->setMaxValue(this->m_actor->getStats()->hp_max);

		// base draw
		UiWidget::draw(rt);
	}
}
