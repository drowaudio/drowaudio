/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
int main (int argc, char* argv[])
{
    const StringArray cmd (argv, argc);
    
    if (cmd.size() > 1)
    {
        drow::UnityProjectBuilder parser (cmd[1]);
        parser.setNumFilesToSplitBetween (4);
        parser.setLogOutput (true);
        parser.setBuildDirectoryName ("BuildUnity");
        
        if (parser.run() && cmd.size() > 1)
            parser.saveProject (cmd[2]);
    }

    return 0;
}
