/*
  ==============================================================================

    ChannelMapperList.cpp
    Created: 22 Jun 2024 10:24:22am
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "ChannelMapperList.h"

namespace Artix::Midi {
    ChannelMapperList::ChannelMapperList(const juce::ValueTree& v) : ChannelMapperValueTreeList(v) {
        jassert(v.hasType(Artix::ID::MidiChannelMapperList));
    }

    ChannelMapperList::~ChannelMapperList() {}

    juce::ReferenceCountedArray<ChannelMapper> ChannelMapperList::getItems() const {
        return items;
    }

    bool ChannelMapperList::isSuitable(const juce::ValueTree& v) const {
        return v.hasType(Artix::ID::MidiChannelMapper);
    }

    ChannelMapper* ChannelMapperList::createItem(const juce::ValueTree& v) {
        auto item = new ChannelMapper(v);
        items.add(item);
        return item;
    }

    void ChannelMapperList::deleteItem(ChannelMapper* item) {
        items.removeObject(item);
    }

    void ChannelMapperList::onItemAdded(ChannelMapper* item) {}
    void ChannelMapperList::onItemRemoved(ChannelMapper* item) {}
    void ChannelMapperList::onItemsOrderChanged() {}
    void ChannelMapperList::onListRedirected() {}
}