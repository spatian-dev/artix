/*
  ==============================================================================

	MidiChannelMapperBankPanel.h
	Created: 28 Jun 2024 8:18:23pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cstdint>
#include <cmath>
#include <memory>

#include "Theme/Themes.h"
#include "Component/DigitalSelectorPanel.h"
#include "../Midi/MidiChannelMapperBank.h"
#include "../Utils/UTF8.h"

namespace Artix::Ui {
	class MidiChannelMapperBankPanel : public juce::Component, private Theme::Themable {
		public:
		MidiChannelMapperBankPanel(Midi::MidiChannelMapperBank& mapperBank, Theme::ThemePtr theme);
		~MidiChannelMapperBankPanel() override;

		void paint(juce::Graphics&) override;
		void resized() override;

		void setTheme(Theme::ThemePtr v) noexcept override;

		private:
		juce::Rectangle<float> innerArea;

		Midi::MidiChannelMapperBank& mapperBank;

		using OutputChannelDigitalSelector = Ui::Component::DigitalSelectorPanel<uint8_t>;
		using MapperDigitalSelector = Ui::Component::DigitalSelectorPanel<int8_t>;

		OutputChannelDigitalSelector outputChannel;
		std::vector<std::shared_ptr<MapperDigitalSelector>> mappers;
		
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiChannelMapperBankPanel)
	};
}
