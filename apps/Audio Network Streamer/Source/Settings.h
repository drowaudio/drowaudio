/*
  ==============================================================================

    Settings.h
    Created: 9 Jun 2011 5:13:48am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __SETTINGS_H_49C1F712__
#define __SETTINGS_H_49C1F712__

#include "../JuceLibraryCode/JuceHeader.h"

static const Identifier SettingsNames[] = {
											"mode",
											"port",
											"host",
											"compress",
											"audioSettings",
											"status"
										  };

//==============================================================================
class Settings	:	public DeletedAtShutdown
{
public:
	
	juce_DeclareSingleton(Settings, true);
	
	Settings();
	
	~Settings();
	
	ValueTree getValueTree()	{	return settingsTree;	}
	
	//==============================================================================
	enum ControlSettings
	{
		mode,
		port,
		host,
		compress,
		audioSettings,
		status,
		numControlSettings
	};
	
	ScopedPointer<AudioDeviceManager> audioManager;

private:
	
	File getResourcesFolder();
	bool loadSettingsFile(File settingsFile, ValueTree &treeToFill);
	bool buildDefaultSettings(ValueTree &treeToFill);
	bool saveSettingsFile(File settingsFile, ValueTree &treeToSave);
	
	ValueTree settingsTree;
};

#endif  // __SETTINGS_H_49C1F712__
