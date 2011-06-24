/*
  ==============================================================================

    AudioSettingsComponent.h
    Created: 11 Jun 2011 7:45:58pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __AUDIOSETTINGSCOMPONENT_H_28BDDA35__
#define __AUDIOSETTINGSCOMPONENT_H_28BDDA35__

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
 A component containing controls to let the user change the audio settings of
 an AudioDeviceManager object.
 
 Very easy to use - just create one of these and show it to the user.
 
 @see AudioDeviceManager
 */
class AudioSettingsComponent  :	public Component,
								public ComboBox::Listener,
								public Button::Listener,
								public ChangeListener
{
public:
    //==============================================================================
    /** Creates the component.
	 
	 If your app needs only output channels, you might ask for a maximum of 0 input
	 channels, and the component won't display any options for choosing the input
	 channels. And likewise if you're doing an input-only app.
	 
	 @param deviceManager            the device manager that this component should control
	 @param minAudioInputChannels    the minimum number of audio input channels that the application needs
	 @param maxAudioInputChannels    the maximum number of audio input channels that the application needs
	 @param minAudioOutputChannels   the minimum number of audio output channels that the application needs
	 @param maxAudioOutputChannels   the maximum number of audio output channels that the application needs
	 @param showMidiInputOptions     if true, the component will allow the user to select which midi inputs are enabled
	 @param showMidiOutputSelector   if true, the component will let the user choose a default midi output device
	 @param showChannelsAsStereoPairs    if true, channels will be treated as pairs; if false, channels will be
	 treated as a set of separate mono channels.
	 @param hideAdvancedOptionsWithButton    if true, only the minimum amount of UI components
	 are shown, with an "advanced" button that shows the rest of them
	 */
    AudioSettingsComponent (AudioDeviceManager& deviceManager,
							const int minAudioInputChannels,
							const int maxAudioInputChannels,
							const int minAudioOutputChannels,
							const int maxAudioOutputChannels,	
							const bool showMidiInputOptions,
							const bool showMidiOutputSelector,
							const bool showChannelsAsStereoPairs,
							const bool hideAdvancedOptionsWithButton);
	
    /** Destructor */
    ~AudioSettingsComponent();
	
	int getHeightOfComponents()	{	return heightOfComponents;	}
	
    //==============================================================================
    /** @internal */
    void resized();
    /** @internal */
    void comboBoxChanged (ComboBox*);
    /** @internal */
    void buttonClicked (Button*);
    /** @internal */
    void changeListenerCallback (ChangeBroadcaster*);
    /** @internal */
    void childBoundsChanged (Component*);
	
private:
    //==============================================================================
    AudioDeviceManager& deviceManager;
    ScopedPointer<ComboBox> deviceTypeDropDown;
    ScopedPointer<Label> deviceTypeDropDownLabel;
    ScopedPointer<Component> audioDeviceSettingsComp;
    String audioDeviceSettingsCompType;
    const int minOutputChannels, maxOutputChannels, minInputChannels, maxInputChannels;
    const bool showChannelsAsStereoPairs;
    const bool hideAdvancedOptionsWithButton;
	
    class MidiInputSelectorComponentListBox;
    friend class ScopedPointer<MidiInputSelectorComponentListBox>;
    ScopedPointer<MidiInputSelectorComponentListBox> midiInputsList;
    ScopedPointer<ComboBox> midiOutputSelector;
    ScopedPointer<Label> midiInputsLabel, midiOutputLabel;
	
	int heightOfComponents;

	void updateAllControls();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSettingsComponent);
};

#endif  // __AUDIOSETTINGSCOMPONENT_H_28BDDA35__
