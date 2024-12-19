/*
  ==============================================================================

	MidiChannelMapperBank.cpp
	Created: 24 Jun 2024 8:31:21pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "MidiChannelMapperBank.h"

namespace Artix::Midi {
	MidiChannelMapperBank::MidiChannelMapperBank(Channel outputChannel) :
		outputChannel(outputChannel) {

		for (size_t i = 0; i < mappers.size(); i++) {
			auto& m = mappers[i];
			mapperErrorIds[i] = m.onError.add([this](const juce::String err) {
				onError.callSafely(err);
			});
			mapperDirtyChangedIds[i] = m.onDirtyChanged.add([this](const bool v) {
				if (v) setIsDirty(true);
			});
		}
	}

	MidiChannelMapperBank::~MidiChannelMapperBank() {
		for (size_t i = 0; i < mappers.size(); i++) {
			auto& m = mappers[i];
			if (mapperErrorIds[i])
				m.onError.remove(mapperErrorIds[i].value());
			if (mapperDirtyChangedIds[i])
				m.onDirtyChanged.remove(mapperDirtyChangedIds[i].value());
		}
	}

	Channel MidiChannelMapperBank::getOutputChannel() const noexcept {
		return outputChannel;
	}

	void MidiChannelMapperBank::setOutputChannel(Channel v, bool muteCallbacks) {
		jassertValidMidiChannel(v);
		const Channel newChannel = clampChannel(v);
		if (outputChannel == newChannel) return;
		outputChannel = newChannel;
		setIsDirty(true, muteCallbacks || muteDirty);

		if (muteCallbacks) return;
		onOutputChannelChanged.callSafely(outputChannel);
	}

	void MidiChannelMapperBank::setOutputChannel(int v, bool muteCallbacks) {
		setOutputChannel(clampChannel(v), muteCallbacks);
	}

	bool MidiChannelMapperBank::getIsDirty() const noexcept {
		return isDirty;
	}

	void MidiChannelMapperBank::setIsDirty(bool v, bool muteCallbacks) {
		if (isDirty == v) return;
		isDirty = v;

		if (muteCallbacks) return;
		onDirtyChanged.callSafely(v);
	}

	juce::ValueTree MidiChannelMapperBank::toValueTree() const {
		const juce::ScopedReadLock lock(mutex);
		auto vt = juce::ValueTree(Id::MidiChannelMapperBank);
		
		vt.setProperty(Id::OutputChannel, (int) outputChannel.load(), nullptr);
		for (auto& m : mappers) {
			vt.addChild(m.toValueTree(), -1, nullptr);
		}

		return vt;
	}

	bool MidiChannelMapperBank::fromValueTree(const juce::ValueTree& vt, bool muteCallbacks) {
		if (!(vt.isValid() && vt.hasType(Id::MidiChannelMapperBank))) {
			onError.callOnMessageThread("Invalid ValueTree type");
			return false;
		}

		bool result = true;
		muteDirty = true;

		setOutputChannel(vt.getProperty(Id::OutputChannel, (int) outputChannel.load()), muteCallbacks);

		int i = 0;
		for (auto child : vt) {
			if (!child.hasType(Id::MidiChannelMapper)) {
				continue;
			}

			if (mappers[i].fromValueTree(child, muteCallbacks)) {
				i++;
			}

			if (i >= CHANNEL_COUNT)
				break;
		}
		muteDirty = false;

		if (i != CHANNEL_COUNT) {
			onError.callOnMessageThread("ValueTree is missing children");
			result = false;
		}

		setIsDirty(!result, muteCallbacks);
		return result;
	}

	juce::var MidiChannelMapperBank::toVar() const {
		juce::DynamicObject::Ptr obj = new juce::DynamicObject();
		obj->setProperty(Id::OutputChannel, (uint8_t) getOutputChannel());

		auto outgoingMappers = juce::var();
		for (auto& m : mappers) {
			outgoingMappers.append(m.toVar());
		}
		obj->setProperty(Id::MidiChannelMapperList, outgoingMappers);
		return obj.get();
	}

	bool MidiChannelMapperBank::fromVar(const juce::var& data, bool muteCallbacks) {
		auto vt = juce::ValueTree(Id::MidiChannelMapperBank);
		vt.setProperty(
			Id::OutputChannel, data.getProperty(Id::OutputChannel, (uint8_t) getOutputChannel()), nullptr
		);

		auto incomingMappers = data.getProperty(Id::MidiChannelMapperList, juce::var()).getArray();
		if (incomingMappers) {
			int count = std::min<int>(incomingMappers->size(), CHANNEL_COUNT);

			for (int i = 0; i < count; i++) {
				if (mappers[i].fromVar((*incomingMappers)[i], muteCallbacks)) {
					vt.addChild(mappers[i].toValueTree(), -1, nullptr);
				}
			}
		}
		return fromValueTree(vt, muteCallbacks);
	}

	const juce::String MidiChannelMapperBank::getDescription() const {
		std::stringstream ss;
		ss << "Output Channel: " << (int) outputChannel.load() << "\n";
		for (auto& m : mappers) {
			ss << "\t" << m.getDescription();
		}
		return ss.str();
	}
}