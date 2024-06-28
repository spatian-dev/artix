/*
  ==============================================================================

	MidiChannelMapperBank.cpp
	Created: 24 Jun 2024 8:31:21pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "MidiChannelMapperBank.h"

namespace Artix::Midi {
	MidiChannelMapperBank::MidiChannelMapperBank(Channel outputChannel, juce::String name) :
		outputChannel(outputChannel), name(name) {}

	Channel MidiChannelMapperBank::getOutputChannel() const noexcept {
		return outputChannel;
	}

	void MidiChannelMapperBank::setOutputChannel(Channel v) noexcept {
		jassertValidMidiChannel(v);
		outputChannel = clampChannel(v);
		juce::MessageManager::callAsync([this]() {
			if (onOutputChannelChanged) onOutputChannelChanged(outputChannel);
		});
	}

	void MidiChannelMapperBank::setOutputChannel(int v) noexcept {
		setOutputChannel(clampChannel(v));
	}

	const juce::String MidiChannelMapperBank::getName() const noexcept {
		const juce::ScopedReadLock lock(mutex);
		return name;
	}

	void MidiChannelMapperBank::setName(juce::String v) noexcept {
		const juce::ScopedWriteLock lock(mutex);
		name = v;
		juce::MessageManager::callAsync([this]() {
			if (onNameChanged) onNameChanged(name);
		});
	}

	juce::ValueTree MidiChannelMapperBank::toValueTree() const noexcept {
		const juce::ScopedReadLock lock(mutex);
		auto vt = juce::ValueTree(Id::MidiChannelMapperBank);
		vt.setProperty(Id::Name, name, nullptr);
		vt.setProperty(Id::OutputChannel, (int) outputChannel.load(), nullptr);

		for (auto& m : mappers) {
			vt.addChild(m.toValueTree(), -1, nullptr);
		}

		return vt;
	}

	void MidiChannelMapperBank::fromValueTree(const juce::ValueTree& vt) noexcept {
		jassert(vt.hasType(Id::MidiChannelMapperBank));
		if (!vt.hasType(Id::MidiChannelMapperBank)) {
			juce::MessageManager::callAsync([this]() {
				if (onError) {
					onError("Invalid ValueTree type", Error::Code::BadPreset, Error::Code::InvalidValueTree);
				}
			});
			return;
		}

		setName(vt.getProperty(Id::Name, this->name));
		setOutputChannel((int) vt.getProperty(Id::OutputChannel, (int) outputChannel.load()));

		int i = 0;
		for (auto child : vt) {
			if (!child.hasType(Id::MidiChannelMapper)) {
				continue;
			}

			mappers[i++].fromValueTree(child);
		}

		if (i != CHANNEL_COUNT) {
			juce::MessageManager::callAsync([this]() {
				if (onError) {
					onError(
						"ValueTree is missing children", Error::Code::BadPreset, Error::Code::MissingChildren
					);
				}
			});
		}
	}

	//======================================================================
	/** Mirroring std::array container functions */
	MidiChannelMapperBank::Mappers::reference MidiChannelMapperBank::at(Mappers::size_type pos) {
		return mappers.at(pos);
	}
	MidiChannelMapperBank::Mappers::const_reference MidiChannelMapperBank::at(Mappers::size_type pos) const {
		return mappers.at(pos);
	}

	MidiChannelMapperBank::Mappers::reference MidiChannelMapperBank::operator[](Mappers::size_type pos) {
		return mappers.operator[](pos);
	}
	MidiChannelMapperBank::Mappers::const_reference MidiChannelMapperBank::operator[](Mappers::size_type pos) const {
		return mappers.operator[](pos);
	}

	MidiChannelMapperBank::Mappers::iterator MidiChannelMapperBank::begin() noexcept {
		return mappers.begin();
	}
	MidiChannelMapperBank::Mappers::const_iterator MidiChannelMapperBank::begin() const noexcept {
		return mappers.begin();
	}
	MidiChannelMapperBank::Mappers::const_iterator MidiChannelMapperBank::cbegin() const noexcept {
		return mappers.cbegin();
	}

	MidiChannelMapperBank::Mappers::iterator MidiChannelMapperBank::end() noexcept {
		return mappers.end();
	}
	MidiChannelMapperBank::Mappers::const_iterator MidiChannelMapperBank::end() const noexcept {
		return mappers.end();
	}
	MidiChannelMapperBank::Mappers::const_iterator MidiChannelMapperBank::cend() const noexcept {
		return mappers.cend();
	}

	constexpr bool MidiChannelMapperBank::empty() const noexcept {
		return mappers.empty();
	}
	constexpr MidiChannelMapperBank::Mappers::size_type MidiChannelMapperBank::size() const noexcept {
		return mappers.size();
	}
	//======================================================================
}