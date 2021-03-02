#pragma once

#include <string>
#include <vector>

namespace arena {
	class Actor;
	class Ability;
	class ActorManager;
	class Tilemap;

	struct AbilityTargetSet {
		Actor* target;
		Ability* ability;
	};

	class Ability {
	public:
		Ability(Actor* owner, std::string name, int cooldown);
		virtual ~Ability();
		virtual void findTargets(std::vector<AbilityTargetSet>& ats, ActorManager* actor_manager, Tilemap* tilemap) = 0;

	protected:

	private:
		Actor* m_owner;
		std::string m_name;
		int m_cooldown;
		int m_cooldown_current;
		int m_range;
		int m_requires_los;
	};

	class AbilityMeleeAttack : public Ability {
	public:
		AbilityMeleeAttack();
		~AbilityMeleeAttack();
		void findTargets(std::vector<AbilityTargetSet>& ats, ActorManager* actor_manager, Tilemap* tilemap) override;

	protected:

	private:
	};
}
