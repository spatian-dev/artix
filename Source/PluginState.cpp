/*
  ==============================================================================

    PluginState.cpp
    Created: 18 Dec 2024 9:43:53pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "PluginState.h"

namespace Artix {
    PluginState::PluginState(Settings& settings, juce::String name): State(name), settings(settings) {
        height = settings.getWindowHeight();
        
        lastOpenFolder = this->settings.getDataDirectory().getFullPathName();
        lastSaveFolder = this->settings.getDataDirectory().getFullPathName();

        juce::ScopedWriteLock lock(themeMutex);
        theme = Ui::Theme::Themes::getInstance().tryFind(
            settings.getThemeName(),
            Ui::Theme::Themes::getInstance().begin()->second
        );
    }

    PluginState::~PluginState() {}

    int PluginState::getHeight() const noexcept {
        return height;
    }

    void PluginState::setHeight(int v, bool muteCallbacks) {
        if (height == v) return;
        height = v;

        settings.setWindowHeight(height);

        if (muteCallbacks) return;
        onHeightChanged.callSafely(height);
    }

    Ui::Theme::ThemePtr PluginState::getTheme() const {
        juce::ScopedReadLock lock(themeMutex);
        return theme;
    }

    void PluginState::setTheme(Ui::Theme::ThemePtr v, bool muteCallbacks) {
        juce::ScopedWriteLock lock(themeMutex);
        if (theme.get() == v.get()) return;
        theme = v;
        
        settings.setThemeName(theme->getName());

        if (muteCallbacks) return;
        onThemeChanged.callSafely(theme);
    }

    juce::String PluginState::getLastOpenFolder() const {
        juce::ScopedReadLock lock(lastOpenFolderMutex);
        return lastOpenFolder;
    }

    void PluginState::setLastOpenFolder(juce::String v) {
        juce::ScopedWriteLock lock(lastOpenFolderMutex);
        if (lastOpenFolder == v) return;
        lastOpenFolder = v;
    }

    juce::String PluginState::getLastSaveFolder() const {
        juce::ScopedReadLock lock(lastSaveFolderMutex);
        return lastSaveFolder;
    }

    void PluginState::setLastSaveFolder(juce::String v) {
        juce::ScopedWriteLock lock(lastSaveFolderMutex);
        if (lastSaveFolder == v) return;
        lastSaveFolder = v;
    }
}