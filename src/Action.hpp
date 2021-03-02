#pragma once

#include <SFML/System.hpp>
#include <list>

namespace arena {
	class Actor;

	class Action {
	public:
		Action();
		virtual ~Action();
		virtual bool update(int ms) = 0;

	protected:

	private:
	};

	class ActionSequence : public Action {
	public:
		ActionSequence();
		~ActionSequence();
		bool update(int ms) override;
		void addAction(std::unique_ptr<Action> action);

	protected:

	private:
		std::list<std::unique_ptr<Action>> m_actions;
	};

	class ActionMoveTo : public Action {
	public:
		ActionMoveTo(Actor* actor, int duration, sf::Vector2f end);
		~ActionMoveTo();
		bool update(int ms) override;

	protected:

	private:
		int m_duration;
		int m_current_duration;
		Actor* m_actor;
		sf::Vector2f m_start_position;
		sf::Vector2f m_end_position;
	};

	class ActionMoveBy : public ActionMoveTo {
	public:
		ActionMoveBy(Actor* actor, int duration, sf::Vector2f amount);
		~ActionMoveBy();

	protected:

	private:
	};

	class ActionMeleeAttack : public Action {
	public:
		ActionMeleeAttack(Actor* actor, Actor* target, int duration, int tilesize);
		~ActionMeleeAttack();
		bool update(int ms) override;

	protected:

	private:
		Actor* m_actor;
		Actor* m_target;
		ActionSequence m_sequence;
	};
}
