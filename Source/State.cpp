/*
  ==============================================================================

    State.cpp
    Created: 28 Jun 2024 5:16:36pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "State.h"

namespace Artix {
    State::State(juce::String name): name(name) {
        bankErrorId = mapperBank.onError.add([this](const juce::String err) {
            onError.callSafely(err);
        });
        bankDirtyChangedId = mapperBank.onDirtyChanged.add([this](const bool v) {
            if (v) setIsDirty(true);
        });
    }

    State::~State() {
        if (bankErrorId)
            mapperBank.onError.remove(bankErrorId.value());
        if (bankDirtyChangedId)
            mapperBank.onDirtyChanged.remove(bankDirtyChangedId.value());
    }

    juce::String State::getName() const {
        juce::ScopedReadLock lock(nameMutex);
        return name;
    }

    void State::setName(juce::String v, bool muteCallbacks) {
        juce::ScopedWriteLock lock(nameMutex);

        if (name == v) return;
        name = v;
        setIsDirty(true, muteCallbacks);

        if (muteCallbacks) return;
        onNameChanged.callSafely(name);
    }

    bool State::getIsDirty() const noexcept {
        return isDirty;
    }

    void State::setIsDirty(bool v, bool muteCallbacks) {
        if (isDirty == v) return;
        isDirty = v;

        if (muteCallbacks || muteDirty) return;
        onDirtyChanged.callSafely(v);
    }

    Midi::MidiChannelMapperBank& State::getMapperBank() noexcept {
        return mapperBank;
    }

    const Midi::MidiChannelMapperBank& State::getMapperBank() const noexcept {
        return mapperBank;
    }

    juce::ValueTree State::toValueTree() const {
        auto vt = juce::ValueTree(Id::State);

        vt.setProperty(Id::Name, getName(), nullptr);
        vt.addChild(mapperBank.toValueTree(), -1, nullptr);
        return vt;
    }

    bool State::fromValueTree(const juce::ValueTree& vt, bool muteCallbacks) {
        if (!(vt.isValid() && vt.hasType(Id::State))) {
            onError.callOnMessageThread("Invalid ValueTree type");
            return false;
        }

        bool result = true;
        muteDirty = true;

        setName(vt.getProperty(Id::Name, this->name), muteCallbacks);
        
        auto bank = vt.getChildWithName(Id::MidiChannelMapperBank);
        if (!(bank.isValid() && mapperBank.fromValueTree(bank, muteCallbacks)))
            result = false;

        muteDirty = false;
        setIsDirty(!result, muteCallbacks);
        return result;
    }

    juce::var State::toVar() const {
        juce::DynamicObject::Ptr obj = new juce::DynamicObject();
        obj->setProperty(Id::Name, getName());
        obj->setProperty(Id::MidiChannelMapperBank, mapperBank.toVar());
        return obj.get();
    }

    bool State::fromVar(const juce::var& data, bool muteCallbacks) {
        auto vt = juce::ValueTree(Id::State);
        vt.setProperty(Id::Name, data.getProperty(Id::Name, getName()), nullptr);
        
        if (!mapperBank.fromVar(data.getProperty(Id::MidiChannelMapperBank, juce::var()), muteCallbacks))
            return false;

        vt.addChild(mapperBank.toValueTree(), -1, nullptr);
        return fromValueTree(vt, muteCallbacks);
    }

    bool State::fromState(const State& state, bool muteCallbacks) {
        return this->fromValueTree(state.toValueTree(), muteCallbacks);
    }

    juce::String State::toJson() const {
        return juce::JSON::toString(toVar());
    }

    bool State::fromJson(const juce::String json, bool muteCallbacks) {
        juce::var data;
        const auto result = juce::JSON::parse(json, data);

        if (!result.wasOk()) {
            onError.callOnMessageThread("Failed to load preset: Invalid data");
            return false;
        }

        return fromVar(data, muteCallbacks);
    }
}