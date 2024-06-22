/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 21 Jun 2024 08:29:56pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Artix::ID {
#define DECLARE_ID(name) const juce::Identifier name (#name);

	DECLARE_ID(MidiChannelMapper)
	DECLARE_ID(MidiChannelMapperList)
	DECLARE_ID(InputChannelNumber)
	DECLARE_ID(OutputNoteNumber)

#undef DECLARE_ID
}