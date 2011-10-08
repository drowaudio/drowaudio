/*
  ==============================================================================

    Library.cpp
    Created: 29 Jan 2011 9:07:43pm
    Author:  David Rowland

  ==============================================================================
*/

#include "Library.h"

Library::Library()
{
	addAndMakeVisible(prepareButton = new TextButton("Prepare"));
	prepareButton->setClickingTogglesState(true);
	prepareButton->addListener(this);
	
	addAndMakeVisible(filterEditor = new TextEditor());
	filterEditor->addListener(this);
	
	addAndMakeVisible(resizeBar = new StretchableLayoutResizerBar(&layoutManager, 1, false));
	
	addAndMakeVisible(prepareTable = new PrepareTable());
	addAndMakeVisible(libraryTable = new MusicLibraryTable());

    ITunesLibrary* iTunesLibrary(ITunesLibrary::getInstance());
	libraryTable->setLibraryToUse(iTunesLibrary);
    File savedLibrary (File::getSpecialLocation(File::userDesktopDirectory).getChildFile("testLibrary.xml"));
//    iTunesLibrary->loadSavedLibraryIfNewer (savedLibrary);
    ValueTree treeToUse (readValueTreeFromFile (savedLibrary));
	iTunesLibrary->setLibraryTree (treeToUse);
    iTunesLibrary->setLibraryFile (ITunesLibrary::getDefaultITunesLibraryFile());
    
	layoutManager.setItemLayout(0, -0.1, -0.9, -0.5);
	layoutManager.setItemLayout(1, 5, 5, 5);
	layoutManager.setItemLayout(2, -0.1, -0.9, -0.5);
}

Library::~Library()
{
    File output(File::getSpecialLocation(File::userDesktopDirectory).getChildFile("testLibrary.xml"));
    ValueTree tree(ITunesLibrary::getInstance()->getLibraryTree());
    
//    if (tree.isValid()) {
//        DBG(ScopedPointer<XmlElement>(tree.createXml())->createDocument(""));
//    }
    
    ScopedPointer<XmlElement> xmlTree(tree.createXml());
    xmlTree->writeToFile(output, "");
    
	deleteAllChildren();
}

void Library::resized()
{
	int w = getWidth();
	int h = getHeight();
	int m = 5;
	
	filterEditor->setBounds(w-m-200, m, 200, 20);
	prepareButton->setBounds(filterEditor->getX() - 60 - m, m, 60, 20);

	if (prepareButton->getToggleState()) {
		Component* comps[] = { prepareTable, resizeBar, libraryTable };
		
		// this will position the 2 components, one above the other, to fit
		// vertically into the rectangle provided.
		layoutManager.layOutComponents (comps, 3,
										0, filterEditor->getBottom() + m, w, h-20-2*m,
										true, true);
	}
	else {
		libraryTable->setBounds(0, filterEditor->getBottom() + m, w, h-20-2*m);	
	}
}

void Library::paint(Graphics &g)
{
}
//================================================================================
void Library::buttonClicked(Button* button)
{
	resized();
}

void Library::textEditorTextChanged (TextEditor& editor)
{
	libraryTable->setFilterText(editor.getText());
}
