#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace arena {
	class Actor;
	class Ability;
	class ActorManager;
	class Tilemap;
	class ActionSequence;
	class Action;

	struct AbilityTargetSet {
		Actor* target;
		Ability* ability;
		AbilityTargetSet(Actor* target, Ability* ability) {
			this->target = target;
			this->ability = ability;
		}
	};

	class Ability {
	public:
		Ability(Actor* owner, std::string name, int cooldown, int range, bool require_los);
		virtual ~Ability();
		virtual void findTargets(std::vector<AbilityTargetSet>& ats, ActorManager* actor_manager, Tilemap* tilemap);
		virtual std::unique_ptr<Action> generateAction(Actor* target) = 0;
		Actor* getOwner();

	protected:
		virtual bool isValidTargetActor(Actor* target) = 0;

	private:
		Actor* m_owner;
		std::string m_name;
		int m_cooldown;
		int m_cooldown_current;
		int m_range;
		bool m_requires_los;
	};

	class AbilityMeleeAttack : public Ability {
	public:
		AbilityMeleeAttack(Actor* owner);
		~AbilityMeleeAttack();
		std::unique_ptr<Action> generateAction(Actor* target) override;

	protected:
		bool isValidTargetActor(Actor* target) override;

	private:
	};
}
