/*
  ==============================================================================

    Button.h
    Created: 17 Dec 2024 3:47:49am
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Theme/Themes.h"
#include "../../Utils/CallbackList.h"

namespace Artix::Ui::Component {
    class Button: public juce::Component, private Theme::Themable {
        public:
        using DrawablePtr = std::shared_ptr<juce::Drawable>;
        using ClickedCallback = Utils::CallbackList<juce::MouseEvent>;

        enum AutoSize {
            AutoSizeNone= 0,
            AutoSizeWidth = 1,
            AutoSizeHeight = 2,
            AutoSizeBoth = (AutoSizeWidth | AutoSizeHeight),
        };

        Button(Theme::ThemePtr theme, juce::String text = "");
        virtual ~Button() override = default;

        virtual AutoSize getAutoSize() const noexcept;
        virtual void setAutoSize(AutoSize v) noexcept;

        virtual juce::String getText() const noexcept;
        virtual void setText(juce::String v) noexcept;

        virtual Metric getFontSize() const noexcept;
        virtual void setFontSize(Metric v) noexcept;

        virtual juce::Justification getJustification() const noexcept;
        virtual void setJustification(juce::Justification v) noexcept;

        virtual juce::Colour getButtonColor() const noexcept;
        virtual void setButtonColor(juce::Colour v, bool deriveScheme = true) noexcept;

        virtual juce::Colour getButtonHoverColor() const noexcept;
        virtual void setButtonHoverColor(juce::Colour v) noexcept;

        virtual juce::Colour getButtonDownColor() const noexcept;
        virtual void setButtonDownColor(juce::Colour v) noexcept;

        virtual juce::Colour getTextColor() const noexcept;
        virtual void setTextColor(juce::Colour v, bool deriveScheme = true) noexcept;

        virtual juce::Colour getTextHoverColor() const noexcept;
        virtual void setTextHoverColor(juce::Colour v) noexcept;

        virtual juce::Colour getTextDownColor() const noexcept;
        virtual void setTextDownColor(juce::Colour v) noexcept;

        void mouseDown(const juce::MouseEvent& event) override;
        void mouseUp(const juce::MouseEvent& event) override;
        void mouseEnter(const juce::MouseEvent& event) override;
        void mouseExit(const juce::MouseEvent& event) override;

        void resized() override;
        void paint(juce::Graphics&) override;

        void setTheme(Theme::ThemePtr v) noexcept override;

        ClickedCallback onClick;

        private:
        enum State {
            NORMAL = 0,
            HOVER = 1,
            DOWN = 2,
        };

        AutoSize autoSize = AutoSize::AutoSizeNone;
        juce::Rectangle<int> innerArea;

        Metric fontSize = Metric::MEDIUM;
        juce::Justification justification = juce::Justification::centred;

        juce::Colour textColor;
        juce::Colour textHoverColor;
        juce::Colour textDownColor;
        juce::Colour buttonColor;
        juce::Colour buttonHoverColor;
        juce::Colour buttonDownColor;

        juce::String text;
        State state = State::NORMAL;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Button)
    };
}