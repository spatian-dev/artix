/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cmath>
#include <optional>

#include "../AppState.h"
#include "../Midi/Presets.h"
#include "../PluginProcessor.h"
#include "Theme/Themes.h"
#include "Header.h"
#include "MidiChannelMapperBankPanel.h"
#include "Footer.h"

namespace Artix::Ui {
	class PluginEditor : public juce::AudioProcessorEditor, private Theme::Themable {
		public:
		PluginEditor(ArtixAudioProcessor&, AppState&, Midi::Presets&);
		~PluginEditor() override;

		void paint(juce::Graphics&) override;
		void resized() override;

		void setTheme(Theme::ThemePtr v) noexcept override;

		private:
		constexpr static inline float MIN_HEIGHT = 800;
		constexpr static inline float MAX_HEIGHT = 1200;
		constexpr static inline float ASPECT_RATIO = 0.75;

		ArtixAudioProcessor& audioProcessor;
		AppState& state;

		AppState::HeightChangedCallbacks::Identifier heightChangedCallbackId;
		AppState::ThemeChangedCallbacks::Identifier themeChangedCallbackId;

		juce::Rectangle<float> innerArea;

		MidiChannelMapperBankPanel mapperBank;
		Header header;
		Footer footer;

		void stateHeightChanged(int v);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
	};
}