/*
  ==============================================================================

    State.h
    Created: 28 Jun 2024 5:16:36pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <atomic>
#include "Utils/CallbackList.h"
#include "Id/Identifiers.h"
#include "Error/Error.h"
#include "Ui/Theme/Themes.h"
#include "Midi/MidiChannelMapperBank.h"

namespace Artix {
    class State {
        public:
        using DirtyChangedCallback = Utils::CallbackList<bool>;
        using NameChangedCallbacks = Utils::CallbackList<juce::String>;

        State(juce::String name = "(unnamed)");
        ~State();

        juce::String getName() const;
        void setName(juce::String v, bool muteCallbacks = false);

        bool getIsDirty() const noexcept;
        void setIsDirty(bool v, bool muteCallbacks = false);

        Midi::MidiChannelMapperBank& getMapperBank() noexcept;
        const Midi::MidiChannelMapperBank& getMapperBank() const noexcept;

        juce::ValueTree toValueTree() const;
        bool fromValueTree(const juce::ValueTree& vt, bool muteCallbacks = false);

        juce::String toJson() const;
        bool fromJson(const juce::String json, bool muteCallbacks = false);

        juce::var toVar() const;
        bool fromVar(const juce::var& data, bool muteCallbacks = false);

        bool fromState(const State& state, bool muteCallbacks = false);

        NameChangedCallbacks onNameChanged;
        DirtyChangedCallback onDirtyChanged;
        Error::ErrorCallback onError;

        private:
        DirtyChangedCallback::Identifier bankDirtyChangedId;
        Error::ErrorCallback::Identifier bankErrorId;

        juce::ReadWriteLock nameMutex;
        juce::String name;

        Midi::MidiChannelMapperBank mapperBank;

        std::atomic<bool> isDirty = false;
        std::atomic<bool> muteDirty = false;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(State)
    };

    inline bool operator==(const State& left, const State& right) {
        return (left.getName() == right.getName()) &&
            (left.getMapperBank() == right.getMapperBank());
    };
    inline bool operator!=(const State& left, const State& right) {
        return !(left == right);
    };
}