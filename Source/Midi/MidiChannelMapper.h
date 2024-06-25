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
#include "Midi.h"

static_assert(std::atomic<int8_t>::is_always_lock_free);

namespace Artix::Midi {
    class MidiChannelMapper {
        public:
        MidiChannelMapper(
            int8_t note = -1, juce::String name = juce::CharPointer_UTF8("\xe2\x80\x94")
        );

        int8_t getNote() const noexcept;
        void setNote(int8_t v) noexcept;

        const juce::String getName() const noexcept;
        void setName(juce::String v) noexcept;

        bool isActive() const noexcept;

        juce::ValueTree toValueTree() const noexcept;
        void fromValueTree(const juce::ValueTree& vt) noexcept;

        private:
        std::atomic<int8_t> note;
        juce::ReadWriteLock mutex;
        juce::String name;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiChannelMapper)
    };
}