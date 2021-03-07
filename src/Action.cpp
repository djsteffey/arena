#include "Action.hpp"
#include "Actor.hpp"
#include "AssetManager.hpp"
#include "Effect.hpp"

namespace arena {
	Action::Action() {

	}

	Action::~Action() {

	}

	void Action::draw(sf::RenderTarget* rt) {

	}

	//////////////////////////////////////////////////////////////////////////////

	ActionCustom::ActionCustom(std::function<bool(int)> update_function) {
		this->m_update_function = update_function;
	}

	ActionCustom::~ActionCustom() {

	}

	bool ActionCustom::update(int ms) {
		return this->m_update_function(ms);
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

	void ActionSequence::draw(sf::RenderTarget* rt) {
		this->m_actions.front()->draw(rt);
	}

	void ActionSequence::addAction(std::unique_ptr<Action> action) {
		this->m_actions.push_back(std::move(action));
	}

	//////////////////////////////////////////////////////////////////////////////

	ActionMoveTo::ActionMoveTo(Actor* actor, int duration, sf::Vector2f start, sf::Vector2f end) {
		this->m_duration = duration;
		this->m_current_duration = 0;
		this->m_actor = actor;
		this->m_start_position = start;
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
	
	ActionMoveBy::ActionMoveBy(Actor* actor, int duration, sf::Vector2f start, sf::Vector2f amount) : ActionMoveTo(actor, duration, start, actor->getWorldPosition() + amount) {

	}

	ActionMoveBy::~ActionMoveBy() {

	}

	//////////////////////////////////////////////////////////////////////////////

	ActionMeleeAttack::ActionMeleeAttack(Actor* actor, Actor* target, int duration) {
		// compute start and end
		sf::Vector2f start = actor->getWorldPosition();
		sf::Vector2f end = target->getWorldPosition();
		end = (end - start) / 2.0f + start;

		// bump towards
		std::unique_ptr<Action> ptr = std::make_unique<ActionMoveTo>(
			actor,
			duration / 2,
			start,
			end
		);
		this->addAction(std::move(ptr));

		// damage
		ptr = std::make_unique<ActionCustom>(
			[actor, target](int ms) -> bool {
				// calculate damage
				int damage = 2;
				target->onDamage(2);

				// done
				return true;
			}
		);
		this->addAction(std::move(ptr));

		// bump away
		ptr = std::make_unique<ActionMoveTo>(
			actor,
			duration / 2,
			end,
			start
			);
		this->addAction(std::move(ptr));
	}

	ActionMeleeAttack::~ActionMeleeAttack() {

	}

	//////////////////////////////////////////////////////////////////////////////

	ActionProjectileAttack::ActionProjectileAttack(AssetManager* am, Actor* actor, Actor* target, int duration) {
		// compute start and end
		sf::Vector2f start = actor->getWorldPositionCenter();
		sf::Vector2f end = target->getWorldPositionCenter();

		// create a projectile
		std::unique_ptr<Effect> effect = std::make_unique<EffectProjectile>(
			start.x,
			start.y,
			end.x,
			end.y,
			duration
		);

		// add as an action
		std::unique_ptr<Action> ptr = std::make_unique<ActionEffect>(std::move(effect));
		this->addAction(std::move(ptr));

		// damage
		auto damage = std::make_unique<ActionCustom>(
			[actor, target](int ms) -> bool {
				// calculate damage
				int damage = 1;
				target->onDamage(1);

				// done
				return true;
			}
		);
		this->addAction(std::move(damage));
	}

	ActionProjectileAttack::~ActionProjectileAttack() {

	}


	//////////////////////////////////////////////////////////////////////////////

	ActionDelay::ActionDelay(int ms) {
		this->m_duration = ms;
		this->m_duration_current = 0;
	}

	ActionDelay::~ActionDelay() {

	}

	bool ActionDelay::update(int ms) {
		this->m_duration_current += ms;
		if (this->m_duration_current > this->m_duration) {
			this->m_duration_current = this->m_duration;
		}
		if (this->m_duration_current == this->m_duration) {
			return true;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////

	ActionEffect::ActionEffect(std::unique_ptr<Effect> effect) {
		this->m_effect = std::move(effect);
	}

	ActionEffect::~ActionEffect() {

	}

	bool ActionEffect::update(int ms) {
		return this->m_effect->update(ms);
	}

	void ActionEffect::draw(sf::RenderTarget* rt) {
		this->m_effect->draw(rt);
	}
}
