#include "JuceHeader.h"

int main (int argc, char* argv[])
{
    const StringArray cmd (argv, argc);

    if (cmd.size() > 1)
    {
        drow::UnityProjectBuilder parser (cmd[1]);
        parser.setNumFilesToSplitBetween (4);
        parser.setLogOutput (true);
        parser.setBuildDirectoryName ("BuildUnity");
        
        if (parser.run())
            parser.saveProject (cmd[2]);
    }

    return 0;
}
