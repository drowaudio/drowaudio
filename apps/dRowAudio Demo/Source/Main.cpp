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

#include "DemoHeader.h"
#include "MainWindow.h"


//==============================================================================
class dRowAudioDemoApplication  : public JUCEApplication
{
public:
    //==============================================================================
    dRowAudioDemoApplication()
    {
    }

    ~dRowAudioDemoApplication()
    {
    }

    //==============================================================================
    void initialise (const String& /*commandLine*/)
    {
#if JUCE_DEBUG && DROWAUDIO_UNIT_TESTS
        UnitTestRunner testRunner;
        testRunner.runAllTests();
#endif
        
        ScopedPointer<SplashScreen> splash = new SplashScreen ("dRowAudio Demo",
                                                               ImageCache::getFromMemory (BinaryData::splash_screen_png, BinaryData::splash_screen_pngSize),
                                                              #if JUCE_MAC
                                                               true);
                                                              #else
                                                               false);
                                                              #endif
        
        // Do your application's initialisation code here..
        mainWindow = new MainAppWindow();
    }

    void shutdown()
    {
        // Do your application's shutdown code here..
        mainWindow = 0;
    }

    //==============================================================================
    void systemRequestedQuit()
    {
        quit();
    }

    //==============================================================================
    const String getApplicationName()
    {
        return "dRowAudio Demo";
    }

    const String getApplicationVersion()
    {
        return ProjectInfo::versionString;
    }

    bool moreThanOneInstanceAllowed()
    {
        return true;
    }

    void anotherInstanceStarted (const String& /*commandLine*/)
    {
        
    }

private:
    ScopedPointer <MainAppWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that starts the app.
START_JUCE_APPLICATION(dRowAudioDemoApplication)
