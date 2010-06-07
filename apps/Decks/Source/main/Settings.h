/*
  ==============================================================================

    Settings.h
    Created: 7 May 2010 4:58:00pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DECKS_SETTINGS_H_101B9845__
#define __DECKS_SETTINGS_H_101B9845__

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>
#include "MixerSettings.h"

class Settings	:	public ValueTree::Listener
{	
public:

	Settings();

	~Settings();

	juce_DeclareSingleton(Settings, false);
	
	ValueTree getValueTree()			{ return settings;	}
	ValueTree* getValueTreePointer()	{ return &settings;	}
	
	// use these macros to shorten getting the various settings
#define CHANNEL_SETTING(setting) MixerSettings::ChannelSettings::Names[MixerSettings::ChannelSettings::setting]
#define XFADER_SETTING(setting) MixerSettings::xFaderSettings::Names[MixerSettings::xFaderSettings::setting]
#define MASTER_SETTING(setting) MixerSettings::MasterSettings::Names[MixerSettings::MasterSettings::setting]
	
	var getPropertyOfChild(String child, String property)
	{
		return settings.getChildWithName(child).getPropertyAsValue(property, 0).getValue();
	}
	
	var getPropertyOfChannel(int channelNo, String property)
	{
		String channelName(MixerSettings::ChannelSettings::SectionName);
		channelName << channelNo;
		return settings.getChildWithName(channelName).getPropertyAsValue(property, 0).getValue();
	}
	
	var getPropertyOfXFader(String property)
	{
		return getPropertyOfChild(MixerSettings::xFaderSettings::SectionName, property);
	}
	
	var getPropertyOfMaster(String property)
	{
		return getPropertyOfChild(MixerSettings::MasterSettings::SectionName, property);
	}
	
	bool loadSettingsFile(String path, ValueTree &treeToFill);
	
	bool buildDefaultSettings(ValueTree &treeToFill);
	
	bool rebuildChannelsTree(int newNoChannels);
	
//===============================================================================
	void valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const var::identifier  &property)
	{
		String message(property.name);
		message << " *-* " << treeWhosePropertyHasChanged.getProperty(property).toString();
		DBG(message);
		
		if (property.name == "noChannels")
			rebuildChannelsTree(treeWhosePropertyHasChanged.getProperty(property));
	}
	
	void valueTreeChildrenChanged (ValueTree &treeWhoseChildHasChanged)
	{
		DBG("child changed");
	}
	
	void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged)
	{
		DBG("parent changed");
	}
	
//===============================================================================
	
private:
	ValueTree settings;
	String settingsPath;
	
};

#endif  // __DECKS_SETTINGS_H_101B9845__
