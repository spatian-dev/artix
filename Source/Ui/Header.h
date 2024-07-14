/*
  ==============================================================================

    Header.h
    Created: 1 Jul 2024 3:43:46pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Theme/Themes.h"

namespace Artix::Ui {
    class Header : public juce::Component, private Theme::Themable {
        public:
        Header(Theme::ThemePtr theme);
        ~Header() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        void setTheme(Theme::ThemePtr v) noexcept override;

        private:
        juce::Rectangle<float> innerArea;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
    };
}