/*
  ==============================================================================

    Presets.cpp
    Created: 14 Dec 2024 11:55:41pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "Presets.h"

/* Warning: Non-thread safe. Call only from message thread. */

namespace Artix::Midi {
    Presets::Presets(juce::File dataDirectory) {
        items.add(makeFactoryPreset("(default)", "default_json"));
        items.add(makeFactoryPreset("My Awesome Preset", "My_Awesome_Preset_json"));

        userPresetStartIndex = items.size();
        refreshUserPresets(dataDirectory);
    }

    Presets::~Presets() {}

    void Presets::refreshUserPresets(juce::File dataDirectory) {
        items.removeRange(userPresetStartIndex, items.size() - userPresetStartIndex);

        const auto files = dataDirectory.findChildFiles(
            juce::File::TypesOfFileToFind::findFiles, true, "*" + extension, juce::File::FollowSymlinks::no
        );
        for (const auto& f : files) {
            items.add(makePreset(
                f.getRelativePathFrom(dataDirectory).trimCharactersAtEnd(extension),
                f.loadFileAsString(),
                f
            ));
        }
    }

    void Presets::add(const juce::String name, const juce::String content) {
        items.add(makePreset(name, content, juce::File()));
    }

    PresetPtr Presets::getNextPreset(int current, bool wrap) const {
        if (current >= items.size() - 1) {
            return wrap ? getPreset(0) : nullptr;
        }
        return  getPreset(current + 1);
    }

    PresetPtr Presets::getPreviousPreset(int current, bool wrap) const {
        if (current <= 1) {
            return wrap ? getPreset(items.size() - 1) : nullptr;
        }
        return  getPreset(current - 1);
    }

    PresetPtr Presets::getPreset(int index) const {
        return ((index < 0) || (index >= items.size())) ? nullptr : items.getUnchecked(index);
    }

    PresetPtr Presets::operator[] (int index) const {
        return getPreset(index);
    }

    int Presets::size() const noexcept {
        return items.size();
    }

    int Presets::indexOf(const PresetPtr& item) const {
        return items.indexOf(item);
    }

    int Presets::indexOf(const juce::String name) const {
        int i = 0;
        for (const auto& item : items) {
            if (item->name == name)
                return i;
            i++;
        }
        return -1;
    }

    bool Presets::contains(const PresetPtr& item) const {
        return items.contains(item);
    }

    bool Presets::contains(const juce::String name) const {
        return indexOf(name) > -1;
    }
    
    PresetPtr* Presets::begin() {
        return items.begin();
    }

    const PresetPtr* Presets::begin() const {
        return items.begin();
    }

    PresetPtr* Presets::end() {
        return items.end();
    }

    const PresetPtr* Presets::end() const {
        return items.end();
    }

    int Presets::factoryPresetCount() const {
        return userPresetStartIndex;
    }

    PresetPtr Presets::makeUserPreset(const juce::String name, const juce::String content) {
        return Presets::makePreset(name, content, juce::File(), false);
    }

    PresetPtr Presets::makeFactoryPreset(const juce::String name, const char* resourceName) {
        int size = 0;
        const auto data = BinaryData::getNamedResource(resourceName, size);
        return Presets::makePreset(
            name,
            data != nullptr ? juce::String::createStringFromData(data, size) : "{}",
            juce::File(),
            true
        );
    }

    PresetPtr Presets::makePreset(juce::String name, juce::String content, juce::File file, bool isFactory) {
        return std::make_shared<Preset>(name, content, file, isFactory);
    }
}