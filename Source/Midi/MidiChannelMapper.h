/*
  ==============================================================================

    MidiChannelMapper.h
    Created: 24 Jun 2024 7:42:20pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cstdint>
#include <atomic>
#include <sstream>
#include "../Id/Identifiers.h"
#include "../Error/Error.h"
#include "../Utils/CallbackList.h"
#include "Midi.h"

namespace Artix::Midi {
    class MidiChannelMapper {
        public:
        using DirtyChangedCallback = Utils::CallbackList<bool>;
        using NameChangedCallback = Utils::CallbackList<juce::String>;
        using NoteChangedCallback = Utils::CallbackList<Note>;

        MidiChannelMapper(Note note = Note::None, juce::String name = juce::String());

        Note getNote() const noexcept;
        void setNote(Note v, bool muteCallbacks = false);
        void setNote(int v, bool muteCallbacks = false);

        const juce::String getName() const;
        void setName(juce::String v, bool muteCallbacks = false);

        bool getIsDirty() const noexcept;
        void setIsDirty(bool v, bool muteCallbacks = false);

        bool isActive() const noexcept;

        const juce::String getDescription() const;

        juce::ValueTree toValueTree() const;
        bool fromValueTree(const juce::ValueTree& vt, bool muteCallbacks = false);

        juce::var toVar() const;
        bool fromVar(const juce::var& vt, bool muteCallbacks = false);

        NameChangedCallback onNameChanged;
        NoteChangedCallback onNoteChanged;
        DirtyChangedCallback onDirtyChanged;
        Error::ErrorCallback onError;

        private:
        juce::ReadWriteLock mutex;
        std::atomic<Note> note;
        juce::String name;
        std::atomic<bool> isDirty = false;
        std::atomic<bool> muteDirty = false;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiChannelMapper)
    };

    inline bool operator==(const MidiChannelMapper& left, const MidiChannelMapper& right) {
        return (left.getName() == right.getName()) && (left.getNote() == right.getNote());
    };
    inline bool operator!=(const MidiChannelMapper& left, const MidiChannelMapper& right) {
        return !(left == right);
    };
}