/*
  ==============================================================================

	MidiChannelMapper.cpp
	Created: 24 Jun 2024 7:42:20pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "MidiChannelMapper.h"

namespace Artix::Midi {
	MidiChannelMapper::MidiChannelMapper(int8_t note, juce::String name) :
		note(note), name(name) {}

	int8_t MidiChannelMapper::getNote() const noexcept {
		return note;
	}

	void MidiChannelMapper::setNote(int8_t v) noexcept {
		jassertValidMidiNoteOrNone(v);
		note= v;
	}

	const juce::String MidiChannelMapper::getName() const noexcept {
		const juce::ScopedReadLock lock(mutex);
		return name;
	}

	void MidiChannelMapper::setName(juce::String v) noexcept {
		const juce::ScopedWriteLock lock(mutex);
		name = v;
	}

	bool MidiChannelMapper::isActive() const noexcept {
		return note != Notes::NONE;
	}

	juce::ValueTree MidiChannelMapper::toValueTree() const noexcept {
		const juce::ScopedReadLock lock(mutex);
		auto vt = juce::ValueTree(Id::MidiChannelMapper);
		vt.setProperty(Id::Name, name, nullptr);
		vt.setProperty(Id::Note, note.load(), nullptr);
		return vt;
	}

	void MidiChannelMapper::fromValueTree(const juce::ValueTree& vt) noexcept {
		jassert(vt.hasType(Id::MidiChannelMapper));
		if (!vt.hasType(Id::MidiChannelMapper))
			return;

		this->name = vt.getProperty(Id::Name, this->name);

		jassert(vt.hasProperty(Id::Note));
		if (vt.hasProperty(Id::Note)) {
			// `operator int()` will return 0 if string is not interpretable as int
			this->note = vt.getProperty(Id::Note).operator int();
		}
	}
}