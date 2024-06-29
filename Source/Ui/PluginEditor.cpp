/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 20 Jun 2024 02:55:24pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "PluginEditor.h"

namespace Artix::Ui {
	PluginEditor::PluginEditor(ArtixAudioProcessor& audioProcessor, AppState& state)
		: AudioProcessorEditor(&audioProcessor), audioProcessor(audioProcessor),
		state(state), mapperBank(state.getMapperBank()) {

		stateSizeChanged(state.getWidth(), state.getHeight());
		state.onSizeChanged = [this](int w, int h) { stateSizeChanged(w, h); };
		setResizable(true, false);

		addAndMakeVisible(mapperBank);
	}

	PluginEditor::~PluginEditor() {}

	void PluginEditor::paint(juce::Graphics& g) {
		g.fillAll(Color::ControlBackground);
	}

	void PluginEditor::resized() {
		state.setSize(getWidth(), getHeight());
		
		mapperBank.setSize(
			getWidth() - (2 * Spacing::Small),
			getHeight() - (2 * Spacing::Small)
		);
		mapperBank.setTopLeftPosition(Spacing::Small, Spacing::Small);
	}

	void PluginEditor::stateSizeChanged(int width, int height) {
		setSize(width, height);
		setResizeLimits(640, 480, 1024, 768);
	}
}