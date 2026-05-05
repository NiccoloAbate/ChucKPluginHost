PluginHost plugin;

plugin.forceSynchronous(true);

plugin.load("/Library/Audio/Plug-Ins/VST3/Pianoteq 8.vst3");

// window title
GG.windowTitle("ChucK/ChuGl/JUCE");

false => int done;

// time loop
while( true )
{
    GG.nextFrame() => now;

    if (!done)
    {
        plugin.load("/Library/Audio/Plug-Ins/VST3/Pianoteq 8.vst3");
        true => done;
    }
}