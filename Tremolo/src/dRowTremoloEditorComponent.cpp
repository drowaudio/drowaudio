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
#include "dRowTremoloCommon.h"
#include "dRowTremoloEditorComponent.h"
#include "dRowTremoloFilter.h"
#include "dRowLookAndFeel.h"
#include "BitwiseFontResource.h"

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
dRowTremoloEditorComponent::dRowTremoloEditorComponent (dRowTremoloFilter* const ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
	dRowAudioLookAndFeel* myLookAndFeel;
	myLookAndFeel = new dRowAudioLookAndFeel;
	setLookAndFeel(myLookAndFeel);
	
	// Instantiate the font
	Font* bitwiseFont = 0;
	MemoryInputStream fontStream (bitwiseFontResource::bitwiseFontBinary, bitwiseFontResource::bitwiseFontBinary_Size, false);
	Typeface* bitwiseTypeface = new Typeface (fontStream);
	bitwiseFont = new Font (*bitwiseTypeface);
	delete bitwiseTypeface;			// Because the font stores it's own typeface
	
	// title label
	addAndMakeVisible(titleLabel = new Label(T("title"),T("dRowAudio: Tremolo")));
	bitwiseFont->setHeight(28);
	titleLabel->setColour(Label::textColourId, Colours::lightblue);
	titleLabel->setFont(*bitwiseFont);
	
	// create our gain slider..
    addAndMakeVisible (gainSlider = new Slider (T("gainSlider")));
    gainSlider->addListener (this);
    gainSlider->setRange (0.0, 1.0, 0.01);
    gainSlider->setTooltip (T("Changes the volume of the audio that runs through the plugin"));

    // get the gain parameter from the filter and use it to set up our slider
    gainSlider->setValue (ownerFilter->getParameter (TremoloInterface::Parameters::Gain), false);
	
	addAndMakeVisible(gainLabel = new Label(T("gainLabel"),TremoloInterface::Parameters::Names[TremoloInterface::Parameters::Gain]));
	
	
	// create the sliders
	addAndMakeVisible(rateSlider = new Slider (T("rateSlider")));
	rateSlider->setSliderStyle(Slider::Rotary);
	rateSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
	rateSlider->addListener (this);
    rateSlider->setRange (0.0, 1.0, 0.01);
	rateSlider->setSkewFactor(2);
    rateSlider->setTooltip (T("Changes the rate of the tremolo effect"));
    rateSlider->setValue (ownerFilter->getParameter (TremoloInterface::Parameters::Rate), false);
	addAndMakeVisible(rateLabel = new Label(T("rateLabel"),TremoloInterface::Parameters::Names[TremoloInterface::Parameters::Rate]));
	rateLabel->setJustificationType(Justification::centred);
	rateLabel->attachToComponent(rateSlider, false);
	
	addAndMakeVisible(depthSlider = new Slider (T("depthSlider")));
	depthSlider->setSliderStyle(Slider::Rotary);
	depthSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
	depthSlider->addListener (this);
    depthSlider->setRange (0.0, 1.0, 0.01);
    depthSlider->setTooltip (T("Changes the depth of the tremolo effect"));
    depthSlider->setValue (ownerFilter->getParameter (TremoloInterface::Parameters::Depth), false);
	addAndMakeVisible(depthLabel = new Label(T("depthLabel"),TremoloInterface::Parameters::Names[TremoloInterface::Parameters::Depth]));
	depthLabel->setJustificationType(Justification::centred);
	depthLabel->attachToComponent(depthSlider, false);
	
	addAndMakeVisible(shapeSlider = new Slider(T("shapeSlider")));
	shapeSlider->setSliderStyle(Slider::Rotary);
	shapeSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
	shapeSlider->setSkewFactorFromMidPoint(1);
	shapeSlider->setRange(0.0, 10, 0.001);
	shapeSlider->setValue(1);
	shapeSlider->addListener(this);
	addAndMakeVisible(shapeLabel = new Label(T("shapeLabel"),T("Shape")));
	shapeLabel->setJustificationType(Justification::centred);
	shapeLabel->attachToComponent(shapeSlider, false);
	
	// create the buffer view
	addAndMakeVisible(bufferView1 = new dRowBufferView(ownerFilter->tremoloBuffer, ownerFilter->tremoloBufferSize));
	bufferView1->setInterceptsMouseClicks(false, false);
	
    // create and add the midi keyboard component..
    addAndMakeVisible (midiKeyboard
        = new MidiKeyboardComponent (ownerFilter->keyboardState,
                                     MidiKeyboardComponent::horizontalKeyboard));

    // add a label that will display the current timecode and status..
    addAndMakeVisible (infoLabel = new Label (String::empty, String::empty));

    // add the triangular resizer component for the bottom-right of the UI
    addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
    resizeLimits.setSizeLimits (150, 150, 800, 300);

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (ownerFilter->lastUIWidth,
             ownerFilter->lastUIHeight);

    // register ourselves with the filter - it will use its ChangeBroadcaster base
    // class to tell us when something has changed, and this will call our changeListenerCallback()
    // method.
    ownerFilter->addChangeListener (this);
}

dRowTremoloEditorComponent::~dRowTremoloEditorComponent()
{
    getFilter()->removeChangeListener (this);

    deleteAllChildren();
}

//==============================================================================
void dRowTremoloEditorComponent::paint (Graphics& g)
{
    // just clear the window
    g.fillAll (Colour::greyLevel (0.3f));
//    g.fillAll (Colours::black);
	g.setColour(Colour::greyLevel(0.4f));
	g.fillRect(Rectangle(getWidth()-95, (getHeight()*0.5f)+5, 90, (getHeight()*0.5f)-10));
}

void dRowTremoloEditorComponent::resized()
{
    titleLabel->setBounds(0, 0, 200, 30);
	
	gainLabel->setBounds(10, 30, 40, 22);
	gainSlider->setBounds (50, 30, 200, 22);
//    infoLabel->setBounds (10, 35, 450, 20);
		
	rateSlider->setBounds (10, 75, 50, 60);
	depthSlider->setBounds (80, 75, 50, 60);
	shapeSlider->setBounds(150, 75, 50, 60);
	
	bufferView1->setBounds (getWidth()-95, 5, 90, (getHeight()*0.5f)-10);

//    const int keyboardHeight = 70;
//    midiKeyboard->setBounds (4, getHeight() - keyboardHeight - 4,
//                             getWidth() - 8, keyboardHeight);

    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);

    // if we've been resized, tell the filter so that it can store the new size
    // in its settings
    getFilter()->lastUIWidth = getWidth();
    getFilter()->lastUIHeight = getHeight();
}

//==============================================================================
void dRowTremoloEditorComponent::changeListenerCallback (void* source)
{
    // this is the filter telling us that it's changed, so we'll update our
    // display of the time, midi message, etc.
    updateParametersFromFilter();
}

void dRowTremoloEditorComponent::sliderValueChanged (Slider* changedSlider)
{
    if (changedSlider == gainSlider)
		getFilter()->setParameterNotifyingHost (TremoloInterface::Parameters::Gain, (float)gainSlider->getValue());
	
	else if (changedSlider == rateSlider)
		getFilter()->setParameterNotifyingHost (TremoloInterface::Parameters::Rate, (float)rateSlider->getValue());
	
	else if (changedSlider == depthSlider)
		getFilter()->setParameterNotifyingHost (TremoloInterface::Parameters::Depth, (float)depthSlider->getValue());
	
	else if (changedSlider == shapeSlider)
		getFilter()->setParameterNotifyingHost (TremoloInterface::Parameters::Shape, (float)shapeSlider->getValue());
}

//==============================================================================
void dRowTremoloEditorComponent::updateParametersFromFilter()
{
    dRowTremoloFilter* const filter = getFilter();

    // we use this lock to make sure the processBlock() method isn't writing to the
    // lastMidiMessage variable while we're trying to read it, but be extra-careful to
    // only hold the lock for a minimum amount of time..
    filter->getCallbackLock().enter();

    // take a local copy of the info we need while we've got the lock..
    const AudioPlayHead::CurrentPositionInfo positionInfo (filter->lastPosInfo);
    const float newGain = filter->getParameter (TremoloInterface::Parameters::Gain);
	const float newRate = filter->getParameter (TremoloInterface::Parameters::Rate);
	const float newTremDepth = filter->getParameter (TremoloInterface::Parameters::Depth);
	const float newShape = filter->getParameter (TremoloInterface::Parameters::Shape);

    // ..release the lock ASAP
    filter->getCallbackLock().exit();
	
	// resize the buffer display
	bufferView1->resized();

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
    gainSlider->setValue (newGain, false);
	rateSlider->setValue (newRate, false);
	depthSlider->setValue (newTremDepth, false);
	shapeSlider->setValue (newShape, false);

    setSize (filter->lastUIWidth,
             filter->lastUIHeight);
}
