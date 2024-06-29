/*
  ==============================================================================

	AppState.cpp
	Created: 28 Jun 2024 5:16:36pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "AppState.h"

namespace Artix {
	AppState::AppState(int width, int height) : width(width), height(height) {}

	int AppState::getWidth() const noexcept {
		return width;
	}

	void AppState::setWidth(int v, bool muteCallbacks) noexcept {
		const bool hasChanged = (this->width != width);
		width = v;
		if (!hasChanged || muteCallbacks) return;

		juce::MessageManager::callAsync([this]() {
			if (onSizeChanged) onSizeChanged(width, height);
		});
	}

	int AppState::getHeight() const noexcept {
		return height;
	}

	void AppState::setHeight(int v, bool muteCallbacks) noexcept {
		const bool hasChanged = (this->height != height);
		height = v;
		if (!hasChanged || muteCallbacks) return;

		juce::MessageManager::callAsync([this]() {
			if (onSizeChanged) onSizeChanged(width, height);
		});
	}

	void AppState::setSize(int width, int height, bool muteCallbacks) noexcept {
		const bool hasChanged = (this->width != width) || (this->height != height);
		this->width = width;
		this->height = height;
		if (!hasChanged || muteCallbacks) return;

		juce::MessageManager::callAsync([this]() {
			if (onSizeChanged) onSizeChanged(this->width, this->height);
		});
	}

	Midi::MidiChannelMapperBank& AppState::getMapperBank() noexcept {
		return mapperBank;
	}

	juce::ValueTree AppState::toValueTree() const noexcept {
		auto vt = juce::ValueTree(Id::AppState);
		vt.setProperty(Id::Width, width.load(), nullptr);
		vt.setProperty(Id::Height, height.load(), nullptr);
		vt.addChild(mapperBank.toValueTree(), -1, nullptr);

		return vt;
	}

	void AppState::fromValueTree(const juce::ValueTree& vt) noexcept {
		jassert(vt.hasType(Id::AppState));
		if (!vt.hasType(Id::AppState)) {
			juce::MessageManager::callAsync([this]() {
				if (onError) {
					onError("Invalid ValueTree type", Error::Code::BadState, Error::Code::InvalidValueTree);
				}
			});
			return;
		}

		setSize(
			vt.getProperty(Id::Width, (int) this->width),
			vt.getProperty(Id::Height, (int) this->height)
		);

		int i = 0;
		for (auto child : vt) {
			if (child.hasType(Id::MidiChannelMapperBank)) {
				mapperBank.fromValueTree(child);
				break;
			}
		}
	}
}