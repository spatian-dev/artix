/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 20 Jun 2024 02:55:24pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cmath>
#include <memory>

#include "../AppState.h"
#include "../PluginProcessor.h"
#include "Theme/Themes.h"
#include "Header.h"
#include "MidiChannelMapperBankPanel.h"
#include "Footer.h"

namespace Artix::Ui {
	class PluginEditor : public juce::AudioProcessorEditor, private Theme::Themable {
		public:
		PluginEditor(ArtixAudioProcessor&, AppState&);
		~PluginEditor() override;

		void paint(juce::Graphics&) override;
		void resized() override;

		void setTheme(Theme::ThemePtr v) noexcept override;

		private:
		constexpr static inline float MIN_SIZE = 600;
		constexpr static inline float ASPECT_RATIO = 0.75;
		constexpr static inline float MAX_SIZE = 900;

		void stateSizeChanged(int width, int height);

		ArtixAudioProcessor& audioProcessor;
		AppState& state;
		
		juce::Rectangle<float> innerArea;

		MidiChannelMapperBankPanel mapperBank;
		Header header;
		Footer footer;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
	};
}
