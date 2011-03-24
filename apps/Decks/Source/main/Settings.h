/*
  ==============================================================================

    Settings.h
    Created: 7 May 2010 4:58:00pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DECKS_SETTINGS_H_101B9845__
#define __DECKS_SETTINGS_H_101B9845__

#include <dRowAudio/dRowAudio.h>
#include "MixerSettings.h"
#include "UISettings.h"

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
	Value getPropertyOfChildAsValue(Identifier child, Identifier property)
	{
		return settings.getChildWithName(child).getPropertyAsValue(property, 0);
	}
	
	Value getPropertyOfChannelAsValue(int channelNo, Identifier property)
	{
		String channelName(MixerSettings::ChannelSettings::SectionName);
		channelName << channelNo;
		return settings.getChildWithName(MixerSettings::ChannelSettings::SectionName).getChildWithName(channelName).getPropertyAsValue(property, 0);
	}
		
	Value getPropertyOfXFaderAsValue(Identifier property)
	{
		return getPropertyOfChildAsValue(MixerSettings::xFaderSettings::SectionName, property);
	}
	
	Value getPropertyOfMasterAsValue(Identifier property)
	{
		return getPropertyOfChildAsValue(MixerSettings::MasterSettings::SectionName, property);
	}
	
	var getPropertyOfChild(Identifier child, Identifier property)		{ return getPropertyOfChildAsValue(child, property).getValue();	}
	
	var getPropertyOfChannel(int channelNo, Identifier property)	{ return getPropertyOfChannelAsValue(channelNo, property).getValue(); }
	
	var getPropertyOfXFader(Identifier property)					{ return getPropertyOfXFaderAsValue(property).getValue(); }
	
	var getPropertyOfMaster(Identifier property)					{ return getPropertyOfMasterAsValue(property).getValue(); }
	
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
		
		if (property == MixerSettings::ChannelSettings::SectionName)
			rebuildChannelsTree(treeWhosePropertyHasChanged.getProperty(property));
	}
	
	void valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded)	{}
	void valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved) {}
	void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved) {}
	
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
