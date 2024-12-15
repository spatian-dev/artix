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

	void MidiChannelMapper::setNote(Note v, bool muteCallbacks) noexcept {
		jassertValidMidiNoteOrNone(v);
		const Note newNote = clampNote(v, true);
		if (note == newNote) return;
		note = newNote;

		if (muteCallbacks) return;
		onNoteChanged.callSafely(note);
	}

	void MidiChannelMapper::setNote(int v, bool muteCallbacks) noexcept {
		setNote(clampNote(v, true), muteCallbacks);
	}

	const juce::String MidiChannelMapper::getName() const noexcept {
		const juce::ScopedReadLock lock(mutex);
		return name;
	}

	void MidiChannelMapper::setName(juce::String v, bool muteCallbacks) noexcept {
		const juce::ScopedWriteLock lock(mutex);
		if (name == v) return;
		name = v;

		if (muteCallbacks) return;
		onNameChanged.callSafely(name);
	}

	bool MidiChannelMapper::isActive() const noexcept {
		return note != Note::None;
	}

	const juce::String MidiChannelMapper::getDescription() const {
		std::stringstream ss;
		ss << "Mapper: \"" << name << "\", note: " << (int) note.load() << "\n";
		return ss.str();
	}

	juce::ValueTree MidiChannelMapper::toValueTree() const noexcept {
		const juce::ScopedReadLock lock(mutex);
		auto vt = juce::ValueTree(Id::MidiChannelMapper);
		vt.setProperty(Id::Name, name, nullptr);
		vt.setProperty(Id::Note, (int) note.load(), nullptr);
		return vt;
	}

	void MidiChannelMapper::fromValueTree(const juce::ValueTree& vt) noexcept {
		if (!vt.hasType(Id::MidiChannelMapper)) {
			onError.callOnMessageThread({
				"Invalid ValueTree type", Error::Code::BadState, Error::Code::InvalidValueTree
			});
			return;
		}

		setName(vt.getProperty(Id::Name, name));
		setNote(vt.getProperty(Id::Note, (int) note.load()));
	}

	juce::var MidiChannelMapper::toVar() const noexcept {
		juce::DynamicObject::Ptr obj = new juce::DynamicObject();
		obj->setProperty("name", getName());
		obj->setProperty("note", (int8_t) getNote());

		return obj.get();
	}

	juce::ValueTree MidiChannelMapper::fromVar(const juce::var& data) noexcept {
		auto vt = juce::ValueTree(Id::MidiChannelMapper);
		vt.setProperty(Id::Name, data.getProperty("name", getName()), nullptr);
		vt.setProperty(
			Id::Note, data.getProperty("note", (int8_t) getNote()), nullptr
		);
		return vt;
	}
}