/*
  ==============================================================================

    Settings.cpp
    Created: 6 Jun 2010 3:24:07am
    Author:  David Rowland

  ==============================================================================
*/

#include "Settings.h"

juce_ImplementSingleton(Settings);

Settings::Settings()
: settings("settings")
{
	settings.addListener(this);
	
	// create resources folder if needed
	File resourcesFolder = getResourcesFolder();
	resourcesFolder.createDirectory();
	
	settingsPath = resourcesFolder.getFullPathName();
	settingsPath << "/settings.xml";
	DBG(settingsPath);
	
	// attempt to load XML,if not build defaults
	if (!loadSettingsFile(settingsPath, settings))
		buildDefaultSettings(settings);
}

Settings::~Settings()
{
	// save current settings
	File xmlFile (settingsPath);
	if (xmlFile == File::nonexistent) {
		xmlFile.create();
	}
	
	if (settings.isValid())
	{
		if (settings.createXml()->writeToFile(xmlFile, "DECKS_SETTINGS")) {
			DBG("XML File written");
		}
		else {
			DBG("Error in saving settings");
		}
	}
	else
		DBG("Settings not valid to save");
	
	settings.removeListener(this);
}

bool Settings::loadSettingsFile(String path, ValueTree &treeToFill)
{
	File settingsFile(path); 
	if (settingsFile.existsAsFile())
	{			
		XmlDocument settingsDoc (settingsFile);
		XmlElement* settingsXML = settingsDoc.getDocumentElement();
		
		treeToFill = ValueTree::fromXml(*settingsXML);
		
		if (treeToFill.isValid())
			return true;
		else
			return false;
	}
	
	return false;
}

bool Settings::buildDefaultSettings(ValueTree &treeToFill)
{
	ValueTree noChannels("noChannels");
	noChannels.setProperty("noChannels", MixerSettings::noChannels, 0);
	treeToFill.addChild(noChannels, -1, 0);
	
	ValueTree channels("channels");
	for (int i = 0; i < MixerSettings::noChannels; i++)
	{
		String name("channel");
		name<<i;
		ValueTree deck(name);
		
		for (int p = 0; p < MixerSettings::ChannelSettings::noChannelSettings; p++)
		{
			deck.setProperty(MixerSettings::ChannelSettings::Names[p], MixerSettings::ChannelSettings::Values[p], 0);
		}
		
		channels.addChild(deck, -1, 0);
	}
	treeToFill.addChild(channels, -1, 0);
	
	ValueTree xFaderTree("xFader");
	for (int p = 0; p < MixerSettings::xFaderSettings::noXFaderSettings; p++)
	{
		xFaderTree.setProperty(MixerSettings::xFaderSettings::Names[p], MixerSettings::xFaderSettings::Values[p], 0);
	}
	treeToFill.addChild(xFaderTree, -1, 0);
	
	ValueTree masterTree("master");
	for (int p = 0; p < MixerSettings::MasterSettings::noMasterSettings; p++)
	{
		masterTree.setProperty(MixerSettings::MasterSettings::Names[p], MixerSettings::MasterSettings::Values[p], 0);
	}
	treeToFill.addChild(masterTree, -1, 0);
	
	return treeToFill.isValid();
}

bool Settings::rebuildChannelsTree(int newNoChannels)
{
	ValueTree channels = settings.getChildWithName("channels");
	int currentNoChannels = channels.getNumChildren();
	DBG(String(currentNoChannels));
	
	if (newNoChannels > currentNoChannels) {
		for (int i = currentNoChannels-1; i < newNoChannels; i++)
		{
			String name("channel");
			name<<i;
			ValueTree deck(name);
			
			for (int p = 0; p < MixerSettings::ChannelSettings::noChannelSettings; p++)
			{
				deck.setProperty(MixerSettings::ChannelSettings::Names[p], MixerSettings::ChannelSettings::Values[p], 0);
			}
			
			channels.addChild(deck, -1, 0);
		}
	}
	else if (newNoChannels < currentNoChannels)
	{
		for (int i = currentNoChannels-1; i > newNoChannels-1; i--) {
			channels.removeChild(i, 0);
		}
	}
	
	return settings.isValid();
}