/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 21 Jun 2024 09:21:44pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "ChannelMapper.h"

namespace Artix::Midi {
	ChannelMapper::ChannelMapper(
		const juce::ValueTree& v, juce::UndoManager* undoManager
	) : state(v), undoManager(undoManager) {
		jassert(v.hasType(Artix::ID::MidiChannelMapper));
	}

	ChannelMapper::~ChannelMapper() {
	}

	int ChannelMapper::getInputChannel() const {
		return state[Artix::ID::InputChannelNumber];
	}
	void ChannelMapper::setInputChannel(int ch) {
		jassertValidMidiChannel(ch);
		state.setProperty(Artix::ID::InputChannelNumber, ch, nullptr);
	}

	int ChannelMapper::getOutputChannel() const {
		return state[Artix::ID::OutputChannelNumber];
	}
	void ChannelMapper::setOutputChannel(int ch) {
		jassertValidMidiChannel(ch);
		state.setProperty(Artix::ID::OutputChannelNumber, ch, nullptr);
	}

	int ChannelMapper::getOutputNote() const {
		return state[Artix::ID::OutputNoteNumber];
	}
	void ChannelMapper::setOutputNote(int note) {
		jassertValidMidiNoteOrNone(note);

		state.setProperty(Artix::ID::OutputNoteNumber, note, nullptr);
	}

	bool ChannelMapper::isActive() const {
		return (getOutputNote() != NOTE_NONE);
	}

	juce::ValueTree ChannelMapper::getState() {
		return state;
	}

	juce::ValueTree ChannelMapper::makeState(int inChannel, int outChannel, int outNote) noexcept {
		jassertValidMidiChannel(inChannel);
		jassertValidMidiChannel(outChannel);
		jassertValidMidiNoteOrNone(outNote);

		return juce::ValueTree(Artix::ID::MidiChannelMapper)
				.setProperty(Artix::ID::InputChannelNumber, inChannel, nullptr)
				.setProperty(Artix::ID::OutputChannelNumber, outChannel, nullptr)
				.setProperty(Artix::ID::OutputNoteNumber, outNote, nullptr);
	}
}