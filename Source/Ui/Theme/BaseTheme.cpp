/*
  ==============================================================================

	BaseTheme.cpp
	Created: 30 Jun 2024 7:55:34pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "BaseTheme.h"

namespace Artix::Ui::Theme {
	BaseTheme::BaseTheme(const juce::String name) : name(name) {}

	juce::Rectangle<float> BaseTheme::getInnerArea(juce::Component* component, Metric borderThickness, Metric padding) const noexcept {
		auto area = component->getLocalBounds();
		auto const offset = (int) ((getThickness(borderThickness) / 2) + getSpacing(padding));
		return area.reduced(offset, offset).toFloat();
	}

	void BaseTheme::fillBackground(juce::Component* component, juce::Graphics& g, UIColor backgroundColor) {
		g.fillAll(getUIColor(backgroundColor));
	}

	void BaseTheme::drawContainer(
		juce::Component* component, juce::Graphics& g, juce::Rectangle<float> area, bool refill,
		float borderThickness, float borderRounding, juce::Colour borderColor, juce::Colour backgroundColor
	) {
		if (refill) {
			g.setColour(backgroundColor);
			g.fillRoundedRectangle(area, borderRounding);
		}
		g.setColour(borderColor);
		g.drawRoundedRectangle(area.reduced(borderThickness / 2), borderRounding, borderThickness);
	}

	void BaseTheme::drawContainer(
		juce::Component* component, juce::Graphics& g, juce::Rectangle<float> area, bool refill,
		Metric borderThickness, UIColor borderColor, UIColor backgroundColor
	) {
		drawContainer(
			component, g, area, refill, getThickness(borderThickness), 0, getUIColor(borderColor),
			getUIColor(backgroundColor)
		);
	}

	void BaseTheme::drawRounderContainer(
		juce::Component* component, juce::Graphics& g, juce::Rectangle<float> area, bool refill,
		Metric borderThickness, Metric borderRounding, UIColor borderColor, UIColor backgroundColor
	) {
		drawContainer(
			component, g, area, refill, getThickness(borderThickness), getRounding(borderRounding),
			getUIColor(borderColor), getUIColor(backgroundColor)
		);
	}

	bool BaseTheme::isDark() const noexcept {
		return colorScheme.isDark;
	}

	const juce::String& BaseTheme::getName() const noexcept {
		return name;
	}

	const juce::Typeface::Ptr BaseTheme::getSansSerifTypeface() {
		return fonts.sansSerifTypeface;
	}
	const juce::Font BaseTheme::getSansSerifFont(float size, int styleFlags) {
		return getSansSerifFont().withHeight(size).withStyle(styleFlags);
	}
	const juce::Font BaseTheme::getSansSerifFont(Metric size, int styleFlags) {
		return getSansSerifFont(getFontSize(size), styleFlags);
	}
	const juce::Font BaseTheme::getSansSerifFont() {
		return juce::FontOptions(fonts.sansSerifTypeface);
	}

	const juce::Typeface::Ptr BaseTheme::getMonospaceTypeface() {
		return fonts.monospacedTypeface;
	}
	const juce::Font BaseTheme::getMonospaceFont(float size, int styleFlags) {
		return getMonospaceFont().withHeight(size).withStyle(styleFlags);
	}
	const juce::Font BaseTheme::getMonospaceFont(Metric size, int styleFlags) {
		return getMonospaceFont(getFontSize(size), styleFlags);
	}
	const juce::Font BaseTheme::getMonospaceFont() {
		return juce::FontOptions(fonts.monospacedTypeface);
	}

	float BaseTheme::getFontSize(Metric size) const noexcept {
		return fontSize[static_cast<uint8_t>(size)] * (scaleFontSize ? scaler : 1);
	}

	float BaseTheme::getSpacing(Metric size) const noexcept {
		return spacing[static_cast<uint8_t>(size)] * (scaleSpacing ? scaler : 1);
	}

	float BaseTheme::getRounding(Metric size) const noexcept {
		return rounding[static_cast<uint8_t>(size)] * (scaleRounding ? scaler : 1);
	}

	float BaseTheme::getThickness(Metric size) const noexcept {
		return thickness[static_cast<uint8_t>(size)] * (scaleThickness ? scaler : 1);
	}

	const juce::Colour BaseTheme::getUIColor(UIColor uiColor) const noexcept {
		return colorScheme.colors[static_cast<uint16_t>(uiColor)];
	}

	float BaseTheme::getScaler() const noexcept {
		return scaler;
	}
	void BaseTheme::setScaler(float v) noexcept {
		scaler = v;
	}

	float BaseTheme::scale(float v) const noexcept {
		return v * scaler;
	}

	bool BaseTheme::getScaledFontSize() const noexcept {
		return scaleFontSize;
	}
	void BaseTheme::setScaledFontSize(bool v) noexcept {
		scaleFontSize = v;
	}

	bool BaseTheme::getScaledSpacing() const noexcept {
		return scaleSpacing;
	}
	void BaseTheme::setScaledSpacing(bool v) noexcept {
		scaleSpacing = v;
	}

	bool BaseTheme::getScaledRounding() const noexcept {
		return scaleRounding;
	}
	void BaseTheme::setScaledRounding(bool v) noexcept {
		scaleRounding = v;
	}

	bool BaseTheme::getScaledThickness() const noexcept {
		return scaleThickness;
	}
	void BaseTheme::setScaledThickness(bool v) noexcept {
		scaleThickness = v;
	}
}