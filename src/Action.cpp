#include "Action.hpp"
#include "Actor.hpp"

namespace arena {
	Action::Action() {

	}

	Action::~Action() {

	}

	bool Action::update(int ms) {
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////

	ActionSequence::ActionSequence() {
		
	}

	ActionSequence::~ActionSequence() {

	}

	bool ActionSequence::update(int ms) {
		// update the front
		if (this->m_actions.front()->update(ms)) {
			// front action is done
			this->m_actions.pop_front();

			// see if out of actions
			if (this->m_actions.empty()) {
				// all done
				return true;
			}
		}

		// not done yet
		return false;
	}

	void ActionSequence::addAction(std::unique_ptr<Action> action) {
		this->m_actions.push_back(std::move(action));
	}

	//////////////////////////////////////////////////////////////////////////////

	ActionMoveTo::ActionMoveTo(Actor* actor, int duration, sf::Vector2f end) {
		this->m_duration = duration;
		this->m_current_duration = 0;
		this->m_actor = actor;
		this->m_start_position = actor->getWorldPosition();
		this->m_end_position = end;
	}

	ActionMoveTo::~ActionMoveTo() {

	}

	bool ActionMoveTo::update(int ms) {
		this->m_current_duration += ms;
		if (this->m_current_duration > this->m_duration) {
			this->m_current_duration = this->m_duration;
		}
		float percent = static_cast<float>(this->m_current_duration) / this->m_duration;
		float x = this->m_start_position.x + (this->m_end_position.x - this->m_start_position.x) * percent;
		float y = this->m_start_position.y + (this->m_end_position.y - this->m_start_position.y) * percent;
		this->m_actor->setSpritePosition(x, y);
		if (this->m_current_duration == this->m_duration) {
			return true;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////
	
	ActionMoveBy::ActionMoveBy(Actor* actor, int duration, sf::Vector2f amount) : ActionMoveTo(actor, duration, actor->getWorldPosition() + amount) {

	}

	ActionMoveBy::~ActionMoveBy() {

	}

	//////////////////////////////////////////////////////////////////////////////

	ActionMeleeAttack::ActionMeleeAttack(Actor* actor, Actor* target, int duration, int tilesize) {
		// save the actors
		this->m_actor = actor;
		this->m_target = target;

		// compute start and end
		sf::Vector2f start = sf::Vector2f(static_cast<float>(this->m_actor->getTilePositionX() * tilesize), static_cast<float>(this->m_actor->getTilePositionY() * tilesize));
		sf::Vector2f end = sf::Vector2f(static_cast<float>(this->m_target->getTilePositionX() * tilesize), static_cast<float>(this->m_target->getTilePositionY() * tilesize));
		end = (end - start) / 2.0f + start;

		// setup sequence
		auto ptr = std::make_unique<ActionMoveTo>(
			actor,
			duration / 2,
			start,
			end
		);
		this->m_sequence.addAction(std::move(ptr));

		ptr = std::make_unique<ActionMoveTo>(
			actor,
			duration / 2,
			end,
			start
		);
		this->m_sequence.addAction(std::move(ptr));
	}

	ActionMeleeAttack::~ActionMeleeAttack() {

	}

	bool ActionMeleeAttack::update(int ms) {
		return this->m_sequence.update(ms);
	}
}
