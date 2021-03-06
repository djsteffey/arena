#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <functional>
#include <memory>

namespace arena {
	class Actor;
	class AssetManager;
	class Effect;

	class Action {
	public:
		Action();
		virtual ~Action();
		virtual bool update(int ms) = 0;
		virtual void draw(sf::RenderTarget* rt);

	protected:


	private:

	};

	class ActionCustom : public Action {
	public:
		ActionCustom(std::function<bool(int)> update_function);
		~ActionCustom();
		bool update(int ms) override;

	protected:

	private:
		std::function<bool(int)> m_update_function;
	};

	class ActionSequence : public Action {
	public:
		ActionSequence();
		~ActionSequence();
		bool update(int ms) override;
		void draw(sf::RenderTarget* rt) override;
		void addAction(std::unique_ptr<Action> action);

	protected:

	private:
		std::list<std::unique_ptr<Action>> m_actions;
	};

	class ActionMoveTo : public Action {
	public:
		ActionMoveTo(Actor* actor, int duration, sf::Vector2f start, sf::Vector2f end);
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
		ActionMoveBy(Actor* actor, int duration, sf::Vector2f start, sf::Vector2f amount);
		~ActionMoveBy();

	protected:

	private:
	};

	class ActionMeleeAttack : public ActionSequence {
	public:
		ActionMeleeAttack(Actor* actor, Actor* target, int duration);
		~ActionMeleeAttack();
		
	protected:

	private:
	};

	class ActionProjectileAttack : public ActionSequence {
	public:
		ActionProjectileAttack(AssetManager* am, Actor* actor, Actor* target, int duration);
		~ActionProjectileAttack();

	protected:

	private:

	};

	class ActionDelay : public Action {
	public:
		ActionDelay(int ms);
		~ActionDelay();
		bool update(int ms) override;

	protected:

	private:
		int m_duration;
		int m_duration_current;
	};

	class ActionEffect : public Action {
	public:
		ActionEffect(std::unique_ptr<Effect> effect);
		~ActionEffect();
		bool update(int ms) override;
		void draw(sf::RenderTarget* rt) override;

	protected:

	private:
		std::unique_ptr<Effect> m_effect;
	};
}
