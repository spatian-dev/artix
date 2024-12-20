/*
  ==============================================================================

    Label.cpp
    Created: 18 Dec 2024 1:08:21am
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "Label.h"

namespace Artix::Ui::Component {
    void Label::mouseDown(const juce::MouseEvent&) {
        state = State::DOWN;
    }

    void Label::mouseUp(const juce::MouseEvent& event) {
        if (state == State::DOWN && (event.getNumberOfClicks() == 1)) {
            this->onClick.callSafely(event);
        }
        state = isMouseOver() ? State::HOVER : State::NORMAL;
    }

    void Label::mouseEnter(const juce::MouseEvent&) {
        state = isMouseButtonDown() ? State::DOWN : State::HOVER;
    }

    void Label::mouseExit(const juce::MouseEvent&) {
        state = State::NORMAL;
    }
}