/*
  ==============================================================================

    Settings.cpp
    Created: 18 Oct 2024 8:49:27am
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "Settings.h"

namespace Artix {
    Settings::Settings(std::unique_ptr<juce::InterProcessLock>& processLock) {
        options.applicationName = JucePlugin_Name;
        options.filenameSuffix = ".config.xml";
        options.folderName = JucePlugin_Manufacturer;
        options.osxLibrarySubFolder = "Application Support";
        options.commonToAllUsers = true;
        options.ignoreCaseOfKeyNames = true;
        options.millisecondsBeforeSaving = -1;
        options.storageFormat = juce::PropertiesFile::StorageFormat::storeAsXML;
        options.processLock = processLock.get();

        const auto docsDirectory =
            juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory)
                .getChildFile(JucePlugin_Manufacturer).getChildFile(JucePlugin_Name);

        defaults = std::make_unique<juce::PropertySet>();
        defaults->setValue("data_directory", docsDirectory.getFullPathName());

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

    bool Settings::setDataDirectory(juce::File directory) {
        props->setValue("data_directory", directory.getFullPathName());
        return true;
    }
}