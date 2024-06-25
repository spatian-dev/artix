/*
  ==============================================================================

    MidiChannelMapperBank.h
    Created: 24 Jun 2024 8:31:21pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cstdint>
#include <atomic>
#include <span>
#include "../Identifiers.h"
#include "Midi.h"
#include "MidiChannelMapper.h"

static_assert(std::atomic<uint8_t>::is_always_lock_free);

namespace Artix::Midi {
    class MidiChannelMapperBank {
        public:
        MidiChannelMapperBank(
            uint8_t outCh = 1, juce::String name = "untitled"
        );

        uint8_t getOutputChannel() const noexcept;
        void setOutputChannel(uint8_t v) noexcept;

        const juce::String getName() const noexcept;
        void setName(juce::String v) noexcept;

        MidiChannelMapper& getMapper(int channel) noexcept;
        std::span<MidiChannelMapper> getMappers() noexcept;

        juce::ValueTree toValueTree() const noexcept;
        void fromValueTree(const juce::ValueTree& vt) noexcept;

        private:
        MidiChannelMapper mappers[16];
        std::atomic<uint8_t> outCh;
        juce::ReadWriteLock mutex;
        juce::String name;

       JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiChannelMapperBank)
    };
}