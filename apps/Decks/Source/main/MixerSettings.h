/*
  ==============================================================================

    MixerSettings.h
    Created: 6 May 2010 3:52:24pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DECKS_MIXERSETTINGS_H_F14E7836__
#define __DECKS_MIXERSETTINGS_H_F14E7836__

#include <juce/juce.h>

namespace MixerSettings
{
	static const int noChannels = 4;
	
	namespace ChannelSettings {
		static const char *SectionName = "channel";

		enum ChannelSettings{
			on,
			bypass,
			gain,
			level,
			highGain,
			midGain,
			lowGain,
			highKill,
			midKill,
			lowKill,
			cue,
			fxASend,
			fxBSend,
			noChannelSettings
		};
		
		static const char UNUSED_NOWARN *Names[] = {
			"on",
			"bypass",
			"gain",
			"level",
			"highGain",
			"midGain",
			"lowGain",
			"highKill",
			"midKill",
			"lowKill",
			"cue",
			"fxASend",
			"fxBSend"			
		};
		
		static const var Values[] = 
		{
			true,		//on
			false,		//bypass
			1.0f,		//gain
			0.0,		//level
			1.0f,		//highGain
			1.0f,		//midGain
			1.0f,		//lowGain
			false,		//highKill
			false,		//midKill
			false,		//lowKill
			true,		//cue
			0.0f,		//fxASend
			0.0f,		//fxBSend	
		};
	}
		
	namespace xFaderSettings {
		static const char *SectionName = "xFader";

		enum xFaderSettings {
			assignX,
			assignY,
			level,
			noXFaderSettings
		};
		
		static const char UNUSED_NOWARN *Names[] = {
			"assignX",
			"assignY",
			"level"
		};
		
		static const var Values[] = 
		{
			0.0f,		//assignX
			0.0f,		//assignY
			0.5f		//level
		};
	}
		
	namespace MasterSettings {
		static const char *SectionName = "master";
		
		enum MasterSettings {
			gain,
			cue,
			faderCurve,
			xFaderCurve,
			noMasterSettings
		};
		
		static const char UNUSED_NOWARN *Names[] = {
			"gain",
			"cue",
			"faderCurve",
			"xFaderCurve"
		};
		
		static const var Values[] = 
		{
			1.0f,		//gain
			false,		//cue
			1.0f,		//faderCurve
			0.5f		//xFaderCurve
		};		
	}
	
	static void buildDefaultMixerSettings(ValueTree& treeToFill)
	{
		ValueTree noChannels("noChannels");
		noChannels.setProperty("noChannels", MixerSettings::noChannels, 0);
		treeToFill.addChild(noChannels, -1, 0);
		
		ValueTree channels(MixerSettings::ChannelSettings::SectionName);
		for (int i = 0; i < MixerSettings::noChannels; i++)
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
		treeToFill.addChild(channels, -1, 0);
		
		ValueTree xFaderTree(MixerSettings::xFaderSettings::SectionName);
		for (int p = 0; p < MixerSettings::xFaderSettings::noXFaderSettings; p++)
		{
			xFaderTree.setProperty(MixerSettings::xFaderSettings::Names[p], MixerSettings::xFaderSettings::Values[p], 0);
		}
		treeToFill.addChild(xFaderTree, -1, 0);
		
		ValueTree masterTree(MixerSettings::MasterSettings::SectionName);
		for (int p = 0; p < MixerSettings::MasterSettings::noMasterSettings; p++)
		{
			masterTree.setProperty(MixerSettings::MasterSettings::Names[p], MixerSettings::MasterSettings::Values[p], 0);
		}
		treeToFill.addChild(masterTree, -1, 0);
	}
};


#endif  // __DECKS_MIXERSETTINGS_H_F14E7836__
