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

static const String CompressionLevelNames[] = {
	"No Compression",
	"Basic",
	"Short Lossy",
	"Short GZip",
	"Short Derivative Lossy"
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
	
	enum CompressionLevel
	{
		noCompression = 1,
		gZipCompression = 2,
		shortCompression = 3,
		shortGzipCompression = 4,
		shortDerivativeCompression = 5,
		numCompressionLevels = 5
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
