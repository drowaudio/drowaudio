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

#include "DRowAudioEditorComponent.h"

//==============================================================================
DRowAudioEditorComponent::DRowAudioEditorComponent (DRowAudioFilter* const ownerFilter)
    :	AudioProcessorEditor (ownerFilter),
		noButtons(3)
{
	customLookAndFeel = new dRowLookAndFeel;
	setLookAndFeel(customLookAndFeel);
//	customLookAndFeel->setColour(Label::textColourId, (Colours::black).withBrightness(0.9f));
	
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
		sliders[i]->setTextBoxStyle(Slider::TextBoxRight, false, 70, 20);
		sliders[i]->setValue (ownerFilter->getScaledParameter(i), false);
		
		ownerFilter->getParameterPointer(i)->setupSlider(*sliders[i]);
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
	
	buttons[2]->setButtonText(parameterNames[FILTER]);
	buttons[2]->setClickingTogglesState(true);
	buttons[2]->addButtonListener(this);
	
	// set up the meters
	addAndMakeVisible(meterLeft = new MeterComponent(&ownerFilter->RMSLeft, &ownerFilter->peakLeft, &ownerFilter->getCallbackLock()));
	addAndMakeVisible(meterRight = new MeterComponent(&ownerFilter->RMSRight, &ownerFilter->peakRight, &ownerFilter->getCallbackLock()));

	addAndMakeVisible( ownerFilter->waveformDisplayPre );
	addAndMakeVisible( ownerFilter->waveformDisplayPost );

	addAndMakeVisible(incLabel = new Label(T("incLabel"), T("0.0")));
	addAndMakeVisible(currentLabel = new Label(T("currentLabel"), T("0.0")));
	
    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (400, 500);

    // register ourselves with the filter - it will use its ChangeBroadcaster base
    // class to tell us when something has changed, and this will call our changeListenerCallback()
    // method.
    ownerFilter->addChangeListener (this);
	
	// start the timer to update the meters
	startTimer(50);
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
	g.drawFittedText(T("dRowAudio: Gate"),
					 getWidth()/2 - (getWidth()/2), 5,
					 getWidth(), getHeight(),
					 Justification::centredTop,
					 1);
	
}

void DRowAudioEditorComponent::resized()
{
    comboBox->setBounds (getWidth()/2 - 100, 40,
						200, 20);
	
	for (int i = 0; i < noParams; i++)
		sliders[i]->setBounds (70, 70 + (30*i), getWidth()-140, 20);
	
//	meterLeft->setBounds(getWidth()-65, 70, 25, 290);
//	meterRight->setBounds(getWidth()-35, 70, 25, 290);
		
	for ( int i = 0; i < noButtons; i++ )
		buttons[i]->setBounds( 10 + (i * ((getWidth()-20)/noButtons) + ((noButtons-1)*5)), 370,
							  ((getWidth()-20)/noButtons)-(((noButtons-1)*5)), 20);
	
	getFilter()->waveformDisplayPre->setBounds(10, buttons[0]->getBottom()+10,
											   getWidth()-20, (getHeight()-buttons[0]->getBottom()-20)*0.5-1);
	getFilter()->waveformDisplayPost->setBounds(10, getFilter()->waveformDisplayPre->getBottom()+2,
												getWidth()-20, (getHeight()-buttons[0]->getBottom()-20)*0.5-1);

	
	incLabel->setBounds(5, 5, 100, 20);
	currentLabel->setBounds(5, 25, 100, 20);
}

//==============================================================================
void DRowAudioEditorComponent::sliderValueChanged (Slider* changedSlider)
{
    DRowAudioFilter* currentFilter = getFilter();
	
	for (int i = 0; i < noParams; i++)
		if ( changedSlider == sliders[i] )
			currentFilter->setScaledParameterNotifyingHost (i, (float) sliders[i]->getValue());
	if (changedSlider == sliders[9])	{
		currentFilter->waveformDisplayPre->setHorizontalZoom(sliders[9]->getValue());
		currentFilter->waveformDisplayPost->setHorizontalZoom(sliders[9]->getValue());
	}
}

void DRowAudioEditorComponent::buttonClicked(Button* clickedButton)
{
	DRowAudioFilter* currentFilter = getFilter();
	
	if (clickedButton == buttons[0])
	{
		if(clickedButton->getToggleState())
			currentFilter->setScaledParameterNotifyingHost(MONITOR, 1.0);
		else
			currentFilter->setScaledParameterNotifyingHost(MONITOR, 0.0);	
	}
	if (clickedButton == buttons[2])
	{
		if(clickedButton->getToggleState())
			currentFilter->setScaledParameterNotifyingHost(FILTER, 1.0);
		else
			currentFilter->setScaledParameterNotifyingHost(FILTER, 0.0);	
	}
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

	
	for(int i = 0; i < noParams; i++)
		tempParamVals[i] =  filter->getScaledParameter (i);

    // ..release the lock ASAP
    filter->getCallbackLock().exit();



	for(int i = 0; i < noParams; i++)
		sliders[i]->setValue (tempParamVals[i], false);
}


void DRowAudioEditorComponent::timerCallback()
{
	incLabel->setText(String(getFilter()->RMSLeft), false);
	currentLabel->setText(String(getFilter()->RMSRight), false);
	currentLabel->setText(String(getFilter()->RMSRight), false);
	currentLabel->setColour(Label::backgroundColourId, Colours::black.withBrightness(getFilter()->RMSRight));
}