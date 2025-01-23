#pragma once

#include "Modules.h"

#include "AnimatedSprite.h"

namespace Animation
{

	class AnimatedSpriteModule
		: public UI::Module
	{
	public:
		AnimatedSpriteModule();
		AnimatedSpriteModule(const AnimationData& _data);
		virtual ~AnimatedSpriteModule() override;

		virtual void update(float _deltaTime) override;

		AnimatedSprite& asSprite() { return m_sprite; }
		const AnimatedSprite& asSprite() const { return m_sprite; }

		AnimatedSprite& operator()();
	private:
		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;
	private:
		AnimatedSprite m_sprite;
	};

}