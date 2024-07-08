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
		state(state), header(theme), mapperBank(state.getMapperBank(), theme), footer(theme) {
		
		getLookAndFeel().setDefaultSansSerifTypeface(theme.getSansSerifTypeface());

		setBufferedToImage(true);
		theme.setScaledFontSize(true);

		getConstrainer()->setFixedAspectRatio(ASPEC_RATIO);
		setResizable(true, false);
		stateSizeChanged(state.getWidth(), state.getHeight());
		state.onSizeChanged = [this](int w, int h) { stateSizeChanged(w, h); };

		addAndMakeVisible(header);
		addAndMakeVisible(mapperBank);
		addAndMakeVisible(footer);
		resized();
	}

	PluginEditor::~PluginEditor() {}

	void PluginEditor::paint(juce::Graphics& g) {
		const auto margin = theme.getSpacing(Metric::SMALL);
		theme.fillBackground(this, g);
	}

	void PluginEditor::resized() {
		state.setSize(getWidth(), getHeight());
		
		const auto scaler = 1 + (getWidth() - MIN_SIZE) / (4 * (MAX_SIZE - MIN_SIZE));
		theme.setScaler(scaler);

		innerArea = theme.getInnerArea(this, Metric::SMALL, Metric::SMALL);

		const auto padding = theme.getSpacing(Metric::SMALL);
		const float panelHeight = theme.scale(36);

		header.setBounds(innerArea.getX(), innerArea.getY(), innerArea.getWidth(), panelHeight);
		footer.setBounds(
			innerArea.getX(), innerArea.getBottom() - panelHeight,
			innerArea.getWidth(), panelHeight
		);

		mapperBank.setBounds(
			innerArea.getX(), header.getBottom() + padding,
			innerArea.getWidth(), innerArea.getHeight() - (2 * (padding + panelHeight))
		);
	}

	void PluginEditor::stateSizeChanged(int width, int height) {
		setSize(width, height);
		setResizeLimits(MIN_SIZE, MIN_SIZE / ASPEC_RATIO, MAX_SIZE, MAX_SIZE / ASPEC_RATIO);
	}
}