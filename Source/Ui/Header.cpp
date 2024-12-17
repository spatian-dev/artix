/*
  ==============================================================================

    Header.cpp
    Created: 1 Jul 2024 3:43:46pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "Header.h"

namespace Artix::Ui {
    Header::Header(AppState& state, Midi::Presets& presets, Theme::ThemePtr theme) :
        state(state), presets(presets), Themable(theme) {
        setTheme(theme);

        prevPresetButton.setImages(icons.caretLeftSolid.get(), nullptr, icons.caretLeftOutline.get());

        addAndMakeVisible(prevPresetButton);
    }

    Header::~Header() {}

    void Header::paint(juce::Graphics& g) {
        theme->drawRounderContainer(
            this, g, getLocalBounds().toFloat(), true, Metric::SMALL, Metric::TINY,
            UIColor::BORDER_MUTED, UIColor::BACKGROUND_MUTED
        );
    }

    void Header::resized() {
        innerArea = theme->getInnerArea(this, Metric::SMALL, Metric::SMALL);
        prevPresetButton.setBounds(
            (int) innerArea.getX(), (int) innerArea.getY(),
            (int) innerArea.getHeight(), (int) innerArea.getHeight()
        );
    }
    void Header::setTheme(Theme::ThemePtr v) noexcept {
        Themable::setTheme(v);
        resized();
        repaint();
    }
}