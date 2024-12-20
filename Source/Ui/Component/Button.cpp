/*
  ==============================================================================

    Button.cpp
    Created: 17 Dec 2024 3:47:49am
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "Button.h"

namespace Artix::Ui::Component {
    Button::Button(Theme::ThemePtr theme, juce::String text): Themable(theme), text(text) {
        setTheme(theme);
        setText(text);
    }

    Button::AutoSize Button::getAutoSize() const noexcept {
        return autoSize;
    }

    void Button::setAutoSize(Button::AutoSize v) noexcept {
        autoSize = v;
        resized();
    }

    juce::String Button::getText() const noexcept {
        return text;
    }

    void Button::setText(juce::String v) noexcept {
        text = v;
        resized();
    }

    Metric Button::getFontSize() const noexcept {
        return fontSize;
    }

    void Button::setFontSize(Metric v) noexcept {
        fontSize = v;
        resized();
    }

    juce::Justification Button::getJustification() const noexcept {
        return justification;
    }

    void Button::setJustification(juce::Justification v) noexcept {
        justification = v;
    }

    juce::Colour Button::getButtonColor() const noexcept {
        return buttonColor;
    }

    void Button::setButtonColor(juce::Colour v, bool deriveScheme) noexcept {
        buttonColor = v;

        if (deriveScheme) {
            buttonHoverColor = theme->isDark() ? buttonColor.brighter(0.125f) : buttonColor.darker(0.125f);
            buttonDownColor = theme->isDark() ? buttonColor.darker(0.125f) : buttonColor.brighter(0.125f);
        }
    }

    juce::Colour Button::getButtonHoverColor() const noexcept {
        return buttonHoverColor;
    }

    void Button::setButtonHoverColor(juce::Colour v) noexcept {
        buttonHoverColor = v;
    }

    juce::Colour Button::getButtonDownColor() const noexcept {
        return buttonDownColor;
    }

    void Button::setButtonDownColor(juce::Colour v) noexcept {
        buttonDownColor = v;
    }

    juce::Colour Button::getTextColor() const noexcept {
        return textColor;
    }

    void Button::setTextColor(juce::Colour v, bool deriveScheme) noexcept {
        textColor = v;

        if (deriveScheme) {
            textHoverColor = theme->isDark() ? textColor.brighter(0.6f) : textColor.darker(0.6f);
            textDownColor = theme->isDark() ? textColor.darker(0.3f) : textColor.brighter(0.3f);
        }
    }

    juce::Colour Button::getTextHoverColor() const noexcept {
        return textHoverColor;
    }

    void Button::setTextHoverColor(juce::Colour v) noexcept {
        textHoverColor = v;
    }

    juce::Colour Button::getTextDownColor() const noexcept {
        return buttonDownColor;
    }

    void Button::setTextDownColor(juce::Colour v) noexcept {
        buttonDownColor = v;
    }

    void Button::mouseDown(const juce::MouseEvent&) {
        state = State::DOWN;
        repaint();
    }

    void Button::mouseUp(const juce::MouseEvent& event) {
        if (state == State::DOWN) {
            this->onClick.callSafely(event);
        }
        state = /*isMouseOver() ? State::HOVER :*/ State::NORMAL;
        repaint();
    }

    void Button::mouseEnter(const juce::MouseEvent&) {
        state = isMouseButtonDown() ? State::DOWN : State::HOVER;
        repaint();
    }

    void Button::mouseExit(const juce::MouseEvent&) {
        state = State::NORMAL;
        repaint();
    }

    void Button::resized() {
        if (autoSize && text.isNotEmpty()) {
            const auto font = theme->getSansSerifFont(fontSize);
            
            auto minimumSafeHeight = this->getHeight();
            if ((autoSize & AutoSize::AutoSizeHeight) == AutoSize::AutoSizeHeight) {
                minimumSafeHeight = (int) std::ceil(
                    font.getHeight() + (theme->getSpacing(Metric::TINY) * 2)
                );
            }
            
            auto minimumSafeWidth = this->getWidth();
            if ((autoSize & AutoSize::AutoSizeWidth) == AutoSize::AutoSizeWidth) {
                minimumSafeWidth = (int) std::ceil(
                    juce::GlyphArrangement::getStringWidth(font, text) + (theme->getSpacing(Metric::TINY) * 2)
                );
            }

            this->setSize(minimumSafeWidth, minimumSafeHeight);
        }

        innerArea = theme->getInnerArea(this, Metric::TINY, Metric::TINY).toNearestIntEdges();
    }

    void Button::paint(juce::Graphics& g) {
        auto btnColor = buttonColor;
        auto txtColor = textColor;
        switch (state) {
            case Artix::Ui::Component::Button::HOVER:
                txtColor = textHoverColor;
                btnColor = buttonHoverColor;
                break;

            case Artix::Ui::Component::Button::DOWN:
                txtColor = textDownColor;
                btnColor = buttonDownColor;
                break;

            default:
                break;
        }

        g.setColour(btnColor);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), theme->getRounding(Metric::TINY));

        g.setFont(juce::FontOptions(theme->getFontSize(fontSize), juce::Font::plain));
        g.setColour(txtColor);
        g.drawFittedText(text, innerArea, justification, 1);
    }
    void Button::setTheme(Theme::ThemePtr v) noexcept {
        Themable::setTheme(v);
        const auto bg = theme->getUIColor(UIColor::BACKGROUND_SECONDARY);
        setButtonColor(bg);
        setTextColor(bg.contrasting(0.6f));
        resized();
        repaint();
    }
}