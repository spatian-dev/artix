# Artix
A simple VST that maps MIDI channel to note, to simplify your keyswitches and more.


### Debugging
To make debugging easy while developping:
- Set the project `Artix_VST3` as your startup project.
- Set the debugging command in your IDE to JUCE's included [`AudioPluginHost`](https://github.com/juce-framework/JUCE/tree/master/extras/AudioPluginHost).
- Set the debugging command argumnets to the full path of [`Debug.filtergraph`](Debug.filtergraph).

If additionally you would like to have the [Address Sanitizer \(ASan\)](https://learn.microsoft.com/en-us/cpp/sanitizers/asan?view=msvc-170), then make sure to:
- Rebuild the AudioPluginHost with ASan enabled.
- Have ASan enabled in all projects in the solution. (*`Properties`* > *`C/C++`* > *`Enable Address Sanitizer`*).