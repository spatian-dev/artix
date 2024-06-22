/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 21 Jun 2024 09:21:44pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "ChannelMapper.h"

namespace Artix::Midi {
	ChannelMapper::ChannelMapper(const juce::ValueTree& v) : state(v) {
		jassert(v.hasType(Artix::ID::MidiChannelMapper));
	}

	ChannelMapper::~ChannelMapper() {
	}

	int ChannelMapper::getInputChannel() const {
		return state[Artix::ID::InputChannelNumber];
	}
	void ChannelMapper::setInputChannel(int ch) {
		jassert((ch >= -1) && (ch <= 15));
		state.setProperty(Artix::ID::InputChannelNumber, ch, nullptr);
	}
	int ChannelMapper::getOutputNote() const {
		return state[Artix::ID::OutputNoteNumber];;
	}
	void ChannelMapper::setOutputNote(int note) {
		jassert((note >= 0) && (note <= 127));
		state.setProperty(Artix::ID::OutputNoteNumber, note, nullptr);
	}

	juce::ValueTree ChannelMapper::getState() {
		return state;
	}
}