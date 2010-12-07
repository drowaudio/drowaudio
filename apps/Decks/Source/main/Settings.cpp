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
	DBG("Settings deleted");
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
	MixerSettings::buildDefaultMixerSettings(treeToFill);
	
	return treeToFill.isValid();
}

bool Settings::rebuildChannelsTree(int newNoChannels)
{
	ValueTree channels = settings.getChildWithName(MixerSettings::ChannelSettings::SectionName);
	int currentNoChannels = channels.getNumChildren();
	String message("rebuilding channels tree for ");
	message << currentNoChannels;
	DBG(message);
	
	if (newNoChannels > currentNoChannels) {
		for (int i = currentNoChannels-1; i < newNoChannels; i++)
		{
			String name(MixerSettings::ChannelSettings::SectionName);
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