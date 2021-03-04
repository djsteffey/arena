#include "Ability.hpp"
#include "ActorManager.hpp"
#include "Actor.hpp"
#include "misc.hpp"
#include "Action.hpp"

namespace arena {
	Ability::Ability(Actor* owner, std::string name, int cooldown, int range, bool require_los) {
		// save
		this->m_owner = owner;
		this->m_name = name;
		this->m_cooldown = cooldown;
		this->m_cooldown_current = 0;
		this->m_range = range;
		this->m_requires_los = require_los;
	}

	Ability::~Ability() {
		// nothing
	}

	void Ability::findTargets(std::vector<AbilityTargetSet>& ats, ActorManager* actor_manager, Tilemap* tilemap) {
		// first check if on cooldown
		if (this->m_cooldown_current > 0) {
			// on cooldown; nothing
			return;
		}

		// go through each actor in the manager
		for (int i = 0; i < actor_manager->getActorCount(); ++i) {
			// get handle to the other actor
			Actor* other = actor_manager->getActorAtIndex(i);

			// make sure it a valid target
			if (this->isValidTargetActor(other) == false) {
				// not valid for the ability
				continue;
			}

			// check if in range
			int range = misc::getTileDistanceManhattan(
				this->m_owner->getTilePositionX(),
				this->m_owner->getTilePositionY(),
				other->getTilePositionX(),
				other->getTilePositionY()
			);
			if (std::round(range) > this->m_range) {
				// out of range
				continue;
			}

			// check los
			if (this->m_requires_los) {
				// bresenhamms from owner to target
				bool finished = misc::bresenham(
					this->m_owner->getTilePositionX(),
					this->m_owner->getTilePositionY(),
					other->getTilePositionX(),
					other->getTilePositionY(),
					[](int x, int y) {
						return true;
					}
				);

				// check if we finished bresenham
				if (finished == false) {
					// didnt have los
					continue;
				}
			}

			// passed all the checks so add as an ats
			ats.push_back(AbilityTargetSet(other, this));
		}
	}

	Actor* Ability::getOwner() {
		return this->m_owner;
	}

	//////////////////////////////////////////////////////////////////////////////

	AbilityMeleeAttack::AbilityMeleeAttack(Actor* owner) : Ability(owner, "Attack", 0, 1, true) {

	}

	AbilityMeleeAttack::~AbilityMeleeAttack() {

	}

	std::unique_ptr<Action> AbilityMeleeAttack::generateAction(Actor* target) {
		// create the action
		std::unique_ptr<Action> as = std::make_unique<ActionMeleeAttack>(
			this->getOwner(),
			target,
			250
		);

		// done
		return as;
	}

	bool AbilityMeleeAttack::isValidTargetActor(Actor* target) {
		// actor must be alive
		if (target->getIsAlive() == false) {
			return false;
		}

		// cant be owner
		if (target == this->getOwner()) {
			return false;
		}

		// passed all the checks
		return true;
	}
}
