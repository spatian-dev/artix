/*
  ==============================================================================

    Settings.h
    Created: 18 Oct 2024 8:49:27am
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <memory>

namespace Artix {
	class Settings {
		public:
		Settings(std::unique_ptr<juce::InterProcessLock>& processLock, int msBeforeSaving = -1);
		~Settings();

		juce::String getFilePath() const;

		juce::File getDataDirectory() const;
		void setDataDirectory(juce::File directory);

		int getWindowHeight() const;
		void setWindowHeight(int height);

		juce::String getThemeName() const;
		void setThemeName(const juce::String name);

		bool save() const;

		private:
		juce::PropertiesFile::Options options;
		std::unique_ptr<juce::PropertySet> defaults;
		std::unique_ptr<juce::PropertiesFile> props;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Settings)
	};
}