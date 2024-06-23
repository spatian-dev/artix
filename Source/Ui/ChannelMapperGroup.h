/*
  ==============================================================================

    ChannelMapperGroup.h
    Created: 22 Jun 2024 2:18:16pm
    Author:  nerax

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ChannelMapperGroup : public juce::Component
{
public:
    ChannelMapperGroup();
    ~ChannelMapperGroup() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelMapperGroup)
};
