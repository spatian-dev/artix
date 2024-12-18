/*
  ==============================================================================

    Label.h
    Created: 18 Dec 2024 1:08:56am
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Utils/CallbackList.h"

namespace Artix::Ui::Component {
	class Label: public juce::Label {
		public:
		using ClickedCallback = Utils::CallbackList<juce::MouseEvent>;

		ClickedCallback onClick;

		protected:
        enum State {
            NORMAL = 0,
            HOVER = 1,
            DOWN = 2,
        }; 
        
        void mouseDown(const juce::MouseEvent& event) override;
        void mouseUp(const juce::MouseEvent& event) override;
        void mouseEnter(const juce::MouseEvent& event) override;
        void mouseExit(const juce::MouseEvent& event) override;

        State state = State::NORMAL;
	};
}