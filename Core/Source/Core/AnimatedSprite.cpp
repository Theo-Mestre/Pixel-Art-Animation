#include "corepch.h"
#include "AnimatedSprite.h"

namespace Animation
{
	AnimatedSprite::AnimatedSprite()
		: m_frameSize(0, 0)
		, m_frameNumber(0, 0)
		, m_currentFrame(0, 0)
		, m_elapsedTime(0.0f)
		, m_frameDuration(0.0f)
	{
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(4);

		m_shader.loadFromFile("Shaders/AnimatedSprite.vert", "Shaders/AnimatedSprite.frag");
	}

	AnimatedSprite::AnimatedSprite(AnimationData _data)
		: m_frameNumber(_data.FrameNumber)
		, m_frameDuration(_data.FrameDuration)
		, m_currentFrame(0, 0)
		, m_elapsedTime(0.0f)
	{
		initialize(_data);
	}

	AnimatedSprite::~AnimatedSprite()
	{
	}

	void AnimatedSprite::initialize(AnimationData _data)
	{
		m_frameNumber = _data.FrameNumber;
		m_frameDuration = _data.FrameDuration;

		if (!m_texture.loadFromFile(_data.TextureName))
		{
			std::cout << "Failed to load texture: " << _data.TextureName << std::endl;
		}

		if (!m_animation.loadFromFile(_data.AnimationName))
		{
			std::cout << "Failed to load texture: " << _data.AnimationName << std::endl;
		}

		auto texSize = m_animation.getSize();
		m_frameSize.x = texSize.x / m_frameNumber.x;
		m_frameSize.y = texSize.y / m_frameNumber.y;

		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(4);

		const std::string vertName = "Shaders/AnimatedSprite.vert";
		const std::string fragName = "Shaders/AnimatedSprite.frag";

		if (!m_shader.loadFromFile(vertName, fragName))
		{
			std::cout << "Failed to load shaders: " << vertName << " - " << fragName << std::endl;
		}
	}

	void AnimatedSprite::update(float _deltaTime)
	{
		m_elapsedTime += _deltaTime;
		if (m_elapsedTime >= m_frameDuration)
		{
			m_elapsedTime -= m_frameDuration;
			m_currentFrame.x++;
			if (m_currentFrame.x >= m_frameNumber.x)
			{
				m_currentFrame.x = 0;
				m_currentFrame.y++;
				if (m_currentFrame.y >= m_frameNumber.y)
				{
					m_currentFrame.y = 0;
				}
			}
		}
		
		updateVertices();

		m_shader.setUniform("u_texture", m_texture);
		m_shader.setUniform("u_animation", m_animation);
	}

	void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &m_animation;
		states.shader = &m_shader;
		target.draw(m_vertices, states);
	}

	void AnimatedSprite::updateVertices()
	{
		float width = static_cast<float>(m_frameSize.x);
		float height = static_cast<float>(m_frameSize.y);
		float x = static_cast<float>(m_currentFrame.x * m_frameSize.x);
		float y = static_cast<float>(m_currentFrame.y * m_frameSize.y);

		m_vertices[0].position = sf::Vector2f(0.0f, 0.0f);
		m_vertices[1].position = sf::Vector2f(0.0f, height);
		m_vertices[2].position = sf::Vector2f(width, height);
		m_vertices[3].position = sf::Vector2f(width, 0.0f);

		m_vertices[0].texCoords = sf::Vector2f(x, y);
		m_vertices[1].texCoords = sf::Vector2f(x, y + height);
		m_vertices[2].texCoords = sf::Vector2f(x + width, y + height);
		m_vertices[3].texCoords = sf::Vector2f(x + width, y);
	}
}