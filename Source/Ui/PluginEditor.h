/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 20 Jun 2024 02:55:24pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../AppState.h"
#include "../PluginProcessor.h"
#include "Colors.h"
#include "MidiChannelMapperBankComponent.h"

namespace Artix::Ui {
	class PluginEditor : public juce::AudioProcessorEditor {
		public:
		PluginEditor(ArtixAudioProcessor&, AppState&);
		~PluginEditor() override;

		void paint(juce::Graphics&) override;
		void resized() override;

		private:

		void stateSizeChanged(int width, int height);

		ArtixAudioProcessor& audioProcessor;
		AppState& state;

		MidiChannelMapperBankComponent mapperBank;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
	};
}
