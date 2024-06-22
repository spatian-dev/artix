# Artix
A simple VST that maps MIDI channel to note, to simplify your keyswitches and more.

### Debugging
To make debugging easy:
- Set the project `Artix_VST3` as your startup project.
- Set the debugging command in your IDE to JUCE's included [`AudioPluginHost`](https://github.com/juce-framework/JUCE/tree/master/extras/AudioPluginHost).
- Set the debugging command argumnets to the full path of [`Debug.filtergraph`](Debug.filtergraph).

>#### ⚠️ Note
>`AudioPluginHost` seems to come with some memory leaks. The following leaks are detected even on an empty project:
>
>```
>Detected memory leaks!
>Dumping objects ->
>{38729} normal block at 0x00000240B6C7A640, 2 bytes long.
> Data: <c > 63 00 
>{38728} normal block at 0x00000240B0005A30, 16 bytes long.
> Data: <@Y  @   @   @   > 40 59 00 B0 40 02 00 00 40 A6 C7 B6 40 02 00 00 
>{30248} normal block at 0x00000240B0005800, 6 bytes long.
> Data: <en-us > 65 6E 2D 75 73 00 
>{30247} normal block at 0x00000240B0005940, 16 bytes long.
> Data: <         X  @   > 00 00 00 00 00 00 00 00 00 58 00 B0 40 02 00 00 
>{30246} normal block at 0x00000240B6A7FDA0, 216 bytes long.
> Data: <                > 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
>{30194} normal block at 0x00000240B04D2BB0, 184 bytes long.
> Data: <                > 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
>Object dump complete.
>```