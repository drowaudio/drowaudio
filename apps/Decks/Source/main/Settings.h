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

class Settings	:	public ValueTree::Listener,
					public DeletedAtShutdown
{	
public:

	Settings();

	~Settings();

	juce_DeclareSingleton(Settings, false);
	
	ValueTree getValueTree()			{ return settings;	}
	ValueTree* getValueTreePointer()	{ return &settings;	}
	
	//===============================================================================
	// use these macros to shorten getting the various settings
#define CHANNEL_SETTING(setting) MixerSettings::ChannelSettings::Names[MixerSettings::ChannelSettings::setting]
#define XFADER_SETTING(setting) MixerSettings::xFaderSettings::Names[MixerSettings::xFaderSettings::setting]
#define MASTER_SETTING(setting) MixerSettings::MasterSettings::Names[MixerSettings::MasterSettings::setting]
	
//	var getPropertyOfChild(String child, String property)
//	{
//		return settings.getChildWithName(child).getPropertyAsValue(property, 0).getValue();
//	}
//	
//	var getPropertyOfChannel(int channelNo, String property)
//	{
//		String channelName(MixerSettings::ChannelSettings::SectionName);
//		channelName << channelNo;
//		return settings.getChildWithName(channelName).getPropertyAsValue(property, 0).getValue();
//	}
//	
//	var getPropertyOfXFader(String property)
//	{
//		return getPropertyOfChild(MixerSettings::xFaderSettings::SectionName, property);
//	}
//	
//	var getPropertyOfMaster(String property)
//	{
//		return getPropertyOfChild(MixerSettings::MasterSettings::SectionName, property);
//	}
	
	Value getPropertyOfChildAsValue(String child, String property)
	{
		return settings.getChildWithName(child).getPropertyAsValue(property, 0);
	}
	
	Value getPropertyOfChannelAsValue(int channelNo, String property)
	{
		String channelName(MixerSettings::ChannelSettings::SectionName);
		channelName << channelNo;
		return settings.getChildWithName(MixerSettings::ChannelSettings::SectionName).getChildWithName(channelName).getPropertyAsValue(property, 0);
	}
		
	Value getPropertyOfXFaderAsValue(String property)
	{
		return getPropertyOfChildAsValue(MixerSettings::xFaderSettings::SectionName, property);
	}
	
	Value getPropertyOfMasterAsValue(String property)
	{
		return getPropertyOfChildAsValue(MixerSettings::MasterSettings::SectionName, property);
	}
	
	var getPropertyOfChild(String child, String property)		{ return getPropertyOfChildAsValue(child, property).getValue();	}
	
	var getPropertyOfChannel(int channelNo, String property)	{ return getPropertyOfChannelAsValue(channelNo, property).getValue(); }
	
	var getPropertyOfXFader(String property)					{ return getPropertyOfXFaderAsValue(property).getValue(); }
	
	var getPropertyOfMaster(String property)					{ return getPropertyOfMasterAsValue(property).getValue(); }
	
//===============================================================================
	bool loadSettingsFile(String path, ValueTree &treeToFill);
	
	bool buildDefaultSettings(ValueTree &treeToFill);
	
	bool rebuildChannelsTree(int newNoChannels);
	
//===============================================================================
	void valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property)
	{
		String message;
		message << treeWhosePropertyHasChanged.getType().toString();
		message << " - " << property.toString();
		message << " - " << treeWhosePropertyHasChanged.getProperty(property).toString();
		DBG(message);
		
		if (property.toString() == MixerSettings::ChannelSettings::SectionName)
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
