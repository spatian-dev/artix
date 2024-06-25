/*
  ==============================================================================

	MidiChannelMapperBank.cpp
	Created: 24 Jun 2024 8:31:21pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "MidiChannelMapperBank.h"

namespace Artix::Midi {
	MidiChannelMapperBank::MidiChannelMapperBank(uint8_t outCh, juce::String name) :
		outCh(outCh), name(name) {}

	uint8_t MidiChannelMapperBank::getOutputChannel() const noexcept {
		return outCh;
	}
	void MidiChannelMapperBank::setOutputChannel(uint8_t v) noexcept {
		jassertValidMidiChannel(v);
		outCh = v;
	}

	const juce::String MidiChannelMapperBank::getName() const noexcept {
		const juce::ScopedReadLock lock(mutex);
		return name;
	}

	void MidiChannelMapperBank::setName(juce::String v) noexcept {
		const juce::ScopedWriteLock lock(mutex);
		name = v;
	}

	MidiChannelMapper& MidiChannelMapperBank::getMapper(int channel) noexcept {
		jassertValidMidiChannel(channel);
		return mappers[channel];
	}

	std::span<MidiChannelMapper> MidiChannelMapperBank::getMappers() noexcept {
		std::span span{mappers, std::size(mappers)};
		return span;
	}

	juce::ValueTree MidiChannelMapperBank::toValueTree() const noexcept {
		const juce::ScopedReadLock lock(mutex);
		auto vt = juce::ValueTree(Id::MidiChannelMapperBank);
		vt.setProperty(Id::Name, name, nullptr);

		for (auto& m : mappers) {
			vt.addChild(m.toValueTree(), -1, nullptr);
		}
		
		return vt;
	}

	void MidiChannelMapperBank::fromValueTree(const juce::ValueTree& vt) noexcept {
		jassert(vt.hasType(Id::MidiChannelMapperBank));
		if (!vt.hasType(Id::MidiChannelMapperBank))
			return;

		this->name = vt.getProperty(Id::Name, this->name);

		jassert(vt.getNumChildren() == std::size(mappers));
		if (vt.getNumChildren() != std::size(mappers))
			return;

		int i = 0;
		for (auto child : vt) {
			jassert(child.hasType(Id::MidiChannelMapper));
			if (!child.hasType(Id::MidiChannelMapper))
				return;

			mappers[i++].fromValueTree(child);
		}
	}
}