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

        juce::String getLastOpenFolder() const;
        void setLastOpenFolder(juce::String v);

        juce::String getLastSaveFolder() const;
        void setLastSaveFolder(juce::String v);

        HeightChangedCallbacks onHeightChanged;
        ThemeChangedCallbacks onThemeChanged;
        
        private:
        Settings& settings;
        std::atomic<int> height;

        juce::ReadWriteLock themeMutex;
        Ui::Theme::ThemePtr theme;

        juce::String lastOpenFolder;
        juce::ReadWriteLock lastOpenFolderMutex;

        juce::String lastSaveFolder;
        juce::ReadWriteLock lastSaveFolderMutex;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginState)
    };
}