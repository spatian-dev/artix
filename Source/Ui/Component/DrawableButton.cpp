/*
  ==============================================================================

    DrawableButton.cpp
    Created: 17 Dec 2024 3:47:49am
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "DrawableButton.h"

namespace Artix::Ui::Component {
	DrawableButton::DrawableButton(
		Theme::ThemePtr theme, juce::String text, juce::DrawableButton::ButtonStyle style
	): Themable(theme), juce::DrawableButton("", style) {
		setTheme(theme);
	}
	
	/*Metric DrawableButton::getFontSize() const noexcept {
		return fontSize;
	}

	void DrawableButton::setFontSize(Metric v) noexcept {
		fontSize = v;
	}

	juce::Justification DrawableButton::getJustification() const noexcept {
		return justification;
	}

	void DrawableButton::setJustification(juce::Justification v) noexcept {
		justification = v;
	}

	juce::Colour DrawableButton::getTextColor() const noexcept {
		return textColor;
	}

	void DrawableButton::setTextColor(juce::Colour v, bool deriveScheme) noexcept {
		textColor = v;

		if (deriveScheme) {
			hoverColor = theme->isDark() ? textColor.brighter(0.6f) : textColor.darker(0.6f);
		}
	}

	juce::Colour DrawableButton::getHoverColor() const noexcept {
		return hoverColor;
	}

	void DrawableButton::setHoverColor(juce::Colour v) noexcept {
		hoverColor = v;
	}*/

	void DrawableButton::paint(juce::Graphics& g) {
		/*g.setFont(juce::FontOptions(theme->getFontSize(fontSize), juce::Font::underlined));
		g.setColour(isMouseOver() ? hoverColor : textColor);
		g.drawFittedText(text, getLocalBounds(), justification, 1);*/
	}
	void DrawableButton::setTheme(Theme::ThemePtr v) noexcept {
		Themable::setTheme(v);
		// setTextColor(theme->getUIColor(UIColor::TEXT_PRIMARY));
		resized();
		repaint();
	}
}