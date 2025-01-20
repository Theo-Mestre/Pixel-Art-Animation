#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf
{
	class Event;
}

namespace UI
{
	class UIElement;

	class UIModule
		: public sf::Drawable
		, public sf::Transformable
	{
	public:
		UIModule() = default;
		virtual ~UIModule() = default;
		virtual void receiveEvent(const sf::Event& _event);
		virtual void update(float _deltaTime) = 0;

	protected:
		UIElement* m_owner;
		friend class UIElement;
	};

	class TextModule
		: public UIModule
	{
	public:
		TextModule();
		TextModule(const std::string& _text);
		virtual ~TextModule() = default;

		virtual void update(float _deltaTime) override;

		void setText(const std::string& text);
		void setFont(sf::Font* font);
		void setColor(const sf::Color& color);
		void setTextCentered();

		sf::Text& operator()();
	private:
		void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	private:
		sf::Text m_text;
		sf::Font m_font;
	};
}