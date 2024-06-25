/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 20 Jun 2024 02:55:24pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Ui/PluginEditor.h"

ArtixAudioProcessor::ArtixAudioProcessor() : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
	.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
	.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
) {}

ArtixAudioProcessor::~ArtixAudioProcessor() {}

const juce::String ArtixAudioProcessor::getName() const {
	return JucePlugin_Name;
}

bool ArtixAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool ArtixAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool ArtixAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double ArtixAudioProcessor::getTailLengthSeconds() const {
	return 0.0;
}

int ArtixAudioProcessor::getNumPrograms() {
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int ArtixAudioProcessor::getCurrentProgram() {
	return 0;
}

void ArtixAudioProcessor::setCurrentProgram(int index) {}

const juce::String ArtixAudioProcessor::getProgramName(int index) {
	return {};
}

void ArtixAudioProcessor::changeProgramName(int index, const juce::String& newName) {}

void ArtixAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void ArtixAudioProcessor::releaseResources() {
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

bool ArtixAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}

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

		message.setChannel(mapperBank.getOutputChannel());

		const auto& mapper = mapperBank.getMapper(inputChannel);

		if (mapper.isActive() && message.isNoteOn()) {
			midiOut.addEvent(
				juce::MidiMessage::noteOn(
					mapperBank.getOutputChannel(), mapper.getNote(), message.getVelocity()
				),
				timestamp
			);
		}

		midiOut.addEvent(message, timestamp);

		if (mapper.isActive() && message.isNoteOff()) {
			midiOut.addEvent(
				juce::MidiMessage::noteOff(
					mapperBank.getOutputChannel(), mapper.getNote(), message.getVelocity()
				),
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
	return new Artix::UI::PluginEditor(*this);
}

void ArtixAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
	if (auto xmlState = mapperBank.toValueTree().createXml()) {
		copyXmlToBinary(*xmlState, destData);
	}
}

void ArtixAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
	if (auto xmlState = getXmlFromBinary(data, sizeInBytes)) {
		mapperBank.fromValueTree(juce::ValueTree::fromXml(*xmlState));
	}

	DBG(mapperBank.toValueTree().toXmlString());
}

Artix::Midi::MidiChannelMapperBank& ArtixAudioProcessor::getMapperBank() noexcept {
	return mapperBank;
}

//juce::UndoManager& ArtixAudioProcessor::getUndoManager() noexcept {
//	return undoManager;
//}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
	return new ArtixAudioProcessor();
}
