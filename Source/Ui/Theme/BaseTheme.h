/*
  ==============================================================================

	BaseTheme.h
	Created: 30 Jun 2024 7:55:34pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cstdint>
#include <utility>

#include "../Font/Fonts.h"

namespace Artix::Ui {
	enum class Metric : uint8_t {
		TINY,
		SMALL,
		MEDIUM,
		LARGE,
		HUGE,
	};

	enum class UIColor : uint16_t {
		TRANSPARENT,

		BACKGROUND,
		BACKGROUND_MUTED,
		BACKGROUND_PRIMARY,
		BACKGROUND_SECONDARY,
		BACKGROUND_CONTRAST,
		BACKGROUND_ELEMENT,

		TEXT,
		TEXT_MUTED,
		TEXT_PRIMARY,
		TEXT_SECONDARY,
		TEXT_CONTRAST,
		TEXT_ELEMENT,
		TEXT_ELEMENT_MUTED,

		BORDER,
		BORDER_MUTED,
		BORDER_PRIMARY,
		BORDER_SECONDARY,
		BORDER_CONTRAST,
		BORDER_ELEMENT,
	};

	namespace Theme {
		class BaseTheme {
			public:
			BaseTheme(const juce::String name);
			virtual ~BaseTheme() = default;

			protected:
			using MetricValues = std::array<float, 5>;
			struct FontSize : MetricValues {};
			struct Spacing : MetricValues {};
			struct Rounding : MetricValues {};
			struct Thickness : MetricValues {};
			struct ColorScheme {
				bool isDark;
				std::array<juce::Colour, 20> colors;
			};

			juce::String name;
			FontSize fontSize;
			Spacing spacing;
			Rounding rounding;
			Thickness thickness;
			ColorScheme colorScheme;

			float scaler = 1;
			bool scaleFontSize = false;
			bool scaleSpacing = false;
			bool scaleRounding = false;
			bool scaleThickness = false;

			public:
			juce::Rectangle<float> getInnerArea(
				juce::Component* component, Metric borderThickness = Metric::MEDIUM,
				Metric padding = Metric::MEDIUM
			) const noexcept;

			virtual void fillBackground(
				juce::Component* component, juce::Graphics& g, UIColor backgroundColor = UIColor::BACKGROUND
			);

			virtual void drawContainer(
				juce::Component* component, juce::Graphics& g, juce::Rectangle<float> area, bool refill,
				float borderThickness, float borderRounding, juce::Colour borderColor, juce::Colour background
			);

			virtual void drawContainer(
				juce::Component* component, juce::Graphics& g, juce::Rectangle<float> area, bool refill = false,
				Metric borderThickness = Metric::SMALL, UIColor borderColor = UIColor::BORDER,
				UIColor backgroundColor = UIColor::BACKGROUND
			);

			virtual void drawRounderContainer(
				juce::Component* component, juce::Graphics& g, juce::Rectangle<float> area, bool refill = false,
				Metric borderThickness = Metric::SMALL, Metric borderRounding = Metric::SMALL,
				UIColor borderColor = UIColor::BORDER, UIColor backgroundColor = UIColor::BACKGROUND
			);
						
			virtual const juce::String& getName() const noexcept;

			virtual const juce::Typeface::Ptr getSansSerifTypeface();
			virtual const juce::Font getSansSerifFont(
				float size, int styleFlags = juce::Font::FontStyleFlags::plain
			);
			virtual const juce::Font getSansSerifFont(
				Metric size, int styleFlags = juce::Font::FontStyleFlags::plain
			);
			virtual const juce::Font getSansSerifFont();

			virtual const juce::Typeface::Ptr getMonospaceTypeface();
			virtual const juce::Font getMonospaceFont(
				float size, int styleFlags = juce::Font::FontStyleFlags::plain
			);
			virtual const juce::Font getMonospaceFont(
				Metric size, int styleFlags = juce::Font::FontStyleFlags::plain
			);
			virtual const juce::Font getMonospaceFont();

			virtual float getFontSize(Metric size) const noexcept;
			virtual float getSpacing(Metric size) const noexcept;
			virtual float getRounding(Metric size) const noexcept;
			virtual float getThickness(Metric size) const noexcept;
			virtual const juce::Colour getUIColor(UIColor uiColor) const noexcept;
			virtual bool isDark() const noexcept;

			virtual float getScaler() const noexcept;
			virtual void setScaler(float v) noexcept;
			virtual float scale(float v) const noexcept;

			virtual bool getScaledFontSize() const noexcept;
			virtual void setScaledFontSize(bool v) noexcept;

			virtual bool getScaledSpacing() const noexcept;
			virtual void setScaledSpacing(bool v) noexcept;

			virtual bool getScaledRounding() const noexcept;
			virtual void setScaledRounding(bool v) noexcept;

			virtual bool getScaledThickness() const noexcept;
			virtual void setScaledThickness(bool v) noexcept;

			private:
			JUCE_LEAK_DETECTOR(BaseTheme)
		};
	}
}