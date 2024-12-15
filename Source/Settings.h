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
		Settings(std::unique_ptr<juce::InterProcessLock>& processLock);
		~Settings();

		juce::String getFilePath() const;

		juce::File getDataDirectory() const;
		bool setDataDirectory(juce::File directory);

		private:
		juce::PropertiesFile::Options options;
		std::unique_ptr<juce::PropertySet> defaults;
		std::unique_ptr<juce::PropertiesFile> props;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Settings)
	};
}