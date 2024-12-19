/*
  ==============================================================================

    PluginState.h
    Created: 18 Dec 2024 9:43:53pm
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
#include "Settings.h"
#include "State.h"

namespace Artix {
    class PluginState: public State {
        public:
        using HeightChangedCallbacks = Utils::CallbackList<int>;
        using ThemeChangedCallbacks = Utils::CallbackList<Ui::Theme::ThemePtr>;

        PluginState(Settings& settings, juce::String name = "(unnamed)");
        ~PluginState();

        int getHeight() const noexcept;
        void setHeight(int v, bool muteCallbacks = false);

        Ui::Theme::ThemePtr getTheme() const;
        void setTheme(Ui::Theme::ThemePtr v, bool muteCallbacks = false);

        HeightChangedCallbacks onHeightChanged;
        ThemeChangedCallbacks onThemeChanged;
        
        private:
        Settings& settings;
        std::atomic<int> height;

        juce::ReadWriteLock themeMutex;
        Ui::Theme::ThemePtr theme;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginState)
    };

    /*inline bool operator==(const State& left, const State& right) {
        return (left.getName() == right.getName()) &&
            (left.getHeight() == right.getHeight()) &&
            (left.getMapperBank() == right.getMapperBank());
    };
    inline bool operator!=(const State& left, const State& right) {
        return !(left == right);
    };*/
}