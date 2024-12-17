/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Ui/PluginEditor.h"

ArtixAudioProcessor::ArtixAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
    )
#endif
{
    processLock = std::make_unique<juce::InterProcessLock>(juce::String(JucePlugin_Name));
    
    settings = std::make_unique<Artix::Settings>(processLock);
    DBG("Settings file: " + settings->getFilePath());
    DBG(settings->getDataDirectory().getFullPathName());

    presets = std::make_unique<Artix::Midi::Presets>(settings->getDataDirectory());
    if (presets->factoryPresetCount() > 0)
        state.load(presets->getPreset(0));
    
    state.onError.add([this](const Artix::Error::ErrorDetails err) {
        juce::NativeMessageBox::showMessageBox(
            juce::MessageBoxIconType::WarningIcon,
            "Something went wrong",
            "Failed to load preset: " + err.msg,
            getActiveEditor()
        );
    });
}

ArtixAudioProcessor::~ArtixAudioProcessor() {}

const juce::String ArtixAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool ArtixAudioProcessor::acceptsMidi() const {
    return true;
}

bool ArtixAudioProcessor::producesMidi() const {
    return true;
}

bool ArtixAudioProcessor::isMidiEffect() const {
    return true;
}

double ArtixAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int ArtixAudioProcessor::getNumPrograms() {
    return std::max(presets->factoryPresetCount(), 1);
}

int ArtixAudioProcessor::getCurrentProgram() {
    return presets->indexOf(state.getCurrentPreset());
}

void ArtixAudioProcessor::setCurrentProgram(int index) {
    const auto preset = presets->getPreset(index);
    if (!preset)
        return;
    state.load(preset);
}

const juce::String ArtixAudioProcessor::getProgramName(int index) {
    const auto preset = presets->getPreset(index);
    return preset ? preset->name : "";
}

void ArtixAudioProcessor::changeProgramName(int index, const juce::String& newName) {}

void ArtixAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {}

void ArtixAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ArtixAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    juce::ignoreUnused(layouts);
    return true;
}
#endif

void ArtixAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiIn) {
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    juce::MidiBuffer midiOut;

    for (const auto metadata : midiIn) {
        auto message = metadata.getMessage();

        const auto timestamp = metadata.samplePosition;

        const auto inputChannel = message.getChannel();
        if (inputChannel < 1)
            continue;

        const int outCh = (int) state.getMapperBank().getOutputChannel();
        message.setChannel(outCh);

        auto& mapper = state.getMapperBank()[inputChannel - 1];

        if (mapper.isActive() && message.isNoteOn()) {
            midiOut.addEvent(
                juce::MidiMessage::noteOn(outCh, (int) mapper.getNote(), message.getVelocity()),
                timestamp
            );
        }

        midiOut.addEvent(message, timestamp);

        if (mapper.isActive() && message.isNoteOff()) {
            midiOut.addEvent(
                juce::MidiMessage::noteOff(outCh, (int) mapper.getNote(), message.getVelocity()),
                timestamp
            );
        }
    }
    midiIn.swapWith(midiOut);
}

bool ArtixAudioProcessor::hasEditor() const {
    return true;
}

juce::AudioProcessorEditor* ArtixAudioProcessor::createEditor() {
    return new Artix::Ui::PluginEditor(*this, state, *presets);
}

void ArtixAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    if (auto xmlState = state.toValueTree().createXml()) {
        copyXmlToBinary(*xmlState, destData);
    }
}

void ArtixAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    if (auto xmlState = getXmlFromBinary(data, sizeInBytes)) {
        state.load(juce::ValueTree::fromXml(*xmlState));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new ArtixAudioProcessor();
}
