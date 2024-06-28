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
#include "../Identifiers.h"
#include "../Errors.h"
#include "../Utils/UTF8.h"
#include "Midi.h"

namespace Artix::Midi {
    class MidiChannelMapper {
        public:
        MidiChannelMapper(Note note = Note::None, juce::String name = Utils::UTF8::EmDash);

        Note getNote() const noexcept;
        void setNote(Note v) noexcept;
        void setNote(int v) noexcept;

        const juce::String getName() const noexcept;
        void setName(juce::String v) noexcept;

        bool isActive() const noexcept;

        juce::ValueTree toValueTree() const noexcept;
        void fromValueTree(const juce::ValueTree& vt) noexcept;

        std::function<void(const juce::String&)> onNameChanged;
        std::function<void(Note)> onNoteChanged;
        std::function<void(const juce::String&, Error::Code, Error::Code)> onError;

        private:
        juce::ReadWriteLock mutex;
        std::atomic<Note> note;
        juce::String name;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiChannelMapper)
    };
}