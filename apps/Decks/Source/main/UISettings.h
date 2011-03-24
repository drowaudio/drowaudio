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
#define DRAGGABLE_SETTING(setting) UISettings::DraggableDisplaySettings::Names[UISettings::DraggableDisplaySettings::setting]

#define DRAGGABLE_SETTING_VALUE(setting) UISettings::DraggableDisplaySettings::Values[UISettings::DraggableDisplaySettings::setting]

namespace UISettings {
	
	namespace DraggableDisplaySettings {
		static const Identifier SectionName = "draggableDisplaySettings";
		
		enum DraggableSettings {
			zoom,
			centrePos,
			noDraggableSettings
		};
		
		static const Identifier UNUSED_NOWARN Names[] = {
			"zoom",
			"centrePos"
		};
		
		static const var Values[] = 
		{
			0.0,		//zoom
			0.5,		//centrePos
		};
	}

	static void buildDefaultUISettings(ValueTree& treeToFill)
	{
		ValueTree uiSettingsTree(UISettings::DraggableDisplaySettings::SectionName);
		for (int p = 0; p < UISettings::DraggableDisplaySettings::noDraggableSettings; p++)
		{
			uiSettingsTree.setProperty(UISettings::DraggableDisplaySettings::Names[p], UISettings::DraggableDisplaySettings::Values[p], 0);
		}
		treeToFill.addChild(uiSettingsTree, -1, 0);
	}
	
}	//UISettings

#endif  // __UISETTINGS_H_C80EB321__
