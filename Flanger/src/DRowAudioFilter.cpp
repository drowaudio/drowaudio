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
	:	RMSLeft(0),
		RMSRight(0),
		peakLeft(0),
		peakRight(0)

{
	setupParams();
	
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
    return "dRowAudio: Flanger";
}

void DRowAudioFilter::setupParams()
{
/*	void init(const String& name_, ParameterUnit unit_, String description_,
			  double value_, double min_ =0.0f, double max_ =1.0f, double default_ =0.0f);*/
	
	params[RATE].init(parameterNames[RATE], UnitHertz, T("Changes the rate"),
					  0.5, 0.0, 20.0, 0.5);
	params[RATE].setSkewFactor(0.5f);
	params[RATE].setSmoothCoeff(0.1);
	params[DEPTH].init(parameterNames[DEPTH], UnitPercent, T("Changes the depth"),
					   20.0, 0.0, 100.0, 20.0);
	params[DEPTH].setSkewFactor(0.7f);
	params[DEPTH].setSmoothCoeff(0.1);
	params[FEEDBACK].init(parameterNames[FEEDBACK], UnitPercent, T("Changes the feedback ammount"),
						  0.0, 0.0, 99.0, 0.0);
	params[FEEDBACK].setStep(1);
	params[MIX].init(parameterNames[MIX], UnitPercent, T("Changes the output mix"),
					 100.0, 0.0, 100.0, 100.0);
	params[MIX].setStep(1);
}

int DRowAudioFilter::getNumParameters()
{
    return noParams;
}

float DRowAudioFilter::getParameter (int index)
{
	if (index >= 0 && index < noParams)
		return params[index].getNormalisedValue();
	else return 0.0f;
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
			if (params[i].getNormalisedValue() != newValue) {
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

double  DRowAudioFilter::getParameterStep(int index)
{
	for (int i = 0; i < noParams; i++)
		if (index == i)
			return params[i].getStep();	
    return 0.0f;
}
double  DRowAudioFilter::getParameterSkewFactor(int index)
{
	for (int i = 0; i < noParams; i++)
		if (index == i)
			return params[i].getSkewFactor();	
    return 0.0f;
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
	
	// set up wave buffer and fill with triangle data
	iLookupTableSize = 8192;
	iLookupTableSizeMask =  iLookupTableSize-1;
	
	pfLookupTable = new float[iLookupTableSize];
	float fPhaseStep = (2 * double_Pi) / iLookupTableSize;
//	for(int i = 0; i < iLookupTableSize; i++){
//		float val = sin(i*fPhaseStep);
//		pfLookupTable[i] = val;
//	}
	for(int i = 0; i < iLookupTableSize; i++){
		if(i < iLookupTableSize * 0.5) {
			float val = -1.0f + (2.0/double_Pi)*i*fPhaseStep;
			pfLookupTable[i] = val;
		}
		else {
			float val = 3.0f - (2.0/double_Pi)*i*fPhaseStep;
			pfLookupTable[i] = val;
		}
	}
	iLookupTablePos = 0;
	iSamplesProcessed = 0;
	
	
	// set up circular buffer
	iBufferSize = (int)sampleRate;
	pfCircularBuffer = new float[iBufferSize];
	for (int i = 0; i < iBufferSize; i++)
		pfCircularBuffer[i] = 0;
	iBufferWritePos = 0;
	
	
	// do your pre-playback setup stuff here..
    keyboardState.reset();
	
	updateFilters();
}

void DRowAudioFilter::releaseResources()
{
	delete[] pfLookupTable;
	delete[] pfCircularBuffer;
}

void DRowAudioFilter::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
	smoothParameters();
	
	const int numInputChannels = getNumInputChannels();

	if (numInputChannels == 2)
	{
//		const float oneOverNumInputChannels = 1.0f / numInputChannels;

		// create parameters to use
		float fRate = params[RATE].getSmoothedValue();
		float fDepth = (params[DEPTH].getSmoothedNormalisedValue() * 0.006f) + 0.0001f;
		float fFeedback = params[FEEDBACK].getSmoothedNormalisedValue();
		float fWetDryMix = params[MIX].getSmoothedNormalisedValue();
		
		// calculate current phase step
		float samplesPerCycle = currentSampleRate / fRate;
		float phaseStep = iLookupTableSize / samplesPerCycle;
		
		
		// set up array of pointers to samples
		int numSamples = buffer.getNumSamples();
		float* pfSample[numInputChannels];
		for (int channel = 0; channel < getNumInputChannels(); channel++)
			pfSample[channel] = buffer.getSampleData(channel);
		
		
		
				
		//========================================================================
		while (--numSamples >= 0)
		{
			float fMix = 0.5f * (*pfSample[0] + *pfSample[1]);
			
			iBufferWritePos++;
			if (iBufferWritePos >= iBufferSize)
				iBufferWritePos = 0;
//			pfCircularBuffer[iBufferWritePos] = fMix;
			
			
			int index =  (int)(iSamplesProcessed * phaseStep) &	iLookupTableSizeMask;

			iSamplesProcessed++;
			float fOsc = (pfLookupTable[index] * fDepth) + fDepth;
			
			float fBufferReadPos1 = (iBufferWritePos - (fOsc * iBufferSize));
			if (fBufferReadPos1 < 0)
				fBufferReadPos1 += iBufferSize;
			
			// linear interpolation section
			int iPos1, iPos2;
			float fDiff, fDel;
			
			iPos1 = (int)fBufferReadPos1;
			iPos2 = iPos1 + 1;
			if (iPos2 == iBufferSize)
				iPos2 = 0;
			fDiff = fBufferReadPos1 - iPos1;
			fDel = pfCircularBuffer[iPos2]*fDiff + pfCircularBuffer[iPos1]*(1-fDiff);
			
		    float fOut = 0.5f * (fMix + fWetDryMix*fDel);
			
			pfCircularBuffer[iBufferWritePos] = fMix + (fFeedback * fDel);
			
			
			// process channels as interleaved
			for (int channel = 0; channel < numInputChannels; channel++)
			{	
				*pfSample[channel] = fOut;
				
				// incriment sample pointers
				pfSample[channel]++;			
			}
		}
		//========================================================================	
		
		// update the sample to use in the meter display
		RMSLeft = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
		peakLeft = buffer.getMagnitude(0, 0, buffer.getNumSamples());
		RMSRight = buffer.getRMSLevel(1 & (numInputChannels-1), 0, buffer.getNumSamples());
		peakRight = buffer.getMagnitude(1 & (numInputChannels-1), 0, buffer.getNumSamples());
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

//===============================================================================
void DRowAudioFilter::updateFilters()
{

}


void DRowAudioFilter::updateParameters()
{
}