/*
  ==============================================================================

    Presets.h
    Created: 14 Dec 2024 11:55:41pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <map>
#include <memory>
#include <JuceHeader.h>

/* Warning: Non-thread safe. Call only from message thread. */

namespace Artix::Midi {
    struct Preset {
        juce::String name;
        juce::String data;
        juce::File file;
        bool isFactory = false;
    };

    using PresetPtr = std::shared_ptr<Preset>;
    using PresetList = juce::Array<PresetPtr>;

    class Presets {
        public:
        Presets(const juce::File dataDirectory);
        ~Presets();

        void refreshUserPresets(const juce::File dataDirectory);

        void add(const juce::String name, const juce::String content);

        PresetPtr getNextPreset(int current, bool wrap = true) const;
        PresetPtr getPreviousPreset(int current, bool wrap = true) const;
        PresetPtr getPreset(int index) const;
        PresetPtr operator[] (int index) const;

        int size() const noexcept;
        int indexOf(const PresetPtr& item) const;
        int indexOf(const juce::String name) const;
        bool contains(const PresetPtr& item) const;
        bool contains(const juce::String name) const;
        PresetPtr* begin();
        const PresetPtr* begin() const;
        PresetPtr* end();
        const PresetPtr* end() const;

        int factoryPresetCount() const;

        private:
        static PresetPtr makeFactoryPreset(const juce::String name, const char* content);
        static PresetPtr makePreset(
            const juce::String name, const juce::String content,
            const juce::File file, const bool isFactory = false
        );

        int userPresetStartIndex = 0;
        const juce::String extension = "artix";
        PresetList items;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Presets)
    };
}