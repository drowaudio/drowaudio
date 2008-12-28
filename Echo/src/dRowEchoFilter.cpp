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
#include "dRowEchoCommon.h"
#include "dRowEchoFilter.h"
#include "dRowEchoEditorComponent.h"

#define pi 3.14159265358979323846264338327950288
#define MAXIMUM_RATE 20

/** List of todo's:
	
	@todo Change buffer contents according to user
	@todo Make stereo including a phase difference
 */

//==============================================================================
/**
    This function must be implemented to create a new instance of your
    plugin object.
*/
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new dRowEchoFilter();
}

//==============================================================================
#pragma mark -
#pragma mark EchoConstructor
dRowEchoFilter::dRowEchoFilter()
{
    gain = 1.0f;
	rate = 1.0f;
	depth = 1.0f;
    lastUIWidth = 400;
    lastUIHeight = 140;

	
    zeromem (&lastPosInfo, sizeof (lastPosInfo));
    lastPosInfo.timeSigNumerator = 4;
    lastPosInfo.timeSigDenominator = 4;
    lastPosInfo.bpm = 120;
}

dRowEchoFilter::~dRowEchoFilter()
{
}
#pragma mark -
#pragma mark Echo Effect Methods
//==============================================================================
const String dRowEchoFilter::getName() const
{
    return "dRowAudio Echo";
}

int dRowEchoFilter::getNumParameters()
{
    return EchoInterface::Parameters::Count;
}

// Deals with getting parameters from the UI components
float dRowEchoFilter::getParameter (int index)
{
    // gain parameter
	if (index == EchoInterface::Parameters::Gain)
		return gain;
	
	// rate parameter
	else if (index == EchoInterface::Parameters::Rate)
		return rate;
	
	// depth parameter
	else if (index == EchoInterface::Parameters::Depth)
		return depth;

	else
		return 0.0f;
}

// Deals with setting the UI components
void dRowEchoFilter::setParameter (int index, float newValue)
{
	if (index == EchoInterface::Parameters::Gain)
    {
        if (gain != newValue)
        {
            gain = newValue;
            sendChangeMessage (this);
        }
    }
	
	else if (index == EchoInterface::Parameters::Rate)
    {
        if (rate != newValue)
        {
			rate = newValue;
            sendChangeMessage (this);
        }
    }
	
	else if (index == EchoInterface::Parameters::Depth)
    {
        if (depth != newValue)
        {
            depth = newValue;
            sendChangeMessage (this);
        }
    }
}

const String dRowEchoFilter::getParameterName (int index)
{
    // Name parameters
	if (index == EchoInterface::Parameters::Gain)
        return EchoInterface::Parameters::Names[EchoInterface::Parameters::Gain];
	else if (index == EchoInterface::Parameters::Rate)
        return EchoInterface::Parameters::Names[EchoInterface::Parameters::Rate];
	else if (index == EchoInterface::Parameters::Depth)
        return EchoInterface::Parameters::Names[EchoInterface::Parameters::Depth];
	else
		return String::empty;
}

const String dRowEchoFilter::getParameterText (int index)
{
    if (index == EchoInterface::Parameters::Gain)
        return String (gain, 2);
	else if (index == EchoInterface::Parameters::Rate)
        return String(rate, 2);
	else if (index == EchoInterface::Parameters::Depth)
        return String (depth, 2);
	else
		return String::empty;
}

const String dRowEchoFilter::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String dRowEchoFilter::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool dRowEchoFilter::isInputChannelStereoPair (int index) const
{
    return false;
}

bool dRowEchoFilter::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool dRowEchoFilter::acceptsMidi() const
{
    return true;
}

bool dRowEchoFilter::producesMidi() const
{
    return true;
}

#pragma mark -
#pragma mark Real-Time Processing
//==============================================================================
void dRowEchoFilter::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // do your pre-playback setup stuff here..
    keyboardState.reset();
	
	currentSampleRate = sampleRate;
	
	iBufferSize = (int)sampleRate;
	pfCircularBuffer = new float[iBufferSize];
	for(int iPos = 0; iPos < iBufferSize; iPos++)
		pfCircularBuffer[iPos] = 0;
	iBufferWritePos = 0;
	iBufferReadPos = 0;
}

void dRowEchoFilter::releaseResources()
{
    delete pfCircularBuffer;
	// when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#pragma mark Main Process Function
void dRowEchoFilter::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
	// pointer to current sample
	float* sample;
	int currentSample = 0;
				
	// find the number of samples in the buffer to process
	int numSamples = buffer.getNumSamples();
		
	//===================================================================
	// Main Sample Loop
	//===================================================================
	while (--numSamples >= 0)
	{
		// reset mono sample value
		float fMix = 0;
		
		// process channels
		for (int channel = 0; channel < getNumInputChannels(); ++channel)
		{
			// get pointer to current sample in current channel to process
			sample = buffer.getSampleData(channel, currentSample);
			
			// monoize sample
			fMix = (fMix + *sample) * 0.5;
		}
				
		iBufferReadPos = iBufferWritePos - (currentSampleRate * 0.25);
		if (iBufferReadPos < 0)
			iBufferReadPos = iBufferReadPos + iBufferSize;
		
		fMix += (pfCircularBuffer[iBufferReadPos]* depth);
		
		pfCircularBuffer[iBufferWritePos] = fMix;
		
		iBufferWritePos++;
		if (iBufferWritePos == iBufferSize)
			iBufferWritePos = 0;

		for (int channel = 0; channel < getNumInputChannels(); ++channel)
		{
			// get pointer to current sample in current channel to process
			sample = buffer.getSampleData(channel, currentSample);
			*sample = fMix;
		}
		// incriment current sample counter
		currentSample++;
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
AudioProcessorEditor* dRowEchoFilter::createEditor()
{
    return new dRowEchoEditorComponent (this);
}

#pragma mark -
#pragma mark State Information
//==============================================================================
void dRowEchoFilter::getStateInformation (MemoryBlock& destData)
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
    xmlState.setAttribute (T("uiWidth"), lastUIWidth);
    xmlState.setAttribute (T("uiHeight"), lastUIHeight);

    // you could also add as many child elements as you need to here..


    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xmlState, destData);
}

void dRowEchoFilter::setStateInformation (const void* data, int sizeInBytes)
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

            lastUIWidth = xmlState->getIntAttribute (T("uiWidth"), lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute (T("uiHeight"), lastUIHeight);

            sendChangeMessage (this);
        }

        delete xmlState;
    }
}
