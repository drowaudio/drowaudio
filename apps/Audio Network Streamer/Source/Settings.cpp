/*
  ==============================================================================

    Settings.cpp
    Created: 9 Jun 2011 5:13:48am
    Author:  David Rowland

  ==============================================================================
*/

#include "Settings.h"

juce_ImplementSingleton(Settings);

Settings::Settings()
:	settingsTree("settings")
{
	// load previous settings if possible
	File resourcesFolder(getResourcesFolder());
	resourcesFolder.createDirectory();
	File settingsFile(resourcesFolder.getChildFile("settings.xml"));
	if (loadSettingsFile(settingsFile, settingsTree) == false)
	{
		buildDefaultSettings(settingsTree);
	}	
}

Settings::~Settings()
{
	File settingsFile(getResourcesFolder().getChildFile("settings.xml"));
	saveSettingsFile(settingsFile, settingsTree);
	
	ScopedPointer<XmlElement> xml(settingsTree.createXml());
	if (&xml != nullptr) {
		DBG(xml->createDocument(""));
	}
}

File Settings::getResourcesFolder()
{
	return File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getParentDirectory().getChildFile("Resources");
}

bool Settings::loadSettingsFile(File settingsFile, ValueTree &treeToFill)
{
	if (settingsFile.existsAsFile())
	{			
		XmlDocument settingsDoc (settingsFile);
		ScopedPointer<XmlElement> settingsXML(settingsDoc.getDocumentElement());
		
		treeToFill = ValueTree::fromXml(*settingsXML);
		
		return treeToFill.isValid();
	}
	
	return false;
}

bool Settings::buildDefaultSettings(ValueTree &treeToFill)
{
	settingsTree = ValueTree("settings");
	settingsTree.setProperty(SettingsNames[Settings::mode], 1, nullptr);
	settingsTree.setProperty(SettingsNames[Settings::port], "12345", nullptr);
	settingsTree.setProperty(SettingsNames[Settings::host], "192.168.0.2", nullptr);
	settingsTree.setProperty(SettingsNames[Settings::compress], false, nullptr);
//	ValueTree audioTree(SettingsNames[Settings::audioSettings]);
//	settingsTree.addChild(audioTree, -1, nullptr);
	
	return treeToFill.isValid();
}

bool Settings::saveSettingsFile(File settingsFile, ValueTree &treeToSave)
{
	if (! settingsFile.existsAsFile()) {
		settingsFile.create();
	}
	
	if (treeToSave.isValid())
	{
		ScopedPointer<XmlElement> settingsXml(treeToSave.createXml());
		if (settingsXml->writeToFile(settingsFile, "")) {
			DBG("XML File written");
			return true;
		}
		else {
			DBG("Error in saving settings");
			return false;
		}
	}
	else
		DBG("Settings not valid to save");
	
	return false;
}