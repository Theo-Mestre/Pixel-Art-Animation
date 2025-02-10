#include "corepch.h"

#include "Button.h"
#include "Label.h"

#include "EditorLayoutSettings.h"
#include "EditorDataPanel.h"
#include "FileUtility.h"
#include "Editor.h"	

namespace Animation
{
	using namespace LayoutSettings;

	static const size_t X = 0;
	static const size_t Y = 1;

	EditorDataPanel::EditorDataPanel()
		: UI::Panel()
		, m_data(nullptr)
		, m_textureData()
		, m_animCountUI()
		, m_texts()
	{
	}

	EditorDataPanel::~EditorDataPanel()
	{
		m_data = nullptr;

		// Texture Data
		for (size_t i = 0; i < TextureType::Count; i++)
		{
			m_textureData[i].Label = nullptr;
			m_textureData[i].Button = nullptr;
			m_textureData[i].ButtonLabel = nullptr;
		}

		// Animation Count UI
		m_animCountUI.Label = nullptr;

		for (size_t i = 0; i < DimensionNumber; i++)
		{
			m_animCountUI.EditUI[i].Minus = nullptr;
			m_animCountUI.EditUI[i].MinusLabel = nullptr;
			m_animCountUI.EditUI[i].Plus = nullptr;
			m_animCountUI.EditUI[i].PlusLabel = nullptr;
			m_animCountUI.EditUI[i].Value = nullptr;
		}

		for (size_t i = 0; i < DimensionNumber; i++)
		{
			m_texts[i] = nullptr;
		}
	}

	void EditorDataPanel::initialize()
	{
		UI::Panel::initialize();

		initializeTextureDataEditor();

		initializeAnimCountEditor();
	}
	void EditorDataPanel::receiveEvent(const sf::Event& _event)
	{
		UI::Panel::receiveEvent(_event);
	}

	void EditorDataPanel::update(float _deltaTime)
	{
		UI::Panel::update(_deltaTime);
	}

	void EditorDataPanel::setData(EditorData* _editorData)
	{
		m_data = _editorData;
	}

	EditorData* EditorDataPanel::getData() const
	{
		return m_data;
	}

	void EditorDataPanel::setTextFont(const sf::Font& _font)
	{
		for (auto text : m_texts)
		{
			text->setFont(_font);
		}
	}

	void EditorDataPanel::UpdateData()
	{
		if (m_data == nullptr) return;

		// Update Texture Path
		m_textureData[TextureType::Animation].Label->setString("Animation: " + Utility::TruncatePath(m_data->AnimationPath));
		m_textureData[TextureType::Texture].Label->setString("Texture: " + Utility::TruncatePath(m_data->TexturePath));

		// Update Animation Count

		m_animCountUI.EditUI[X].Value->setString(std::to_string(m_data->AnimationCount.x));
		m_animCountUI.EditUI[Y].Value->setString(std::to_string(m_data->AnimationCount.y));

		CenterTexts();
	}

	void EditorDataPanel::CenterTexts()
	{
		// Texture Data
		for (size_t i = 0; i < TextureType::Count; i++)
		{
			if (m_textureData[i].Label == nullptr) continue;

			// Texture Path Label
			auto& label = m_textureData[i].Label->asText();

			float height = label.getGlobalBounds().height;
			float yPos = m_textureData[i].Label->getSize().y / 2.0f;

			label.setPosition({ PanelPadding, yPos });
			label.setOrigin({ 0.0f, height / 1.25f });

			// Button Label
			auto& buttonLabel = m_textureData[i].ButtonLabel->asText();

			float width = buttonLabel.getGlobalBounds().width;
			height = buttonLabel.getGlobalBounds().height;
			float xPos = m_textureData[i].ButtonLabel->getSize().x / 2.0f;
			yPos = m_textureData[i].ButtonLabel->getSize().y / 2.0f;

			buttonLabel.setPosition({ xPos, yPos });
			buttonLabel.setOrigin({ width / 2.0f, height / 1.25f });
		}

		// Animation Count UI
		for (size_t i = 0; i < DimensionNumber; i++)
		{
			auto& label = m_animCountUI.EditUI[i].Label->asText();
			float height = label.getGlobalBounds().height;
			float yPos = m_animCountUI.EditUI[i].Label->getSize().y / 2.0f;
			label.setPosition({ PanelPadding, yPos });
			label.setOrigin({ 0.0f, height / 1.25f });

			auto& value = m_animCountUI.EditUI[i].Value->asText();
			float width = value.getGlobalBounds().width;
			height = value.getGlobalBounds().height;
			float xPos = m_animCountUI.EditUI[i].Value->getSize().x / 2.0f;
			yPos = m_animCountUI.EditUI[i].Value->getSize().y / 2.0f;
			value.setPosition({ xPos, yPos });
			value.setOrigin({ width / 2.0f, height / 1.25f });

			auto& minusLabel = m_animCountUI.EditUI[i].MinusLabel->asText();
			width = minusLabel.getGlobalBounds().width;
			height = minusLabel.getGlobalBounds().height;
			xPos = m_animCountUI.EditUI[i].MinusLabel->getSize().x / 2.0f;
			yPos = m_animCountUI.EditUI[i].MinusLabel->getSize().y / 2.0f;
			minusLabel.setPosition({ xPos, yPos - 15.0f });
			minusLabel.setOrigin({ width / 2.0f, height / 2.0f });

			auto& plusLabel = m_animCountUI.EditUI[i].PlusLabel->asText();
			width = plusLabel.getGlobalBounds().width;
			height = plusLabel.getGlobalBounds().height;
			xPos = m_animCountUI.EditUI[i].PlusLabel->getSize().x / 2.0f;
			yPos = m_animCountUI.EditUI[i].PlusLabel->getSize().y / 2.0f;
			plusLabel.setPosition({ xPos, yPos - 10.0f });
			plusLabel.setOrigin({ width / 2.0f, height / 1.25f });
		}
	}

	void EditorDataPanel::OpenTextureFile(size_t _textureType)
	{
		if (_textureType < 0 || _textureType >= TextureType::Count) return;

		std::string path = Utility::OpenFileDialog("Textures", "png");

		if (path.empty()) return;

		auto& texPath = _textureType == TextureType::Animation ? m_data->AnimationPath : m_data->TexturePath;
		texPath = path;

		UpdateData();
	}

	void EditorDataPanel::initializeTextureDataEditor()
	{
		float labelSizeX = LabelWidthInPercent * m_size.x;
		float labelSizeY = LabelHeightInPercent * m_size.y;
		labelSizeY /= static_cast<float>(TextureType::Count);

		float buttonSizeX = m_size.x - labelSizeX - PanelPadding * static_cast<float>(TextureType::Count + 1);

		// Texture Data
		for (size_t i = 0; i < TextureType::Count; i++)
		{
			float yPos = PanelPadding + i * labelSizeY + i * PanelPadding;

			m_textureData[i].Label = new UI::Label();
			m_textureData[i].Label->initialize();
			m_textureData[i].Label->setPosition({ PanelPadding, yPos });
			m_textureData[i].Label->setSize({ labelSizeX , labelSizeY });
			m_textureData[i].Label->setColor(LabelBackgroundColor);
			m_textureData[i].Label->setTextColor(LabelColor);

			addChild(m_textureData[i].Label);
			m_texts.push_back(&m_textureData[i].Label->asText());

			m_textureData[i].Button = new UI::Button();
			m_textureData[i].Button->setPosition({ PanelPadding * 2.0f + labelSizeX, yPos });
			m_textureData[i].Button->setSize({ buttonSizeX, labelSizeY });
			m_textureData[i].Button->setColor(ButtonColor);
			m_textureData[i].Button->initialize();
			m_textureData[i].Button->setCallback([this, i]() { OpenTextureFile(i); });

			addChild(m_textureData[i].Button);

			m_textureData[i].ButtonLabel = new UI::Label();
			m_textureData[i].ButtonLabel->initialize();
			m_textureData[i].ButtonLabel->setString(OpenButtonText);
			m_textureData[i].ButtonLabel->setTextColor(LabelColor);
			m_textureData[i].ButtonLabel->setColor(sf::Color::Transparent);
			m_textureData[i].ButtonLabel->setSize({ buttonSizeX, labelSizeY });
			m_textureData[i].ButtonLabel->setPosition({ PanelPadding * 2.0f + labelSizeX, yPos });

			addChild(m_textureData[i].ButtonLabel);
			m_texts.push_back(&m_textureData[i].ButtonLabel->asText());
		}
	}

	void EditorDataPanel::decrementAnimCount(size_t _index)
	{
		if (m_data == nullptr) return;

		if (_index == X && m_data->AnimationCount.x > 1)
		{
			m_data->AnimationCount.x--;
		}
		else if (_index == Y && m_data->AnimationCount.y > 1)
		{
			m_data->AnimationCount.y--;
		}

		UpdateData();
	}

	void EditorDataPanel::incrementAnimCount(size_t _index)
	{
		if (m_data == nullptr) return;

		if (_index == X)
		{
			m_data->AnimationCount.x++;
		}
		else if (_index == Y)
		{
			m_data->AnimationCount.y++;
		}

		UpdateData();
	}

	void EditorDataPanel::initializeAnimCountEditor()
	{
		float labelY = LabelHeightInPercent * m_size.y + AnimCountButtonNumber * PanelPadding;

		float height = AnimCountLabelHeightInPercent * m_size.y;
		float labelWidth = AnimCountLabelWidthInPercent * m_size.x;
		float buttonWidth = (m_size.x - labelWidth - 6.0f * PanelPadding) / AnimCountButtonNumber;

		//// Animation Count UI
		m_animCountUI.Label = new UI::Label();
		m_animCountUI.Label->initialize();
		m_animCountUI.Label->setString(AnimCountText);
		m_animCountUI.Label->setColor(LabelBackgroundColor);
		m_animCountUI.Label->setTextColor(LabelColor);
		m_animCountUI.Label->setSize({ labelWidth, height });
		m_animCountUI.Label->setPosition({ PanelPadding, labelY });

		addChild(m_animCountUI.Label);
		m_texts.push_back(&m_animCountUI.Label->asText());

		float buttonPos = m_animCountUI.Label->getSize().x + 2.0f * PanelPadding;

		for (size_t i = 0; i < DimensionNumber; i++)
		{
			uint32_t j = 0;
			float yPos = labelY + (PanelPadding + height) * i;

			m_animCountUI.EditUI[i].Label = new UI::Label();
			m_animCountUI.EditUI[i].Label->initialize();
			m_animCountUI.EditUI[i].Label->setString(DimensionText[i]);
			m_animCountUI.EditUI[i].Label->setSize({ buttonWidth, height });
			m_animCountUI.EditUI[i].Label->setColor(LabelColor);
			m_animCountUI.EditUI[i].Label->setColor(LabelBackgroundColor);
			m_animCountUI.EditUI[i].Label->setPosition({ buttonPos + j++ * (PanelPadding + buttonWidth), yPos });

			addChild(m_animCountUI.EditUI[i].Label);
			m_texts.push_back(&m_animCountUI.EditUI[i].Label->asText());

			m_animCountUI.EditUI[i].Minus = new UI::Button();
			m_animCountUI.EditUI[i].Minus->initialize();
			m_animCountUI.EditUI[i].Minus->setSize({ buttonWidth, height });
			m_animCountUI.EditUI[i].Minus->setColor(ButtonColor);
			m_animCountUI.EditUI[i].Minus->setCallback([this, i]() { decrementAnimCount(i); });
			m_animCountUI.EditUI[i].Minus->setPosition({ buttonPos + j * (PanelPadding + buttonWidth), yPos });

			addChild(m_animCountUI.EditUI[i].Minus);

			m_animCountUI.EditUI[i].MinusLabel = new UI::Label();
			m_animCountUI.EditUI[i].MinusLabel->initialize();
			m_animCountUI.EditUI[i].MinusLabel->setString("-");
			m_animCountUI.EditUI[i].MinusLabel->setSize({ buttonWidth, height });
			m_animCountUI.EditUI[i].MinusLabel->setColor(LabelColor);
			m_animCountUI.EditUI[i].MinusLabel->setColor(sf::Color::Transparent);
			m_animCountUI.EditUI[i].MinusLabel->setPosition({ buttonPos + j++ * (PanelPadding + buttonWidth), yPos });

			addChild(m_animCountUI.EditUI[i].MinusLabel);
			m_texts.push_back(&m_animCountUI.EditUI[i].MinusLabel->asText());

			m_animCountUI.EditUI[i].Value = new UI::Label();
			m_animCountUI.EditUI[i].Value->initialize();
			m_animCountUI.EditUI[i].Value->setSize({ buttonWidth, height });
			m_animCountUI.EditUI[i].Value->setColor(LabelColor);
			m_animCountUI.EditUI[i].Value->setColor(LabelBackgroundColor);
			m_animCountUI.EditUI[i].Value->setPosition({ buttonPos + j++ * (PanelPadding + buttonWidth), yPos });

			addChild(m_animCountUI.EditUI[i].Value);
			m_texts.push_back(&m_animCountUI.EditUI[i].Value->asText());

			m_animCountUI.EditUI[i].Plus = new UI::Button();
			m_animCountUI.EditUI[i].Plus->initialize();
			m_animCountUI.EditUI[i].Plus->setSize({ buttonWidth, height });
			m_animCountUI.EditUI[i].Plus->setColor(ButtonColor);
			m_animCountUI.EditUI[i].Plus->setCallback([this, i]() { incrementAnimCount(i); });
			m_animCountUI.EditUI[i].Plus->setPosition({ buttonPos + j * (PanelPadding + buttonWidth), yPos });

			addChild(m_animCountUI.EditUI[i].Plus);

			m_animCountUI.EditUI[i].PlusLabel = new UI::Label();
			m_animCountUI.EditUI[i].PlusLabel->initialize();
			m_animCountUI.EditUI[i].PlusLabel->setString("+");
			m_animCountUI.EditUI[i].PlusLabel->setSize({ buttonWidth, height });
			m_animCountUI.EditUI[i].PlusLabel->setColor(LabelColor);
			m_animCountUI.EditUI[i].PlusLabel->setColor(sf::Color::Transparent);
			m_animCountUI.EditUI[i].PlusLabel->setPosition({ buttonPos + j++ * (PanelPadding + buttonWidth), yPos });

			addChild(m_animCountUI.EditUI[i].PlusLabel);
			m_texts.push_back(&m_animCountUI.EditUI[i].PlusLabel->asText());
		}
	}
}