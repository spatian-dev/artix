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

	void MidiChannelMapper::setNote(Note v, bool muteCallbacks) {
		jassertValidMidiNoteOrNone(v);
		const Note newNote = clampNote(v, true);
		if (note == newNote) return;
		note = newNote;
		setIsDirty(true, muteCallbacks);

		if (muteCallbacks) return;
		onNoteChanged.callSafely(note);
	}

	void MidiChannelMapper::setNote(int v, bool muteCallbacks) {
		setNote(clampNote(v, true), muteCallbacks);
	}

	const juce::String MidiChannelMapper::getName() const {
		const juce::ScopedReadLock lock(mutex);
		return name;
	}

	void MidiChannelMapper::setName(juce::String v, bool muteCallbacks) {
		const juce::ScopedWriteLock lock(mutex);
		if (name == v) return;
		name = v;
		setIsDirty(true, muteCallbacks);

		if (muteCallbacks) return;
		onNameChanged.callSafely(name);
	}

	bool MidiChannelMapper::getIsDirty() const noexcept {
		return isDirty;
	}

	void MidiChannelMapper::setIsDirty(bool v, bool muteCallbacks) {
		if (isDirty == v) return;
		isDirty = v;

		if (muteCallbacks || muteDirty) return;
		onDirtyChanged.callSafely(v);
	}

	bool MidiChannelMapper::isActive() const noexcept {
		return note != Note::None;
	}

	const juce::String MidiChannelMapper::getDescription() const {
		std::stringstream ss;
		ss << "Mapper: \"" << getName() << "\", note: " << (int) note.load() << "\n";
		return ss.str();
	}

	juce::ValueTree MidiChannelMapper::toValueTree() const {
		const juce::ScopedReadLock lock(mutex);
		auto vt = juce::ValueTree(Id::MidiChannelMapper);

		vt.setProperty(Id::Name, getName(), nullptr);
		vt.setProperty(Id::Note, (int) note.load(), nullptr);

		return vt;
	}

	bool MidiChannelMapper::fromValueTree(const juce::ValueTree& vt, bool muteCallbacks) {
		if (!(vt.isValid() && vt.hasType(Id::MidiChannelMapper))) {
			onError.callOnMessageThread("Invalid ValueTree type");
			return false;
		}

		muteDirty = true;
		
		setName(vt.getProperty(Id::Name, getName()), muteCallbacks);
		setNote(vt.getProperty(Id::Note, (int) note.load()), muteCallbacks);

		muteDirty = false;

		setIsDirty(false);
		return true;
	}
	
	juce::var MidiChannelMapper::toVar() const {
		juce::DynamicObject::Ptr obj = new juce::DynamicObject();
		obj->setProperty(Id::Name, getName());
		obj->setProperty(Id::Note, (int8_t) getNote());

		return obj.get();
	}

	bool MidiChannelMapper::fromVar(const juce::var& data, bool muteCallbacks) {
		auto vt = juce::ValueTree(Id::MidiChannelMapper);
		vt.setProperty(Id::Name, data.getProperty(Id::Name, getName()), nullptr);
		vt.setProperty(
			Id::Note, data.getProperty(Id::Note, (int8_t) getNote()), nullptr
		);
		return fromValueTree(vt, muteCallbacks);
	}
}