#pragma once

#include "Panel.h"

namespace UI
{
	class Button;
	class Label;
}


namespace Animation
{
	class EditorData;

	constexpr size_t DimensionNumber = 2;

	class EditorDataPanel
		: public UI::Panel
	{
	public:
		EditorDataPanel();
		virtual ~EditorDataPanel();

		void initialize();
		void receiveEvent(const sf::Event& _event);
		void update(float _deltaTime);

		// Editor Data 
		void setData(EditorData* _editorData);
		EditorData* getData() const;

		void setTextFont(const sf::Font& _font);

		void UpdateData();
	private:
		void CenterTexts();

		void OpenTextureFile(size_t _textureType);

		void initializeTextureDataEditor();
		void initializeAnimCountEditor();

		void decrementAnimCount(size_t _index);
		void incrementAnimCount(size_t _index);

	private:
		struct TextureData
		{
			UI::Label* Label = nullptr;
			UI::Button* Button = nullptr;
			UI::Label* ButtonLabel = nullptr;
		};

		struct AnimCountEditUI
		{
			UI::Label* Label = nullptr;
			UI::Label* MinusLabel = nullptr;
			UI::Button* Minus = nullptr;
			UI::Label* Value = nullptr;
			UI::Label* PlusLabel = nullptr;
			UI::Button* Plus = nullptr;
		};

		struct AnimCountUI
		{
			UI::Label* Label = nullptr;
			AnimCountEditUI EditUI[DimensionNumber] = { nullptr };
		};

		enum TextureType
		{
			Animation,
			Texture,
			Count
		};

	private:
		EditorData* m_data;
		TextureData m_textureData[Count];
		AnimCountUI m_animCountUI;

		std::vector<sf::Text*> m_texts;
	};
}