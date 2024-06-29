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
#include "../Identifiers.h"
#include "Midi.h"
#include "MidiChannelMapper.h"

namespace Artix::Midi {
    class MidiChannelMapperBank {
        private:
        using Mappers = std::array<MidiChannelMapper, CHANNEL_COUNT>;

        public:
        MidiChannelMapperBank(Channel outputChannel = Channel::First, juce::String name = "untitled");

        Channel getOutputChannel() const noexcept;
        void setOutputChannel(Channel v, bool muteCallbacks = false) noexcept;
        void setOutputChannel(int v, bool muteCallbacks = false) noexcept;

        const juce::String getName() const noexcept;
        void setName(juce::String v, bool muteCallbacks = false) noexcept;

        juce::ValueTree toValueTree() const noexcept;
        void fromValueTree(const juce::ValueTree& vt) noexcept;

        std::function<void(const juce::String&)> onNameChanged;
        std::function<void(Channel)> onOutputChannelChanged;
        Error::ErrorCallback onError;

        //======================================================================
        /** Mirroring std::array container functions */
        Mappers::reference at(Mappers::size_type pos);
        Mappers::const_reference at(Mappers::size_type pos) const;

        Mappers::reference operator[](Mappers::size_type pos);
        Mappers::const_reference operator[](Mappers::size_type pos) const;

        Mappers::iterator begin() noexcept;
        Mappers::const_iterator begin() const noexcept;
        Mappers::const_iterator cbegin() const noexcept;

        Mappers::iterator end() noexcept;
        Mappers::const_iterator end() const noexcept;
        Mappers::const_iterator cend() const noexcept;

        constexpr bool empty() const noexcept;
        constexpr Mappers::size_type size() const noexcept;
        //======================================================================

        private:
        Mappers mappers;
        
        juce::ReadWriteLock mutex;
        std::atomic<Channel> outputChannel;        
        juce::String name;

       JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiChannelMapperBank)
    };
}