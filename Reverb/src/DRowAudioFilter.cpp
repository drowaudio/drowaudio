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
	params[PREDELAY].init(parameterNames[PREDELAY], UnitMilliseconds, String::empty,
						  50, 0.025, 200, 50);
	params[PREDELAY].setStep(0.1);
	params[PREDELAY].setUnitSuffix("ms");
	
	params[ROOMSHAPE].init(parameterNames[ROOMSHAPE], UnitGeneric, String::empty,
						   3, 1, 7, 3);
	params[ROOMSHAPE].setStep(1);
	prevRoomShape = 0;
	
	params[EARLYDECAY].setValue(0.9);
	params[EARLYDECAY].setUnitSuffix("s");
	
	params[EARLYLATEMIX].setValue(0.5);
	params[EARLYLATEMIX].setUnitSuffix("%");
	
	params[FBCOEFF].setValue(0.9);
	params[FBCOEFF].setUnitSuffix("s");
	
	params[DELTIME].init(parameterNames[DELTIME], UnitGeneric, String::empty,
						80, 0.15, 100, 80);
	params[DELTIME].setSkewFactor(0.5);
	params[DELTIME].setUnitSuffix("m");
	
	params[FILTERCF].init(parameterNames[FILTERCF], UnitHertz, String::empty,
						  1500, 50, 16000, 1500);
	params[FILTERCF].setSkewFactor(0.5);
	params[FILTERCF].setStep(1);
	params[FILTERCF].setUnitSuffix("Hz");
	
	params[DIFFUSION].setValue(0.7);
	params[DIFFUSION].setUnitSuffix("%");
	
	params[SPREAD].init(parameterNames[SPREAD], UnitGeneric, String::empty,
						0.5, 0.5, 1, 0.5);
	params[SPREAD].setUnitSuffix("%");
	
	params[LOWEQ].init(parameterNames[LOWEQ], UnitHertz, String::empty,
						1, 0, 2, 1);
	params[LOWEQ].setUnitSuffix("dB");

	params[HIGHEQ].init(parameterNames[HIGHEQ], UnitHertz, String::empty,
						1, 0, 2, 1);
	params[HIGHEQ].setUnitSuffix("dB");

	params[WETDRYMIX].setUnitSuffix("%");

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
		
		smoothParameters();
		
//		float gain = params[GAIN].getSmoothedNormalisedValue();
		float preDelay = (float)params[PREDELAY].getSmoothedValue();
		float earlyDecay = (float)params[EARLYDECAY].getSmoothedValue();
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
		preDelayFilterR.setDelayTime(currentSampleRate, preDelay);
		
		
		// early reflections section
		int roomShape = roundFloatToInt(params[ROOMSHAPE].getValue());
		if (roomShape != prevRoomShape)
		{
			/*int shapeDiff = roomShape - prevRoomShape;
			
			if (shapeDiff > 0)	// adding
				for (int i = prevRoomShape; i < roomShape; i++) {
					delayLineL.addTapAtTime(earlyReflectionCoeffs[i], currentSampleRate);
					delayLineR.addTapAtTime(earlyReflectionCoeffs[i], currentSampleRate);
				}
			else				// removing
				for (int i = prevRoomShape; i > roomShape; --i) {
					delayLineL.removeTapAtIndex(i-1);
					delayLineR.removeTapAtIndex(i-1);
				}*/
			delayLineL.removeAllTaps();
			delayLineR.removeAllTaps();
			for(int i = 0; i < 7; i++) {
				delayLineL.addTapAtTime(earlyReflectionCoeffs[roomShape-1][i], currentSampleRate);
				delayLineR.addTapAtTime(earlyReflectionCoeffs[roomShape-1][i], currentSampleRate);				
			}
			
			prevRoomShape = roomShape;
		}

		float delayCoeff = 10.0f * (float)params[DELTIME].getSmoothedNormalisedValue();
		float earlyReflectionWidth = (float)params[SPREAD].getSmoothedNormalisedValue();
		delayLineL.setTapSpacing(delayCoeff);
		delayLineL.scaleFeedbacks(earlyDecay);
		delayLineR.setTapSpacing(delayCoeff + earlyReflectionWidth);
		delayLineR.scaleFeedbacks(earlyDecay);
		
		
		// comb filter section
		for (int i = 0; i < 8; ++i)
		{
			delayTime *= filterMultCoeffs[i];
			delayTimeRight *= filterMultCoeffs[i];
			setupFilter(combFilterL[i], fbCoeff, delayTime, filterCf);
			setupFilter(combFilterR[i], fbCoeff, delayTime + width, filterCf);
		}		
		
		// allpass section
		for (int i = 0; i < 4; ++i)
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
		//	Processing
		//========================================================================
		int noSamples = buffer.getNumSamples();
		int noChannels = buffer.getNumChannels();
		
		AudioSampleBuffer wetBuffer(noChannels, noSamples);
		wetBuffer.copyFrom(0, 0, buffer, 0, 0, noSamples);
		wetBuffer.copyFrom(1, 0, buffer, 1, 0, noSamples);
		
		// mono mix wet buffer
		float *pfWetL = wetBuffer.getSampleData(0);
		float *pfWetR = wetBuffer.getSampleData(1);
		while (--numSamples >= 0)
		{
			*pfWetL = *pfWetR = (0.5f * (*pfWetL + *pfWetR));
			pfWetL++;
			pfWetR++;
		}
		numSamples = buffer.getNumSamples();
		
		preDelayFilterL.processSamples(wetBuffer.getSampleData(0), noSamples);
		preDelayFilterR.processSamples(wetBuffer.getSampleData(1), noSamples);
		

		AudioSampleBuffer earlyReflections(noChannels, noSamples);
		earlyReflections.copyFrom(0, 0, wetBuffer, 0, 0, noSamples);
		earlyReflections.copyFrom(1, 0, wetBuffer, 1, 0, noSamples);

		delayLineL.processSamples(earlyReflections.getSampleData(0), noSamples);
		delayLineR.processSamples(earlyReflections.getSampleData(1), noSamples);

		
		AudioSampleBuffer lateReverb(noChannels, noSamples);
		lateReverb.clear();
		
		float *pfLateL = lateReverb.getSampleData(0);
		float *pfLateR = lateReverb.getSampleData(1);
		pfWetL = wetBuffer.getSampleData(0);
		pfWetR = wetBuffer.getSampleData(1);

		// comb filter section
		for (int i = 0; i < 8; ++i)
		{
			combFilterL[i].processSamplesAdding(pfWetL, pfLateL, noSamples);
			combFilterR[i].processSamplesAdding(pfWetR, pfLateR, noSamples);
		}
		
		// allpass filter section
		for (int i = 0; i < 4; ++i)
		{
			allpassFilterL[i].processSamples(lateReverb.getSampleData(0), noSamples);
			allpassFilterR[i].processSamples(lateReverb.getSampleData(1), noSamples);
		}
		
		
		// clear wet buffer
		wetBuffer.clear();
		// add early reflections to output buffer
		wetBuffer.addFrom(0, 0, earlyReflections, 0, 0, noSamples, early);
		wetBuffer.addFrom(1, 0, earlyReflections, 1, 0, noSamples, early);
		// add early reflections to output buffer
		lateReverb.applyGain(0, noSamples, 0.1f);
		wetBuffer.addFrom(0, 0, lateReverb, 0, 0, noSamples, late);
		wetBuffer.addFrom(1, 0, lateReverb, 1, 0, noSamples, late);
		
		// final EQ
		lowEQL.processSamples(wetBuffer.getSampleData(0), noSamples);
		lowEQR.processSamples(wetBuffer.getSampleData(1), noSamples);

		highEQL.processSamples(wetBuffer.getSampleData(0), noSamples);
		highEQR.processSamples(wetBuffer.getSampleData(1), noSamples);
		
		// create stereo spread
		pfWetL = wetBuffer.getSampleData(0);
		pfWetR = wetBuffer.getSampleData(1);
		while (--numSamples >= 0)
		{
			float fLeft = *pfWetL;
			float fRight = *pfWetR;
			*pfWetL = (fLeft * spread1) + (fRight * spread2);
			*pfWetR = (fRight * spread1) + (fLeft * spread2);
			pfWetL++;
			pfWetR++;
		}
		numSamples = buffer.getNumSamples();
		
		// add wet buffer to output buffer
		wetBuffer.applyGain(0, noSamples, wet);
		buffer.applyGain(0, noSamples, dry);
		buffer.addFrom(0, 0, wetBuffer, 0, 0, noSamples);
		buffer.addFrom(1, 0, wetBuffer, 1, 0, noSamples);
	}
	//========================================================================

	
    // in case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
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