/*
  ==============================================================================

    Header.cpp
    Created: 1 Jul 2024 3:43:46pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "Header.h"

namespace Artix::Ui {
    Header::Header(Theme::BaseTheme& theme) : theme(theme) {}

    Header::~Header() {}

    void Header::paint(juce::Graphics& g) {
        theme.drawRounderContainer(
            this, g, getLocalBounds().toFloat(), true, Metric::SMALL, Metric::TINY,
            UIColor::BORDER_MUTED, UIColor::BACKGROUND_MUTED
        );
    }

    void Header::resized() {
        innerArea = theme.getInnerArea(this, Metric::SMALL, Metric::SMALL);
    }
}