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
		const bool hasChanged = (outputChannel != newChannel);
		outputChannel = newChannel;

		if (!hasChanged || muteCallbacks) return;
		juce::MessageManager::callAsync([this]() {
			if (onOutputChannelChanged) onOutputChannelChanged(outputChannel);
		});
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
		const bool hasChanged = (name != v);
		name = v;

		if (!hasChanged || muteCallbacks) return;
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
					onError("Invalid ValueTree type", Error::Code::BadState, Error::Code::InvalidValueTree);
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
						"ValueTree is missing children", Error::Code::BadState, Error::Code::MissingChildren
					);
				}
			});
		}
	}
}