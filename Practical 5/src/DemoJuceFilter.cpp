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
#include "DemoJuceFilter.h"
#include "DemoEditorComponent.h"


//==============================================================================
/**
    This function must be implemented to create a new instance of your
    plugin object.
*/
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DemoJuceFilter();
}

//==============================================================================
DemoJuceFilter::DemoJuceFilter()
	:	RMSLeft(0),
		RMSRight(0),
		peakLeft(0),
		peakRight(0)

{
    for (int i = 0; i < noParams; i++)
		params[i] = 1.0f;
	
	// set default values for parameters here:
	params[REDUCTION] = 0.2f;
	params[LOWCF] = 1.0f;
	params[HIGHCF] = 0.0f;
	
    zeromem (&lastPosInfo, sizeof (lastPosInfo));
    lastPosInfo.timeSigNumerator = 4;
    lastPosInfo.timeSigDenominator = 4;
    lastPosInfo.bpm = 120;
}

DemoJuceFilter::~DemoJuceFilter()
{
}

//==============================================================================
const String DemoJuceFilter::getName() const
{
    return "Practical5";
}

int DemoJuceFilter::getNumParameters()
{
    return noParams;
}

float DemoJuceFilter::getParameter (int index)
{
	if (index >= 0
		&& index < noParams)
			return params[index];
	else return 0.0f;

//	return (index == 0) ? gain
//                        : 0.0f;
}

void DemoJuceFilter::setParameter (int index, float newValue)
{
	for (int i = 0; i < noParams; i++)
	{
		if (index == i) {
			if (params[i] != newValue) {
				params[i] = newValue;
				sendChangeMessage (this);
			}
		}
	}
	
	updateFilters();
}

const String DemoJuceFilter::getParameterName (int index)
{
	for (int i = 0; i < noParams; i++)
		if (index == i)
			return String(parameterNames[i]);	
	
//	if (index == 0)
//        return T("gain");

    return String::empty;
}

const String DemoJuceFilter::getParameterText (int index)
{
	for (int i = 0; i < noParams; i++)
		if (index == i)
			return String(params[i], 2);	
	
//	if (index == 0)
//        return String (gain, 2);

    return String::empty;
}

const String DemoJuceFilter::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String DemoJuceFilter::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool DemoJuceFilter::isInputChannelStereoPair (int index) const
{
    return false;
}

bool DemoJuceFilter::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool DemoJuceFilter::acceptsMidi() const
{
    return true;
}

bool DemoJuceFilter::producesMidi() const
{
    return true;
}

//==============================================================================
void DemoJuceFilter::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	currentSampleRate = sampleRate;
	
	// set up meter variables
	iMeasureLength = (int)sampleRate * 0.001f;
	iMeasuredItems = 0;
	fMax0 = 0;
	
	fOutMultCurrent = fOutMultTarget = 0;
	
	// do your pre-playback setup stuff here..
    keyboardState.reset();
	
	updateFilters();
}

void DemoJuceFilter::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void DemoJuceFilter::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
	const int numInputChannels = getNumInputChannels();
	if (numInputChannels > 0)
	{
		// create parameters to use
		float fThresh = params[THRESH] * params[THRESH] * params[THRESH];
		float fReduction = params[REDUCTION]*params[REDUCTION]*params[REDUCTION];
		float fAttack = (params[ATTACK]*params[ATTACK]*params[ATTACK]*0.099f) + 0.001;
		float fRelease = (params[RELEASE]*params[RELEASE]*params[RELEASE]*0.0099) + 0.0001;
		float fGain = params[GAIN]*params[GAIN]*params[GAIN];
		
		int numSamples = buffer.getNumSamples();
		float* pfSample[numInputChannels];
		for (int channel = 0; channel < getNumInputChannels(); channel++)
			pfSample[channel] = buffer.getSampleData(channel);
		
		// set-up mixed mono buffer
		AudioSampleBuffer mixedBuffer(1, buffer.getNumSamples());
		float* pfMixedSample = mixedBuffer.getSampleData(0);
		
		// fill mono mixed buffer
		for(int i = 0; i < mixedBuffer.getNumSamples(); i++) {
			*pfMixedSample = 0.5f * (*pfSample[0] + *pfSample[1]);
			pfMixedSample++;
			pfSample[0]++;
			pfSample[1]++;
		}
		
		// filter mixed buffer
		lowpassFilter.processSamples(mixedBuffer.getSampleData(0), mixedBuffer.getNumSamples());
		highpassFilter.processSamples(mixedBuffer.getSampleData(0), mixedBuffer.getNumSamples());
		
		// reset buffer pointers
		for (int channel = 0; channel < getNumInputChannels(); channel++)
			pfSample[channel] = buffer.getSampleData(channel);
		pfMixedSample = mixedBuffer.getSampleData(0);
		
				
		//========================================================================
		while (--numSamples >= 0)
		{
//			float fMix = 0.5f * (*pfMixedSample[0] + *pfMixedSample[1]);
			float fMix = *pfMixedSample;
			pfMixedSample++;
//			pfMixedSample[1]++;
//			lowpassFilter.processSamples(&fMix, 1);
			
			if(fabsf(fMix) > fMax0)
				fMax0 = fabs(fMix);
			
			if (iMeasuredItems >= iMeasureLength)
			{
				if (fThresh < fMax0)
					fOutMultTarget = fReduction;
				else
					fOutMultTarget = 1;
				
				if (fOutMultTarget > fOutMultCurrent)
					fOutMultCurrent += (fOutMultTarget - fOutMultCurrent) * fAttack;
				else
					fOutMultCurrent -= (fOutMultCurrent - fOutMultTarget) * fRelease;
				
				
				fMax0 = 0;
				iMeasuredItems = 0;
			}
			iMeasuredItems++;
			
			// process channels as interleaved
			for (int channel = 0; channel < numInputChannels; channel++)
			{			
				*pfSample[channel] *= fOutMultCurrent;
				
				// apply gain
				*pfSample[channel] *= fGain;
				
				// incriment sample pointers
				pfSample[channel]++;			
			}
		}
		//========================================================================	
		
	}

	// update the sample to use in the meter display
//	RMSLeft = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
//	RMSRight = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
	peakLeft = buffer.getMagnitude(0, 0, buffer.getNumSamples());
	peakRight = buffer.getMagnitude(1, 0, buffer.getNumSamples());
	peakLeft = fOutMultCurrent;
	
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
AudioProcessorEditor* DemoJuceFilter::createEditor()
{
    return new DemoEditorComponent (this);
}

//==============================================================================
void DemoJuceFilter::getStateInformation (MemoryBlock& destData)
{
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML..

    // create an outer XML element..
    XmlElement xmlState (T("MYPLUGINSETTINGS"));

    // add some attributes to it..
    xmlState.setAttribute (T("pluginVersion"), 1);
//    xmlState.setAttribute (T("gainLevel"), gain);

    // you could also add as many child elements as you need to here..


    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xmlState, destData);
}

void DemoJuceFilter::setStateInformation (const void* data, int sizeInBytes)
{
    // use this helper function to get the XML from this binary blob..
    XmlElement* const xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != 0)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName (T("MYPLUGINSETTINGS")))
        {
            // ok, now pull out our parameters..
//            gain = (float) xmlState->getDoubleAttribute (T("gainLevel"), gain);


            sendChangeMessage (this);
        }

        delete xmlState;
    }
}

void DemoJuceFilter::updateFilters()
{
	lowpassFilter.makeLowPass(currentSampleRate, (params[LOWCF]*params[LOWCF]*9800) + 200);
	lowpassFilter.reset();
	highpassFilter.makeHighPass(currentSampleRate, params[HIGHCF]*params[HIGHCF]*4800 + 200);
	highpassFilter.reset();
}

void DemoJuceFilter::clip(AudioSampleBuffer &buffer)
{
//	const int numSamples = buffer.getNumSamples();
//	
//	for ( int i = 0; i < buffer.getNumInputChannels(); i++ )
//	{
//		while (--numSamples >= 0)
//		{
//			params[dist]*5
//		}
//	}
}






