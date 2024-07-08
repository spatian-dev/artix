/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 20 Jun 2024 02:55:24pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <memory>

#include "../AppState.h"
#include "../PluginProcessor.h"
#include "Theme/PicoDark.h"
#include "Theme/PicoLight.h"
#include "Header.h"
#include "MidiChannelMapperBankPanel.h"
#include "Footer.h"

namespace Artix::Ui {
	class PluginEditor : public juce::AudioProcessorEditor {
		public:
		PluginEditor(ArtixAudioProcessor&, AppState&);
		~PluginEditor() override;

		void paint(juce::Graphics&) override;
		void resized() override;

		private:
		constexpr static inline float MIN_SIZE = 600;
		constexpr static inline float ASPEC_RATIO = 0.75;
		constexpr static inline float MAX_SIZE = 900;

		void stateSizeChanged(int width, int height);

		ArtixAudioProcessor& audioProcessor;
		AppState& state;
		
		//Theme::BaseTheme theme = Theme::PicoLight();
		Theme::BaseTheme theme = Theme::PicoDark();
		juce::Rectangle<float> innerArea;

		MidiChannelMapperBankPanel mapperBank;
		Header header;
		Footer footer;
	};
}
