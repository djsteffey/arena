#include "UiManager.hpp"
#include "UiWidget.hpp"

namespace arena {
	UiManager::UiManager() {

	}

	UiManager::~UiManager() {

	}

	void UiManager::update(int ms) {
		for (auto& widget : this->m_widgets) {
			widget->update(ms);
		}
	}

	void UiManager::draw(sf::RenderTarget* rt) {
		for (auto& widget : this->m_widgets) {
			widget->draw(rt);
		}
	}

	void UiManager::add(std::unique_ptr<UiWidget> widget) {
		this->m_widgets.push_back(std::move(widget));
	}
}
