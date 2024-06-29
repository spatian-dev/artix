/*
  ==============================================================================

	Errors.h
	Created: 28 Jun 2024 12:25:25am
	Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

namespace Artix::Error {
	enum class Code : int {
		None = 0,
		BadState = 1000,
		InvalidValueTree = 1001,
		MissingChildren = 1002
	};

	using ErrorCallback = std::function<void(const juce::String&, Error::Code, Error::Code)>;
}