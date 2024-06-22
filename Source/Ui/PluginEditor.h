/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 20 Jun 2024 02:55:24pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

namespace Artix::UI {
	class PluginEditor : public juce::AudioProcessorEditor {
		public:
		PluginEditor(ArtixAudioProcessor&);
		~PluginEditor() override;

		void paint(juce::Graphics&) override;
		void resized() override;

		private:
		ArtixAudioProcessor& audioProcessor;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
	};
}
