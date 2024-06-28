/*
  ==============================================================================

	Midi.h
	Created: 24 Jun 2024 7:51:12pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "Midi.h"

static_assert(std::atomic<Artix::Midi::Note>::is_always_lock_free);
static_assert(std::atomic<Artix::Midi::Channel>::is_always_lock_free);

namespace Artix::Midi {
	Note clampNote(Note v, bool withNone) {
		return std::clamp(v, (withNone ? Note::None : Note::First), Note::Last);
	}

	Note clampNote(int v, bool withNone) {
		return (Note) std::clamp<int>(v, (int) (withNone ? Note::None : Note::First), (int) Note::Last);
	}

	Channel clampChannel(Channel v, bool withNone) {
		return std::clamp(v, (withNone ? Channel::None : Channel::First), Channel::Last);
	}

	Channel clampChannel(int v, bool withNone) {
		return (Channel) std::clamp((int) v, (int) (withNone ? Channel::None : Channel::First), (int) Channel::Last);
	}
}