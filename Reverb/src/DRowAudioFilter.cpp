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
#include "DRowAudioFilter.h"
#include "DRowAudioEditorComponent.h"


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
    zeromem (&lastPosInfo, sizeof (lastPosInfo));
    lastPosInfo.timeSigNumerator = 4;
    lastPosInfo.timeSigDenominator = 4;
    lastPosInfo.bpm = 120;
	
	params[GAIN].setValue(0.5);

	params[PREDELAY].init(parameterNames[PREDELAY], UnitMilliseconds, String::empty,
						  50, 0.025, 200, 50);

	params[ROOMSHAPE].init(parameterNames[ROOMSHAPE], UnitGeneric, String::empty,
						   3, 1, 7, 3);
	params[ROOMSHAPE].setStep(1);
	prevRoomShape = 0;
	
	params[FBCOEFF].setValue(0.9);
	
	params[DELTIME].init(parameterNames[DELTIME], UnitGeneric, String::empty,
						80, 0.15, 100, 80);
	params[DELTIME].setSkewFactor(0.5);
		
	params[FILTERCF].init(parameterNames[FILTERCF], UnitHertz, String::empty,
						  1500, 50, 16000, 1500);
	params[FILTERCF].setSkewFactor(0.5);
	
	params[DIFFUSION].setValue(0.7);
	
	params[SPREAD].init(parameterNames[SPREAD], UnitGeneric, String::empty,
						0.5, 0.5, 1, 0.5);
	
	params[LOWEQ].init(parameterNames[LOWEQ], UnitHertz, String::empty,
						1, 0, 2, 1);

	params[HIGHEQ].init(parameterNames[HIGHEQ], UnitHertz, String::empty,
						1, 0, 2, 1);

	DBG(String((int)sizeof(DRowAudioFilter)));
}

DRowAudioFilter::~DRowAudioFilter()
{
}

//==============================================================================
const String DRowAudioFilter::getName() const
{
    return "dRowAudio: Reverb";
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

dRowParameter* DRowAudioFilter::getParameterPointer(int index)
{
	for (int i = 0; i < noParams; i++)
		if (index == i)
			return &params[i];	
	
    return 0;
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
	
	preDelayFilterL.setMaxDelayTime(currentSampleRate, params[PREDELAY].getMax());
	preDelayFilterR.setMaxDelayTime(currentSampleRate, params[PREDELAY].getMax());

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
	if (numInputChannels > 0)
	{
		int numSamples = buffer.getNumSamples();
		float* pfSample[numInputChannels];
		for (int channel = 0; channel < getNumInputChannels(); channel++)
			pfSample[channel] = buffer.getSampleData(channel, 0);
		
		
		smoothParameters();
		
//		float gain = params[GAIN].getSmoothedNormalisedValue();
		float preDelay = (float)params[PREDELAY].getSmoothedValue();
		float earlyLateRatio = (float)params[EARLYLATEMIX].getSmoothedNormalisedValue();
		float early = earlyLateRatio;
		float late = 1.0f - early;
		float fbCoeff = -(float)params[FBCOEFF].getSmoothedValue();
		float delayTime = (float)params[DELTIME].getSmoothedValue();
		float filterCf = (float)params[FILTERCF].getSmoothedValue();
		float allpassCoeff = (float)params[DIFFUSION].getSmoothedValue();
		float spread = (float)params[SPREAD].getSmoothedValue();
		float lowEQGain = (float)params[LOWEQ].getSmoothedValue();
		float highEQGain = (float)params[HIGHEQ].getSmoothedValue();
		float wetDryRatio = (float)params[WETDRYMIX].getSmoothedNormalisedValue();
		float wet = wetDryRatio;
		float dry = 1.0f - wet;
		
		float width = (spread-0.5f) * 10.0f * (float)params[DELTIME].getSmoothedNormalisedValue();
		float delayTimeRight = delayTime + width;
		float spread1 = spread;
		float spread2 = 1.0f - spread1;
		
		
		// pre-delay section
		preDelayFilterL.setDelayTime(currentSampleRate, preDelay);
		preDelayFilterL.setFBCoeff(0.0);
		preDelayFilterL.setFFCoeff(0.0);

		preDelayFilterR.setDelayTime(currentSampleRate, preDelay);
		preDelayFilterR.setFBCoeff(0.0);
		preDelayFilterR.setFFCoeff(0.0);
		
		
		// early reflections section
		int roomShape = roundFloatToInt(params[ROOMSHAPE].getValue());
		if (roomShape != prevRoomShape)
		{
			int shapeDiff = roomShape - prevRoomShape;
			
			if (shapeDiff > 0)	// adding
				for (int i = prevRoomShape; i < roomShape; i++) {
					delayLineL.addTapAtTime(earlyReflectionCoeffs[i], currentSampleRate);
					delayLineR.addTapAtTime(earlyReflectionCoeffs[i], currentSampleRate);
				}
			else				// removing
				for (int i = prevRoomShape; i > roomShape; --i) {
					delayLineL.removeTapAtIndex(i-1);
					delayLineR.removeTapAtIndex(i-1);
				}
			prevRoomShape = roomShape;
		}

		float delayCoeff = 10 * (float)params[DELTIME].getSmoothedNormalisedValue();
		float earlyReflectionWidth = (float)params[SPREAD].getSmoothedNormalisedValue();
		delayLineL.setTapSpacing(delayCoeff);
		delayLineL.scaleFeedbacks(fbCoeff);
		delayLineR.setTapSpacing(delayCoeff + earlyReflectionWidth);
		delayLineR.scaleFeedbacks(fbCoeff);
		
		
		// comb filter section
		for ( int i = 0; i < 8; i++)
		{
			delayTime *= filterMultCoeffs[i];
			delayTimeRight *= filterMultCoeffs[i];
			setupFilter(combFilterL[i], fbCoeff, delayTime, filterCf);
			setupFilter(combFilterR[i], fbCoeff, delayTime + width, filterCf);
		}		
		
		// allpass section
		for ( int i = 0; i < 4; i++)
		{
			delayTime *= allpassMultCoeffs[i];
			delayTimeRight *= allpassMultCoeffs[i];
			
			allpassFilterL[i].setGain(allpassCoeff);
			allpassFilterL[i].setDelayTime(currentSampleRate, delayTime);
			
			allpassFilterR[i].setGain(allpassCoeff);
			allpassFilterR[i].setDelayTime(currentSampleRate, delayTime + width);
		}

		lowEQL.makeLowShelf(currentSampleRate, 500, 1, lowEQGain);
		lowEQR.makeLowShelf(currentSampleRate, 500, 1, lowEQGain);
		highEQL.makeHighShelf(currentSampleRate, 3000, 1, highEQGain);
		highEQR.makeHighShelf(currentSampleRate, 3000, 1, highEQGain);
		
		//========================================================================
		while (--numSamples >= 0)
		{
			float dryL = *pfSample[0];
			float dryR = *pfSample[1];
			float inR;
			float inL = inR = (dryL + dryR) * 0.15f;
			
			// pre-delay
			inL = preDelayFilterL.processSingleSample(inL); 
			inR = preDelayFilterR.processSingleSample(inR);
			
			// early relections
			float earlyL = inL;
			float earlyR = inR;
			
			earlyL = delayLineL.processSingleSample(earlyL);
			earlyR = delayLineR.processSingleSample(earlyR);

			// comb filter for reflections and absorption
			inL = combFilterL[0].processSingleSample(inL)
				  + combFilterL[1].processSingleSample(inL)
				  + combFilterL[2].processSingleSample(inL)
				  + combFilterL[3].processSingleSample(inL)
				  + combFilterL[4].processSingleSample(inL)
				  + combFilterL[5].processSingleSample(inL)
				  + combFilterL[6].processSingleSample(inL)
				  + combFilterL[7].processSingleSample(inL);
							
			inR = combFilterR[0].processSingleSample(inR)
				  + combFilterR[1].processSingleSample(inR)
				  + combFilterR[2].processSingleSample(inR)
				  + combFilterR[3].processSingleSample(inR)
				  + combFilterR[4].processSingleSample(inR)
				  + combFilterR[5].processSingleSample(inR)
				  + combFilterR[6].processSingleSample(inR)
				  + combFilterR[7].processSingleSample(inR);
			
			// allpass section for disperstion
			inL = allpassFilterL[0].processSingleSample(inL);
			inL = allpassFilterL[1].processSingleSample(inL);
			inL = allpassFilterL[2].processSingleSample(inL);
			inL = allpassFilterL[3].processSingleSample(inL);
			
			inR = allpassFilterR[0].processSingleSample(inR);
			inR = allpassFilterR[1].processSingleSample(inR);
			inR = allpassFilterR[2].processSingleSample(inR);
			inR = allpassFilterR[3].processSingleSample(inR);
			
			inL = lowEQL.processSingleSampleRaw(inL);
			inR = lowEQR.processSingleSampleRaw(inR);
			inL = highEQL.processSingleSampleRaw(inL);
			inR = highEQR.processSingleSampleRaw(inR);
			
			*pfSample[0] = (early * earlyL) + (late * wet*((inL * spread1) + (inR * spread2))) + (dryL * dry);
			*pfSample[1] = (early * earlyR) + (late * wet*((inR * spread1) + (inL * spread2))) + (dryR * dry);
			
			
			// incriment sample pointers
			pfSample[0]++;			
			pfSample[1]++;			
		}
		//========================================================================
		
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
/*    keyboardState.processNextMidiBuffer (midiMessages,
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
    }*/
}

void DRowAudioFilter::setupFilter(LBCF &filter, float fbCoeff, float delayTime, float filterCf)
{
	filter.setFBCoeff(fbCoeff);
	filter.setDelayTime(currentSampleRate, delayTime);
	filter.setLowpassCutoff(currentSampleRate, filterCf);
}

//==============================================================================
AudioProcessorEditor* DRowAudioFilter::createEditor()
{
    return new DRowAudioEditorComponent (this);
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
	
}