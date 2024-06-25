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
#define DECLARE_ID(name)\
	const juce::Identifier name(#name);\
	namespace Str { const auto name = #name;}

	DECLARE_ID(AppState)

	DECLARE_ID(Name)
	DECLARE_ID(MidiChannelMapper)
	DECLARE_ID(MidiChannelMapperBank)

	DECLARE_ID(Note)
	DECLARE_ID(InputChannel)
	DECLARE_ID(OutputChannel)

#undef DECLARE_ID
}