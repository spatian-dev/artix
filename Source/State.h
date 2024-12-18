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
        using HeightChangedCallbacks = Utils::CallbackList<int>;
        using ThemeChangedCallbacks = Utils::CallbackList<Ui::Theme::ThemePtr>;

        State(juce::String name = "(unnamed)", int height = 800);
        ~State();

        juce::String getName() const;
        void setName(juce::String v, bool muteCallbacks = false);

        int getHeight() const noexcept;
        void setHeight(int v, bool muteCallbacks = false);

        Ui::Theme::ThemePtr getTheme() const;
        void setTheme(Ui::Theme::ThemePtr v, bool muteCallbacks = false);

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
        HeightChangedCallbacks onHeightChanged;
        ThemeChangedCallbacks onThemeChanged;
        DirtyChangedCallback onDirtyChanged;
        Error::ErrorCallback onError;

        DirtyChangedCallback::Identifier bankDirtyChangedId;
        Error::ErrorCallback::Identifier bankErrorId;

        private:
        juce::ReadWriteLock nameMutex;
        juce::String name;

        std::atomic<int> height;
        Midi::MidiChannelMapperBank mapperBank;

        juce::ReadWriteLock themeMutex;
        Ui::Theme::ThemePtr theme;

        std::atomic<bool> isDirty = false;
        std::atomic<bool> muteDirty = false;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(State)
    };

    inline bool operator==(const State& left, const State& right) {
        return (left.getName() == right.getName()) &&
            (left.getHeight() == right.getHeight()) &&
            (left.getMapperBank() == right.getMapperBank());
    };
    inline bool operator!=(const State& left, const State& right) {
        return !(left == right);
    };
}