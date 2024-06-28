/*
  ==============================================================================

    UTF8.h
    Created: 26 Jun 2024 12:54:49pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//#define JUCE_UTF8_CHARPOINTER(name, str) const auto #name = juce::CharPointer_UTF8("\xe2\x80\x94");

#define DECLARE_JUCE_UTF8_CHARPOINT(name) extern const juce::CharPointer_UTF8 name

namespace Artix::Utils::UTF8 {
    DECLARE_JUCE_UTF8_CHARPOINT(EmDash);
}