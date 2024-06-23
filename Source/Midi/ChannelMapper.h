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

#define jassertValidMidiChannel(ch) jassert((ch >= 1) && (ch <= 16))
#define jassertValidMidiNote(note) jassert((note>= 0) && (note<= 127))
#define jassertValidMidiNoteOrNone(note) jassert((note>= -1) && (note<= 127))

namespace Artix::Midi {
	class ChannelMapper : public juce::ReferenceCountedObject {
		public:

		static const int NOTE_NONE = -1;

		ChannelMapper(const juce::ValueTree& v, juce::UndoManager* undoManager = nullptr);
		~ChannelMapper();

		int getInputChannel() const;
		void setInputChannel(int ch);

		int getOutputChannel() const;
		void setOutputChannel(int ch);

		int getOutputNote() const;
		void setOutputNote(int note);

		bool isActive() const;

		juce::ValueTree getState();

		static juce::ValueTree makeState(
			int inChannel, int outChannel, int outNote = NOTE_NONE
		) noexcept;

		private:
		juce::ValueTree state;
		juce::UndoManager*  undoManager;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelMapper)
	};
}