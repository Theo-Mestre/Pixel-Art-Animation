#include "corepch.h"
#include "AnimatedSpriteModule.h"

namespace Animation
{
	AnimatedSpriteModule::AnimatedSpriteModule()
		: Module()
		, m_sprite()
	{
	}

	AnimatedSpriteModule::AnimatedSpriteModule(const AnimationData& _data)
		: Module()
		, m_sprite(_data)
	{
	}

	AnimatedSpriteModule::~AnimatedSpriteModule()
	{
	}

	void AnimatedSpriteModule::update(float _deltaTime)
	{
		m_sprite.update(_deltaTime);
	}

	AnimatedSprite& AnimatedSpriteModule::operator()()
	{
		return m_sprite;
	}

	void AnimatedSpriteModule::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
	{
		_states.transform *= getTransform();
		_target.draw(m_sprite, _states);
	}
}