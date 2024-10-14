/*
  ==============================================================================

	Link.cpp
	Created: 5 Jul 2024 11:57:12am
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "Link.h"

namespace Artix::Ui::Component {
	Link::Link(Theme::ThemePtr theme, const juce::String link, const juce::String text)
		: Themable(theme), link(link), text(text) {
		setTheme(theme);
	}

	Link::Link(Theme::ThemePtr theme, const juce::String link) : Link(theme, link, link) {}

	Metric Link::getFontSize() const noexcept {
		return fontSize;
	}

	void Link::setFontSize(Metric v) noexcept {
		fontSize = v;
	}

	juce::Justification Link::getJustification() const noexcept {
		return justification;
	}

	void Link::setJustification(juce::Justification v) noexcept {
		justification = v;
	}

	juce::Colour Link::getTextColor() const noexcept {
		return textColor;
	}

	void Link::setTextColor(juce::Colour v, bool deriveScheme) noexcept {
		textColor = v;

		if (deriveScheme) {
			hoverColor = theme->isDark() ? textColor.brighter(0.6f) : textColor.darker(0.6f);
		}
	}

	juce::Colour Link::getHoverColor() const noexcept {
		return hoverColor;
	}

	void Link::setHoverColor(juce::Colour v) noexcept {
		hoverColor = v;
	}

	const juce::String& Link::getLink() const noexcept {
		return link;
	}

	void Link::setLink(juce::String v, bool updateText) noexcept {
		link = link;

		if (updateText) {
			setText(link);
		}
	}

	const juce::String& Link::getText() const noexcept {
		return text;
	}

	void Link::setText(juce::String v) noexcept {
		text = v;
	}

	void Link::mouseUp(const juce::MouseEvent& event) {
		const auto url = juce::URL::createWithoutParsing(link);
		url.launchInDefaultBrowser();
	}

	void Link::mouseEnter(const juce::MouseEvent& event) {
		setMouseCursor(juce::MouseCursor::PointingHandCursor);
		repaint();
	}

	void Link::mouseExit(const juce::MouseEvent& event) {
		setMouseCursor(juce::MouseCursor::NormalCursor);
		repaint();
	}

	void Link::paint(juce::Graphics& g) {
		g.setFont(juce::FontOptions(theme->getFontSize(fontSize), juce::Font::underlined));
		g.setColour(isMouseOver() ? hoverColor : textColor);
		g.drawFittedText(text, getLocalBounds(), justification, 1);
	}
	void Link::setTheme(Theme::ThemePtr v) noexcept {
		Themable::setTheme(v);
		setTextColor(theme->getUIColor(UIColor::TEXT_PRIMARY));
		resized();
		repaint();
	}
}