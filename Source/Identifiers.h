/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 21 Jun 2024 08:29:56pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Artix::Id {
#define DEFINE_JUCE_ID(name)\
	extern const juce::Identifier name;\
	namespace Str { constexpr auto name = #name;}

	DEFINE_JUCE_ID(AppState)
	DEFINE_JUCE_ID(Width)
	DEFINE_JUCE_ID(Height)

	DEFINE_JUCE_ID(MidiChannelMapper)
	DEFINE_JUCE_ID(MidiChannelMapperBank)

	DEFINE_JUCE_ID(Name)
	DEFINE_JUCE_ID(Note)
	DEFINE_JUCE_ID(InputChannel)
	DEFINE_JUCE_ID(OutputChannel)

#undef DEFINE_JUCE_ID
}