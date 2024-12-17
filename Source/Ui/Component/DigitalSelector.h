/*
  ==============================================================================

    DigitalSelector.h
    Created: 5 Jul 2024 9:56:21pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "../Theme/Themes.h"
#include "../../Utils/CallbackList.h"

namespace Artix::Ui::Component {

    template <typename T> requires std::is_integral_v<T>
	class DigitalSelector :
		public juce::Component,
		private juce::Timer,
		private Theme::Themable
	{
		public:
		using CustomFormatterCallback = std::function<const juce::String(T, T, T)>;
		using ValueChangedCallback = Utils::CallbackList<T>;

		DigitalSelector(Theme::ThemePtr theme, T initialValue, juce::Colour baseColor)
			: Themable(theme), value(initialValue) {
			placeholder = std::string(maxChars, '-');
			setTheme(theme);
			setBaseColor(baseColor);
			setValue(initialValue, true);
			resized();
		}

		DigitalSelector(Theme::ThemePtr theme, T initialValue = 0)
			: DigitalSelector(theme, initialValue, theme->getUIColor(UIColor::BACKGROUND_PRIMARY)) {}
		~DigitalSelector() override = default;

		ValueChangedCallback onValueChanged;

		inline T getMaxChars() const noexcept {
			return maxChars;
		}
		inline T getMaxDigits() const noexcept {
			return maxDigits;
		}

		inline T getMin() const noexcept {
			return min;
		}

		void setMin(T v) noexcept {
			min = v;
			if (max < min) max = min;
			setValue(getValue());
		}

		inline T getMax() const noexcept {
			return max;
		}

		void setMax(T v) noexcept {
			max = v;
			if (min > max) min = max;
			setValue(getValue());
		}

		inline T getValue() const noexcept {
			return value;
		}

		void setValue(T v, bool muteCallbacks = false) noexcept {
			const auto oldValue = value;

			value = std::clamp(v, min, max);
			dragValue = value;

			updateText();
			
			if ((oldValue == value) || muteCallbacks) return;
			onValueChanged.callSafely(v);
		}

		void setMinMax(T vMin, T vMax) noexcept {
			if (vMin > vMax) std::swap(vMin, vMax);
			min = vMin;
			max = vMax;
			setValue(getValue());
		}

		juce::Colour getBaseColor() const noexcept {
			return backgroundColor;
		}
		void setBaseColor(juce::Colour v) {
			backgroundColor = v;
			backgroundHoverColor = theme->isDark() ? v.brighter(0.125f) : v.darker(0.125f);
			borderColor = theme->isDark() ? v.darker(0.25f) : v.brighter(0.25f);
			textColor = v.contrasting(0.6f);
		};

		Metric getFontSize() const noexcept {
			return fontSize;
		}
		void setFontSize(Metric v) noexcept {
			fontSize = v;
			updateFont();
		}

		juce::Justification getJustification() const noexcept {
			return justification;
		}
		void setJustification(juce::Justification v) noexcept {
			justification = v;
		}

		float getMinimumSafeWidth() const noexcept {
			return minimumSafeWidth;
		}
		float getMinimumSafeHeight() const noexcept {
			return minimumSafeHeight;
		}

		CustomFormatterCallback getCustomFormatter() const {
			return customFormatter;
		}
		void setCustomFormatter(CustomFormatterCallback v) {
			customFormatter = v;
			updateText();
		}

		void paint(juce::Graphics& g) override {
			const auto bgColor = (isMouseOver() || isBeingDragged) ?
				backgroundHoverColor : backgroundColor;

			theme->drawContainer(
				this, g, getLocalBounds().toFloat(), true,
				theme->getThickness(borderThickness), theme->getRounding(rounding),
				borderColor, bgColor
			);

			g.setFont(theme->getMonospaceFont(fontSize));
			g.setColour(textColor);
			g.drawFittedText(
				text, innerArea, justification, 1
			);
		}

		void resized() override {
			innerArea = theme->getInnerArea(this, borderThickness, padding).toNearestIntEdges();
			updateFont();
		}

		void setTheme(Theme::ThemePtr v) noexcept override {
			Themable::setTheme(v);
			resized();
			repaint();
		}

		private:
		using TLimits = std::numeric_limits<T>;

		const T maxChars = TLimits::digits10 + (TLimits::is_signed ? 2 : 1);
		const T maxDigits = maxChars - (TLimits::is_signed ? 1 : 0);
		float minimumSafeWidth, minimumSafeHeight;
		std::string placeholder;

		juce::Rectangle<int> innerArea;

		juce::Colour backgroundColor;
		juce::Colour backgroundHoverColor;
		juce::Colour borderColor;
		juce::Colour textColor;

		Metric fontSize = Metric::MEDIUM;
		juce::Justification justification = juce::Justification::centred;

		juce::String text;

		const Metric borderThickness = Metric::SMALL;
		const Metric padding = Metric::SMALL;
		const Metric rounding = Metric::TINY;

		CustomFormatterCallback customFormatter;

		T value{0};
		T min{TLimits::min()};
		T max{TLimits::max()};

		const float dragVelocityLimit = 10.0;
		float dragVelocity = 0, dragValue = 0, dragScale = 96;
		bool isBeingDragged = false;

		void mouseEnter(const juce::MouseEvent&) override {
			setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
			repaint();
		}
		void mouseExit(const juce::MouseEvent&) override {
			setMouseCursor(juce::MouseCursor::NormalCursor);
			repaint();
		}

		void mouseDown(const juce::MouseEvent&) override {
			dragVelocity = 0;
			isBeingDragged = true;
			startTimer(50);
		}
		void mouseUp(const juce::MouseEvent&) override {
			stopTimer();
			isBeingDragged = false;
			setValue(static_cast<T>(std::clamp(dragValue, (float) TLimits::min(), (float) TLimits::max())));
		}

		void mouseDrag(const juce::MouseEvent& event) override {
			const auto scale = getApproximateScaleFactorForComponent(this) * dragScale;
			dragVelocity = std::clamp(
				-event.getDistanceFromDragStartY() / scale, -dragVelocityLimit, dragVelocityLimit
			);
		}

		void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails& wheel) override {
			if (isBeingDragged || (wheel.deltaY == 0))
				return;

			const int sign = ((wheel.deltaY > 0) ? 1 : -1) * (wheel.isReversed ? -1 : 1);
			
			T localValue = value;
			if ((localValue < TLimits::max()) && (sign > 0)) {
				localValue++;
			} else if ((localValue > TLimits::min()) && (sign < 0)) {
				localValue--;
			}

			setValue(localValue);
		}

		void timerCallback() override {
			dragValue = std::clamp(dragValue + dragVelocity, (float) min, (float) max);
			updateText();
		}

		void updateText() noexcept {
			const auto base = isBeingDragged ? static_cast<T>(std::floor(dragValue)) : value;

			if (customFormatter) {
				text = customFormatter(base, maxChars, maxDigits);
			} else if (base < 0) {
				text = '-' + juce::String(-base).paddedLeft('0', maxDigits);
			} else {
				text = juce::String(base).paddedLeft('0', maxChars);
			}
			repaint();
		}

		void updateFont() {
			const auto font = theme->getMonospaceFont(fontSize);
			minimumSafeHeight = std::ceil(font.getHeight() + (2 * innerArea.getY()));
			minimumSafeWidth = std::ceil(
				juce::GlyphArrangement::getStringWidth(font, placeholder) + (2 * innerArea.getX())
			);
		}

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DigitalSelector)
    };
}