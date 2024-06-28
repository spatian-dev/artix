/*
  ==============================================================================

	UTF8.cpp
	Created: 26 Jun 2024 7:28:43pm
	Author:  nerax

  ==============================================================================
*/

#include "UTF8.h"

#define DEFINE_JUCE_UTF8_CHARPOINT(name, val)\
    const juce::CharPointer_UTF8 name = juce::CharPointer_UTF8(val)

namespace Artix::Utils::UTF8 {
	DEFINE_JUCE_UTF8_CHARPOINT(EmDash, "\xe2\x80\x94");
}