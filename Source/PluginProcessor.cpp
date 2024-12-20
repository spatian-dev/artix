/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Ui/PluginEditor.h"

ArtixAudioProcessor::ArtixAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(getBusesLayout())
#endif
{
    processLock = std::make_unique<juce::InterProcessLock>(juce::String(JucePlugin_Name));

    settings = std::make_unique<Artix::Settings>(processLock, 1000);
    state = std::make_unique<Artix::PluginState>(*settings);

    state->onError.add([this](const juce::String err) {
        juce::NativeMessageBox::showMessageBox(
            juce::MessageBoxIconType::WarningIcon,
            "Something went wrong",
            "Failed to load preset: " + err,
            getActiveEditor()
        );
    });

    presets = std::make_unique<Artix::Midi::Presets>(settings->getDataDirectory());
    if (presets->factoryPresetCount() > 0) {
        state->fromState(*presets->getPreset(0)->state);
    }

    state->getMapperBank().onOutputChannelChanged.add([this](const Artix::Midi::Channel) {
        shouldGlobalNoteOff = true;
    });
    for (auto& m : state->getMapperBank()) {
        m.onNoteChanged.add([this](const Artix::Midi::Note) {
            shouldGlobalNoteOff = true;
        });
    }

    state->onDirtyChanged.add([this](const bool) {
        shouldGlobalNoteOff = true;
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
    return 1;
}

int ArtixAudioProcessor::getCurrentProgram() {
    return 1;
}

void ArtixAudioProcessor::setCurrentProgram(int /*index*/) {}

const juce::String ArtixAudioProcessor::getProgramName(int /*index*/) {
    return "(none)";
}

void ArtixAudioProcessor::changeProgramName(int /*index*/, const juce::String& /*newName*/) {}

void ArtixAudioProcessor::prepareToPlay(double /*sampleRate*/, int /*samplesPerBlock*/) {}

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

    const int outCh = (int) state->getMapperBank().getOutputChannel();
    if (shouldGlobalNoteOff) {
        for (int ch = (int) (Artix::Midi::Channel::First); ch < ((int) (Artix::Midi::Channel::Last)) + 1; ch++) {
            // This is apparently not enough for some hosts ...
            midiOut.addEvent(juce::MidiMessage::allNotesOff(ch), 0);

            for (int n = (int) (Artix::Midi::Note::First); n < ((int) (Artix::Midi::Note::Last)) + 1; n++) {
                // So we're forced to do this ugly workaround ...
                midiOut.addEvent(juce::MidiMessage::noteOff(ch, n), 0);
            }
        }
        shouldGlobalNoteOff = false;
    }

    for (const auto metadata : midiIn) {
        auto message = metadata.getMessage();
        const auto inputChannel = message.getChannel();

        if (!message.isNoteOnOrOff() || (inputChannel < 1)) {
            midiOut.addEvent(message, metadata.samplePosition);
            continue;
        }
        
        message.setChannel(outCh);
        auto& mapper = state->getMapperBank()[inputChannel - 1];

        if (mapper.isActive() && message.isNoteOn()) {
            midiOut.addEvent(
                juce::MidiMessage::noteOn(outCh, (int) mapper.getNote(), message.getVelocity()),
                metadata.samplePosition
            );
        }

        midiOut.addEvent(message, metadata.samplePosition);

        if (mapper.isActive() && message.isNoteOff()) {
            midiOut.addEvent(
                juce::MidiMessage::noteOff(outCh, (int) mapper.getNote(), message.getVelocity()),
                metadata.samplePosition
            );
        }
    }

    midiIn.swapWith(midiOut);
}

bool ArtixAudioProcessor::hasEditor() const {
    return true;
}

juce::AudioProcessorEditor* ArtixAudioProcessor::createEditor() {
    return new Artix::Ui::PluginEditor(*this, *state, *presets, *settings);
}

void ArtixAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    if (auto xmlState = state->toValueTree().createXml()) {
        copyXmlToBinary(*xmlState, destData);
    }
}

void ArtixAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    if (auto xmlState = getXmlFromBinary(data, sizeInBytes)) {
        state->fromValueTree(juce::ValueTree::fromXml(*xmlState));
    }
}

juce::AudioProcessor::BusesProperties ArtixAudioProcessor::getBusesLayout() {
    // Live and Cakewalk don't like to load midi-only plugins, so we add an audio output there.
    const juce::PluginHostType host;
    return host.isAbletonLive() || host.isSonar()
        ? BusesProperties().withOutput("out", juce::AudioChannelSet::stereo())
        : BusesProperties();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new ArtixAudioProcessor();
}
