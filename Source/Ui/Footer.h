/*
  ==============================================================================

    Footer.h
    Created: 1 Jul 2024 12:34:36am
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <sstream>
#include "Theme/BaseTheme.h"
#include "Component/Link.h"

namespace Artix::Ui {
    class Footer : public juce::Component {
        public:
        Footer(Theme::BaseTheme& theme);
        ~Footer() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        private:
        Theme::BaseTheme& theme;
        
        juce::Label versionLabel;
        Ui::Component::Link websiteLink{theme, JucePlugin_ManufacturerWebsite};

        juce::Rectangle<float> innerArea;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Footer)
    };
}