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

		mapperBankOutputChannelChangedCallbackId = mapperBank.onOutputChannelChanged.add(
			[this](Midi::Channel ch) { outputChannel.setValue(static_cast<uint8_t>(ch)); }
		);
		outputChannelValueChangedCallbackId = outputChannel.onValueChanged.add([&mapperBank](uint8_t v) {
			mapperBank.setOutputChannel(v);
		});
		mapperBank.onOutputChannelChanged.callSafely(mapperBank.getOutputChannel());

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

			mapperBankInputNameChangedCallbackId.push_back(
				mapper.onNameChanged.add([mapperSelector](const juce::String& v) {
					mapperSelector->setLabel(v);
				})
			);
			mapperLabelTextChangedCallbackIds.push_back(
				mapperSelector->onLabelTextChanged.add([&mapper](const juce::String v) {
					mapper.setName(v);
				})
			);
			mapper.onNameChanged.callSafely(mapper.getName());

			mapperBankInputNoteChangedCallbackId.push_back(
				mapper.onNoteChanged.add([mapperSelector](Midi::Note note) {
					mapperSelector->setValue(static_cast<int8_t>(note));
				})
			);
			mapperNoteChangedCallbackIds.push_back(
				mapperSelector->onValueChanged.add([&mapper](int8_t v) {
					mapper.setNote(v);
				})
			);
			mapper.onNoteChanged.callSafely(mapper.getNote());


			addAndMakeVisible(*mapperSelector);
		}
		setTheme(theme);
		resized();
	}

	MidiChannelMapperBankPanel::~MidiChannelMapperBankPanel() {
		if (outputChannelValueChangedCallbackId)
			outputChannel.onValueChanged.remove(outputChannelValueChangedCallbackId.value());

		if (mapperBankOutputChannelChangedCallbackId)
			mapperBank.onOutputChannelChanged.remove(mapperBankOutputChannelChangedCallbackId.value());

		for (size_t i = 0; i < mapperBank.size(); i++) {
			if (mapperNoteChangedCallbackIds[i])
				mappers[i]->onValueChanged.remove(mapperNoteChangedCallbackIds[i].value());

			if (mapperLabelTextChangedCallbackIds[i])
				mappers[i]->onLabelTextChanged.remove(mapperLabelTextChangedCallbackIds[i].value());

			if (mapperBankInputNameChangedCallbackId[i])
				mapperBank[i].onNameChanged.remove(mapperBankInputNameChangedCallbackId[i].value());

			if (mapperBankInputNoteChangedCallbackId[i])
				mapperBank[i].onNoteChanged.remove(mapperBankInputNoteChangedCallbackId[i].value());
		}
	}

	void MidiChannelMapperBankPanel::paint(juce::Graphics& g) {
		theme->drawRounderContainer(
			this, g, getLocalBounds().toFloat(), false, Metric::SMALL, Metric::TINY,
			UIColor::BORDER_ELEMENT, UIColor::BACKGROUND
		);
	}

	void MidiChannelMapperBankPanel::resized() {
		innerArea = theme->getInnerArea(this, Metric::SMALL, Metric::SMALL);

		outputChannel.setBounds(
			(int) innerArea.getX(), (int) innerArea.getY(),
			(int) innerArea.getWidth(), (int) theme->scale(48.0f)
		);

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
				const auto i = x + (y * (int) columns);
				mappers[i]->setBounds((int) left, (int) top, (int) columnWidth, (int) rowHeight);
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