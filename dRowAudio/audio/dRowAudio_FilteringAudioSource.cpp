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



namespace
{
    // modelled on a Pioneer DJM-800 mixer
    const float defaultSettings[FilteringAudioSource::numFilters][FilteringAudioSource::numFilterSettings] = {
        {70,   0.25f},
        {1000,  0.25f},
        {13000,  0.25f}
    };
}

//========================================================================
FilteringAudioSource::FilteringAudioSource (AudioSource* inputSource,
                                            bool deleteInputWhenDeleted)
    : input         (inputSource, deleteInputWhenDeleted),
	  sampleRate    (44100.0),
	  filterSource  (true)
{
    gains[Low] = 1.0f;
    gains[Mid] = 1.0f;
    gains[High] = 1.0f;

	// configure the filters
    resetFilters();
}

FilteringAudioSource::~FilteringAudioSource()
{
    releaseResources();
}

//==============================================================================
void FilteringAudioSource::setGain (FilterType setting, float newGain)
{
    switch (setting)
    {
        case Low:
            gains[Low] = newGain;
            filter[0][Low].makeLowShelf     (sampleRate, defaultSettings[Low][CF], defaultSettings[Low][Q], gains[Low]);
            filter[1][Low].makeLowShelf     (sampleRate, defaultSettings[Low][CF], defaultSettings[Low][Q], gains[Low]);
            break;
        case Mid:
            gains[Mid] = newGain;
            filter[0][Mid].makeBandPass     (sampleRate, defaultSettings[Mid][CF], defaultSettings[Mid][Q], gains[Mid]);
            filter[1][Mid].makeBandPass     (sampleRate, defaultSettings[Mid][CF], defaultSettings[Mid][Q], gains[Mid]);
            break;
        case High:
            gains[High] = newGain;
            filter[0][High].makeHighShelf   (sampleRate, defaultSettings[High][CF], defaultSettings[High][Q], gains[High]);
            filter[1][High].makeHighShelf   (sampleRate, defaultSettings[High][CF], defaultSettings[High][Q], gains[High]);
            break;
        default:
            break;
    }
}

//==============================================================================
void FilteringAudioSource::prepareToPlay (int samplesPerBlockExpected,
                                          double sampleRate_)
{
    const ScopedLock sl (callbackLock);

    sampleRate = sampleRate_;

    resetFilters();
    
    if (input != nullptr)
        input->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void FilteringAudioSource::releaseResources()
{
    const ScopedLock sl (callbackLock);
	
    if (input != nullptr)
        input->releaseResources();
}

void FilteringAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
    const ScopedLock sl (callbackLock);
		
    if (input != nullptr)
    {
        input->getNextAudioBlock (info);

		if (filterSource && info.buffer->getNumChannels() > 0)
		{
			// filter samples
			const int bufferNumSamples = info.numSamples;
			float* sampleDataL = info.buffer->getSampleData (0, info.startSample);
            
            filter[0][Low].processSamples   (sampleDataL, bufferNumSamples);
			filter[0][Mid].processSamples   (sampleDataL, bufferNumSamples);
			filter[0][High].processSamples  (sampleDataL, bufferNumSamples);

            if (info.buffer->getNumChannels() > 1)
            {
                float* sampleDataR = info.buffer->getSampleData (1, info.startSample);

                filter[1][Low].processSamples   (sampleDataR, bufferNumSamples);
                filter[1][Mid].processSamples   (sampleDataR, bufferNumSamples);
                filter[1][High].processSamples  (sampleDataR, bufferNumSamples);
            }
		}
    }
    else
    {
        info.clearActiveBufferRegion();
    }
}

void FilteringAudioSource::resetFilters()
{
	filter[0][Low].makeLowShelf   (sampleRate, defaultSettings[Low][CF], defaultSettings[Low][Q], gains[Low]);
	filter[1][Low].makeLowShelf   (sampleRate, defaultSettings[Low][CF], defaultSettings[Low][Q], gains[Low]);
	filter[0][Mid].makeBandPass   (sampleRate, defaultSettings[Mid][CF], defaultSettings[Mid][Q], gains[Mid]);
	filter[1][Mid].makeBandPass   (sampleRate, defaultSettings[Mid][CF], defaultSettings[Mid][Q], gains[Mid]);
	filter[0][High].makeHighShelf (sampleRate, defaultSettings[High][CF], defaultSettings[High][Q], gains[High]);
	filter[1][High].makeHighShelf (sampleRate, defaultSettings[High][CF], defaultSettings[High][Q], gains[High]);	
}

