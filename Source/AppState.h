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
#include "Utils/CallbackList.h"
#include "Id/Identifiers.h"
#include "Error/Error.h"
#include "Ui/Theme/Themes.h"
#include "Midi/MidiChannelMapperBank.h"

namespace Artix {
    class AppState {
        public:
        using HeightChangedCallbacks = Utils::CallbackList<int>;
        using ThemeChangedCallbacks = Utils::CallbackList<Ui::Theme::ThemePtr>;

        AppState(int height = 800);
        
        int getHeight() const noexcept;
        void setHeight(int v, bool muteCallbacks = false) noexcept;

        Ui::Theme::ThemePtr getTheme() const noexcept;
        void setTheme(Ui::Theme::ThemePtr v, bool muteCallbacks = false) noexcept;
        
        Midi::MidiChannelMapperBank& getMapperBank() noexcept;

        juce::ValueTree toValueTree() const noexcept;
        void fromValueTree(const juce::ValueTree& vt) noexcept;

        HeightChangedCallbacks onHeightChanged;
        ThemeChangedCallbacks onThemeChanged;
        Error::ErrorCallbacks onError;

        private:
        std::atomic<int> height = 0;
        Midi::MidiChannelMapperBank mapperBank;

        juce::ReadWriteLock themeMutex;
        Ui::Theme::ThemePtr theme;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppState)
    };
}