/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 22 Jun 2024 10:24:22am
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Identifiers.h"
#include "ChannelMapper.h"
#include "ValueTreeList.h"

namespace Artix::Midi {
	using ChannelMapperValueTreeList = Artix::Util::ValueTreeList<ChannelMapper>;

	class ChannelMapperList :
		private ChannelMapperValueTreeList, public juce::ReferenceCountedObject {

		public:
		ChannelMapperList(const juce::ValueTree& v);
		~ChannelMapperList();

		juce::ReferenceCountedArray<ChannelMapper> getItems() const;

		private:
		bool isSuitable(const juce::ValueTree& v) const;
		ChannelMapper* createItem(const juce::ValueTree& v);
		void deleteItem(ChannelMapper* item);

		void onItemAdded(ChannelMapper* item);
		void onItemRemoved(ChannelMapper* item);
		void onItemsOrderChanged();
		void onListRedirected();

		juce::ReferenceCountedArray<ChannelMapper, juce::CriticalSection> items;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelMapperList)
	};
}