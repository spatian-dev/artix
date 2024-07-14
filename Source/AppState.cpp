/*
  ==============================================================================

	AppState.cpp
	Created: 28 Jun 2024 5:16:36pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "AppState.h"

namespace Artix {
	AppState::AppState(int height) : height(height) {
		{
			juce::ScopedWriteLock lock(themeMutex);
			theme = Ui::Theme::Themes::getInstance().begin()->second;
		}
	}

	int AppState::getHeight() const noexcept {
		return height;
	}

	void AppState::setHeight(int v, bool muteCallbacks) noexcept {
		if (height == v) return;
		height = v;

		if (muteCallbacks) return;
		onHeightChanged.callSafely(height);
	}

	Ui::Theme::ThemePtr AppState::getTheme() const noexcept {
		juce::ScopedReadLock lock(themeMutex);
		return theme;
	}

	void AppState::setTheme(Ui::Theme::ThemePtr v, bool muteCallbacks) noexcept {
		juce::ScopedWriteLock lock(themeMutex);
		if (theme.get() == v.get()) return;
		theme = v;

		if (muteCallbacks) return;		
		onThemeChanged.callSafely(theme);
	}

	

	Midi::MidiChannelMapperBank& AppState::getMapperBank() noexcept {
		return mapperBank;
	}

	juce::ValueTree AppState::toValueTree() const noexcept {
		auto vt = juce::ValueTree(Id::AppState);
		vt.setProperty(Id::Height, height.load(), nullptr);
		vt.setProperty(Id::Theme, getTheme()->getName(), nullptr);
		vt.addChild(mapperBank.toValueTree(), -1, nullptr);

		return vt;
	}

	void AppState::fromValueTree(const juce::ValueTree& vt) noexcept {
		jassert(vt.hasType(Id::AppState));
		if (!vt.hasType(Id::AppState)) {
			onError.callOnMessageThread({
				"Invalid ValueTree type", Error::Code::BadState, Error::Code::InvalidValueTree
			});
			return;
		}

		setHeight(vt.getProperty(Id::Height, (int) this->height));

		auto localTheme = getTheme();
		setTheme(Ui::Theme::Themes::getInstance().tryFind(
			vt.getProperty(Id::Theme, localTheme->getName()), localTheme
		));

		int i = 0;
		for (auto child : vt) {
			if (child.hasType(Id::MidiChannelMapperBank)) {
				mapperBank.fromValueTree(child);
				break;
			}
		}
	}
}