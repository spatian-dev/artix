/*
  ==============================================================================

    Header.cpp
    Created: 1 Jul 2024 3:43:46pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "Header.h"

namespace Artix::Ui {
    Header::Header(State& state, Midi::Presets& presets, Theme::ThemePtr theme, Settings& settings) :
        Themable(theme), settings(settings), state(state), presets(presets) {
        setTheme(theme);

        prevPresetButtonClickedId = prevPresetButton.onClick.add([this](juce::MouseEvent evt) {
            switchPreset(this->presets.getPreviousPreset(this->presets.indexOf(this->state)));
        });
        addAndMakeVisible(prevPresetButton);

        nextPresetButtonClickedId = nextPresetButton.onClick.add([this](juce::MouseEvent evt) {
            switchPreset(this->presets.getNextPreset(this->presets.indexOf(this->state)));
        });
        addAndMakeVisible(nextPresetButton);

        saveButton.setAutoSize(Button::AutoSize::AutoSizeWidth);
        saveButtonClickedId = saveButton.onClick.add([](juce::MouseEvent evt) {
            DBG("Save");
        });
        addAndMakeVisible(saveButton);

        loadButton.setAutoSize(Button::AutoSize::AutoSizeWidth);
        loadButtonClickedId = loadButton.onClick.add([](juce::MouseEvent evt) {
            DBG("Load");
        });
        addAndMakeVisible(loadButton);

        settingsButton.setAutoSize(Button::AutoSize::AutoSizeWidth);
        settingsButtonClickedId = settingsButton.onClick.add([this](juce::MouseEvent evt) {
            settingsMenu().showMenuAsync(juce::PopupMenu::Options{}.withTargetComponent(settingsButton));
        });
        addAndMakeVisible(settingsButton);
        
        presetLabel.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        presetLabel.setEditable(false, true);
        setPresetName(state.getName());
        presetLabel.onTextChange = [this]() {
            this->state.setName(presetLabel.getText());
        };
        presetLabelClickedId = presetLabel.onClick.add([this](juce::MouseEvent evt) {
            presetsMenu().showMenuAsync(juce::PopupMenu::Options{}.withTargetComponent(presetLabel));
        });
        addAndMakeVisible(presetLabel);

        stateDirtyChangedId = state.onDirtyChanged.add([this](const bool isDirty) {
            presetLabel.setColour( juce::Label::textColourId, this->theme->getUIColor(
                isDirty ? UIColor::TEXT_PRIMARY : UIColor::TEXT_ELEMENT
            ));
            resized();
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

        if (stateDirtyChangedId)
            state.onDirtyChanged.remove(stateDirtyChangedId.value());

        if (presetLabelClickedId)
            presetLabel.onClick.remove(presetLabelClickedId.value());
    }

    void Header::paint(juce::Graphics& g) {
        theme->drawRounderContainer(
            this, g, getLocalBounds().toFloat(), true, Metric::SMALL, Metric::TINY,
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
        presetLabel.setColour(juce::Label::textColourId, theme->getUIColor(UIColor::TEXT_ELEMENT));
        presetLabel.setColour(juce::Label::textWhenEditingColourId, theme->getUIColor(UIColor::TEXT_ELEMENT));
        resized();
        repaint();
    }
    
    juce::PopupMenu Header::settingsMenu() {
        juce::PopupMenu themesMenu;        
        for (const auto& t : Theme::Themes::getInstance()) {
            themesMenu.addItem(t.first, true, t.second == state.getTheme(), [this, t]() {
                state.setTheme(t.second);
            });
        }

        juce::PopupMenu settingsMenu;
        settingsMenu.addSubMenu("Theme", themesMenu);
        settingsMenu.addItem("Change Data Folder", [this]() {
            auto dlg = juce::FileChooser(
                "Choose data folder", settings.getDataDirectory()
            );
            if (!dlg.browseForDirectory())
                return;
            settings.setDataDirectory(dlg.getResult());
            presets.refreshUserPresets(settings.getDataDirectory());
        });
        return settingsMenu;
    }

    juce::PopupMenu Header::presetsMenu() {
        juce::PopupMenu factoryPresetsMenu;
        juce::PopupMenu userPresetsMenu;
        for (auto& preset : presets) {
            bool isCurrent = *preset->state == state;
            auto menu = preset->isFactory ? &factoryPresetsMenu : &userPresetsMenu;
            menu->addItem(
                preset->state->getName(), true, isCurrent, [this, preset, isCurrent]() {
                if (!isCurrent) switchPreset(preset);
            }
            );
        }

        juce::PopupMenu presetsMenu;
        presetsMenu.addSubMenu("Factory", factoryPresetsMenu);
        presetsMenu.addSubMenu("User", userPresetsMenu);
        return presetsMenu;
    }

    void Header::switchPreset(const Midi::PresetPtr preset) {
        this->state.fromState(*preset->state);
        setPresetName(this->state.getName());
        resized();
    }

    void Header::setPresetName(juce::String v) {
        this->presetLabel.setText(v, juce::NotificationType::dontSendNotification);
    }
}