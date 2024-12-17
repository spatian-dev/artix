/*
  ==============================================================================

    Header.cpp
    Created: 1 Jul 2024 3:43:46pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "Header.h"

namespace Artix::Ui {
    Header::Header(ArtixAudioProcessor& proc, AppState& state, Midi::Presets& presets, Theme::ThemePtr theme) :
        audioProcessor(proc), Themable(theme), state(state), presets(presets) {
        setTheme(theme);

        prevPresetButtonClickedId = prevPresetButton.onClick.add([this](juce::MouseEvent evt) {
            this->state.load(
                this->presets.getPreviousPreset(
                    this->presets.indexOf(this->state.getCurrentPreset())
                )
            );
            setPresetName(this->state.getCurrentPreset()->name);
        });
        addAndMakeVisible(prevPresetButton);

        nextPresetButtonClickedId = nextPresetButton.onClick.add([this](juce::MouseEvent evt) {
            this->state.load(
                this->presets.getNextPreset(
                    this->presets.indexOf(this->state.getCurrentPreset())
                )
            );
            setPresetName(this->state.getCurrentPreset()->name);
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

        dataFolderButton.setAutoSize(Button::AutoSize::AutoSizeWidth);
        dataFolderButtonClickedId = dataFolderButton.onClick.add([](juce::MouseEvent evt) {
            DBG("Data folder");
        });
        addAndMakeVisible(dataFolderButton);

        presetName.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        presetName.setEditable(false, true); 
        setPresetName(state.getCurrentPreset()->name);
        presetName.onTextChange = [this]() {
            this->state.getCurrentPreset()->name = this->presetName.getText();
            this->audioProcessor.notifyHostOfChange();
        };
        addAndMakeVisible(presetName);
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

        if (dataFolderButtonClickedId)
            dataFolderButton.onClick.remove(dataFolderButtonClickedId.value());
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

        dataFolderButton.setBounds(
            intInnerArea.getRight() - dataFolderButton.getWidth(),
            top, height, height
        );

        loadButton.setBounds(
            dataFolderButton.getX() - (loadButton.getWidth() + padding),
            top, height, height
        );

        saveButton.setBounds(
            loadButton.getX() - (saveButton.getWidth() + padding),
            top, height, height
        );

        nextPresetButton.setBounds(
            saveButton.getX() - (height + padding), top, height, height
        );

        presetName.setBounds(
            prevPresetButton.getRight() + padding, top,
            nextPresetButton.getX() - (prevPresetButton.getRight() + (2 * padding)), height
        );
    }
    void Header::setTheme(Theme::ThemePtr v) noexcept {
        Themable::setTheme(v);
        prevPresetButton.setTheme(v);
        presetName.setColour(juce::Label::textColourId, theme->getUIColor(UIColor::TEXT_ELEMENT));
        presetName.setColour(juce::Label::textWhenEditingColourId, theme->getUIColor(UIColor::TEXT_ELEMENT));
        resized();
        repaint();
    }
    void Header::setPresetName(juce::String v) {
        this->presetName.setText(v, juce::NotificationType::dontSendNotification);
    }
}