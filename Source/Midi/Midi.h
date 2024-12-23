/*
  ==============================================================================

	Midi.h
	Created: 24 Jun 2024 7:51:12pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <cstdint>
#include <algorithm>
#include <atomic>

namespace Artix::Midi {
	enum class Note : int8_t {
		Cn2 = 0,
		Cn2_Sharp = 1,
		Dn2_Flat = 1,
		Dn2 = 2,
		Dn2_Sharp = 3,
		En2_Flat = 3,
		En2 = 4,
		Fn2 = 5,
		Fn2_Sharp = 6,
		Gn2_Flat = 6,
		Gn2 = 7,
		Gn2_Sharp = 8,
		An2_Flat = 8,
		An2 = 9,
		An2_Sharp = 10,
		Bn2_Flat = 10,
		Bn2 = 11,
		Cn1 = 12,
		Cn1_Sharp = 13,
		Dn1_Flat = 13,
		Dn1 = 14,
		Dn1_Sharp = 15,
		En1_Flat = 15,
		En1 = 16,
		Fn1 = 17,
		Fn1_Sharp = 18,
		Gn1_Flat = 18,
		Gn1 = 19,
		Gn1_Sharp = 20,
		An1_Flat = 20,
		An1 = 21,
		An1_Sharp = 22,
		Bn1_Flat = 22,
		Bn1 = 23,
		C0 = 24,
		C0_Sharp = 25,
		D0_Flat = 25,
		D0 = 26,
		D0_Sharp = 27,
		E0_Flat = 27,
		E0 = 28,
		F0 = 29,
		F0_Sharp = 30,
		G0_Flat = 30,
		G0 = 31,
		G0_Sharp = 32,
		A0_Flat = 32,
		A0 = 33,
		A0_Sharp = 34,
		B0_Flat = 34,
		B0 = 35,
		C1 = 36,
		C1_Sharp = 37,
		D1_Flat = 37,
		D1 = 38,
		D1_Sharp = 39,
		E1_Flat = 39,
		E1 = 40,
		F1 = 41,
		F1_Sharp = 42,
		G1_Flat = 42,
		G1 = 43,
		G1_Sharp = 44,
		A1_Flat = 44,
		A1 = 45,
		A1_Sharp = 46,
		B1_Flat = 46,
		B1 = 47,
		C2 = 48,
		C2_Sharp = 49,
		D2_Flat = 49,
		D2 = 50,
		D2_Sharp = 51,
		E2_Flat = 51,
		E2 = 52,
		F2 = 53,
		F2_Sharp = 54,
		G2_Flat = 54,
		G2 = 55,
		G2_Sharp = 56,
		A2_Flat = 56,
		A2 = 57,
		A2_Sharp = 58,
		B2_Flat = 58,
		B2 = 59,
		C3 = 60,
		C3_Sharp = 61,
		D3_Flat = 61,
		D3 = 62,
		D3_Sharp = 63,
		E3_Flat = 63,
		E3 = 64,
		F3 = 65,
		F3_Sharp = 66,
		G3_Flat = 66,
		G3 = 67,
		G3_Sharp = 68,
		A3_Flat = 68,
		A3 = 69,
		A3_Sharp = 70,
		B3_Flat = 70,
		B3 = 71,
		C4 = 72,
		C4_Sharp = 73,
		D4_Flat = 73,
		D4 = 74,
		D4_Sharp = 75,
		E4_Flat = 75,
		E4 = 76,
		F4 = 77,
		F4_Sharp = 78,
		G4_Flat = 78,
		G4 = 79,
		G4_Sharp = 80,
		A4_Flat = 80,
		A4 = 81,
		A4_Sharp = 82,
		B4_Flat = 82,
		B4 = 83,
		C5 = 84,
		C5_Sharp = 85,
		D5_Flat = 85,
		D5 = 86,
		D5_Sharp = 87,
		E5_Flat = 87,
		E5 = 88,
		F5 = 89,
		F5_Sharp = 90,
		G5_Flat = 90,
		G5 = 91,
		G5_Sharp = 92,
		A5_Flat = 92,
		A5 = 93,
		A5_Sharp = 94,
		B5_Flat = 94,
		B5 = 95,
		C6 = 96,
		C6_Sharp = 97,
		D6_Flat = 97,
		D6 = 98,
		D6_Sharp = 99,
		E6_Flat = 99,
		E6 = 100,
		F6 = 101,
		F6_Sharp = 102,
		G6_Flat = 102,
		G6 = 103,
		G6_Sharp = 104,
		A6_Flat = 104,
		A6 = 105,
		A6_Sharp = 106,
		B6_Flat = 106,
		B6 = 107,
		C7 = 108,
		C7_Sharp = 109,
		D7_Flat = 109,
		D7 = 110,
		D7_Sharp = 111,
		E7_Flat = 111,
		E7 = 112,
		F7 = 113,
		F7_Sharp = 114,
		G7_Flat = 114,
		G7 = 115,
		G7_Sharp = 116,
		A7_Flat = 116,
		A7 = 117,
		A7_Sharp = 118,
		B7_Flat = 118,
		B7 = 119,
		C8 = 120,
		C8_Sharp = 121,
		D8_Flat = 121,
		D8 = 122,
		D8_Sharp = 123,
		E8_Flat = 123,
		E8 = 124,
		F8 = 125,
		F8_Sharp = 126,
		G8_Flat = 126,
		G8 = 127,

		None = -1,
		First = Cn2,
		Last = G8,
	};

	enum class Channel : uint8_t {
		One = 1,
		Two= 2,
		Three = 3,
		Four = 4,
		Five = 5,
		Six = 6,
		Seven = 7,
		Eight = 8,
		Nine = 9,
		Ten = 10,
		Eleven = 11,
		Twelve = 12,
		Thirteen = 13,
		Fourteen = 14,
		Fifteen = 15,
		Sixteen = 16,

		None = 0,
		First = One,
		Last = Sixteen
	};
	constexpr uint8_t CHANNEL_COUNT = 1 + (uint8_t) Channel::Last - (uint8_t) Channel::First;

	Note clampNote(Note v, bool withNone = false);
	Note clampNote(int v, bool withNone = false);

	Channel clampChannel(Channel v, bool withNone = false);
	Channel clampChannel(int v, bool withNone = false);
}

#define jassertValidMidiChannel(ch)\
	jassert((ch >= Artix::Midi::Channel::First) && (ch <= Artix::Midi::Channel::Last))

#define jassertValidMidiChannelOrNone(ch)\
	jassert((ch >= Artix::Midi::Channel::None) && (ch <= Artix::Midi::Channel::Last))

#define jassertValidMidiNote(note)\
	jassert((note>= Artix::Midi::Note::First) && (note<= Artix::Midi::Note::Last))

#define jassertValidMidiNoteOrNone(note)\
	jassert((note>= Artix::Midi::Note::None) && (note<= Artix::Midi::Note::Last))