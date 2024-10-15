/*
  ==============================================================================

    Error.h
    Created: 13 Jul 2024 3:32:31pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <functional>

#include "../Utils/CallbackList.h"

namespace Artix::Error {
	enum class Code : int {
		None = 0,
		BadState = 1000,
		InvalidValueTree = 1001,
		MissingChildren = 1002
	};

	struct ErrorDetails {
		const juce::String msg;
		const Code code;
		const Code subCode;
	};

	using ErrorCallback = Utils::CallbackList<ErrorDetails>;
}