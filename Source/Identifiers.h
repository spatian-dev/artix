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
//#define DECLARE_ID(name) const juce::Identifier name (#name);
#define DECLARE_ID(name) const auto name  = juce::String(#name);

	DECLARE_ID(AppState)

	DECLARE_ID(Keyswitch)
	DECLARE_ID(InputChannel)
	DECLARE_ID(OutputChannel)

#undef DECLARE_ID
}