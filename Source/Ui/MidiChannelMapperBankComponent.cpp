/*
  ==============================================================================

	MidiChannelMapperBankComponent.cpp
	Created: 28 Jun 2024 8:18:23pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "MidiChannelMapperBankComponent.h"

namespace Artix::Ui {
	MidiChannelMapperBankComponent::MidiChannelMapperBankComponent(Midi::MidiChannelMapperBank& mapperBank) :
		mapperBank(mapperBank) {}

	MidiChannelMapperBankComponent::~MidiChannelMapperBankComponent() {}

	void MidiChannelMapperBankComponent::paint(juce::Graphics& g) {
		g.fillAll(Color::ControlBackground);

		g.setColour(Color::ControlText);
		g.setFont(juce::FontOptions(FontSize::Medium));
		g.drawText("MidiChannelMapperBankComponent", getLocalBounds(), juce::Justification::centred, true);
	}

	void MidiChannelMapperBankComponent::resized() {
	}
}