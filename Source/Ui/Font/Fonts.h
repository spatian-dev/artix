/*
  ==============================================================================

    Fonts.h
    Created: 5 Jul 2024 9:30:28pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Artix::Ui {
    class Fonts {
        public:
        static const Fonts& get() {
            static Fonts instance;
            return instance;
        }

        const juce::Typeface::Ptr sansSerifTypeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::RobotoCondensedRegular_ttf, BinaryData::RobotoCondensedRegular_ttfSize
        );
        const juce::Typeface::Ptr monospacedTypeface = juce::Typeface::createSystemTypefaceFor(
            BinaryData::Share_Tech_Mono_ttf, BinaryData::Share_Tech_Mono_ttfSize
        );

        private:
        Fonts() = default;
        ~Fonts() = default;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Fonts)
    };
}