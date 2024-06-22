/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 21 Jun 2024 09:21:44pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Identifiers.h"

namespace Artix::Midi {
	class ChannelMapper : public juce::ReferenceCountedObject {
		public:
		ChannelMapper(const juce::ValueTree& v);
		~ChannelMapper();

		int getInputChannel() const;
		void setInputChannel(int ch);

		int getOutputNote() const;
		void setOutputNote(int note);

		juce::ValueTree getState();

		private:
		juce::ValueTree state;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelMapper)
	};
}