/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

#include "includes.h"
#include "DRowAudioFilter.h"
#include "DemoEditorComponent.h"


//==============================================================================
/**
    This function must be implemented to create a new instance of your
    plugin object.
*/
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DRowAudioFilter();
}

//==============================================================================
DRowAudioFilter::DRowAudioFilter()
{
    
	
//	for (int i = 0; i < noParams; i++)
//		params[i].setValue(1.0f);
//	
//	// set default values for parameters here:
//	params[REDUCTION].setValue(0.2f);
//	params[LOWCF].setValue(1.0f);
//	params[HIGHCF].setValue(0.0f);
//	params[BANDCF].setValue(0.0f);
//	params[BANDQ].setValue(0.0f);
	
    zeromem (&lastPosInfo, sizeof (lastPosInfo));
    lastPosInfo.timeSigNumerator = 4;
    lastPosInfo.timeSigDenominator = 4;
    lastPosInfo.bpm = 120;
}

DRowAudioFilter::~DRowAudioFilter()
{
}

//==============================================================================
const String DRowAudioFilter::getName() const
{
    return "dRowAudio: Gate";
}

void DRowAudioFilter::setupParams()
{
//	params[THRESH].init(parameterNames[THRESH], UnitPercent, T("Changes the threshold"),
//						50.0, 0.0, 100.0,
//						50.0, 1.0, 0.0);
}

int DRowAudioFilter::getNumParameters()
{
    return noParams;
}

float DRowAudioFilter::getParameter (int index)
{
//	if (index >= 0 && index < noParams)
//			return params[index].getNormalisedValue();
//	else return 0.0f;
}

double DRowAudioFilter::getScaledParameter (int index)
{
	if (index >= 0 && index < noParams)
		return params[index].getValue();
	else return 0.0f;
}

void DRowAudioFilter::setParameter (int index, float newValue)
{
	for (int i = 0; i < noParams; i++)
	{
		if (index == i) {
			if (params[i].getValue() != newValue) {
				params[i].setNormalisedValue(newValue);
				sendChangeMessage (this);
			}
		}
	}
	
	updateFilters();
}

void DRowAudioFilter::setScaledParameter (int index, float newValue)
{
	for (int i = 0; i < noParams; i++)
	{
		if (index == i) {
			if (params[i].getValue() != newValue) {
				params[i].setValue(newValue);
				sendChangeMessage (this);
			}
		}
	}
	
	updateFilters();
}

void DRowAudioFilter::setScaledParameterNotifyingHost(int index, float newValue)
{
	for (int i = 0; i < noParams; i++)
		if (index == i)
			if (params[i].getValue() != newValue)
				setParameterNotifyingHost(index, params[i].normaliseValue(newValue));
}

const String DRowAudioFilter::getParameterName (int index)
{
	for (int i = 0; i < noParams; i++)
		if (index == i)
			return String(parameterNames[i]);	
	
    return String::empty;
}

const String DRowAudioFilter::getParameterText (int index)
{
	for (int i = 0; i < noParams; i++)
		if (index == i)
			return String(params[i].getValue(), 2);	

    return String::empty;
}

//=====================================================================
// methods for AU Compatibility
double DRowAudioFilter::getParameterMin(int index)
{
	for (int i = 0; i < noParams; i++)
		if (index == i)
			return params[i].getMin();	
    return 0.0f;
}

double DRowAudioFilter::getParameterMax(int index)
{
	for (int i = 0; i < noParams; i++)
		if (index == i)
			return params[i].getMax();	
    return 0.0f;
}

double DRowAudioFilter::getParameterDefault(int index)
{
	for (int i = 0; i < noParams; i++)
		if (index == i)
			return params[i].getDefault();	
    return 0.0f;
}

ParameterUnit DRowAudioFilter::getParameterUnit(int index)
{
	for (int i = 0; i < noParams; i++)
		if (index == i)
			return params[i].getUnit();	
    return (ParameterUnit)0;
}

void DRowAudioFilter::smoothParameters()
{
	for (int i = 0; i < noParams; i++)
		params[i].smooth();
}
//=====================================================================

const String DRowAudioFilter::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String DRowAudioFilter::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool DRowAudioFilter::isInputChannelStereoPair (int index) const
{
    return false;
}

bool DRowAudioFilter::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool DRowAudioFilter::acceptsMidi() const
{
    return true;
}

bool DRowAudioFilter::producesMidi() const
{
    return true;
}

//==============================================================================
void DRowAudioFilter::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	currentSampleRate = sampleRate;
	oneOverCurrentSampleRate = 1.0f/currentSampleRate;
	
	// set up meter variables
	iMeasureLength = (int)sampleRate * 0.001f;
	iMeasuredItems = 0;
	fMax = 0;
		
	fOutMultCurrent = fOutMultTarget = 0;
	
	// do your pre-playback setup stuff here..
    keyboardState.reset();
	
	updateFilters();
}

void DRowAudioFilter::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void DRowAudioFilter::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
	const int numInputChannels = getNumInputChannels();
	if (numInputChannels == 2)
	{
		// create parameters to use
		float fThresh = params[THRESH].getValue() * params[THRESH].getValue() * params[THRESH].getValue();
		float fReduction = params[REDUCTION].getValue()*params[REDUCTION].getValue()*params[REDUCTION].getValue();
		float fAttack = (params[ATTACK].getValue()*params[ATTACK].getValue()*params[ATTACK].getValue()*0.099f) + 0.001;
		float fRelease = (params[RELEASE].getValue()*params[RELEASE].getValue()*params[RELEASE].getValue()*0.0099) + 0.0001;
		float fGain = params[GAIN].getValue()*params[GAIN].getValue()*params[GAIN].getValue();
		
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
		bandpassFilter.processSamples(mixedBuffer.getSampleData(0), mixedBuffer.getNumSamples());
		
		// reset buffer pointers
		for (int channel = 0; channel < getNumInputChannels(); channel++)
			pfSample[channel] = buffer.getSampleData(channel);
		pfMixedSample = mixedBuffer.getSampleData(0);
		
				
		//========================================================================
		while (--numSamples >= 0)
		{
			float fMix = *pfMixedSample;
			pfMixedSample++;
			
			if(fabsf(fMix) > fMax)
				fMax = fabs(fMix);
			
			if (iMeasuredItems >= iMeasureLength)
			{
				if (fThresh < fMax)
					fOutMultTarget = fReduction;
				else
					fOutMultTarget = 1;
				
				if (fOutMultTarget > fOutMultCurrent)
					fOutMultCurrent += (fOutMultTarget - fOutMultCurrent) * fAttack;
				else
					fOutMultCurrent -= (fOutMultCurrent - fOutMultTarget) * fRelease;
				
				
				fMax = 0;
				iMeasuredItems = 0;
			}
			iMeasuredItems++;
			
			// process channels as interleaved
			for (int channel = 0; channel < numInputChannels; channel++)
			{			
				*pfSample[channel] *= fOutMultCurrent;
//				*pfSample[channel] = fMix;
				
				// apply gain
				*pfSample[channel] *= fGain;
				
				// incriment sample pointers
				pfSample[channel]++;			
			}
		}
		//========================================================================	
		
		// update the sample to use in the meter display
		RMSLeft = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
		RMSRight = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
		peakLeft = buffer.getMagnitude(0, 0, buffer.getNumSamples());
		peakRight = buffer.getMagnitude(1, 0, buffer.getNumSamples());
		//	peakLeft = fOutMultCurrent;		
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
AudioProcessorEditor* DRowAudioFilter::createEditor()
{
    return new DemoEditorComponent (this);
}

//==============================================================================
void DRowAudioFilter::getStateInformation (MemoryBlock& destData)
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

void DRowAudioFilter::setStateInformation (const void* data, int sizeInBytes)
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

void DRowAudioFilter::updateFilters()
{
	const float bandCf = (params[BANDCF].getValue()*params[BANDCF].getValue()*9800) + 200;
	const float bandQ = ((1-(0.9f*params[BANDQ].getValue()*params[BANDQ].getValue()))*10);

	bandpassFilter.createBandPass(currentSampleRate, bandCf, bandQ);
	
}

