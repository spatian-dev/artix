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
        using NameChangedCallback = Utils::CallbackList<juce::String>;
        using NoteChangedCallback = Utils::CallbackList<Note>;

        MidiChannelMapper(Note note = Note::None, juce::String name = juce::String());

        Note getNote() const noexcept;
        void setNote(Note v, bool muteCallbacks = false) noexcept;
        void setNote(int v, bool muteCallbacks = false) noexcept;

        const juce::String getName() const noexcept;
        void setName(juce::String v, bool muteCallbacks = false) noexcept;

        bool isActive() const noexcept;

        const juce::String getDescription() const;

        juce::ValueTree toValueTree() const noexcept;
        void fromValueTree(const juce::ValueTree& vt) noexcept;

        NameChangedCallback onNameChanged;
        NoteChangedCallback onNoteChanged;
        Error::ErrorCallback onError;

        private:
        juce::ReadWriteLock mutex;
        std::atomic<Note> note;
        juce::String name;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiChannelMapper)
    };
}