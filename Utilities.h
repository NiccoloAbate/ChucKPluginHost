#pragma once

#include <JuceHeader.h>
#include <iostream>

// bad practice, but is convenient and can be ok
inline void callOnMessageThreadSync(std::function<void()> func)
{
    jassert(func);

    if (juce::MessageManager::existsAndIsCurrentThread())
    {
        func();
        return;
    }

    juce::WaitableEvent event;
    juce::MessageManager::callAsync([func, &event]()
    {
        func();
        event.signal();
    });

    // Warn if the main thread takes too long — likely a deadlock.
    // Common cause: calling a synchronous PluginHost operation inside a ChuGl
    // GG.nextFrame() loop. ChuGl's render loop blocks the main thread waiting
    // for GG.nextFrame(), while the shred is blocked here waiting for the main
    // thread — neither can proceed. Fix: use forceSynchronous(false) and poll
    // asyncEventRunning() each frame instead.
    constexpr int warningTimeoutMs = 1000;
    if (!event.wait(warningTimeoutMs))
    {
        std::cout << "PluginHost: WARNING: main thread has not responded after "
                  << warningTimeoutMs << "ms — possible deadlock.\n"
                  << "  If using ChuGl, set forceSynchronous(false) and poll "
                  << "asyncEventRunning() each GG.nextFrame() iteration.\n"
                  << "  Continuing to wait..." << std::endl;
        event.wait();
    }
}

inline void callOnMessageThread(std::function<void()> func)
{
    jassert(func);

    if (juce::MessageManager::existsAndIsCurrentThread())
        func();
    else
        juce::MessageManager::callAsync(func);
}
