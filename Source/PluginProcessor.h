/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <memory>

#include "Settings.h"
#include "Midi/Presets.h"
#include "State.h"

class ArtixAudioProcessor : public juce::AudioProcessor {
	public:
	ArtixAudioProcessor();
	~ArtixAudioProcessor() override;

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	private:
	std::unique_ptr<juce::InterProcessLock> processLock;
	std::shared_ptr<Artix::Settings> settings;
	std::unique_ptr<Artix::Midi::Presets> presets;
	Artix::State state;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArtixAudioProcessor)
};
