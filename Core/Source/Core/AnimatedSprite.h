#pragma once
#include "SFML/Graphics.hpp"

namespace Animation
{
	struct AnimationData
	{
		const char* AnimationName;
		const char* TextureName;

		sf::Vector2u FrameNumber;
		float FrameDuration;
	};

	class AnimatedSprite
		: public sf::Drawable, public sf::Transformable
	{
	public:	
		AnimatedSprite();
		AnimatedSprite(AnimationData _data);
		~AnimatedSprite();

#pragma region Accessors
		inline void setTexture(const sf::Texture& _texture) { m_texture = _texture; }
		inline void setAnimation(const sf::Texture& _animation) { m_animation = _animation; }

		inline void setFrameSize(const sf::Vector2u& _frameSize) { m_frameSize = _frameSize; }
		inline void setFrameNumber(const sf::Vector2u& _frameNumber) { m_frameNumber = _frameNumber; }
		inline void setCurrentFrame(const sf::Vector2u& _currentFrame) { m_currentFrame = _currentFrame; }

		inline void setFrameDuration(float _frameDuration) { m_frameDuration = _frameDuration; }
		inline void setElapsedTime(float _elapsedTime) { m_elapsedTime = _elapsedTime; }

		inline const sf::Texture& getTexture() const { return m_texture; }
		inline const sf::Texture& getAnimation() const { return m_animation; }

		inline const sf::Vector2u& getFrameSize() const { return m_frameSize; }
		inline const sf::Vector2u& getFrameNumber() const { return m_frameNumber; }
		inline const sf::Vector2u& getCurrentFrame() const { return m_currentFrame; }

		inline float getFrameDuration() const { return m_frameDuration; }
		inline float getElapsedTime() const { return m_elapsedTime; }
#pragma endregion

		void initialize(AnimationData _data);
		void update(float _deltaTime);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void updateVertices();

	private:
		sf::Texture m_texture;
		sf::Texture m_animation;

		sf::VertexArray m_vertices;
		sf::Shader m_shader;

		sf::Vector2u m_frameSize;
		sf::Vector2u m_frameNumber;
		sf::Vector2u m_currentFrame;

		float m_elapsedTime;
		float m_frameDuration;
	};
}