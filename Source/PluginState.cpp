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
}