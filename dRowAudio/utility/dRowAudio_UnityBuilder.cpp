/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

//==============================================================================
UnityBuilder::UnityBuilder()
{
}

UnityBuilder::~UnityBuilder()
{
}

bool UnityBuilder::processDirectory (const File& sourceDirectory)
{
    if (sourceDirectory.isDirectory())
    {
        Array<File> files;
        sourceDirectory.findChildFiles (files, File::findFiles + File::ignoreHiddenFiles, true);
        
        String includeString, sourceString;
        
        for (int i = 0; i < files.size(); ++i)
        {
            bool includeFile = true;
            File& currentFile (files.getReference (i));

            // first check files
            if (! filesToIgnore.contains (currentFile))
            {
                // now check for directories
                for (int i = 0; i < filesToIgnore.size(); ++i)
                {
                    File& currentDir (filesToIgnore.getReference (i));

                    if (currentDir.isDirectory()
                        && currentFile.isAChildOf (currentDir))
                    {
                        includeFile = false;
                        break;
                    }
                }
                
                if (includeFile)
                {
                    const String relativePath (currentFile.getRelativePathFrom (sourceDirectory));

                    if (currentFile.hasFileExtension (".h"))
                        includeString << "#include \"" << relativePath << "\"" << newLine;
                    else if (currentFile.hasFileExtension (".cpp"))
                        sourceString << "#include \"" << relativePath << "\"" << newLine;
                }
            }
        }
        
        // now write the output files
        File outputFile (destinationFile == File::nonexistent ? sourceDirectory : destinationFile);
        
        if (outputFile.hasWriteAccess())
        {
            File headerFile, cppFile;
            
            if (outputFile.isDirectory())
            {
                const String baseName ("UnityBuild");
                
                headerFile = outputFile.getNonexistentChildFile (baseName, ".h");
                cppFile = outputFile.getNonexistentChildFile (baseName, ".cpp");
            }
            else
            {
                headerFile = outputFile.getNonexistentSibling().withFileExtension (".h");
                cppFile = outputFile.getNonexistentSibling().withFileExtension (".cpp");
            }

            String headerOutput (preInclusionString);
            String sourceOutput (preInclusionString);
            
            headerOutput << includeString << postInclusionString;
            sourceOutput << "#include \"" << headerFile.getFileName() << "\"" << newLine << newLine
                << sourceString << postInclusionString;
            
            headerFile.replaceWithText (headerOutput);
            cppFile.replaceWithText (sourceOutput);
            
            return true;
        }
    }
    
    return false;
}

void UnityBuilder::setDestinationFile (const File& newDestinationFile)
{
    destinationFile = newDestinationFile;
}

void UnityBuilder::setFilesToIgnore (const Array<File>& filesToIgnore_)
{
    filesToIgnore = filesToIgnore_;
}

void UnityBuilder::setPreAndPostString (const String& preInclusionString_,
                                        const String& postInclusionString_)
{
    preInclusionString = preInclusionString_;
    postInclusionString = postInclusionString_;
}
