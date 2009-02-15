/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-7 by Raw Material Software ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the
   GNU General Public License, as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later version.

   JUCE is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with JUCE; if not, visit www.gnu.org/licenses or write to the
   Free Software Foundation, Inc., 59 Temple Place, Suite 330,
   Boston, MA 02111-1307 USA

  ------------------------------------------------------------------------------

   If you'd like to release a closed-source product which uses JUCE, commercial
   licenses are also available: visit www.rawmaterialsoftware.com/juce for
   more information.

  ==============================================================================
*/

#include "DemoEditorComponent.h"

//==============================================================================
// quick-and-dirty function to format a timecode string
static const String timeToTimecodeString (const double seconds)
{
    const double absSecs = fabs (seconds);
    const tchar* const sign = (seconds < 0) ? T("-") : T("");

    const int hours = (int) (absSecs / (60.0 * 60.0));
    const int mins  = ((int) (absSecs / 60.0)) % 60;
    const int secs  = ((int) absSecs) % 60;

    return String::formatted (T("%s%02d:%02d:%02d:%03d"),
                              sign, hours, mins, secs,
                              roundDoubleToInt (absSecs * 1000) % 1000);
}

// quick-and-dirty function to format a bars/beats string
static const String ppqToBarsBeatsString (const double ppq,
                                          const double lastBarPPQ,
                                          const int numerator,
                                          const int denominator)
{
    if (numerator == 0 || denominator == 0)
        return T("1|1|0");

    const int ppqPerBar = (numerator * 4 / denominator);
    const double beats  = (fmod (ppq, ppqPerBar) / ppqPerBar) * numerator;

    const int bar       = ((int) ppq) / ppqPerBar + 1;
    const int beat      = ((int) beats) + 1;
    const int ticks     = ((int) (fmod (beats, 1.0) * 960.0));

    String s;
    s << bar << T('|') << beat << T('|') << ticks;
    return s;
}


//==============================================================================
DemoEditorComponent::DemoEditorComponent (DRowAudioFilter* const ownerFilter)
    : AudioProcessorEditor (ownerFilter),
	noButtons(3)
{
	addAndMakeVisible( comboBox = new ComboBox(T("comboBox")) );
	
	for (int i = 0; i < noParams; i++)
	{
		sliders.add( new Slider(String(T("param")) << String(i)) );
		addAndMakeVisible( sliders[i]);
		
		String labelName = ownerFilter->getParameterName(i);
		(new Label(String(T("Label")) << String(i), labelName))->attachToComponent(sliders[i], true);
			
		sliders[i]->addListener (this);
		sliders[i]->setRange (ownerFilter->getParameterMin(i), ownerFilter->getParameterMax(i), ownerFilter->getParameterStep(i));
		sliders[i]->setSkewFactor(ownerFilter->getParameterSkewFactor(i));
		sliders[i]->setTextBoxStyle(Slider::TextBoxRight, false, 50, 20);
		sliders[i]->setValue (ownerFilter->getScaledParameter(i), false);
	}
		
	for ( int i = 0; i < noButtons; i++ )
	{
		buttons.add(new TextButton(String(T("Button ")) << String(i)));
		addAndMakeVisible(buttons[i]);
	}
	buttons[0]->setButtonText(T("Monitor"));
	buttons[0]->setClickingTogglesState(true);
	buttons[0]->addButtonListener(this);
	
	buttons[1]->setButtonText(T("Use RMS"));
	buttons[1]->setClickingTogglesState(true);
	buttons[1]->addButtonListener(this);
	
	// set up the meters
	addAndMakeVisible(meterLeft = new MeterComponent(&ownerFilter->RMSLeft, &ownerFilter->peakLeft, ownerFilter->getCallbackLock()));
	addAndMakeVisible(meterRight = new MeterComponent(&ownerFilter->RMSRight, &ownerFilter->peakRight, ownerFilter->getCallbackLock()));
	
    // create and add the midi keyboard component..
    addAndMakeVisible (midiKeyboard
        = new MidiKeyboardComponent (ownerFilter->keyboardState,
                                     MidiKeyboardComponent::horizontalKeyboard));

    // add a label that will display the current timecode and status..
    addAndMakeVisible (infoLabel = new Label (String::empty, String::empty));

    // add the triangular resizer component for the bottom-right of the UI
//    addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
//    resizeLimits.setSizeLimits (150, 150, 800, 300);

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (400, 500);

    // register ourselves with the filter - it will use its ChangeBroadcaster base
    // class to tell us when something has changed, and this will call our changeListenerCallback()
    // method.
    ownerFilter->addChangeListener (this);
	
	// start the timer to update the meters
	startTimer(50);
}

DemoEditorComponent::~DemoEditorComponent()
{
    getFilter()->removeChangeListener (this);
	sliders.clear();
	buttons.clear();
    deleteAllChildren();
}

//==============================================================================
void DemoEditorComponent::paint (Graphics& g)
{
    // just clear the window
    g.fillAll (Colour::greyLevel (0.9f));
	
	g.setColour(Colours::red);
	g.setFont(30);
	g.drawFittedText(T("dRowAudio: Gate"),
					 getWidth()/2 - (getWidth()/2), 5,
					 getWidth(), getHeight(),
					 Justification::centredTop,
					 1);
}

void DemoEditorComponent::resized()
{
    comboBox->setBounds (getWidth()/2 - 100, 40,
						200, 20);
	
	for (int i = 0; i < noParams; i++)
		sliders[i]->setBounds (70, 70 + (30*i), getWidth()-140, 20);
	
	meterLeft->setBounds(getWidth()-65, 70, 25, 290);
	meterRight->setBounds(getWidth()-35, 70, 25, 290);
		
	for ( int i = 0; i < noButtons; i++ )
		buttons[i]->setBounds( 10 + (i * ((getWidth()-20)/noButtons) + ((noButtons-1)*5)), 370,
							  ((getWidth()-20)/noButtons)-(((noButtons-1)*5)), 20);
	
	infoLabel->setBounds(10, 400, getWidth(), 20);
	
    const int keyboardHeight = 70;
    midiKeyboard->setBounds (4, getHeight() - keyboardHeight - 4,
                             getWidth() - 8, keyboardHeight);
}

//==============================================================================
void DemoEditorComponent::sliderValueChanged (Slider* changedSlider)
{
    DRowAudioFilter* currentFilter = getFilter();
	
	for (int i = 0; i < noParams; i++)
		if ( changedSlider == sliders[i] )
			currentFilter->setScaledParameterNotifyingHost (i, (float) sliders[i]->getValue());
}

void DemoEditorComponent::buttonClicked(Button* clickedButton)
{
	DRowAudioFilter* currentFilter = getFilter();
	
	if (clickedButton == buttons[0])
	{
		if(clickedButton->getToggleState())
			currentFilter->setScaledParameterNotifyingHost(MONITOR, 1.0);
		else
			currentFilter->setScaledParameterNotifyingHost(MONITOR, 0.0);	
	}
}

void DemoEditorComponent::changeListenerCallback (void* source)
{
    // this is the filter telling us that it's changed, so we'll update our
    // display of the time, midi message, etc.
    updateParametersFromFilter();
}

//==============================================================================
void DemoEditorComponent::updateParametersFromFilter()
{
    DRowAudioFilter* const filter = getFilter();
	
	float tempParamVals[noParams];
	
    // we use this lock to make sure the processBlock() method isn't writing to the
    // lastMidiMessage variable while we're trying to read it, but be extra-careful to
    // only hold the lock for a minimum amount of time..
    filter->getCallbackLock().enter();

    // take a local copy of the info we need while we've got the lock..
    const AudioPlayHead::CurrentPositionInfo positionInfo (filter->lastPosInfo);
	
	for(int i = 0; i < noParams; i++)
		tempParamVals[i] =  filter->getScaledParameter (i);

    // ..release the lock ASAP
    filter->getCallbackLock().exit();


    // ..and after releasing the lock, we're free to do the time-consuming UI stuff..
    String infoText;
    infoText << String (positionInfo.bpm, 2) << T(" bpm, ")
             << positionInfo.timeSigNumerator << T("/") << positionInfo.timeSigDenominator
             << T("  -  ") << timeToTimecodeString (positionInfo.timeInSeconds)
             << T("  -  ") << ppqToBarsBeatsString (positionInfo.ppqPosition,
                                                    positionInfo.ppqPositionOfLastBarStart,
                                                    positionInfo.timeSigNumerator,
                                                    positionInfo.timeSigDenominator);

    if (positionInfo.isPlaying)
        infoText << T("  (playing)");

    infoLabel->setText (infoText, false);

    /* Update our slider.

       (note that it's important here to tell the slider not to send a change
       message, because that would cause it to call the filter with a parameter
       change message again, and the values would drift out.
    */
	for(int i = 0; i < noParams; i++)
		sliders[i]->setValue (tempParamVals[i], false);
}


void DemoEditorComponent::timerCallback()
{
//	DRowAudioFilter* currentFilter = getFilter();
//	
//	meterLeft->setMeterLevel(currentFilter->peakLeft, currentFilter->peakLeft);
//	meterRight->setMeterLevel(currentFilter->peakRight, currentFilter->peakRight);
}