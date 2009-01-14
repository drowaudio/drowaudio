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
#include "TremoloTitleImageResource.h"

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
    : AudioProcessorEditor (ownerFilter),
	  ownerFilterGlobal(ownerFilter)
{
	dRowAudioLookAndFeel* myLookAndFeel;
	myLookAndFeel = new dRowAudioLookAndFeel;
	setLookAndFeel(myLookAndFeel);
	
	// load title image to memory cache
	cachedTitleImage = ImageCache::getFromMemory (tremoloTitleImage::tremoloTitleImage_jpg, tremoloTitleImage::tremoloTitleImage_jpgSize);
	
	// Instantiate the bitwise font
	Font* bitwiseFont = 0;
	MemoryInputStream fontStream (bitwiseFontResource::bitwiseFontBinary, bitwiseFontResource::bitwiseFontBinary_Size, false);
	Typeface* bitwiseTypeface = new Typeface (fontStream);
	bitwiseFont = new Font (*bitwiseTypeface);
	delete bitwiseTypeface;			// Because the font stores it's own typeface
	
	// title label
	addAndMakeVisible(titleLabel = new Label(T("title"),T("dRowAudio: Tremolo")));
	bitwiseFont->setHeight(34);
	bitwiseFont->setStyleFlags(Font::italic);
	titleLabel->setColour(Label::textColourId, (Colours::lightblue).withAlpha(0.9f));
	titleLabel->setColour(Label::backgroundColourId, Colours::black);
	titleLabel->setFont(*bitwiseFont);
	
	// create our gain slider..
    addAndMakeVisible (gainSlider = new Slider (T("gainSlider")));
    gainSlider->addListener (this);
    gainSlider->setRange (0.0, 1.0, 0.01);
    gainSlider->setTooltip (T("Changes the volume of the audio that runs through the plugin"));

    // get the gain parameter from the filter and use it to set up our slider
    gainSlider->setValue (ownerFilter->getScaledParameter (TremoloInterface::Parameters::Gain), false);
	
	addAndMakeVisible(gainLabel = new Label(T("gainLabel"),TremoloInterface::Parameters::Names[TremoloInterface::Parameters::Gain]));
	gainLabel->setColour(Label::textColourId, Colours::white);
	
	
	// create the sliders and their labels
	addAndMakeVisible(rateSlider = new Slider (T("rateSlider")));
	rateSlider->setSliderStyle(Slider::Rotary);
	rateSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
	rateSlider->addListener (this);
//  rateSlider->setRange (0.0, 1.0, 0.01);
	rateSlider->setRange(ownerFilter->newRate->getMin(), ownerFilter->newRate->getMax(), 0.01);
//	rateSlider->setSkewFactorFromMidPoint(5);
    rateSlider->setTooltip (T("Changes the rate of the tremolo effect"));
    rateSlider->setValue (ownerFilter->getScaledParameter (TremoloInterface::Parameters::Rate), false);
	rateSlider->setColour(Slider::rotarySliderFillColourId, Colour(0xB1002DFF));
	addAndMakeVisible(rateLabel = new Label(T("rateLabel"),TremoloInterface::Parameters::Names[TremoloInterface::Parameters::Rate]));
	rateLabel->setJustificationType(Justification::centred);
	rateLabel->attachToComponent(rateSlider, false);
	rateLabel->setColour(Label::textColourId, Colours::white);
	
	addAndMakeVisible(depthSlider = new Slider (T("depthSlider")));
	depthSlider->setSliderStyle(Slider::Rotary);
	depthSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
	depthSlider->addListener (this);
//  depthSlider->setRange (0.0, 0.5f, 0.01);
	depthSlider->setRange(ownerFilter->newDepth->getMin(), ownerFilter->newDepth->getMax(), 0.01);
    depthSlider->setTooltip (T("Changes the depth of the tremolo effect"));
    depthSlider->setValue (ownerFilter->getScaledParameter (TremoloInterface::Parameters::Depth), false);
	depthSlider->setColour(Slider::rotarySliderFillColourId, Colour(0xB1002DFF));
	addAndMakeVisible(depthLabel = new Label(T("depthLabel"),TremoloInterface::Parameters::Names[TremoloInterface::Parameters::Depth]));
	depthLabel->setJustificationType(Justification::centred);
	depthLabel->attachToComponent(depthSlider, false);
	depthLabel->setColour(Label::textColourId, Colours::white);
	
	addAndMakeVisible(shapeSlider = new Slider(T("shapeSlider")));
	shapeSlider->setSliderStyle(Slider::Rotary);
	shapeSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
//	shapeSlider->setRange(0.2, 10, 0.001);
	shapeSlider->setRange(ownerFilter->newShape->getMin(), ownerFilter->newShape->getMax(), 0.01);
	shapeSlider->setSkewFactorFromMidPoint(1);
	shapeSlider->setValue (ownerFilter->getScaledParameter (TremoloInterface::Parameters::Shape), false);
	shapeSlider->addListener(this);
    shapeSlider->setColour(Slider::rotarySliderFillColourId, Colour(0xB1002DFF));
	addAndMakeVisible(shapeLabel = new Label(T("shapeLabel"),T("Shape")));
	shapeLabel->setJustificationType(Justification::centred);
	shapeLabel->attachToComponent(shapeSlider, false);
	shapeLabel->setColour(Label::textColourId, Colours::white);
	
	addAndMakeVisible(phaseSlider = new Slider(T("phaseSlider")));
	phaseSlider->setSliderStyle(Slider::Rotary);
	phaseSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
//	phaseSlider->setRange(-0.5f, 0.5f, 0.01);
	phaseSlider->setRange(ownerFilter->newPhase->getMin(), ownerFilter->newPhase->getMax(), 0.01);
	phaseSlider->setValue (ownerFilter->getScaledParameter (TremoloInterface::Parameters::Phase), false);
	phaseSlider->addListener(this);
	phaseSlider->setColour(Slider::rotarySliderFillColourId, Colour(0xB1002DFF));
	addAndMakeVisible(phaseLabel = new Label(T("phaseLabel"),T("Phase")));
	phaseLabel->setJustificationType(Justification::centred);
	phaseLabel->attachToComponent(phaseSlider, false);
	phaseLabel->setColour(Label::textColourId, Colours::white);
	
	// create the buffer views
	addAndMakeVisible(bufferView1 = new dRowBufferView(ownerFilter->tremoloBuffer, ownerFilter->tremoloBufferSize));
	bufferView1->setInterceptsMouseClicks(false, false);
	addAndMakeVisible(bufferView1Label = new Label("lLabel", "L:"));
	bufferView1Label->attachToComponent(bufferView1, true);
	bufferView1Label->setFont(Font (30, Font::plain));
	bufferView1Label->setColour(Label::textColourId, Colours::white);
	
	addAndMakeVisible(bufferView2 = new dRowBufferView(ownerFilter->tremoloBuffer2, ownerFilter->tremoloBufferSize));
	bufferView2->setInterceptsMouseClicks(false, false);
	addAndMakeVisible(bufferView2Label = new Label("rLabel", "R:"));
	bufferView2Label->attachToComponent(bufferView2, true);
	bufferView2Label->setFont(Font (30, Font::plain));
	bufferView2Label->setColour(Label::textColourId, Colours::white);
	
    // create and add the midi keyboard component..
    addAndMakeVisible (midiKeyboard
        = new MidiKeyboardComponent (ownerFilter->keyboardState,
                                     MidiKeyboardComponent::horizontalKeyboard));

    // add a label that will display the current timecode and status..
    addAndMakeVisible (infoLabel = new Label (String::empty, String::empty));


    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (ownerFilter->lastUIWidth,
             ownerFilter->lastUIHeight);
//    setSize (600,
//             300);

    // register ourselves with the filter - it will use its ChangeBroadcaster base
    // class to tell us when something has changed, and this will call our changeListenerCallback()
    // method.
    ownerFilter->addChangeListener (this);
	
	// if plugin is mono set up the accordingly
	if (ownerFilter->getNumInputChannels() < 2)
	{
		phaseSlider->setVisible(false);
		bufferView2->setVisible(false);		
		bufferView1Label->setVisible(false);
		bufferView2Label->setVisible(false);
	}
}

dRowTremoloEditorComponent::~dRowTremoloEditorComponent()
{
    getFilter()->removeChangeListener (this);
	ImageCache::release (cachedTitleImage);
	
    deleteAllChildren();
}

//==============================================================================
void dRowTremoloEditorComponent::paint (Graphics& g)
{
	GradientBrush gradient_1 (Colours::black,
                              0.0f, (float) (getHeight()),
                              Colour (0xff383838),
                              0.0f, (float) (proportionOfHeight (0.95f)),
                              false);
	g.setBrush (&gradient_1);
	g.fillRect (0, 0, getWidth(), getHeight());
	
	GradientBrush gradient_2 (Colour (0xff383838).withBrightness(0.0f),
                              0.0f, (float)40,
                              Colour (0xff383838),
                              0.0f, (float) (40+proportionOfHeight (0.05f)),
                              false);
	g.setBrush (&gradient_2);
	g.fillRect (0, 40, getWidth(), proportionOfHeight (0.05f));
	
	g.drawImageWithin (cachedTitleImage,
                       0, 0, 400, 40,
                       RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
                       false);
	
	// there is probably a better way to do this so the whole UI is not repainted each time
	bufferView1->resized();
	bufferView2->resized();
}

void dRowTremoloEditorComponent::resized()
{
	gainLabel->setBounds(10, 60, 40, 22);
	gainSlider->setBounds (50, 60, 190, 22);
//    infoLabel->setBounds (10, 35, 450, 20);
		
	rateSlider->setBounds (5, 130, 60, 60);
	depthSlider->setBounds (65, 130, 60, 60);
	shapeSlider->setBounds(125, 130, 60, 60);
	phaseSlider->setBounds(185, 130, 60, 60);
	
	if (ownerFilterGlobal->getNumInputChannels() < 2)
		bufferView1->setBounds (getWidth()-145, 4+50, 140, (getHeight())-4-60);
	if (ownerFilterGlobal->getNumInputChannels() > 1)
		bufferView1->setBounds (getWidth()-115, 50, 110, 68);
	bufferView2->setBounds (getWidth()-115, (getHeight()*0.5f)+2+20, 110, 68);
	
//    const int keyboardHeight = 70;
//    midiKeyboard->setBounds (4, getHeight() - keyboardHeight - 4,
//                             getWidth() - 8, keyboardHeight);


    // if we've been resized, tell the filter so that it can store the new size
    // in its settings
//    getFilter()->lastUIWidth = getWidth();
//    getFilter()->lastUIHeight = getHeight();
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
    dRowTremoloFilter* const filter = getFilter();
	
	if (changedSlider == gainSlider)
		filter->setScaledParameterNotifyingHost (TremoloInterface::Parameters::Gain, (float)gainSlider->getValue());
	
	else if (changedSlider == rateSlider)
		filter->setScaledParameterNotifyingHost (TremoloInterface::Parameters::Rate, (float)rateSlider->getValue());
	
	else if (changedSlider == depthSlider)
		filter->setScaledParameterNotifyingHost (TremoloInterface::Parameters::Depth, (float)depthSlider->getValue());
	
	else if (changedSlider == shapeSlider)
		filter->setScaledParameterNotifyingHost (TremoloInterface::Parameters::Shape, (float)shapeSlider->getValue());
	
	else if (changedSlider == phaseSlider)
		filter->setScaledParameterNotifyingHost (TremoloInterface::Parameters::Phase, (float)phaseSlider->getValue());
	
	// refresh the buffer displays
//	if (changedSlider == depthSlider ||
//		changedSlider == shapeSlider)
//		{
//			bufferView1->resized();
//			bufferView2->resized();
//		}
//	if (changedSlider == phaseSlider)
//		bufferView2->resized();
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
    const float newGain = filter->getScaledParameter (TremoloInterface::Parameters::Gain);
	const float newRate = filter->getScaledParameter (TremoloInterface::Parameters::Rate);
	const float newTremDepth = filter->getScaledParameter (TremoloInterface::Parameters::Depth);
	const float newShape = filter->getScaledParameter (TremoloInterface::Parameters::Shape);
	const float newPhase = filter->getScaledParameter (TremoloInterface::Parameters::Phase);

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

    /* Update our sliders.

       (note that it's important here to tell the slider not to send a change
       message, because that would cause it to call the filter with a parameter
       change message again, and the values would drift out.
    */
    gainSlider->setValue (newGain, false);
	rateSlider->setValue (newRate, false);
	depthSlider->setValue (newTremDepth, false);
	shapeSlider->setValue (newShape, false);
	phaseSlider->setValue (newPhase, false);

    setSize (filter->lastUIWidth,
             filter->lastUIHeight);
}
