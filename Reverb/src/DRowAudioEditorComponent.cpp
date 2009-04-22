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

#include "includes.h"
#include "DRowAudioEditorComponent.h"

//==============================================================================
DRowAudioEditorComponent::DRowAudioEditorComponent (DRowAudioFilter* const ownerFilter)
    : AudioProcessorEditor (ownerFilter),
	noButtons(3)
{
	addAndMakeVisible( comboBox = new ComboBox(T("comboBox")) );
	
	for (int i = 0; i < noParams; i++)
	{
		sliders.add( new Slider(String(T("param")) << String(i)) );
		addAndMakeVisible( sliders[i]);
		
		String labelName = parameterNames[i];
		(new Label(String(T("Label")) << String(i), labelName))->attachToComponent(sliders[i], true);
			
		sliders[i]->addListener (this);
		sliders[i]->setRange (0.0, 1.0, 0.01);
		sliders[i]->setTextBoxStyle(Slider::TextBoxRight, false, 50, 20);
		sliders[i]->setValue (ownerFilter->getParameter (i), false);
		
		ownerFilter->getParameterPointer(i)->setupSlider(*sliders[i]);
	}
	
	for ( int i = 0; i < noButtons; i++ )
	{
		buttons.add(new TextButton(String(T("Button ")) << String(i)));
		buttons[i]->addButtonListener(this);
		addAndMakeVisible(buttons[i]);
	}
	buttons[0]->setButtonText(T("Make Allpass"));
	buttons[0]->setClickingTogglesState(true);
	

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
//	startTimer(50);
}

DRowAudioEditorComponent::~DRowAudioEditorComponent()
{
    getFilter()->removeChangeListener (this);
	sliders.clear();
	buttons.clear();
    deleteAllChildren();
}

//==============================================================================
void DRowAudioEditorComponent::paint (Graphics& g)
{
    // just clear the window
    g.fillAll (Colour::greyLevel (0.9f));
	
	g.setColour(Colours::red);
	g.setFont(30);
	g.drawFittedText(T("UWE Plug-in"),
					 getWidth()/2 - (getWidth()/2), 5,
					 getWidth(), 100,
					 Justification::centredTop,
					 1);
}

void DRowAudioEditorComponent::resized()
{
    comboBox->setBounds (getWidth()/2 - 100, 40,
						200, 20);
	
	int height = comboBox->getBottom();
	
	for (int i = 0; i < noParams; i++)	{
		sliders[i]->setBounds (70, 70 + (30*i), getWidth()-140, 20);
		height += 30;
	}
	
	height += 10;
	
	for ( int i = 0; i < noButtons; i++ )
		buttons[i]->setBounds( 10 + (i * ((getWidth()-20)/noButtons) + ((noButtons-1)*5)), height,
							  ((getWidth()-20)/noButtons)-(((noButtons-1)*5)), 20);
	
	height += 30;
	
	infoLabel->setBounds(10, height +=30, getWidth(), 20);
	
	setSize(400, height);
}

//==============================================================================
void DRowAudioEditorComponent::sliderValueChanged (Slider* changedSlider)
{
    DRowAudioFilter* currentFilter = getFilter();
	
	for (int i = 0; i < noParams; i++)
		if ( changedSlider == sliders[i] )
			currentFilter->setScaledParameterNotifyingHost (i, (float) sliders[i]->getValue());
}

void DRowAudioEditorComponent::sliderDragStarted(Slider* changedSlider)
{
	DRowAudioFilter* currentFilter = getFilter();
	
	for (int i = 0; i < noParams; i++)
		if ( changedSlider == sliders[i] )
			currentFilter->beginParameterChangeGesture(i);
}
void DRowAudioEditorComponent::sliderDragEnded(Slider* changedSlider)
{
	DRowAudioFilter* currentFilter = getFilter();
	
	for (int i = 0; i < noParams; i++)
		if ( changedSlider == sliders[i] )
			currentFilter->endParameterChangeGesture(i);
}

void DRowAudioEditorComponent::buttonClicked (Button* button)
{
	if(button == buttons[0]);
}

void DRowAudioEditorComponent::changeListenerCallback (void* source)
{
    // this is the filter telling us that it's changed, so we'll update our
    // display of the time, midi message, etc.
    updateParametersFromFilter();
}

//==============================================================================
void DRowAudioEditorComponent::updateParametersFromFilter()
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