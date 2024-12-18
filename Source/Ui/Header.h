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

#include "../State.h"
#include "../Midi/Presets.h"
#include "Component/Button.h"
#include "Component/Label.h"
#include "Theme/Themes.h"
#include "../Utils/UTF8.h"
#include "../Settings.h"

namespace Artix::Ui {
    class Header : public juce::Component, private Theme::Themable {
        public:
        Header(State& state, Midi::Presets& presets, Theme::ThemePtr theme, Settings& settings);
        ~Header() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        void setTheme(Theme::ThemePtr v) noexcept override;

        private:
        using Button = Ui::Component::Button;
        using Label = Ui::Component::Label;

        Settings& settings;
        juce::Rectangle<float> innerArea;
        juce::Rectangle<int> textArea;
        Midi::Presets& presets;
        State& state;

        const Utils::UTF8& utf8 = Utils::UTF8::get();

        Button::ClickedCallback::Identifier prevPresetButtonClickedId;
        Button prevPresetButton = Ui::Component::Button(theme, utf8.leftIsoscelesRightTriangle);

        Button::ClickedCallback::Identifier nextPresetButtonClickedId;
        Button nextPresetButton = Ui::Component::Button(theme, utf8.rightIsoscelesRightTriangle);

        Button::ClickedCallback::Identifier saveButtonClickedId;
        Button saveButton = Ui::Component::Button(theme, "Save");

        Button::ClickedCallback::Identifier loadButtonClickedId;
        Button loadButton = Ui::Component::Button(theme, "Load");

        Button::ClickedCallback::Identifier settingsButtonClickedId;
        Button settingsButton = Ui::Component::Button(theme, "Settings");

        Label presetLabel;
        Label::ClickedCallback::Identifier presetLabelClickedId;

        State::DirtyChangedCallback::Identifier stateDirtyChangedId;

        juce::PopupMenu presetsMenu();
        juce::PopupMenu settingsMenu();

        void switchPreset(const Midi::PresetPtr preset);
        void setPresetName(juce::String v);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
    };
}