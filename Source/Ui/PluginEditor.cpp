/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

namespace Artix::Ui {
	PluginEditor::PluginEditor(ArtixAudioProcessor& proc, AppState& state, Midi::Presets& presets)
		: AudioProcessorEditor(&proc), audioProcessor(proc), state(state),
		Themable(state.getTheme()), header(proc, state, presets, theme),
		mapperBank(state.getMapperBank(), theme), footer(theme) {
		
		setName("ArtixPluginEditor");
		setDescription("Artix Plugin Editor");

		addAndMakeVisible(header);
		addAndMakeVisible(mapperBank);
		addAndMakeVisible(footer);

		const auto localConstrainer = getConstrainer();
		localConstrainer->setFixedAspectRatio(ASPECT_RATIO);
		localConstrainer->setSizeLimits(
			(int) std::floor(MIN_HEIGHT * ASPECT_RATIO), (int) MIN_HEIGHT,
			(int) std::ceil(MAX_HEIGHT * ASPECT_RATIO), (int) MAX_HEIGHT
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
		theme->fillBackground(this, g);
	}

	void PluginEditor::resized() {
		state.setHeight(getHeight());

		const auto scaler = 1 + (getWidth() - MIN_HEIGHT) / (4 * (MAX_HEIGHT - MIN_HEIGHT));
		theme->setScaler(scaler);

		innerArea = theme->getInnerArea(this, Metric::SMALL, Metric::SMALL);

		const auto padding = theme->getSpacing(Metric::SMALL);
		const float headerHeight = theme->scale(40.0f);
		const float footerHeight = theme->scale(36.0f);

		header.setBounds(
			(int) innerArea.getX(), (int) innerArea.getY(),
			(int) innerArea.getWidth(), (int) headerHeight
		);
		footer.setBounds(
			(int) innerArea.getX(), (int) (innerArea.getBottom() - footerHeight),
			(int) innerArea.getWidth(), (int) footerHeight
		);

		mapperBank.setBounds(
			(int) innerArea.getX(), (int) (header.getBottom() + padding),
			(int) innerArea.getWidth(), (int) (innerArea.getHeight() - ((2 * padding) + headerHeight + footerHeight))
		);
	}

	void PluginEditor::stateHeightChanged(int v) {
		if (v == (int) getHeight())
			return;

		setSize((int) std::floor(v * ASPECT_RATIO), v);
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