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

		for (auto& m : mappers) {
			m.onError.add([this](const Artix::Error::ErrorDetails err) {
				juce::NativeMessageBox::showMessageBox(
					juce::MessageBoxIconType::WarningIcon,
					"Something went wrong",
					"Failed to load preset: " + err.msg
				);
			});
		}
	}

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

	juce::ValueTree MidiChannelMapperBank::toValueTree() const noexcept {
		const juce::ScopedReadLock lock(mutex);
		auto vt = juce::ValueTree(Id::MidiChannelMapperBank);
		vt.setProperty(Id::OutputChannel, (int) outputChannel.load(), nullptr);

		for (auto& m : mappers) {
			vt.addChild(m.toValueTree(), -1, nullptr);
		}

		return vt;
	}

	void MidiChannelMapperBank::fromValueTree(const juce::ValueTree& vt) noexcept {
		if (!vt.hasType(Id::MidiChannelMapperBank)) {
			onError.callOnMessageThread({
				"Invalid ValueTree type", Error::Code::BadState, Error::Code::InvalidValueTree
			});
			return;
		}

		setOutputChannel(vt.getProperty(Id::OutputChannel, (int) outputChannel.load()));

		int i = 0;
		for (auto child : vt) {
			if (!child.hasType(Id::MidiChannelMapper)) {
				continue;
			}

			mappers[i++].fromValueTree(child);
			if (i >= CHANNEL_COUNT)
				break;
		}

		if (i != CHANNEL_COUNT) {
			onError.callOnMessageThread({
				"ValueTree is missing children", Error::Code::BadState, Error::Code::MissingChildren
			});
			return;
		}
	}

	juce::var MidiChannelMapperBank::toVar() const noexcept {
		juce::DynamicObject::Ptr obj = new juce::DynamicObject();
		obj->setProperty("outputChannel", (uint8_t) getOutputChannel());

		auto outgoingMappers = juce::var();
		for (auto& m : mappers) {
			outgoingMappers.append(m.toVar());
		}
		obj->setProperty("mappers", outgoingMappers);
		return obj.get();
	}

	juce::ValueTree MidiChannelMapperBank::fromVar(const juce::var& data) noexcept {
		auto vt = juce::ValueTree(Id::MidiChannelMapperBank);
		vt.setProperty(
			Id::OutputChannel, data.getProperty("outputChannel", (uint8_t) getOutputChannel()), nullptr
		);

		auto incomingMappers = data.getProperty("mappers", juce::var()).getArray();
		if (incomingMappers) {
			int count = std::min<int>(incomingMappers->size(), CHANNEL_COUNT);

			for (int i = 0; i < count; i++) {
				vt.addChild(mappers[i].fromVar((*incomingMappers)[i]), -1, nullptr);
			}
		}
		return vt;
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