/*
  ==============================================================================

	MidiChannelMapperBankComponent.h
	Created: 28 Jun 2024 8:18:23pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Metrics.h"
#include "Colors.h"
#include "../Midi/MidiChannelMapperBank.h"

namespace Artix::Ui {
	class MidiChannelMapperBankComponent : public juce::Component {
		public:
		MidiChannelMapperBankComponent(Midi::MidiChannelMapperBank& mapperBank);
		~MidiChannelMapperBankComponent() override;

		void paint(juce::Graphics&) override;
		void resized() override;

		private:
		Midi::MidiChannelMapperBank& mapperBank;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiChannelMapperBankComponent)
	};
}
