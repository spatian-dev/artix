/*
  ==============================================================================

	MidiChannelMapperBankPanel.cpp
	Created: 28 Jun 2024 8:18:23pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "MidiChannelMapperBankPanel.h"

namespace Artix::Ui {
	MidiChannelMapperBankPanel::MidiChannelMapperBankPanel(
		Midi::MidiChannelMapperBank& mapperBank, Theme::ThemePtr theme
	) : Themable(theme), mapperBank(mapperBank), outputChannel(theme) {

		outputChannel.setLabel("MIDI Output Channel");
		outputChannel.setLayoutDirection(Ui::Component::DigitalSelectorPanelDirection::HORIZONTAL);
		outputChannel.setMinMax(
			static_cast<uint8_t>(Midi::Channel::First), static_cast<uint8_t>(Midi::Channel::Last)
		);
		mapperBank.onOutputChannelChanged = [this](Midi::Channel ch) {
			outputChannel.setValue(static_cast<uint8_t>(ch));
		};
		outputChannel.onValueChanged = [&mapperBank](uint8_t v) {
			mapperBank.setOutputChannel(v);
		};
		mapperBank.onOutputChannelChanged(mapperBank.getOutputChannel());

		addAndMakeVisible(outputChannel);

		int i = 0;
		for (auto& mapper : mapperBank) {
			auto mapperSelector = std::make_shared<MapperDigitalSelector>(
				theme, juce::String(++i), "", true
			);
			mappers.push_back(mapperSelector);

			mapperSelector->setBaseColor(theme->getUIColor(UIColor::BACKGROUND_PRIMARY));
			mapperSelector->setNameJustification(juce::Justification::centredRight);
			mapperSelector->setLabelJustification(juce::Justification::centred);
			mapperSelector->setMinMax(
				static_cast<int8_t>(Midi::Note::None), static_cast<int8_t>(Midi::Note::Last)
			);

			mapperSelector->setCustomFormatter([](int8_t v, int8_t maxChars, int8_t maxDigits) -> juce::String {
				if (v == static_cast<int8_t>(Midi::Note::None)) {
					return std::string(maxChars, '-');
				}
				return juce::MidiMessage::getMidiNoteName(v, true, true, 3);
			});

			mapper.onNameChanged = [mapperSelector](const juce::String& v) {
				mapperSelector->setLabel(v);
			};
			mapperSelector->onLabelTextChanged = [&mapper](const juce::String v) {
				mapper.setName(v);
			};
			mapper.onNameChanged(mapper.getName());

			mapper.onNoteChanged = [mapperSelector](Midi::Note note) {
				mapperSelector->setValue(static_cast<int8_t>(note));
			};
			mapperSelector->onValueChanged = [&mapper](int8_t v) {
				mapper.setNote(v);
			};
			mapper.onNoteChanged(mapper.getNote());


			addAndMakeVisible(*mapperSelector);
		}
		setTheme(theme);
		resized();
	}

	MidiChannelMapperBankPanel::~MidiChannelMapperBankPanel() {}

	void MidiChannelMapperBankPanel::paint(juce::Graphics& g) {
		theme->drawRounderContainer(
			this, g, getLocalBounds().toFloat(), false, Metric::SMALL, Metric::TINY,
			UIColor::BORDER_ELEMENT, UIColor::BACKGROUND
		);
	}

	void MidiChannelMapperBankPanel::resized() {
		innerArea = theme->getInnerArea(this, Metric::SMALL, Metric::SMALL);

		outputChannel.setBounds(innerArea.getX(), innerArea.getY(), innerArea.getWidth(), theme->scale(48));

		constexpr float columns = 4.0f;

		const auto padding = theme->getSpacing(Metric::SMALL);
		float top = outputChannel.getHeight() + padding;
		float left;

		const auto columnWidth = std::floor((innerArea.getWidth() - (padding * (columns - 1))) / columns);
		const auto rows = std::ceil(mappers.size() / columns);
		const auto rowHeight = std::floor(
			(innerArea.getHeight() - top - (padding * (rows - 1))) / rows
		);

		const auto columnWidthWithPadding = columnWidth + padding;
		const auto rowHeightWithPadding = rowHeight + padding;

		top = outputChannel.getBottom() + padding;
		for (auto y = 0; y < rows; y++) {
			left = innerArea.getX();
			for (auto x = 0; x < columns; x++) {
				mappers[x + (y * columns)]->setBounds(left, top, columnWidth, rowHeight);
				left += columnWidthWithPadding;
			}
			top += rowHeightWithPadding;
		}
	}
	void MidiChannelMapperBankPanel::setTheme(Theme::ThemePtr v) noexcept {
		Themable::setTheme(v);
		outputChannel.setTheme(v);
		outputChannel.setBaseColor(theme->getUIColor(UIColor::BACKGROUND_SECONDARY));
		for (auto& mapperSelector : mappers) {
			mapperSelector->setTheme(v);
			mapperSelector->setBaseColor(theme->getUIColor(UIColor::BACKGROUND_PRIMARY));
		}
		resized();
		repaint();
	}
}