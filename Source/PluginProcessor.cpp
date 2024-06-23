/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 20 Jun 2024 02:55:24pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Ui/PluginEditor.h"
#include "Midi/ChannelMapper.h"

ArtixAudioProcessor::ArtixAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
	#if ! JucePlugin_IsMidiEffect
		#if ! JucePlugin_IsSynth
			.withInput("Input", juce::AudioChannelSet::stereo(), true)
		#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
	#endif
	)
#endif
{
	channelMappers = new Artix::Midi::ChannelMapperList(appState, &undoManager);
	for (int i = 1; i <= 16; i++) {
		appState.addChild(Artix::Midi::ChannelMapper::makeState(i, 1), -1, nullptr);
	}
}

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

#ifndef JucePlugin_PreferredChannelConfigurations
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
#endif

void ArtixAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiIn) {
	juce::ScopedNoDenormals noDenormals;
	buffer.clear();

	juce::MidiBuffer midiOut;

	for (const auto metadata : midiIn) {
		auto message = metadata.getMessage();

		const auto timestamp = metadata.samplePosition;
		const auto inChannel = message.getChannel();

		if (inChannel < 1)
			continue;

		const auto mapper = channelMappers->getItems()[inChannel - 1];

		message.setChannel(mapper->getOutputChannel());
		if (mapper->isActive() && message.isNoteOn()) {
			midiOut.addEvent(
				juce::MidiMessage::noteOn(
					mapper->getOutputChannel(), mapper->getOutputNote(), message.getVelocity()
				),
				timestamp
			);
		}

		midiOut.addEvent(message, timestamp);

		if (mapper->isActive() && message.isNoteOff()) {
			midiOut.addEvent(
				juce::MidiMessage::noteOff(
					mapper->getOutputChannel(), mapper->getOutputNote(), message.getVelocity()
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
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void ArtixAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

juce::UndoManager* ArtixAudioProcessor::getUndoManager() noexcept {
	return &undoManager;
}

juce::ValueTree ArtixAudioProcessor::getAppState() noexcept {
	return appState;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
	return new ArtixAudioProcessor();
}
