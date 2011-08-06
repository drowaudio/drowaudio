/*
  ==============================================================================

    UISettings.h
    Created: 15 Mar 2011 11:26:13am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __UISETTINGS_H_C80EB321__
#define __UISETTINGS_H_C80EB321__

// use these macros to shorten getting the various settings
#define DRAGGABLE_SETTING(setting) UISettings::Names[UISettings::setting]
#define MIXTYPE_SETTING(setting) UISettings::Names[UISettings::setting]

#define DRAGGABLE_SETTING_VALUE(setting) UISettings::Values[UISettings::setting]
#define MIXTYPE_SETTING_VALUE(setting) UISettings::Values[UISettings::setting]

namespace UISettings {
	static const Identifier SectionName = "UISettings";
	
//	namespace DraggableDisplaySettings {
//		static const Identifier SectionName = "DraggableSettings";
		
		enum DraggableSettings {
			zoom,
			playheadPos,
			same,
			plusOne,
			minusOne,
			moodChange,
			plusTwoBoost,			
			numDraggableSettings
		};
		
		static const Identifier UNUSED_NOWARN Names[] = {
			"zoom",
			"playheadPos",
			"same",
			"plusOne",
			"minusOne",
			"moodChange",
			"plusTwoBoost"			
		};
		
		static const var Values[] = 
		{
			1.0,		//zoom
			0.5,		//playheadPos
			true,	//same
			true,	//plusOne
			false,	//minusOne
			false,	//moodChange
			true	//plusTwoBoost			
		};
//	}

	/*namespace MixTypeSettings {
		static const Identifier SectionName = "MixTypeSettings";
		
		enum MixTypeSettings {
			plusOne,
			minusOne,
			moodChange,
			plusTwoBoost,
			numMixTypeSettings
		};
		
		static const Identifier UNUSED_NOWARN Names[] = {
			"plusOne",
			"minusOne",
			"moodChange",
			"plusTwoBoost"
		};
		
		static const var Values[] = 
		{
			true,	//plusOne
			false,	//minusOne
			false,	//moodChange
			true	//plusTwoBoost
		};
	}*/
	
	static void buildDefaultUISettings(ValueTree& treeToFill)
	{
		ValueTree uiSettingsTree(UISettings::SectionName);

//		ValueTree draggableTree(UISettings::DraggableDisplaySettings::SectionName);
		for (int p = 0; p < UISettings::numDraggableSettings; p++)
		{
			uiSettingsTree.setProperty(UISettings::Names[p], UISettings::Values[p], 0);
		}
//		uiSettingsTree.addChild(draggableTree, -1, 0);

//		ValueTree mixTypeTree(UISettings::MixTypeSettings::SectionName);
//		for (int p = 0; p < UISettings::MixTypeSettings::numMixTypeSettings; p++)
//		{
//			uiSettingsTree.setProperty(UISettings::Names[p], UISettings::Values[p], 0);
//		}
//		uiSettingsTree.addChild(mixTypeTree, -1, 0);
		
		treeToFill.addChild(uiSettingsTree, -1, 0);
	}
	
}	//UISettings

#endif  // __UISETTINGS_H_C80EB321__
