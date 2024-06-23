/*
  ==============================================================================

    ChannelMapperList.cpp
    Created: 22 Jun 2024 10:24:22am
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "ChannelMapperList.h"

namespace Artix::Midi {
    ChannelMapperList::ChannelMapperList(
        const juce::ValueTree& v, juce::UndoManager* undoManager
    ) : ChannelMapperValueTreeList(v), undoManager(undoManager) {
        buildItems();
    }

    ChannelMapperList::~ChannelMapperList() {
        freeItems();
    }

    juce::ReferenceCountedArray<ChannelMapper> ChannelMapperList::getItems() const {
        return items;
    }

    bool ChannelMapperList::isSuitable(const juce::ValueTree& v) const {
        return v.hasType(Artix::ID::MidiChannelMapper);
    }

    ChannelMapper* ChannelMapperList::createItem(const juce::ValueTree& v) {
        auto item = new ChannelMapper(v, undoManager);
        items.add(item);
        return item;
    }

    void ChannelMapperList::deleteItem(ChannelMapper* item) {
        items.removeObject(item);
    }

    void ChannelMapperList::onItemAdded(ChannelMapper* item) {
        //DBG("Added " << item->getState().toXmlString());
    }

    void ChannelMapperList::onItemRemoved(ChannelMapper* item) {
        //DBG("Removed " << item->getState().toXmlString());
    }

    void ChannelMapperList::onItemsOrderChanged() {
        //DBG("Items Order Changed");
    }

    void ChannelMapperList::onListRedirected() {
        //DBG("List Redirected");
    }
}