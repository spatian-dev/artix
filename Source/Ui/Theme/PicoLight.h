/*
  ==============================================================================

	DarkTheme.h
	Created: 30 Jun 2024 9:21:45pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include "BaseTheme.h"

namespace Artix::Ui::Theme {
	class PicoLight : public BaseTheme {
		public:
		PicoLight() : BaseTheme("Pico (Light)") {
			fontSize = {12, 16, 18, 22, 26};
			spacing = {4, 8, 12, 16, 20};
			rounding = {4, 8, 12, 18, 24};
			thickness = {1, 2, 3, 5, 8};
			colorScheme = {
				false,						// isDark
				juce::Colour(0x0),			// transparent

				juce::Colour(0xffffffff),	// BACKGROUND
				juce::Colour(0xfffbfbfb),	// BACKGROUND_MUTED
				juce::Colour(0xff0172ad),	// BACKGROUND_PRIMARY
				juce::Colour(0xff525f7a),	// BACKGROUND_SECONDARY
				juce::Colour(0xff181c25),	// BACKGROUND_CONTRAST
				juce::Colour(0xfff6f6f6),	// BACKGROUND_ELEMENT				

				juce::Colour(0xff373c44),	// TEXT
				juce::Colour(0xff7b8495),	// TEXT_MUTED
				juce::Colour(0xff0172ad),	// TEXT_PRIMARY
				juce::Colour(0xff5d6b89),	// TEXT_SECONDARY
				juce::Colour(0xff181c25),	// TEXT_CONTRAST
				juce::Colour(0xff23262c),	// TEXT_ELEMENT
				juce::Colour(0xff646b79),	// TEXT_ELEMENT_MUTED

				juce::Colour(0xffe7eaf0),	// BORDER
				juce::Colour(0xffe7eaf0),	// BORDER_MUTED
				juce::Colour(0xff0172ad),	// BORDER_PRIMARY
				juce::Colour(0xff525f7a),	// BORDER_SECONDARY
				juce::Colour(0xff181c25),	// BORDER_CONTRAST
				juce::Colour(0xffcfd5e2),	// BORDER_ELEMENT
			};
		};

		~PicoLight() override = default;
	};
}