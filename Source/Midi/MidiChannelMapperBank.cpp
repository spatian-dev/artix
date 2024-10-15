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

	void MidiChannelMapperBank::setOutputChannel(Channel v, bool muteCallbacks) noexcept {
		jassertValidMidiChannel(v);
		const Channel newChannel = clampChannel(v);
		if (outputChannel == newChannel) return;
		outputChannel = newChannel;

		if (muteCallbacks) return;
		onOutputChannelChanged.callSafely(outputChannel);
	}

	void MidiChannelMapperBank::setOutputChannel(int v, bool muteCallbacks) noexcept {
		setOutputChannel(clampChannel(v), muteCallbacks);
	}

	const juce::String MidiChannelMapperBank::getName() const noexcept {
		const juce::ScopedReadLock lock(mutex);
		return name;
	}

	void MidiChannelMapperBank::setName(juce::String v, bool muteCallbacks) noexcept {
		const juce::ScopedWriteLock lock(mutex);
		if (name == v) return;
		name = v;

		if (muteCallbacks) return;
		onNameChanged.callSafely(name);
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
			onError.callOnMessageThread({
				"Invalid ValueTree type", Error::Code::BadState, Error::Code::InvalidValueTree
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
				onError.callOnMessageThread({
					"ValueTree is missing children", Error::Code::BadState, Error::Code::MissingChildren
				});
			});
		}
	}
	const juce::String MidiChannelMapperBank::getDescription() const {
		std::stringstream ss;
		ss << "Output Bank: \"" << name << "\", output channel: " << (int) outputChannel.load() << "\n";
		for (auto& m : mappers) {
			ss << "\t" << m.getDescription();
		}
		return ss.str();
	}
}