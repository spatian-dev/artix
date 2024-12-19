/*
  ==============================================================================

    Settings.cpp
    Created: 18 Oct 2024 8:49:27am
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "Settings.h"

namespace Artix {
    Settings::Settings(std::unique_ptr<juce::InterProcessLock>& processLock, int msBeforeSaving) {
        options.applicationName = JucePlugin_Name;
        options.filenameSuffix = ".config.xml";
        options.folderName = JucePlugin_Manufacturer;
        options.osxLibrarySubFolder = "Application Support";
        options.commonToAllUsers = true;
        options.ignoreCaseOfKeyNames = true;
        options.millisecondsBeforeSaving = msBeforeSaving;
        options.storageFormat = juce::PropertiesFile::StorageFormat::storeAsXML;
        options.processLock = processLock.get();

        const auto docsDirectory =
            juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory)
                .getChildFile(JucePlugin_Manufacturer).getChildFile(JucePlugin_Name);

        defaults = std::make_unique<juce::PropertySet>();
        defaults->setValue("data_directory", docsDirectory.getFullPathName());
        defaults->setValue("window_height", 800);
        defaults->setValue("theme_name", "");

        props = std::make_unique<juce::PropertiesFile>(options);
        props->setFallbackPropertySet(defaults.get());
    }

    Settings::~Settings() {}

    juce::String Settings::getFilePath() const {
        return props->getFile().getFullPathName();
    }

    juce::File Settings::getDataDirectory() const {
        return juce::File(props->getValue("data_directory"));
    }

    void Settings::setDataDirectory(juce::File directory) {
        props->setValue("data_directory", directory.getFullPathName());
    }

    int Settings::getWindowHeight() const {
        return props->getValue("window_height").getIntValue();
    }

    void Settings::setWindowHeight(int height) {
        props->setValue("window_height", height);
    }

    juce::String Settings::getThemeName() const {
        return props->getValue("theme_name");
    }

    void Settings::setThemeName(const juce::String name) {
        props->setValue("theme_name", name);
    }

    bool Settings::save() const {
        return props->saveIfNeeded();
    }
}