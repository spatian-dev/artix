/*
  ==============================================================================

    DrawableButton.h
    Created: 17 Dec 2024 3:47:49am
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Theme/Themes.h"

namespace Artix::Ui::Component {
    class DrawableButton: public juce::DrawableButton, private Theme::Themable {
        public:
        DrawableButton(Theme::ThemePtr theme, juce::String text, juce::DrawableButton::ButtonStyle style);
        virtual ~DrawableButton() override = default;

        /*virtual Metric getFontSize() const noexcept;
        virtual void setFontSize(Metric v) noexcept;

        virtual juce::Justification getJustification() const noexcept;
        virtual void setJustification(juce::Justification v) noexcept;

        virtual juce::Colour getTextColor() const noexcept;
        virtual void setTextColor(juce::Colour v, bool deriveScheme = true) noexcept;

        virtual juce::Colour getHoverColor() const noexcept;
        virtual void setHoverColor(juce::Colour v) noexcept;*/

        void paint(juce::Graphics&) override;

        void setTheme(Theme::ThemePtr v) noexcept override;

        private:
        /*Metric fontSize = Metric::MEDIUM;
        juce::Justification justification = juce::Justification::centredLeft;
        juce::Colour textColor;
        juce::Colour hoverColor;*/

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawableButton)
    };
}