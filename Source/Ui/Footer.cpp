/*
  ==============================================================================

	Footer.cpp
	Created: 1 Jul 2024 12:34:36am
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "Footer.h"

namespace Artix::Ui {
	Footer::Footer(Theme::BaseTheme& theme) : theme(theme) {
		std::stringstream ss;
		ss << JucePlugin_Manufacturer << " " << JucePlugin_Name << " v" << JucePlugin_VersionString;
		versionLabel.setText(ss.str(), juce::NotificationType::dontSendNotification);
		versionLabel.setJustificationType(juce::Justification::centredLeft);
		versionLabel.setColour(juce::Label::textColourId, theme.getUIColor(UIColor::TEXT));
		versionLabel.setBorderSize(juce::BorderSize<int>(0));
		addAndMakeVisible(versionLabel);

		websiteLink.setFontSize(Metric::MEDIUM);
		websiteLink.setJustification(juce::Justification::centredRight);
		addAndMakeVisible(websiteLink);
		
		resized();
	}
	Footer::~Footer() {}

	void Footer::paint(juce::Graphics& g) {
		theme.drawRounderContainer(
			this, g, getLocalBounds().toFloat(), true, Metric::SMALL, Metric::TINY, UIColor::BORDER_MUTED,
			isMouseButtonDown() ? UIColor::BACKGROUND : UIColor::BACKGROUND_MUTED
		);
	}

	void Footer::resized() {
		innerArea = theme.getInnerArea(this, Metric::SMALL, Metric::SMALL);

		versionLabel.setFont(juce::FontOptions(theme.getFontSize(Metric::MEDIUM), juce::Font::bold));

		juce::FlexBox fb;
		fb.flexWrap = juce::FlexBox::Wrap::noWrap;
		fb.flexDirection = juce::FlexBox::Direction::row;
		fb.items.add(juce::FlexItem(versionLabel).withFlex(1));
		fb.items.add(juce::FlexItem(websiteLink).withFlex(1));
		fb.performLayout(innerArea);
	}
}