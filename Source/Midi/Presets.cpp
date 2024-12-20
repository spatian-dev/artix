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
        const std::array<const char*, 40> factoryPresets = {
            "default_artix",
            "Session_Horns_Pro__Keyswitch_artix",
            "Session_Strings_Pro_2__Basses_artix",
            "Session_Strings_Pro_2__Celli_artix",
            "Session_Strings_Pro_2__Violas_artix",
            "Session_Strings_Pro_2__Violins_artix",
            "Spitfire_Symphony_Orchestra__Curated_Brass_Ensembles_artix",
            "Spitfire_Symphony_Orchestra__Curated_String_Ensembles_artix",
            "Spitfire_Symphony_Orchestra__Curated_Tutti_Ensembles_artix",
            "Spitfire_Symphony_Orchestra__Curated_Woodwind_Ensembles_artix",
            "Symphony_Series_Brass_Ensemble__Brass_Ensemble_artix",
            "Symphony_Series_Brass_Ensemble__Horns_artix",
            "Symphony_Series_Brass_Ensemble__Trombones_artix",
            "Symphony_Series_Brass_Ensemble__Trumpets_artix",
            "Symphony_Series_Brass_Ensemble__Tubas_artix",
            "Symphony_Series_Brass_Solo__Brass_Quartet_artix",
            "Symphony_Series_Brass_Solo__Horn_1_artix",
            "Symphony_Series_Brass_Solo__Horn_2_artix",
            "Symphony_Series_Brass_Solo__Trombone_artix",
            "Symphony_Series_Brass_Solo__Trumpet_artix",
            "Symphony_Series_Brass_Solo__Tuba_artix",
            "Symphony_Series_String_Ensemble__Basses_artix",
            "Symphony_Series_String_Ensemble__Cellos_artix",
            "Symphony_Series_String_Ensemble__String_Ensemble_artix",
            "Symphony_Series_String_Ensemble__Violas_artix",
            "Symphony_Series_String_Ensemble__Violins_artix",
            "Symphony_Series_Woodwind_Ensemble__Bass_Winds_artix",
            "Symphony_Series_Woodwind_Ensemble__Bassoons_artix",
            "Symphony_Series_Woodwind_Ensemble__Clarinets_artix",
            "Symphony_Series_Woodwind_Ensemble__Flutes_artix",
            "Symphony_Series_Woodwind_Ensemble__Oboes_artix",
            "Symphony_Series_Woodwind_Ensemble__Saxophones_artix",
            "Symphony_Series_Woodwind_Ensemble__Woodwind_Ensemble_artix",
            "Symphony_Series_Woodwind_Solo__Bassoon_artix",
            "Symphony_Series_Woodwind_Solo__Clarinet_artix",
            "Symphony_Series_Woodwind_Solo__Contrabassoon_artix",
            "Symphony_Series_Woodwind_Solo__Flute_artix",
            "Symphony_Series_Woodwind_Solo__Oboe_artix",
            "Symphony_Series_Woodwind_Solo__Saxophone_artix",
            "Symphony_Series_Woodwind_Solo__Woodwind_Quintet_artix",
        };
        std::optional<PresetPtr> preset;
        for (auto& presetFile : factoryPresets) {
            if ((preset = makeFactoryPreset(presetFile)).has_value()) {
                items.add(preset.value());
            }
        }

        userPresetStartIndex = items.size();
        refreshUserPresets(dataDirectory);
    }

    Presets::~Presets() {}

    void Presets::refreshUserPresets(juce::File dataDirectory) {
        items.removeRange(userPresetStartIndex, items.size() - userPresetStartIndex);

        const auto files = dataDirectory.findChildFiles(
            juce::File::TypesOfFileToFind::findFiles, true, "*" + extension, juce::File::FollowSymlinks::no
        );

        std::optional<PresetPtr> preset;
        for (const auto& f : files) {
            preset = makePreset(f.loadFileAsString());

            if (preset.has_value()) {
                items.add(preset.value());
            }
        }
    }

    void Presets::add(const juce::String content) {
        std::optional<PresetPtr> preset;
        if ((preset = makePreset(content)).has_value()) {
            items.add(preset.value());
        }
    }

    PresetPtr Presets::getNextPreset(int current, bool wrap) const {
        if (current >= items.size() - 1) {
            return wrap ? getPreset(0) : nullptr;
        }
        return  getPreset(current + 1);
    }

    PresetPtr Presets::getPreviousPreset(int current, bool wrap) const {
        if (current <= 0) {
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
            if (item->state->getName() == name)
                return i;
            i++;
        }
        return -1;
    }

    int Presets::indexOf(const State& state) const {
        int i = 0;
        for (const auto& item : items) {
            if (*item->state == state)
                return i;
            i++;
        }
        return -1;
    }

    bool Presets::contains(const PresetPtr& item) const {
        return items.contains(item);
    }

    bool Presets::contains(const State& state) const {
        return indexOf(state) > -1;
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

    std::optional<PresetPtr> Presets::makeUserPreset(const juce::String content) {
        return makePreset(content, false);
    }

    std::optional<PresetPtr> Presets::makeFactoryPreset(const char* resourceName) {
        int size = 0;
        const auto data = BinaryData::getNamedResource(resourceName, size);
        if (data == nullptr)
            return {};
        return Presets::makePreset(juce::String::createStringFromData(data, size), true
        );
    }

    std::optional<PresetPtr> Presets::makePreset(juce::String content, bool isFactory) {
        auto state = std::make_shared<State>();
        if (!state->fromJson(content))
            return {};

        return std::make_shared<Preset>(state, isFactory);
    }
}