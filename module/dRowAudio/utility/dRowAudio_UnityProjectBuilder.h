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

#ifndef DROWAUDIO_UNITYPROJECTBUILDER_H
#define DROWAUDIO_UNITYPROJECTBUILDER_H

/** Nifty class that bundles up all the source files in an existing Introjucer
    project and spits out a new project with a single "unity build" file to compile.

    This is similar to how the JUCE module system works and can greatly improve
    build times on slow or single core CPUs. The main advantage here is when
    building large projects over VM's where mapping files across the virtual
    memory space takes a long time.

    If memory is a problem you can split the build between several files.

    To use it just create one of these with an existing Introjcuer project and
    call the run() method. It will create a unity cpp file in the project's source
    directory and a new Introjucer project that still references all teh same files
    so you can browse and edit them but only compiles the unity build.
 */
class UnityProjectBuilder
{
public:
    /** Creates a UnityProjectBuilder for a given Introjucer project. */
    UnityProjectBuilder (const juce::File& sourceProject);

    /** Destructor. */
    ~UnityProjectBuilder();

    //==============================================================================
    /** Actually starts the project parsing and generation. */
    bool run();

    /** Optionally sets a number of files to split the unity build into. This could
        speed up compile times on multicore CPUs. A good idea might be to set
        SystemStats::getNumCpus()
    */
    void setNumFilesToSplitBetween (int numFiles);

    /** Sets whether the progress should be written to std::out or not.

        By default this is off but is useful in command line apps.
    */
    void setLogOutput (bool shouldOutput);

    /** Sets the name to use for the output file. */
    void setUnityFileName (const juce::String& fileName);

    /** Returns the name to be used for the output file. */
    const juce::String& getUnityFileName() const noexcept { return unityName; }

    /** If this is not an empty string it will set the build directory of the given
        project to this. This means you can have normal and unity projects running in tandem.

        Note that this will replace everything except the exporter name.

        e.g. setting this to "BuildsUnity" will replace "Builds/MacOSX" with "BuildsUnity/MacOSX"
    */
    void setBuildDirectoryName (const juce::String& buildDirName);

    /** This optional extra step resaves the project with a provided Introjucer
        executable so is useful for generating the new project files.
    */
    void saveProject (const juce::File& introjucerAppFile);

private:
    //==============================================================================
    juce::File projectFile, unityProjectFile;
    juce::ValueTree project;
    juce::StringArray filesToAdd;
    int numFiles;
    bool shouldLog;
    juce::String unityName, buildDir;

    //==============================================================================
    void recurseGroup (juce::ValueTree group, const juce::File& sourceDir);
    void parseFile (juce::ValueTree file, const juce::File& sourceDir);
    juce::Array<juce::File> buildUnityCpp (const juce::File& destDir);
    juce::File buildUnityCpp (const juce::File& destDir, int unityNum, const juce::Range<int> fileRange);
    void updateBuildDirectories();
    void logOutput (const juce::String& output);

    //==============================================================================
    static bool isValidHeaderFile (const juce::File& file);
    static bool isValidSourceFile (const juce::File& file);
    static juce::File getExeFromApp (const juce::File& app);
    static juce::String createAlphaNumericUID();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnityProjectBuilder)
};

#endif //DROWAUDIO_UNITYPROJECTBUILDER_H
