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

#ifndef __DROWAUDIO_COLUMNFILEBROWSER_H__
#define __DROWAUDIO_COLUMNFILEBROWSER_H__

#include "dRowAudio_BasicFileBrowser.h"

class ColumnFileBrowserContents;
class ColumnFileBrowserLookAndFeel;

//==================================================================================
/** A type of FileBrowser persented in columns.
 
    This is similar to OSX's Finder column view. You can navigate around the columns 
    using the keyboard or the mouse. Highligting a number of files and then dragging 
    them will perform an external drag and drop procedure.
 */
class ColumnFileBrowser :	public Viewport
{
public:
    //==================================================================================
    /** Creates a ColumnFileBrowser with a given file filter.
     */
	ColumnFileBrowser (WildcardFileFilter* filesToDisplay);
	
    /** Destructor.
     */
	~ColumnFileBrowser();
	
    /** @internal */
	void resized();
		
    /** @internal */
	void visibleAreaChanged (const juce::Rectangle<int>& newVisibleArea);
	
    /** @internal */
	void mouseWheelMove (const MouseEvent& event, const MouseWheelDetails& wheel);
	
private:
    //==================================================================================
	ScopedPointer<WildcardFileFilter> wildcard;
	ColumnFileBrowserContents* fileBrowser;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColumnFileBrowser);
};

#endif //__DROWAUDIO_COLUMNFILEBROWSER_H__