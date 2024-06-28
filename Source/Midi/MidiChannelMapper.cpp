/*
  ==============================================================================

	MidiChannelMapper.cpp
	Created: 24 Jun 2024 7:42:20pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "MidiChannelMapper.h"

namespace Artix::Midi {
	MidiChannelMapper::MidiChannelMapper(Note note, juce::String name) :
		note(note), name(name) {}

	Note MidiChannelMapper::getNote() const noexcept {
		return note;
	}

	void MidiChannelMapper::setNote(Note v) noexcept {
		jassertValidMidiNoteOrNone(v);
		note = clampNote(v);
		juce::MessageManager::callAsync([this]() {
			if (onNoteChanged) onNoteChanged(note);
		});
	}

	void MidiChannelMapper::setNote(int v) noexcept {
		setNote(clampNote(v));
	}

	const juce::String MidiChannelMapper::getName() const noexcept {
		const juce::ScopedReadLock lock(mutex);
		return name;
	}

	void MidiChannelMapper::setName(juce::String v) noexcept {
		const juce::ScopedWriteLock lock(mutex);
		name = v;
		juce::MessageManager::callAsync([this]() {
			if (onNameChanged) onNameChanged(name);
		});
	}

	bool MidiChannelMapper::isActive() const noexcept {
		return note != Note::None;
	}

	juce::ValueTree MidiChannelMapper::toValueTree() const noexcept {
		const juce::ScopedReadLock lock(mutex);
		auto vt = juce::ValueTree(Id::MidiChannelMapper);
		vt.setProperty(Id::Name, name, nullptr);
		vt.setProperty(Id::Note, (int) note.load(), nullptr);
		return vt;
	}

	void MidiChannelMapper::fromValueTree(const juce::ValueTree& vt) noexcept {
		jassert(vt.hasType(Id::MidiChannelMapper));
		if (!vt.hasType(Id::MidiChannelMapper)) {
			juce::MessageManager::callAsync([this]() {
				if (onError) {
					onError("Invalid ValueTree type", Error::Code::BadPreset, Error::Code::InvalidValueTree);
				}
			});
			return;
		}

		setName(vt.getProperty(Id::Name, name));
		setNote((int) vt.getProperty(Id::Note, (int) note.load()));
	}
}