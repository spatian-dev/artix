/*
  ==============================================================================

    Error.h
    Created: 13 Jul 2024 3:32:31pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/CallbackList.h"

namespace Artix::Error {
	using ErrorCallback = Utils::CallbackList<juce::String>;
}