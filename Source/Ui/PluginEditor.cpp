/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

namespace Artix::Ui {
	PluginEditor::PluginEditor(ArtixAudioProcessor& p, AppState& s)
		: AudioProcessorEditor(&p), audioProcessor(p), state(s),
		Themable(s.getTheme()), header(theme),
		mapperBank(state.getMapperBank(), theme), footer(theme) {
		
		setName("ArtixPluginEditor");
		setDescription("Artix Plugin Editor");

		addAndMakeVisible(header);
		addAndMakeVisible(mapperBank);
		addAndMakeVisible(footer);

		const auto constrainer = getConstrainer();
		constrainer->setFixedAspectRatio(ASPECT_RATIO);
		constrainer->setSizeLimits(
			std::floor(MIN_HEIGHT * ASPECT_RATIO), MIN_HEIGHT,
			std::ceil(MAX_HEIGHT * ASPECT_RATIO), MAX_HEIGHT
		);

		stateHeightChanged(state.getHeight());
		heightChangedCallbackId = state.onHeightChanged.add(
			[this](int v) { stateHeightChanged(v); }
		);

		setBufferedToImage(true);
		setTheme(theme);
		themeChangedCallbackId = state.onThemeChanged.add([this](Theme::ThemePtr v) { setTheme(v); });

		setResizable(true, false);
	}

	PluginEditor::~PluginEditor() {
		if (heightChangedCallbackId)
			state.onHeightChanged.remove(heightChangedCallbackId.value());

		if (themeChangedCallbackId)
			state.onThemeChanged.remove(themeChangedCallbackId.value());
	}

	void PluginEditor::paint(juce::Graphics& g) {
		const auto margin = theme->getSpacing(Metric::SMALL);
		theme->fillBackground(this, g);
	}

	void PluginEditor::resized() {
		state.setHeight(getHeight());

		const auto scaler = 1 + (getWidth() - MIN_HEIGHT) / (4 * (MAX_HEIGHT - MIN_HEIGHT));
		theme->setScaler(scaler);

		innerArea = theme->getInnerArea(this, Metric::SMALL, Metric::SMALL);

		const auto padding = theme->getSpacing(Metric::SMALL);
		const float panelHeight = theme->scale(36);

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

	void PluginEditor::stateHeightChanged(int v) {
		const auto size = juce::Point(getWidth(), getHeight());
		
		if (v == getHeight())
			return;

		setSize(std::floor(v * ASPECT_RATIO), v);
	}

	void PluginEditor::setTheme(Theme::ThemePtr v) noexcept {
		Themable::setTheme(v);
		getLookAndFeel().setDefaultSansSerifTypeface(theme->getSansSerifTypeface());
		theme->setScaledFontSize(true);
		header.setTheme(v);
		mapperBank.setTheme(v);
		footer.setTheme(v);
		resized();
		repaint();
	}
}