/*
  ==============================================================================

    Header.h
    Created: 1 Jul 2024 3:43:46pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <memory>
#include <JuceHeader.h>

#include "../AppState.h"
#include "../Midi/Presets.h"
#include "Theme/Themes.h"
#include "Icons/Icons.h"

namespace Artix::Ui {
    class Header : public juce::Component, private Theme::Themable {
        public:
        Header(AppState& state, Midi::Presets& presets, Theme::ThemePtr theme);
        ~Header() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        void setTheme(Theme::ThemePtr v) noexcept override;

        private:
        juce::Rectangle<float> innerArea;
        Midi::Presets& presets;
        AppState& state;

        const Icons& icons = Icons::get();

        juce::DrawableButton prevPresetButton = juce::DrawableButton(
            "", juce::DrawableButton::ButtonStyle::ImageFitted
        );
        
        /*
        juce::DrawableButton prevPresetButton;
        juce::DrawableButton nextPresetButton;
        juce::DrawableButton saveButton;
        juce::DrawableButton loadButton;
        juce::DrawableButton dataFolderButton;
        */

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
    };
}