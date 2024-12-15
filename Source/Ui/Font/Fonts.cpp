/*
  ==============================================================================

    Fonts.cpp
    Created: 5 Jul 2024 9:30:28pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "Fonts.h"

namespace Artix::Ui::Font {
    const juce::Typeface::Ptr sansSerifTypeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::RobotoCondensedRegular_ttf, BinaryData::RobotoCondensedRegular_ttfSize
    );
    const juce::Typeface::Ptr monospacedTypeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::Share_Tech_Mono_ttf, BinaryData::Share_Tech_Mono_ttfSize
    );
}