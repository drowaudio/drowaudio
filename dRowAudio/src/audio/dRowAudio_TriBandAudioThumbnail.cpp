/*
 *  dRowAudio_TriBandAudioThumbnail.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 11/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "dRowAudio_TriBandAudioThumbnail.h"

TriBandAudioThumbnail::TriBandAudioThumbnail(const int sourceSamplesPerThumbnailSample_,
											 AudioFormatManager& formatManagerToUse_)
	:	formatManagerToUse (formatManagerToUse_),
		source (0),
		reader (0),
		orginalSamplesPerThumbnailSample (sourceSamplesPerThumbnailSample_)
{
}

TriBandAudioThumbnail::~TriBandAudioThumbnail()
{
	deleteAndZero(source);
	deleteAndZero(reader);
	
	deleteAndZero(lowResCacheLowest);
	deleteAndZero(lowResCacheHighest);
	deleteAndZero(lowpassedCacheLowest);
	deleteAndZero(lowpassedCacheHighest);
	deleteAndZero(bandpassedCacheLowest);
	deleteAndZero(bandpassedCacheHighest);
	deleteAndZero(highpassedCacheLowest);
	deleteAndZero(highpassedCacheHighest);
	
	deleteAndZero(sourceData);
	deleteAndZero(sourceDataMixedMono);
}

void TriBandAudioThumbnail::setSource (InputSource* const newSource)
{
	deleteAndZero (source);
	source = newSource;

    if (newSource != 0)
    {
        {
            const ScopedLock sl (readerLock);
            reader = createReader();
        }
		
        if (reader != 0)
        {
            initialiseFromAudioFile (*reader);
			buildWaveform();
        }
    }	
}

void TriBandAudioThumbnail::setSource (AudioFilePlayer* const newFilePlayer)
{
    if (newFilePlayer != 0)
    {
        {
            const ScopedLock sl (readerLock);
            reader = newFilePlayer->getAudioFormatReaderSource()->getAudioFormatReader();
        }
		
        if (reader != 0)
        {
            initialiseFromAudioFile (*reader);
			buildWaveform();
        }
    }	
}


AudioFormatReader* TriBandAudioThumbnail::createReader() const
{
    if (source != 0)
    {
        InputStream* const audioFileStream = source->createInputStream();
		
        if (audioFileStream != 0)
            return formatManagerToUse.createReaderFor (audioFileStream);
    }
	
    return 0;
}

bool TriBandAudioThumbnail::initialiseFromAudioFile (AudioFormatReader& reader)
{
	noSourceSamples = reader.lengthInSamples;
    noChannels = reader.numChannels;
	sampleRate = reader.sampleRate;
    noSamplesCached = 0;
	
	noCacheSamples = (int)(noSourceSamples / orginalSamplesPerThumbnailSample);
	DBG(String("No cache samples = ")<<noCacheSamples);
	
    return noSourceSamples > 0;
}

bool TriBandAudioThumbnail::buildWaveform ()
{
	// create raw data from source file 
	MemoryBlock initialData(noSourceSamples * sizeof(int));
	int *initialDataPointer = (int*)initialData.getData();
	int **sourceDataOriginal = new int*[2];
	sourceDataOriginal[0] = initialDataPointer;
	sourceDataOriginal[1] = 0;

	// and read the file into the array
//	reader->read(sourceDataOriginal,
//				 0,
//				 noSourceSamples);	
    reader->read (sourceDataOriginal, 
				  1,
				  0, 
				  noSourceSamples,
				  true);
	
	// create a mixed mono copy of the source file
	MemoryBlock floatData(noSourceSamples * sizeof(float));	
	float *sourceDataMixedMono = (float*)floatData.getData();
	sourceData = new float*[2];
	sourceData[0] = sourceDataMixedMono;
	sourceData[1] = 0;
	
	
	// find maximum sample
	uint32 max = 0;
	for (int i = 0; i < noSourceSamples; i++)
		if (abs(initialDataPointer[i]) > max)
			max = abs(initialDataPointer[i]);

	// convert data to float
	double oneOverMax = 1.0 / max;
	for (int i = 0; i < noSourceSamples; i++)
		sourceDataMixedMono[i] = initialDataPointer[i] * oneOverMax;

	// build filtered waves
	buildLowpassedWave(floatData);
	buildBandpassedWave(floatData);
	buildHighpassedWave(floatData);
	
	return true;
}

void TriBandAudioThumbnail::buildLowpassedWave(MemoryBlock sourceData)
{
	float *originalWaveform = (float*)sourceData.getData();
	const int noOriginalWaveformSamples = sourceData.getSize()/sizeof(float);

	// filter the copy of the source data
	filter.makeLowPass(reader->sampleRate, 75);
	filter.reset();
	filter.processSamples(originalWaveform, noOriginalWaveformSamples);

	lowpassedCacheHighest = new float[noCacheSamples];
	lowpassedCacheLowest = new float[noCacheSamples];
	
	// find max and min
	findMaxMin(originalWaveform, originalWaveform, noOriginalWaveformSamples,
			   lowpassedCacheHighest, lowpassedCacheLowest, noCacheSamples);
	
/*	for (int i = 0; i < noCacheSamples; i++)
	{
		const int sourceStart = i * orginalSamplesPerThumbnailSample;
		const int sourceEnd = sourceStart + orginalSamplesPerThumbnailSample;		
		
		float lowest = 0.0f, highest = 0.0f;
		
		for ( int c = sourceStart; c < sourceEnd; c++)
		{
			if ( originalWaveform[c] < lowest )
				lowest = originalWaveform[c];
			else if ( originalWaveform[c] > highest )
				highest = originalWaveform[c];
		}
		
		lowpassedCacheHighest[i] = highest;
		lowpassedCacheLowest[i] = lowest;
	}*/
}

void TriBandAudioThumbnail::buildBandpassedWave(MemoryBlock sourceData)
{
	float *originalWaveform = (float*)sourceData.getData();
	
	// filter the copy of the source data
	filter.makeBandPass(reader->sampleRate, 2000, 2);
	filter.reset();
	filter.processSamples(originalWaveform, sourceData.getSize()/sizeof(float));
	
	bandpassedCacheHighest = new float[noCacheSamples];
	bandpassedCacheLowest = new float[noCacheSamples];
	
	// find max and min
	for (int i = 0; i < noCacheSamples; i++)
	{
		const int sourceStart = i * orginalSamplesPerThumbnailSample;
		const int sourceEnd = sourceStart + orginalSamplesPerThumbnailSample;		
		
		float lowest = 0.0f, highest = 0.0f;
		
		for ( int c = sourceStart; c < sourceEnd; c++)
		{
			if ( originalWaveform[c] < lowest )
				lowest = originalWaveform[c];
			else if ( originalWaveform[c] > highest )
				highest = originalWaveform[c];
		}
		
		bandpassedCacheHighest[i] = highest;
		bandpassedCacheLowest[i] = lowest;
	}
}

void TriBandAudioThumbnail::buildHighpassedWave(MemoryBlock sourceData)
{
	float *originalWaveform = (float*)sourceData.getData();
	
	// filter the copy of the source data
	filter.makeHighPass(reader->sampleRate, 4500);
	filter.reset();
	filter.processSamples(originalWaveform, sourceData.getSize()/sizeof(float));
	
	highpassedCacheHighest = new float[noCacheSamples];
	highpassedCacheLowest = new float[noCacheSamples];
	
	// find max and min
	for (int i = 0; i < noCacheSamples; i++)
	{
		const int sourceStart = i * orginalSamplesPerThumbnailSample;
		const int sourceEnd = sourceStart + orginalSamplesPerThumbnailSample;		
		
		float lowest = 0.0f, highest = 0.0f;
		
		for ( int c = sourceStart; c < sourceEnd; c++)
		{
			if ( originalWaveform[c] < lowest )
				lowest = originalWaveform[c];
			else if ( originalWaveform[c] > highest )
				highest = originalWaveform[c];
		}
		
		highpassedCacheHighest[i] = highest;
		highpassedCacheLowest[i] = lowest;
	}
}

void TriBandAudioThumbnail::findMaxMin(float* sourceStartSampleHighest, float* sourceStartSampleLowest, int sourceNumSamples,
									   float* destBufferHighest, float* destBufferLowest, int destBufferSize)
{
	float sourceSamplesPerDestSamples = (float)sourceNumSamples / destBufferSize;
	DBG(String("No. source samples: ")<<sourceNumSamples);
	DBG(String("No. dest samples: ")<<destBufferSize);
	DBG(String("sourceSamplesPerDestSamples: ")<<sourceSamplesPerDestSamples);
	
	for (int i = 0; i < destBufferSize; i++)
	{
		int sourceStart = roundFloatToInt(i * sourceSamplesPerDestSamples);
		int sourceEnd = roundFloatToInt(sourceStart + sourceSamplesPerDestSamples);
//		jlimit(0, sourceNumSamples, sourceStart);
//		jlimit(0, sourceNumSamples, sourceEnd);
		
		float lowest = 0.0f, highest = 0.0f;

		if ( sourceEnd < sourceNumSamples )
		{
			jlimit(0, sourceNumSamples, sourceStart);
			jlimit(0, sourceNumSamples, sourceEnd);

			for ( int c = sourceStart; c < sourceEnd; c++)
			{
				if ( sourceStartSampleLowest[c] < lowest )
					lowest = sourceStartSampleLowest[c];
				if ( sourceStartSampleHighest[c] > highest )
					highest = sourceStartSampleHighest[c];
			}
		}

		destBufferHighest[i] = highest;
		destBufferLowest[i] = lowest;
	}	
}

void TriBandAudioThumbnail::timerCallback()
{
}

void TriBandAudioThumbnail::drawChannel (Graphics& g,
										 int x, int y, int w, int h,
										 double startTime,
										 double endTime,
										 int channelNum,
										 const float verticalZoomFactor)
{
	const int centreY = y + (h * 0.5f);
	const float scale = h * 0.5f * verticalZoomFactor;
	const Rectangle clip (g.getClipBounds());
	
	int startCacheSample = roundFloatToInt( (startTime * 60.0f * sampleRate) / orginalSamplesPerThumbnailSample );
	int endCacheSample = roundFloatToInt( (endTime * 60.0f * sampleRate) / orginalSamplesPerThumbnailSample );
	jlimit(0, noSourceSamples, startCacheSample);
	jlimit(0, noSourceSamples, endCacheSample);
	
	// calculate section to draw
	const int noCacheSamples = endCacheSample - startCacheSample;
	const int noPixelsNeeded = w;
	DBG(String("No. cache samples: ")<<noCacheSamples);
	DBG(String("No. pixels needed: ")<<noPixelsNeeded<<"\n");
	
	
	//====================================================================================
//	float* lowpassedBufferHighest = new float[noPixelsNeeded];
//	float* lowpassedBufferLowest = new float[noPixelsNeeded];
	
	{
		float lowpassedBufferHighest[noPixelsNeeded];
		float lowpassedBufferLowest[noPixelsNeeded];
		
	//	findMaxMin(&lowpassedCacheHighest[startCacheSample], &lowpassedCacheLowest[startCacheSample], noCacheSamples,
	//			   lowpassedBufferHighest, lowpassedBufferLowest, noPixelsNeeded);
		findMaxMin(&lowpassedCacheHighest[startCacheSample], &lowpassedCacheLowest[startCacheSample], noCacheSamples,
				   lowpassedBufferHighest, lowpassedBufferLowest, noPixelsNeeded);
		
		// draw wave
		g.setColour(Colours::red);
		for (int i = 0; i < noPixelsNeeded; i++)
		{
			g.drawLine(x + i, centreY + (lowpassedBufferHighest[i] * scale),
			x + i, centreY + (lowpassedBufferLowest[i] * scale));
		 
			if ((x + i) >= clip.getRight())
			break;
		}
	}
//	delete[] lowpassedBufferHighest;
//	delete[] lowpassedBufferLowest;
	//====================================================================================

	// draw lowpassed wave
/*	g.setColour(Colours::red.withAlpha(0.8f));
	for (int i = 0; i < noCacheSamples; i++)
	{
		g.drawLine(x + i, centreY + (lowpassedCacheHighest[i + startCacheSample] * scale),
				   x + i, centreY + (lowpassedCacheLowest[i + startCacheSample] * scale));
		
		if ((x + i) >= clip.getRight())
			break;
	}
	
	// draw bandpassed wave
	g.setColour(Colours::blue.withAlpha(0.5f));
	for (int i = 0; i < noCacheSamples; i++)
	{
		g.drawLine(x + i, centreY + (bandpassedCacheHighest[i + startCacheSample] * scale),
				   x + i, centreY + (bandpassedCacheLowest[i + startCacheSample] * scale));
		
		if ((x + i) >= clip.getRight())
			break;
	}
	
	// draw highpassed wave
	g.setColour(Colours::green.withBrightness(0.8).withAlpha(0.5f));
	for (int i = 0; i < noCacheSamples; i++)
	{
		g.drawLine(x + i, centreY + (highpassedCacheHighest[i + startCacheSample] * scale),
				   x + i, centreY + (highpassedCacheLowest[i + startCacheSample] * scale));
		
		if ((x + i) >= clip.getRight())
			break;
	}*/
}
