/*
  ==============================================================================

    Header.cpp
    Created: 1 Jul 2024 3:43:46pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "Header.h"

namespace Artix::Ui {
    Header::Header(PluginState& state, Midi::Presets& presets, Theme::ThemePtr theme, Settings& settings):
        Themable(theme), settings(settings), state(state), presets(presets) {
        setTheme(theme);

        prevPresetButtonClickedId = prevPresetButton.onClick.add([this](juce::MouseEvent) {
            switchPreset(this->presets.getPreviousPreset(this->presets.indexOf(this->state)));
        });
        addAndMakeVisible(prevPresetButton);

        nextPresetButtonClickedId = nextPresetButton.onClick.add([this](juce::MouseEvent) {
            switchPreset(this->presets.getNextPreset(this->presets.indexOf(this->state)));
        });
        addAndMakeVisible(nextPresetButton);

        saveButton.setAutoSize(Button::AutoSize::AutoSizeWidth);
        saveButtonClickedId = saveButton.onClick.add([this](juce::MouseEvent) {
            auto dlg = juce::FileChooser(
                "Save preset as", this->state.getLastSaveFolder(),
                "*" + Midi::Presets::extension, true, false, this
            );
            if (!dlg.browseForFileToSave(true))
                return;

            const auto target = dlg.getResult();
            this->state.setLastSaveFolder(target.getFullPathName());

            const auto result = target.replaceWithText(this->state.toJson());
            if (!result) {
                juce::NativeMessageBox::showMessageBox(
                    juce::MessageBoxIconType::WarningIcon,
                    "Something went wrong", "Failed to save preset.", this
                );
                return;
            }
            this->state.setIsDirty(false);
            this->presets.refreshUserPresets(this->settings.getDataDirectory());
        });
        addAndMakeVisible(saveButton);

        loadButton.setAutoSize(Button::AutoSize::AutoSizeWidth);
        loadButtonClickedId = loadButton.onClick.add([this](juce::MouseEvent) {
            auto dlg = juce::FileChooser(
                "Load preset from", this->state.getLastOpenFolder(),
                "*" + Midi::Presets::extension, true, false, this
            );
            if (!dlg.browseForFileToOpen())
                return;

            const auto target = dlg.getResult();
            this->state.setLastOpenFolder(target.getFullPathName());

            auto state = State();
            if (!state.fromJson(target.loadFileAsString(), true)) {
                juce::NativeMessageBox::showMessageBox(
                    juce::MessageBoxIconType::WarningIcon,
                    "Something went wrong", "Failed to load preset.", this
                );
                return;
            }
            switchState(state);
            this->presets.refreshUserPresets(this->settings.getDataDirectory());
        });
        addAndMakeVisible(loadButton);

        settingsButton.setAutoSize(Button::AutoSize::AutoSizeWidth);
        settingsButtonClickedId = settingsButton.onClick.add([this](juce::MouseEvent) {
            settingsMenu().showMenuAsync(juce::PopupMenu::Options{}.withTargetComponent(settingsButton));
        });
        addAndMakeVisible(settingsButton);

        const auto safeThis = juce::Component::SafePointer<Header>(this);

        presetLabel.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        presetLabel.setEditable(false, true);
        stateNameChangedId = this->state.onNameChanged.add([safeThis](const juce::String name) {
            if (safeThis != nullptr)
                safeThis->setPresetName(name);
        });
        setPresetName(this->state.getName());
        presetLabel.onTextChange = [this]() {
            this->state.setName(presetLabel.getText());
        };
        presetLabelClickedId = presetLabel.onClick.add([this](juce::MouseEvent) {
            presetsMenu().showMenuAsync(juce::PopupMenu::Options{}.withTargetComponent(presetLabel));
        });
        addAndMakeVisible(presetLabel);

        stateDirtyChangedId = this->state.onDirtyChanged.add([safeThis](const bool isDirty) {
            if (safeThis == nullptr)
                return;

            safeThis->presetLabel.setColour(juce::Label::textColourId, safeThis->theme->getUIColor(
                isDirty ? UIColor::TEXT_PRIMARY : UIColor::TEXT_ELEMENT
            ));
            safeThis->resized();
        });
    }

    Header::~Header() {
        if (prevPresetButtonClickedId)
            prevPresetButton.onClick.remove(prevPresetButtonClickedId.value());

        if (nextPresetButtonClickedId)
            nextPresetButton.onClick.remove(nextPresetButtonClickedId.value());

        if (saveButtonClickedId)
            saveButton.onClick.remove(saveButtonClickedId.value());

        if (loadButtonClickedId)
            loadButton.onClick.remove(loadButtonClickedId.value());

        if (settingsButtonClickedId)
            settingsButton.onClick.remove(settingsButtonClickedId.value());

        if (presetLabelClickedId)
            presetLabel.onClick.remove(presetLabelClickedId.value());

        if (stateNameChangedId)
            state.onNameChanged.remove(stateNameChangedId.value());

        if (stateDirtyChangedId)
            state.onDirtyChanged.remove(stateDirtyChangedId.value());
    }

    void Header::paint(juce::Graphics& g) {
        theme->drawRounderContainer(
            g, getLocalBounds().toFloat(), true, Metric::SMALL, Metric::TINY,
            UIColor::BORDER_MUTED, UIColor::BACKGROUND_MUTED
        );
    }

    void Header::resized() {
        innerArea = theme->getInnerArea(this, Metric::SMALL, Metric::TINY);

        const auto intInnerArea = innerArea.toNearestIntEdges();
        const auto top = intInnerArea.getY();
        const auto height = intInnerArea.getHeight();

        prevPresetButton.setBounds(intInnerArea.getX(), top, height, height);

        const auto padding = (int) theme->getSpacing(Metric::TINY);

        settingsButton.setBounds(
            intInnerArea.getRight() - settingsButton.getWidth(),
            top, height, height
        );

        loadButton.setBounds(
            settingsButton.getX() - (loadButton.getWidth() + padding),
            top, height, height
        );

        saveButton.setBounds(
            loadButton.getX() - (saveButton.getWidth() + padding),
            top, height, height
        );

        nextPresetButton.setBounds(
            saveButton.getX() - (height + padding), top, height, height
        );

        presetLabel.setFont(juce::FontOptions(theme->getFontSize(Metric::MEDIUM), juce::Font::plain));
        presetLabel.setBounds(
            prevPresetButton.getRight() + padding, top,
            nextPresetButton.getX() - (prevPresetButton.getRight() + (2 * padding)), height
        );
    }

    void Header::setTheme(Theme::ThemePtr v) noexcept {
        Themable::setTheme(v);
        prevPresetButton.setTheme(v);
        presetLabel.setColour(juce::Label::textColourId, this->theme->getUIColor(
            state.getIsDirty() ? UIColor::TEXT_PRIMARY : UIColor::TEXT_ELEMENT
        ));

        presetLabel.setColour(juce::Label::textWhenEditingColourId, theme->getUIColor(UIColor::TEXT_ELEMENT));
        resized();
        repaint();
    }

    juce::PopupMenu Header::settingsMenu() {
        const auto safeThis = juce::Component::SafePointer<Header>(this);
        juce::PopupMenu themesMenu;
        for (const auto& t : Theme::Themes::getInstance()) {
            themesMenu.addItem(t.first, true, t.second == state.getTheme(), [safeThis, t]() {
                if (safeThis == nullptr)
                    return;

                safeThis->state.setTheme(t.second);
                safeThis->settings.save();
            });
        }

        juce::PopupMenu settingsMenu;
        settingsMenu.addSubMenu("Theme", themesMenu);
        settingsMenu.addSeparator();
        settingsMenu.addItem("Open Data Folder", [safeThis]() {
            if (safeThis == nullptr)
                return;
            safeThis->settings.getDataDirectory().revealToUser();
        });
        settingsMenu.addItem("Change Data Folder", [safeThis]() {
            if (safeThis == nullptr)
                return;

            auto dlg = juce::FileChooser(
                "Choose data folder", safeThis->settings.getDataDirectory(),
                juce::String(), true, false, safeThis
            );
            if (!dlg.browseForDirectory())
                return;
            const auto result = dlg.getResult();
            safeThis->settings.setDataDirectory(result);
            safeThis->settings.save();
            safeThis->presets.refreshUserPresets(result);
        });
        settingsMenu.addItem("Rescan Data Folder", [safeThis]() {
            if (safeThis == nullptr)
                return;
            safeThis->presets.refreshUserPresets(safeThis->settings.getDataDirectory());
        });
        return settingsMenu;
    }

    juce::PopupMenu Header::presetsMenu() {
        juce::PopupMenu factoryPresetsMenu;
        juce::PopupMenu userPresetsMenu;

        const auto safeThis = juce::Component::SafePointer<Header>(this);

        for (auto& preset : presets) {
            bool isCurrent = *preset->state == state;
            auto menu = preset->isFactory ? &factoryPresetsMenu : &userPresetsMenu;
            menu->addItem(preset->state->getName(), true, isCurrent, [safeThis, preset, isCurrent]() {
                if (safeThis == nullptr)
                    return;

                if (!isCurrent) safeThis->switchPreset(preset);
            });
        }

        juce::PopupMenu presetsMenu;
        presetsMenu.addSubMenu("Factory", factoryPresetsMenu);
        presetsMenu.addSubMenu("User", userPresetsMenu);
        return presetsMenu;
    }

    void Header::switchState(const State& newState) {
        state.fromState(newState);
        setPresetName(state.getName());
        resized();
    }

    void Header::switchPreset(const Midi::PresetPtr preset) {
        switchState(*preset->state);
    }

    void Header::setPresetName(juce::String v) {
        this->presetLabel.setText(v, juce::NotificationType::dontSendNotification);
    }
}