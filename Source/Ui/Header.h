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

#include "../PluginProcessor.h"
#include "../AppState.h"
#include "../Midi/Presets.h"
#include "Component/Button.h"
#include "Theme/Themes.h"
#include "../Utils/UTF8.h"

namespace Artix::Ui {
    class Header : public juce::Component, private Theme::Themable {
        public:
        Header(ArtixAudioProcessor& proc, AppState& state, Midi::Presets& presets, Theme::ThemePtr theme);
        ~Header() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        void setTheme(Theme::ThemePtr v) noexcept override;

        private:
        using Button = Ui::Component::Button;
        
        ArtixAudioProcessor& audioProcessor;

        juce::Rectangle<float> innerArea;
        juce::Rectangle<int> textArea;
        Midi::Presets& presets;
        AppState& state;

        const Utils::UTF8& utf8 = Utils::UTF8::get();

        Button::ClickedCallback::Identifier prevPresetButtonClickedId;
        Button prevPresetButton = Ui::Component::Button(theme, utf8.leftIsoscelesRightTriangle);

        Button::ClickedCallback::Identifier nextPresetButtonClickedId;
        Button nextPresetButton = Ui::Component::Button(theme, utf8.rightIsoscelesRightTriangle);

        Button::ClickedCallback::Identifier saveButtonClickedId;
        Button saveButton = Ui::Component::Button(theme, "Save");

        Button::ClickedCallback::Identifier loadButtonClickedId;
        Button loadButton = Ui::Component::Button(theme, "Load");

        Button::ClickedCallback::Identifier dataFolderButtonClickedId;
        Button dataFolderButton = Ui::Component::Button(theme, "Folder");

        juce::Label presetName;

        void setPresetName(juce::String v);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
    };
}