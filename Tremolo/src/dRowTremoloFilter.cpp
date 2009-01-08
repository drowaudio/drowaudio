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
#include <cmath>

#define pi 3.14159265358979323846264338327950288
#define MAXIMUM_RATE 20

/** List of todo's:
	
	@todo Improve transfer function for filling buffer
	@todo Make tempo dependant
	@todo Correct generic parameter problems
	@todo Correct text entering problems
 */

//==============================================================================
/**
    This function must be implemented to create a new instance of your
    plugin object.
*/
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new dRowTremoloFilter();
}

//==============================================================================
#pragma mark -
#pragma mark TremoloConstructor
dRowTremoloFilter::dRowTremoloFilter()
{
    // set up the lookup table
	float lookupScale = (2*pi) / 8192;
	sinLookupTable = new float[8192];
	for(int i = 0; i < 8192; i++)
	{
		sinLookupTable[i] = sin(lookupScale * i);
	}
	
	gain = 1.0f;
	rate = 1.0f;
	depth = 0.5f;
	shape = 1.0f;
	phase = 0.0f;
    lastUIWidth = 400;
    lastUIHeight = 200;
	
	currentShape = shape;
	currentDepth = depth;
	currentPhase = phase;
	fillBuffer(tremoloBuffer, 0);
	fillBuffer(tremoloBuffer2, currentPhase);

	fTremoloBufferPosition = 0;
	
    zeromem (&lastPosInfo, sizeof (lastPosInfo));
    lastPosInfo.timeSigNumerator = 4;
    lastPosInfo.timeSigDenominator = 4;
    lastPosInfo.bpm = 120;
	
	// start this here so all hosts see the UI update
	startTimer(400);
}

dRowTremoloFilter::~dRowTremoloFilter()
{
	stopTimer();
}
#pragma mark -
#pragma mark Tremolo Effect Methods
//==============================================================================
const String dRowTremoloFilter::getName() const
{
    return "dRowAudio Tremolo";
}

int dRowTremoloFilter::getNumParameters()
{
    return TremoloInterface::Parameters::Count;
}

// Deals with getting parameters from the UI components
float dRowTremoloFilter::getParameter (int index)
{
	if (index == TremoloInterface::Parameters::Gain)
		return gain;
	
	else if (index == TremoloInterface::Parameters::Rate)
		return rate;
	
	else if (index == TremoloInterface::Parameters::Depth)
		return depth;
	
	else if (index == TremoloInterface::Parameters::Shape)
		return shape;
	
	else if (index == TremoloInterface::Parameters::Phase)
		return phase;

	else
		return 0.0f;
}

// Deals with setting the UI components
void dRowTremoloFilter::setParameter (int index, float newValue)
{
	if (index == TremoloInterface::Parameters::Gain)
    {
        if (gain != newValue)
        {
            gain = newValue;
            sendChangeMessage (this);
        }
    }
	
	else if (index == TremoloInterface::Parameters::Rate)
    {
        if (rate != newValue)
        {
			rate = newValue;
            sendChangeMessage (this);
        }
    }
	
	else if (index == TremoloInterface::Parameters::Depth)
    {
        if (depth != newValue)
        {
            depth = newValue;
            sendChangeMessage (this);
        }
    }
	
	else if (index == TremoloInterface::Parameters::Shape)
    {
        if (shape != newValue)
        {
            shape = newValue;
            sendChangeMessage (this);
        }
    }
	
	else if (index == TremoloInterface::Parameters::Phase)
    {
        if (phase != newValue)
        {
            phase = newValue;
            sendChangeMessage (this);
        }
    }
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
        return String (gain, 2);
	else if (index == TremoloInterface::Parameters::Rate)
        return String(rate, 2);
	else if (index == TremoloInterface::Parameters::Depth)
        return String (depth, 2);
	else if (index == TremoloInterface::Parameters::Shape)
        return String (shape, 2);
	else if (index == TremoloInterface::Parameters::Phase)
        return String (shape, 2);
	else
		return String::empty;
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
	startTimer(400);
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
	float* sample[getNumInputChannels()];
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
        buffer.applyGain (channel, 0, buffer.getNumSamples(), gain);
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
    xmlState.setAttribute (T("gainLevel"), gain);
	xmlState.setAttribute (T("rateAmount"), rate);
	xmlState.setAttribute (T("depthAmount"), depth);
//    xmlState.setAttribute (T("uiWidth"), lastUIWidth);
//    xmlState.setAttribute (T("uiHeight"), lastUIHeight);

    // you could also add as many child elements as you need to here..


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
            gain = (float) xmlState->getDoubleAttribute (T("gainLevel"), gain);
			rate = (float) xmlState->getDoubleAttribute (T("rateAmount"), rate);
			depth = (float) xmlState->getDoubleAttribute (T("depthAmount"), depth);

//            lastUIWidth = xmlState->getIntAttribute (T("uiWidth"), lastUIWidth);
//            lastUIHeight = xmlState->getIntAttribute (T("uiHeight"), lastUIHeight);

            sendChangeMessage (this);
        }

        delete xmlState;
    }
}

void dRowTremoloFilter::timerCallback()
{
	bool refreshBuffer = false;
	
	// update parameter values...
	float nextShape = shape;
	float nextDepth = depth;
	float nextPhase = phase;
	
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
	float samplesPerTremoloCycle = currentSampleRate / (MAXIMUM_RATE * rate);
	nextScalingFactor = tremoloBufferSize / samplesPerTremoloCycle;
	
	// check if the rate has changed and update accordingly
	if ((nextScalingFactor != currentScalingFactor))
	{
		currentScalingFactor = nextScalingFactor;
	}	
}


void dRowTremoloFilter::fillBuffer(float* bufferToFill, float phaseAngle)
{
	// Scale phase
	phaseAngle = (phaseAngle * 2 * pi) - pi;
//	float lookupScale = 8192/(2*pi);// / 8192;
	
	// create buffer with sine data
	for (uint32 i = 0; i < tremoloBufferSize; ++i)
	{
		// fill buffer with sine data
		double radians = i * 2.0 * (pi / tremoloBufferSize);
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
