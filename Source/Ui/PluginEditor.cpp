/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 20 Jun 2024 02:55:24pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "PluginEditor.h"

namespace Artix::UI {
	PluginEditor::PluginEditor(ArtixAudioProcessor& p)
		: AudioProcessorEditor(&p), audioProcessor(p) {
		// Make sure that before the constructor has finished, you've set the
		// editor's size to whatever you need it to be.
		setSize(800, 600);
	}

	PluginEditor::~PluginEditor() {}

	void PluginEditor::paint(juce::Graphics& g) {
		// (Our component is opaque, so we must completely fill the background with a solid colour)
		g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

		g.setColour(juce::Colours::white);
		g.setFont(juce::FontOptions(15.0f));
		g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
	}

	void PluginEditor::resized() {
		// This is generally where you'll want to lay out the positions of any
		// subcomponents in your editor..
	}
}