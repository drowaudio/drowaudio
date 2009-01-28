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
#include "dRowTremoloFilter.h"
#include "dRowTremoloEditorComponent.h"


/** List of todo's:
	
	@todo Smooth parameters
	@todo Improve transfer function for filling buffer
	@todo Make tempo dependant
	@todo Correct text entering problems
 */

//==============================================================================
/**
    This function must be implemented to create a new instance of your
    plugin object.
*/
dRowTremoloFilter* JUCE_CALLTYPE createPluginFilter()
{
    return new dRowTremoloFilter();
}

//==============================================================================
#pragma mark -
#pragma mark TremoloConstructor
dRowTremoloFilter::dRowTremoloFilter()
	:	tremoloBufferSize(2000)
{
    tremoloBuffer = new float[tremoloBufferSize];
	tremoloBuffer2 = new float[tremoloBufferSize];
	
	// set up the lookup table
	float lookupScale = (2*double_Pi) / 8192;
	sinLookupTable = new float[8192];
	for(int i = 0; i < 8192; i++)
	{
		sinLookupTable[i] = sin(lookupScale * i);
	}
	
	// set up parameters
/*	dRowParameter::dRowParameter(const String& name_, ParameterUnit unit_, String description_,
								 double value_, double min_, double max_, double default_,
								 double scale_, double offset_)*/	
	gainParam = new dRowParameter(T("Gain"), UnitGeneric, T("The gain level of the audio"),
								 1.0f, 0.0f, 1.0f, 1.0f,
								 1.0f, 0.0f);
	rateParam = new dRowParameter(T("Rate"), UnitHertz, T("The rate of the effect"),
								 5.0f, 0.0f, 20.0f, 5.0f,
								 20.0f, 0.0f);								 
	depthParam = new dRowParameter(T("Depth"), UnitPercent, T("The depth of the effect"),
								 100.0f, 0.0f, 100.0f, 100.0f,
								 200.0f, 0.0f);
	shapeParam = new dRowParameter(T("Shape"),UnitGeneric, T("The shape of the tremolo effect"),
								 1.0f, 0.2f, 10.0f, 1.0f,
								 1.0f, 0.0f);
	phaseParam = new dRowParameter(T("Phase"), UnitDegrees, T("The level of offset of the second channel"),
								 0.0f, -180.0f, 180.0f, 0.0f,
								 360.0f, -180.0f);

	currentShape = shapeParam->getValue();
	currentDepth = depthParam->getValue();
	currentPhase = phaseParam->getValue();
	
	// fill initial buffers
	fillBuffer(tremoloBuffer, 0);
	fillBuffer(tremoloBuffer2, currentPhase);

	fTremoloBufferPosition = 0;
		
    zeromem (&lastPosInfo, sizeof (lastPosInfo));
    lastPosInfo.timeSigNumerator = 4;
    lastPosInfo.timeSigDenominator = 4;
    lastPosInfo.bpm = 120;
	
	// start this here so all hosts see the UI update
	startTimer(200);
}

dRowTremoloFilter::~dRowTremoloFilter()
{
	stopTimer();
	
	delete tremoloBuffer;
	delete tremoloBuffer2;
	delete sinLookupTable;
	delete gainParam;
	delete rateParam;
	delete depthParam;
	delete shapeParam;
	delete phaseParam;
}
#pragma mark -
#pragma mark Tremolo Effect Methods
//==============================================================================
const String dRowTremoloFilter::getName() const
{
    return "dRowAudio: Tremolo";
}

int dRowTremoloFilter::getNumParameters()
{
    return TremoloInterface::Parameters::Count;
}

// Deals with getting parameters from the UI components
float dRowTremoloFilter::getParameter (int index)
{
	if (index == TremoloInterface::Parameters::Gain)
		return gainParam->getNormalisedValue();
	else if (index == TremoloInterface::Parameters::Rate)
		return rateParam->getNormalisedValue();
	else if (index == TremoloInterface::Parameters::Depth)
		return depthParam->getNormalisedValue();
	else if (index == TremoloInterface::Parameters::Shape)
		return shapeParam->getNormalisedValue();
	else if (index == TremoloInterface::Parameters::Phase)
		return phaseParam->getNormalisedValue();
	else
		return 0.0f;
}

double dRowTremoloFilter::getScaledParameter(int index)
{
	if (index == TremoloInterface::Parameters::Gain)
		return gainParam->getValue();
	else if (index == TremoloInterface::Parameters::Rate)
		return rateParam->getValue();
	else if (index == TremoloInterface::Parameters::Depth)
		return depthParam->getValue();
	else if (index == TremoloInterface::Parameters::Shape)
		return shapeParam->getValue();
	else if (index == TremoloInterface::Parameters::Phase)
		return phaseParam->getValue();
	else
		return 0.0f;
}

void dRowTremoloFilter::setScaledParameter(int index, float newValue)
{
	if (index == TremoloInterface::Parameters::Gain) {
        if (gainParam->getValue() != newValue) {
            gainParam->setValue(newValue);
            sendChangeMessage (this);
        }
    }
	else if (index == TremoloInterface::Parameters::Rate) {
        if (rateParam->getValue() != newValue) {
            rateParam->setValue(newValue);
            sendChangeMessage (this);
        }
    }
	else if (index == TremoloInterface::Parameters::Depth) {
        if (depthParam->getValue() != newValue) {
            depthParam->setValue(newValue);
            sendChangeMessage (this);
        }
    }
	else if (index == TremoloInterface::Parameters::Shape) {
        if (shapeParam->getValue() != newValue) {
            shapeParam->setValue(newValue);
            sendChangeMessage (this);
        }
    }
	else if (index == TremoloInterface::Parameters::Phase) {
        if (phaseParam->getValue() != newValue) {
            phaseParam->setValue(newValue);
            sendChangeMessage (this);
        }
    }
}

// Deals with setting the UI components
void dRowTremoloFilter::setParameter (int index, float newValue)
{
	if (index == TremoloInterface::Parameters::Gain) {
        if (gainParam->getValue() != newValue) {
            gainParam->setNormalisedValue(newValue);
            sendChangeMessage (this);
        }
    }
	else if (index == TremoloInterface::Parameters::Rate) {
        if (rateParam->getValue() != newValue) {
            rateParam->setNormalisedValue(newValue);
            sendChangeMessage (this);
        }
    }
	else if (index == TremoloInterface::Parameters::Depth) {
        if (depthParam->getValue() != newValue) {
            depthParam->setNormalisedValue(newValue);
            sendChangeMessage (this);
        }
    }
	else if (index == TremoloInterface::Parameters::Shape) {
        if (shapeParam->getValue() != newValue) {
            shapeParam->setNormalisedValue(newValue);
            sendChangeMessage (this);
        }
    }	
	else if (index == TremoloInterface::Parameters::Phase) {
        if (phaseParam->getValue() != newValue) {
            phaseParam->setNormalisedValue(newValue);
            sendChangeMessage (this);
        }
    }
}

void dRowTremoloFilter::setScaledParameterNotifyingHost(int index, float newValue)
{
	if (index == TremoloInterface::Parameters::Gain)
        setParameterNotifyingHost(index, gainParam->normaliseValue(newValue));
	else if (index == TremoloInterface::Parameters::Rate)
		setParameterNotifyingHost(index, rateParam->normaliseValue(newValue));
	else if (index == TremoloInterface::Parameters::Depth)
        setParameterNotifyingHost(index, depthParam->normaliseValue(newValue));
	else if (index == TremoloInterface::Parameters::Shape)
        setParameterNotifyingHost(index, shapeParam->normaliseValue(newValue));
	else if (index == TremoloInterface::Parameters::Phase)
        setParameterNotifyingHost(index, phaseParam->normaliseValue(newValue));
}


const String dRowTremoloFilter::getParameterName (int index)
{
    // Name parameters
	if (index == TremoloInterface::Parameters::Gain)
        return TremoloInterface::Parameters::Names[TremoloInterface::Parameters::Gain];
	else if (index == TremoloInterface::Parameters::Rate)
        return TremoloInterface::Parameters::Names[TremoloInterface::Parameters::Rate];
	else if (index == TremoloInterface::Parameters::Depth)
        return TremoloInterface::Parameters::Names[TremoloInterface::Parameters::Depth];
	else if (index == TremoloInterface::Parameters::Shape)
        return TremoloInterface::Parameters::Names[TremoloInterface::Parameters::Shape];
	else if (index == TremoloInterface::Parameters::Phase)
        return TremoloInterface::Parameters::Names[TremoloInterface::Parameters::Phase];
	else
		return String::empty;
}

const String dRowTremoloFilter::getParameterText (int index)
{
    if (index == TremoloInterface::Parameters::Gain)
        return String (gainParam->getValue(), 2);
	else if (index == TremoloInterface::Parameters::Rate)
        return String(rateParam->getValue(), 2);
	else if (index == TremoloInterface::Parameters::Depth)
        return String (depthParam->getValue(), 2);
	else if (index == TremoloInterface::Parameters::Shape)
        return String (shapeParam->getValue(), 2);
	else if (index == TremoloInterface::Parameters::Phase)
        return String (phaseParam->getValue(), 2);
	else
		return String::empty;
}

// custom methods for AU compatibility
double dRowTremoloFilter::getParameterMin(int index)
{
	if (index == TremoloInterface::Parameters::Gain)
		return gainParam->getMin();
	else if (index == TremoloInterface::Parameters::Rate)
		return rateParam->getMin();
	else if (index == TremoloInterface::Parameters::Depth)
		return depthParam->getMin();
	else if (index == TremoloInterface::Parameters::Shape)
		return shapeParam->getMin();
	else if (index == TremoloInterface::Parameters::Phase)
		return phaseParam->getMin();
	else
		return 0.0f;
}
double dRowTremoloFilter::getParameterMax(int index)
{
	if (index == TremoloInterface::Parameters::Gain)
		return gainParam->getMax();
	else if (index == TremoloInterface::Parameters::Rate)
		return rateParam->getMax();
	else if (index == TremoloInterface::Parameters::Depth)
		return depthParam->getMax();
	else if (index == TremoloInterface::Parameters::Shape)
		return shapeParam->getMax();
	else if (index == TremoloInterface::Parameters::Phase)
		return phaseParam->getMax();
	else
		return 0.0f;
}	
double dRowTremoloFilter::getParameterDefault(int index)
{
	if (index == TremoloInterface::Parameters::Gain)
		return gainParam->getDefault();
	else if (index == TremoloInterface::Parameters::Rate)
		return rateParam->getDefault();
	else if (index == TremoloInterface::Parameters::Depth)
		return depthParam->getDefault();
	else if (index == TremoloInterface::Parameters::Shape)
		return shapeParam->getDefault();
	else if (index == TremoloInterface::Parameters::Phase)
		return phaseParam->getDefault();
	else
		return 0.0f;
}	
ParameterUnit dRowTremoloFilter::getParameterUnit(int index)
{
	if (index == TremoloInterface::Parameters::Gain)
		return gainParam->getUnit();
	else if (index == TremoloInterface::Parameters::Rate)
		return rateParam->getUnit();
	else if (index == TremoloInterface::Parameters::Depth)
		return depthParam->getUnit();
	else if (index == TremoloInterface::Parameters::Shape)
		return shapeParam->getUnit();
	else if (index == TremoloInterface::Parameters::Phase)
		return phaseParam->getUnit();
	else
		return (ParameterUnit)0;
}

const String dRowTremoloFilter::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String dRowTremoloFilter::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool dRowTremoloFilter::isInputChannelStereoPair (int index) const
{
    return false;
}

bool dRowTremoloFilter::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool dRowTremoloFilter::acceptsMidi() const
{
    return true;
}

bool dRowTremoloFilter::producesMidi() const
{
    return true;
}

#pragma mark -
#pragma mark Real-Time Processing
//==============================================================================
void dRowTremoloFilter::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // do your pre-playback setup stuff here..
    keyboardState.reset();
	
	currentSampleRate = sampleRate;
	
	fTremoloBufferPosition = 0;
	startTimer(50);
}

void dRowTremoloFilter::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	// we can slow the timer down here as we are only updating the UI for some VST hosts
	startTimer(200);
}

#pragma mark Main Process Function
void dRowTremoloFilter::processBlock (AudioSampleBuffer& buffer,
									  MidiBuffer& midiMessages)
{
	// interpolation variables
	unsigned int iPos1, iPos2;
	float fDiff, fInterpolatedData; 
		
	// find the number of samples in the buffer to process
	int numSamples = buffer.getNumSamples();
	
	// initialise the pointer to samples
	float* sample[numSamples];
	for (int channel = 0; channel < getNumInputChannels(); channel++)
	{
		sample[channel] = buffer.getSampleData(channel, 0);
	}
	
	//===================================================================
	// Main Sample Loop
	//===================================================================
	while (--numSamples >= 0)
	{
		// calculte the required buffer position
		iPos1 = (int)fTremoloBufferPosition;
		iPos2 = iPos1 + 1;
		if (iPos2 == tremoloBufferSize)
			iPos2 = 0;
		fDiff = fTremoloBufferPosition - iPos1;		
		
		// process channels
		for (int channel = 0; channel < getNumInputChannels(); channel++)
		{			
			if (channel%2 == 0) // even channel
				fInterpolatedData = (tremoloBuffer[iPos2] * fDiff) + (tremoloBuffer[iPos1] * (1 - fDiff));
			else // odd channel
				fInterpolatedData = (tremoloBuffer2[iPos2] * fDiff) + (tremoloBuffer2[iPos1] * (1 - fDiff));				
						
			*sample[channel] *= fInterpolatedData;
			
			// incriment sample pointers
			sample[channel]++;			
		}
		
		// incriment buffer position
		fTremoloBufferPosition += currentScalingFactor;
		if (fTremoloBufferPosition >= tremoloBufferSize)
			fTremoloBufferPosition -= tremoloBufferSize;	
	}
	//===================================================================

	// for each of our input channels, we'll attenuate its level by the
    // amount that our volume parameter is set to.
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {
        buffer.applyGain (channel, 0, buffer.getNumSamples(), gainParam->getValue());
    }	
	
    // in case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    // if any midi messages come in, use them to update the keyboard state object. This
    // object sends notification to the UI component about key up/down changes
    keyboardState.processNextMidiBuffer (midiMessages,
                                         0, buffer.getNumSamples(),
                                         true);

	
    // have a go at getting the current time from the host, and if it's changed, tell
    // our UI to update itself.
    AudioPlayHead::CurrentPositionInfo pos;

    if (getPlayHead() != 0 && getPlayHead()->getCurrentPosition (pos))
    {
        if (memcmp (&pos, &lastPosInfo, sizeof (pos)) != 0)
        {
            lastPosInfo = pos;
            sendChangeMessage (this);
        }
    }
    else
    {
        zeromem (&lastPosInfo, sizeof (lastPosInfo));
        lastPosInfo.timeSigNumerator = 4;
        lastPosInfo.timeSigDenominator = 4;
        lastPosInfo.bpm = 120;
    }
}

//==============================================================================
AudioProcessorEditor* dRowTremoloFilter::createEditor()
{
    return new dRowTremoloEditorComponent (this);
}

#pragma mark -
#pragma mark State Information
//==============================================================================
void dRowTremoloFilter::getStateInformation (MemoryBlock& destData)
{
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML..

    // create an outer XML element..
    XmlElement xmlState (T("MYPLUGINSETTINGS"));

    // add some attributes to it..
    xmlState.setAttribute (T("pluginVersion"), 1);
    xmlState.setAttribute (T("gainLevel"), gainParam->getValue());
	xmlState.setAttribute (T("rateAmount"), rateParam->getValue());
	xmlState.setAttribute (T("depthAmount"), depthParam->getValue());
	xmlState.setAttribute (T("shapeAmount"), shapeParam->getValue());
	xmlState.setAttribute (T("phaseAmount"), phaseParam->getValue());

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xmlState, destData);
}

void dRowTremoloFilter::setStateInformation (const void* data, int sizeInBytes)
{
    // use this helper function to get the XML from this binary blob..
    XmlElement* const xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != 0)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName (T("MYPLUGINSETTINGS")))
        {
            // ok, now pull out our parameters..
            gainParam->setValue( (float) xmlState->getDoubleAttribute (T("gainLevel"), gainParam->getValue()) );
			rateParam->setValue( (float) xmlState->getDoubleAttribute (T("rateAmount"), rateParam->getValue()) );
			depthParam->setValue( (float) xmlState->getDoubleAttribute (T("depthAmount"), depthParam->getValue()) );
			shapeParam->setValue( (float) xmlState->getDoubleAttribute (T("shapeAmount"), shapeParam->getValue()) );
			phaseParam->setValue( (float) xmlState->getDoubleAttribute (T("phaseAmount"), phaseParam->getValue()) );			

            sendChangeMessage (this);
        }

        delete xmlState;
    }
}

void dRowTremoloFilter::timerCallback()
{
	bool refreshBuffer = false;
	
	// update parameter values...
	float nextRate = rateParam->getValue();
	float nextShape = shapeParam->getValue();
	float nextDepth = depthParam->getValue();
	float nextPhase = phaseParam->getValue();
	
	// ...and see if they've changed
	if ((nextShape != currentShape))
	{
		refreshBuffer = true;
		currentShape = nextShape;
	}
	if ((nextDepth != currentDepth))
	{
		refreshBuffer = true;
		currentDepth = nextDepth;
	}
	if ((nextPhase != currentPhase))
	{
		refreshBuffer = true;
		currentPhase = nextPhase;
	}
	
	// and if any have refill buffers
	if (refreshBuffer)
	{
		refreshBuffer = false;
		fillBuffer(tremoloBuffer, 0);
		fillBuffer(tremoloBuffer2, currentPhase);
	}
	
	// calculate new scale rate to use
//	if (nextRate != currentRate)
//	{
		float samplesPerTremoloCycle = currentSampleRate / nextRate;
		nextScalingFactor = tremoloBufferSize / samplesPerTremoloCycle;
		currentRate = nextRate;
		
		// check if the rate has changed and update accordingly
		if ((nextScalingFactor != currentScalingFactor))
			currentScalingFactor = nextScalingFactor;		
//	}
}


void dRowTremoloFilter::fillBuffer(float* bufferToFill, float phaseAngle)
{
	// Scale parameters
	float depth = depthParam->getValue() / depthParam->getScale();
	float shape = (shapeParam->getValue() / shapeParam->getScale()) - shapeParam->getOffset();
	phaseAngle /= phaseParam->getScale();
	phaseAngle = (phaseAngle * 2 * double_Pi) - double_Pi;
//	float lookupScale = 8192/(2*pi);// / 8192;
	
	// create buffer with sine data
	for (uint32 i = 0; i < tremoloBufferSize; ++i)
	{
		// fill buffer with sine data
		double radians = i * 2.0 * (double_Pi / tremoloBufferSize);
		float rawBufferData = sin (radians + phaseAngle);
//		float rawBufferData = sinLookupTable [(int)((radians + phaseAngle) * lookupScale)];
		
		if (rawBufferData >= 0)
			bufferToFill[i] = ( (pow(rawBufferData, shape) * depth) + (1-depth));
		else
		{
			rawBufferData *= -1;
			rawBufferData = pow(rawBufferData, shape);
			rawBufferData *= -1;
			bufferToFill[i] = rawBufferData * depth + (1-depth);
		}
    }	
}
