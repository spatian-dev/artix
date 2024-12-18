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
        using DirtyChangedCallback = Utils::CallbackList<bool>;
        using ChannelChangedCallback = Utils::CallbackList<Channel>;

        MidiChannelMapperBank(Channel outputChannel = Channel::First);
        ~MidiChannelMapperBank();

        Channel getOutputChannel() const noexcept;
        void setOutputChannel(Channel v, bool muteCallbacks = false);
        void setOutputChannel(int v, bool muteCallbacks = false);
        
        bool getIsDirty() const noexcept;
        void setIsDirty(bool v, bool muteCallbacks = false);

        juce::ValueTree toValueTree() const;
        bool fromValueTree(const juce::ValueTree& vt, bool muteCallbacks = false);

        juce::var toVar() const;
        bool fromVar(const juce::var& vt, bool muteCallbacks = false);

        const juce::String getDescription() const;

        ChannelChangedCallback onOutputChannelChanged;
        DirtyChangedCallback onDirtyChanged;
        std::array<DirtyChangedCallback::Identifier, CHANNEL_COUNT> mapperDirtyChangedIds;
        Error::ErrorCallback onError;
        std::array<Error::ErrorCallback::Identifier, CHANNEL_COUNT> mapperErrorIds;

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
        std::atomic<bool> isDirty = false;
        std::atomic<bool> muteDirty = false;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiChannelMapperBank)
    };

    inline bool operator==(const MidiChannelMapperBank& left, const MidiChannelMapperBank& right) {
        if (
            (left.size() != right.size()) ||
            (left.getOutputChannel() != right.getOutputChannel())
        ) {
            return false;
        }
        for (size_t i = 0; i < left.size(); i++) {
            if (left[i] != right[i]) {
                return false;
            }
        }

        return true;
    }

    inline bool operator!=(const MidiChannelMapperBank& left, const MidiChannelMapperBank& right) {
        return !(left == right);
    }
}