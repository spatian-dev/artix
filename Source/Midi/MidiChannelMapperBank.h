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
#include <sstream>
#include "../Id/Identifiers.h"
#include "../Error/Error.h"
#include "../Utils/CallbackList.h"
#include "Midi.h"
#include "MidiChannelMapper.h"

namespace Artix::Midi {
    class MidiChannelMapperBank {
        private:
        using Mappers = std::array<MidiChannelMapper, CHANNEL_COUNT>;

        public:
        using NameChangedCallback = Utils::CallbackList<juce::String>;
        using ChannelChangedCallback = Utils::CallbackList<Channel>;

        MidiChannelMapperBank(Channel outputChannel = Channel::First, juce::String name = "untitled");

        Channel getOutputChannel() const noexcept;
        void setOutputChannel(Channel v, bool muteCallbacks = false) noexcept;
        void setOutputChannel(int v, bool muteCallbacks = false) noexcept;

        const juce::String getName() const noexcept;
        void setName(juce::String v, bool muteCallbacks = false) noexcept;

        juce::ValueTree toValueTree() const noexcept;
        void fromValueTree(const juce::ValueTree& vt) noexcept;

        const juce::String getDescription() const;

        NameChangedCallback onNameChanged;
        ChannelChangedCallback onOutputChannelChanged;
        Error::ErrorCallback onError;

        //======================================================================
        /** Mirroring std::array container functions */
        inline Mappers::reference at(Mappers::size_type pos) {
            return mappers.at(pos);
        }
        inline Mappers::const_reference at(Mappers::size_type pos) const {
            return mappers.at(pos);
        }

        inline Mappers::reference operator[](Mappers::size_type pos) {
            return mappers.operator[](pos);
        }
        inline Mappers::const_reference operator[](Mappers::size_type pos) const {
            return mappers.operator[](pos);
        }

        inline Mappers::iterator begin() noexcept {
            return mappers.begin();
        }
        inline Mappers::const_iterator begin() const noexcept {
            return mappers.begin();
        }
        inline Mappers::const_iterator cbegin() const noexcept {
            return mappers.cbegin();
        }

        inline Mappers::iterator end() noexcept {
            return mappers.end();
        }
        inline Mappers::const_iterator end() const noexcept {
            return mappers.end();
        }
        inline Mappers::const_iterator cend() const noexcept {
            return mappers.cend();
        }

        constexpr bool empty() const noexcept {
            return mappers.empty();
        }
        constexpr Mappers::size_type size() const noexcept {
            return mappers.size();
        }
        //======================================================================

        private:
        Mappers mappers;
        
        juce::ReadWriteLock mutex;
        std::atomic<Channel> outputChannel;        
        juce::String name;

       JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiChannelMapperBank)
    };
}