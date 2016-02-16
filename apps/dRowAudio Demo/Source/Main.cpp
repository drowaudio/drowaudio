/*
    ==============================================================================

    This file is part of the dRowAudio JUCE module
    Copyright 2004-13 by dRowAudio.

    ------------------------------------------------------------------------------

    dRowAudio is provided under the terms of The MIT License (MIT):

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    ==============================================================================
*/

#include "MainWindow.h"

class dRowAudioDemoApplication : public JUCEApplication
{
public:
    dRowAudioDemoApplication()
    {
    }

    void initialise (const String&) override
    {
#if JUCE_DEBUG && DROWAUDIO_UNIT_TESTS
        UnitTestRunner testRunner;
        testRunner.runAllTests();
#endif

        ScopedPointer<SplashScreen> splash = new SplashScreen (getApplicationName(),
                                                               ImageCache::getFromMemory (BinaryData::splash_screen_png, BinaryData::splash_screen_pngSize),
                                                              #if JUCE_MAC
                                                               true);
                                                              #else
                                                               false);
                                                              #endif

        mainWindow = new MainAppWindow();
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

    void systemRequestedQuit() override                     { quit(); }
    const String getApplicationName() override              { return ProjectInfo::projectName; }
    const String getApplicationVersion() override           { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override              { return true; }
    void anotherInstanceStarted (const String&) override    { }

private:
    ScopedPointer<MainAppWindow> mainWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (dRowAudioDemoApplication)
};

//==============================================================================
// This macro generates the main() routine that starts the app.
START_JUCE_APPLICATION(dRowAudioDemoApplication)
