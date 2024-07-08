/*
  ==============================================================================

    Header.h
    Created: 1 Jul 2024 3:43:46pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Theme/BaseTheme.h"

namespace Artix::Ui {
    class Header : public juce::Component {
        public:
        Header(Theme::BaseTheme& theme);
        ~Header() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        private:
        Theme::BaseTheme& theme;

        juce::Rectangle<float> innerArea;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
    };
}