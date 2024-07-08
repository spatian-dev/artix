/*
  ==============================================================================

    AppState.h
    Created: 28 Jun 2024 5:16:36pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <atomic>
#include "Identifiers.h"
#include "Errors.h"
#include "Midi/MidiChannelMapperBank.h"

namespace Artix {
    class AppState {
        public:
        AppState(int width = 660, int height = 880);
        
        int getWidth() const noexcept;
        void setWidth(int v, bool muteCallbacks = false) noexcept;

        int getHeight() const noexcept;
        void setHeight(int v, bool muteCallbacks = false) noexcept;

        void setSize(int width, int height, bool muteCallbacks = false) noexcept;

        Midi::MidiChannelMapperBank& getMapperBank() noexcept;

        juce::ValueTree toValueTree() const noexcept;
        void fromValueTree(const juce::ValueTree& vt) noexcept;

        std::function<void(int, int)> onSizeChanged;
        Error::ErrorCallback onError;

        private:
        std::atomic<int> width;
        std::atomic<int> height;
        Midi::MidiChannelMapperBank mapperBank;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppState)
    };
}