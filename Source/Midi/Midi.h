/*
  ==============================================================================

    Midi.h
    Created: 24 Jun 2024 7:51:12pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <cstdint>

namespace Artix::Midi {
	namespace Notes {
		const int8_t NONE = -1;

		const int8_t C5 = 83;
	}
}

#define jassertValidMidiChannel(ch) jassert((ch >= 1) && (ch <= 16))
#define jassertValidMidiNote(note) jassert((note>= 0) && (note<= 127))
#define jassertValidMidiNoteOrNone(note) jassert((note>= Artix::Midi::Notes::NONE) && (note<= 127))