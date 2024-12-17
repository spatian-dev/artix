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
	class PicoDark : public BaseTheme {
		public:
		PicoDark() : BaseTheme("Pico (Dark)") {
			fontSize = {0, 8, 12, 16, 18, 22, 26};
			spacing = {0, 2, 4, 8, 12, 16, 20};
			rounding = {0, 2, 4, 8, 12, 18, 24};
			thickness = {0, 2, 1, 2, 3, 5, 8};
			colorScheme = {
				true,						// isDark
				juce::Colour(0x0),			// transparent

				juce::Colour(0xff13171f),	// BACKGROUND
				juce::Colour(0xff181c25),	// BACKGROUND_MUTED
				juce::Colour(0xff0172ad),	// BACKGROUND_PRIMARY
				juce::Colour(0xff525f7a),	// BACKGROUND_SECONDARY
				juce::Colour(0xffeff1f4),	// BACKGROUND_CONTRAST
				juce::Colour(0xff1c212c),	// BACKGROUND_ELEMENT				

				juce::Colour(0xffc2c7d0),	// TEXT
				juce::Colour(0xff7b8495),	// TEXT_MUTED
				juce::Colour(0xff01aaff),	// TEXT_PRIMARY
				juce::Colour(0xff969eaf),	// TEXT_SECONDARY
				juce::Colour(0xffdfe3eb),	// TEXT_CONTRAST
				juce::Colour(0xffe0e3e7),	// TEXT_ELEMENT
				juce::Colour(0xff8891a4),	// TEXT_ELEMENT_MUTED

				juce::Colour(0xff181c25),	// BORDER
				juce::Colour(0xff202632),	// BORDER_MUTED
				juce::Colour(0xff0172ad),	// BORDER_PRIMARY
				juce::Colour(0xff525f7a),	// BORDER_SECONDARY
				juce::Colour(0xffeff1f4),	// BORDER_CONTRAST
				juce::Colour(0xff2a3140),	// BORDER_ELEMENT
			};
		};

		~PicoDark() override = default;
	};
}