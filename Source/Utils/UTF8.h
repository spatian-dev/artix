/*
  ==============================================================================

    UTF8.h
    Created: 26 Jun 2024 12:54:49pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Artix::Utils {
    class UTF8 {
        public:
        static const UTF8& get() {
            static UTF8 instance;
            return instance;
        }

        const juce::CharPointer_UTF8 EmDash = juce::CharPointer_UTF8("\xe2\x80\x94");

        const juce::CharPointer_UTF8 leftEquilateralArrowhead = juce::CharPointer_UTF8("\xf0\x9f\x9e\x80");
        const juce::CharPointer_UTF8 upEquilateralArrowhead = juce::CharPointer_UTF8("\xf0\x9f\x9e\x81");
        const juce::CharPointer_UTF8 rightEquilateralArrowhead = juce::CharPointer_UTF8("\xf0\x9f\x9e\x82");
        const juce::CharPointer_UTF8 downEquilateralArrowhead = juce::CharPointer_UTF8("\xf0\x9f\x9e\x83");

        const juce::CharPointer_UTF8 leftIsoscelesRightTriangle = juce::CharPointer_UTF8("\xf0\x9f\x9e\x80");
        const juce::CharPointer_UTF8 upIsoscelesRightTriangle = juce::CharPointer_UTF8("\xf0\x9f\x9e\x81");
        const juce::CharPointer_UTF8 rightIsoscelesRightTriangle = juce::CharPointer_UTF8("\xf0\x9f\x9e\x82");
        const juce::CharPointer_UTF8 downIsoscelesRightTriangle = juce::CharPointer_UTF8("\xf0\x9f\x9e\x83");

        const juce::CharPointer_UTF8 folderClosedEmoji = juce::CharPointer_UTF8("\xf0\x9f\x93\x81");
        const juce::CharPointer_UTF8 folderOpenEmoji = juce::CharPointer_UTF8("\xf0\x9f\x93\x82");
        const juce::CharPointer_UTF8 floppyDiskEmoji = juce::CharPointer_UTF8("\xf0\x9f\x92\xbe");

        const juce::CharPointer_UTF8 f = juce::CharPointer_UTF8("\xf0\x9f\x92\xbe");
        private:
        UTF8() = default;
        ~UTF8() = default;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UTF8)
    };
}