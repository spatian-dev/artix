/*
  ==============================================================================

	ChannelMapperList.cpp
	Created: 20 Jun 2024 02:55:24pm
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Identifiers.h"
#include "Midi/MidiChannelMapperBank.h"

class ArtixAudioProcessor final : public juce::AudioProcessor {

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

	//juce::UndoManager& getUndoManager() noexcept;

	Artix::Midi::MidiChannelMapperBank& getMapperBank() noexcept;

	private:
	//juce::UndoManager undoManager = juce::UndoManager();
	Artix::Midi::MidiChannelMapperBank mapperBank;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArtixAudioProcessor)
};
