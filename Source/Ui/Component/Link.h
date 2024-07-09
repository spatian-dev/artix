/*
  ==============================================================================

    Link.h
    Created: 5 Jul 2024 11:57:12am
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Theme/Themes.h"

namespace Artix::Ui::Component {
    class Link : public juce::Component, private Theme::Themable {
        public:
        Link(Theme::ThemePtr theme, const juce::String link, const juce::String text);
        Link(Theme::ThemePtr theme, const juce::String link);
        virtual ~Link() override = default;

        virtual Metric getFontSize() const noexcept;
        virtual void setFontSize(Metric v) noexcept;

        virtual juce::Justification getJustification() const noexcept;
        virtual void setJustification(juce::Justification v) noexcept;
        
        virtual juce::Colour getTextColor() const noexcept;
        virtual void setTextColor(juce::Colour v, bool deriveScheme = true) noexcept;

        virtual juce::Colour getHoverColor() const noexcept;
        virtual void setHoverColor(juce::Colour v) noexcept;

        virtual const juce::String& getLink() const noexcept;
        virtual void setLink(juce::String v, bool updateText = true) noexcept;

        virtual const juce::String& getText() const noexcept;
        virtual void setText(juce::String v) noexcept;

        void mouseUp(const juce::MouseEvent& event) override;
        void mouseEnter(const juce::MouseEvent& event) override;
        void mouseExit(const juce::MouseEvent& event) override;

        void paint(juce::Graphics&) override;

        void setTheme(Theme::ThemePtr v) noexcept override;

        private:
        Metric fontSize = Metric::MEDIUM;
        juce::Justification justification = juce::Justification::centredLeft;
        juce::Colour textColor;
        juce::Colour hoverColor;
        
        juce::String link;
        juce::String text;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Link)
    };
}