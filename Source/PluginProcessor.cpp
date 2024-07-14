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
{}

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
	return 0;
}

void ArtixAudioProcessor::setCurrentProgram(int index) {}

const juce::String ArtixAudioProcessor::getProgramName(int index) {
	return "Init";
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

void ArtixAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	// Make sure to reset the state if your inner loop is processing
	// the samples and the outer loop is handling the channels.
	// Alternatively, you can process the samples with the channels
	// interleaved by keeping the same state.
	for (int channel = 0; channel < totalNumInputChannels; ++channel) {
		auto* channelData = buffer.getWritePointer(channel);

		// ..do something to the data...
	}
}

bool ArtixAudioProcessor::hasEditor() const {
	return true;
}

juce::AudioProcessorEditor* ArtixAudioProcessor::createEditor() {
	return new Artix::Ui::PluginEditor(*this, state);
}

void ArtixAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
	if (auto xmlState = state.toValueTree().createXml()) {
		copyXmlToBinary(*xmlState, destData);
	}
}

void ArtixAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
	if (auto xmlState = getXmlFromBinary(data, sizeInBytes)) {
		state.fromValueTree(juce::ValueTree::fromXml(*xmlState));
	}
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
	return new ArtixAudioProcessor();
}
