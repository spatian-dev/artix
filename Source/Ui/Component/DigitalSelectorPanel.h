/*
  ==============================================================================

	DigitalSelectorPanel.h
	Created: 5 Jul 2024 11:36:09pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Theme/Themes.h"
#include "../../Utils/CallbackList.h"
#include "DigitalSelector.h"

namespace Artix::Ui::Component {
	enum class DigitalSelectorPanelDirection {
		HORIZONTAL,
		VERTICAL,
	};

	template <typename T> requires std::is_integral_v<T>
	class DigitalSelectorPanel : public juce::Component, private Theme::Themable {
		public:
		using LabelTextChangedCallback = Utils::CallbackList<juce::String>;
		using ValueChangedCallback = DigitalSelector<T>::ValueChangedCallback;
		using CustomFormatterCallback = DigitalSelector<T>::CustomFormatterCallback;

		DigitalSelectorPanel(
			Theme::ThemePtr theme, juce::String nameText = juce::String(),
			juce::String labelText = juce::String(), bool editableLabel = false
		) : Themable(theme) {
			
			name.setBorderSize(juce::BorderSize<int>(name.getBorderSize().getTop(), 0, 0, 0));
			setName(nameText);

			label.setBorderSize(juce::BorderSize<int>(label.getBorderSize().getTop(), 0, 0, 0));
			setLabel(labelText);
			setEditableLabel(editableLabel);
			label.onTextChange = [this]() { onLabelTextChanged.callSafely(label.getText()); };

			selector.setFontSize(Metric::LARGE);
			onValueChangedCallbackId = selector.onValueChanged.add(
				[this](T v) { onValueChanged.callSafely(v); }
			);

			addAndMakeVisible(name);
			addAndMakeVisible(label);
			addAndMakeVisible(selector);

			resized();
		};
		~DigitalSelectorPanel() override {
			if (onValueChangedCallbackId)
				selector.onValueChanged.remove(onValueChangedCallbackId.value());
		};

		DigitalSelectorPanelDirection getLayoutDirection() const noexcept {
			return layoutDirection;
		}
		void setLayoutDirection(DigitalSelectorPanelDirection v) noexcept {
			layoutDirection = v;
			resized();
		}

		juce::String getName() const noexcept {
			return name.getText();
		}
		void setName(const juce::String v) noexcept {
			name.setText(v, juce::NotificationType::dontSendNotification);
		}

		juce::String getLabel() const noexcept {
			return label.getText();
		}
		void setLabel(const juce::String v) noexcept {
			label.setText(v, juce::NotificationType::dontSendNotification);
		}

		bool getEditableLabel() const noexcept {
			return label.isEditable();
		}
		void setEditableLabel(bool v) noexcept {
			label.setMouseCursor(
				v ? juce::MouseCursor::PointingHandCursor : juce::MouseCursor::NormalCursor
			);
			label.setEditable(false, v);
		}

		inline T getMin() const noexcept {
			return selector.getMin();
		}
		void setMin(T v) noexcept {
			selector.setMax(v);
		}

		inline T getMax() const noexcept {
			return selector.getMax();
		}
		void setMax(T v) noexcept {
			return selector.getMax(v);
		}

		inline T getValue() const noexcept {
			return selector.getValue();
		}
		void setValue(T v) noexcept {
			return selector.setValue(v);
		}

		void setMinMax(T vMin, T vMax) noexcept {
			return selector.setMinMax(vMin, vMax);
		}

		inline T getSelectorMaxChars() const noexcept {
			return selector.getMaxChars();
		}
		inline T getSelectorMaxDigits() const noexcept {
			return selector.getMaxDigits();
		}

		juce::Colour getBaseColor() const noexcept {
			return selector.getBaseColor();
		}
		void setBaseColor(juce::Colour v) {
			return selector.setBaseColor(v);
		};

		juce::Justification getNameJustification() const noexcept {
			return name.getJustificationType();
		}
		void setNameJustification(juce::Justification v) noexcept {
			name.setJustificationType(v);
		}

		juce::Justification getLabelJustification() const noexcept {
			return label.getJustificationType();
		}
		void setLabelJustification(juce::Justification v) noexcept {
			label.setJustificationType(v);
		}

		juce::Justification getSelectorJustification() const noexcept {
			return selector.getJustification();
		}
		void setSelectorJustification(juce::Justification v) noexcept {
			selector.setJustification(v);
		}
		
		CustomFormatterCallback getCustomFormatter() const {
			return selector.customFormatter;
		}
		void setCustomFormatter(CustomFormatterCallback v) {
			selector.customFormatter = v;
		}

		void paint(juce::Graphics& g) override {
			theme->drawRounderContainer(
				this, g, getLocalBounds().toFloat(), true, Metric::TINY, Metric::TINY,
				UIColor::BORDER_ELEMENT, UIColor::BACKGROUND_ELEMENT
			);

			if (label.getText().isEmpty() && !label.isBeingEdited()) {
				const auto placeholderTextColor = theme->isDark() ?
					theme->getUIColor(UIColor::BACKGROUND_ELEMENT).brighter(0.15f) :
					theme->getUIColor(UIColor::BACKGROUND_ELEMENT).darker(0.15f);
				g.setFont(label.getFont());
				g.setColour(placeholderTextColor);
				g.drawText("(Unnamed)", label.getBounds(), getLabelJustification());
			}
		}

		void resized() override {
			adaptLayout();
		};

		void setTheme(Theme::ThemePtr v) noexcept override {
			Themable::setTheme(v);
			name.setColour(juce::Label::textColourId, theme->getUIColor(UIColor::TEXT_ELEMENT_MUTED));

			label.setColour(juce::Label::textColourId, theme->getUIColor(UIColor::TEXT_ELEMENT));
			label.setColour(juce::Label::textWhenEditingColourId, theme->getUIColor(UIColor::TEXT_ELEMENT));

			selector.setTheme(v);

			resized();
			repaint();
		}

		LabelTextChangedCallback onLabelTextChanged;
		ValueChangedCallback onValueChanged;

		private:
		juce::Rectangle<float> innerArea;

		DigitalSelectorPanelDirection layoutDirection = DigitalSelectorPanelDirection::VERTICAL;
		bool editableLabel = false;

		DigitalSelector<T> selector{theme};
		juce::Label name;
		juce::Label label;

		ValueChangedCallback::Identifier onValueChangedCallbackId;

		void adaptLayout() noexcept {
			const auto padding = theme->getSpacing(Metric::SMALL);
			innerArea = theme->getInnerArea(this, Metric::TINY, Metric::SMALL);

			name.setFont(theme->getFontSize(Metric::TINY));
			label.setFont(theme->getFontSize(Metric::MEDIUM));

			if (layoutDirection == DigitalSelectorPanelDirection::HORIZONTAL) {
				auto displayWidth = (int) selector.getMinimumSafeWidth();

				selector.setBounds(
					(int) innerArea.getRight() - displayWidth, (int) innerArea.getY(), displayWidth, (int) innerArea.getHeight()
				);

				const auto labelsWidth = (int) (selector.getX() - innerArea.getX() - padding);
				name.setBounds(
					(int) innerArea.getX(), (int) innerArea.getY(),
					labelsWidth, (int) theme->scale(getName().isEmpty() ? 0.0f : 16.0f)
				);

				label.setBounds(
					(int) innerArea.getX(), (int) name.getBottom(),
					labelsWidth, (int) (innerArea.getBottom() - name.getBottom())
				);
			} else {
				auto const displayHeight = (int) selector.getMinimumSafeHeight();

				selector.setBounds(
					(int) innerArea.getX(), (int) innerArea.getBottom() - displayHeight,
					(int) innerArea.getWidth(), displayHeight
				);

				name.setBounds(
					(int) innerArea.getX(), (int) innerArea.getY(),
					(int) innerArea.getWidth(), (int) theme->scale(getName().isEmpty() ? 0.0f : 16.0f)
				);

				label.setBounds(
					(int) innerArea.getX(), (int) name.getBottom(),
					(int) innerArea.getWidth(), (int) (selector.getY() - name.getBottom() - padding)
				);
			}
		}
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DigitalSelectorPanel)
	};
}