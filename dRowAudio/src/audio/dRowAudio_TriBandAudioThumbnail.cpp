/*
 *  dRowAudio_TriBandAudioThumbnail.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 11/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
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
    noSamplesCached = 0;
	
	noCacheSamples = (int)(noSourceSamples / orginalSamplesPerThumbnailSample);
	DBG(String("No cache samples = ")<<noCacheSamples);
	
    return noSourceSamples > 0;
}

bool TriBandAudioThumbnail::buildWaveform ()
{
	MemoryBlock initialData(noSourceSamples * sizeof(int));
	int *initialDataPointer = (int*)initialData.getData();
	int **sourceDataOriginal = new int*[2];
	sourceDataOriginal[0] = initialDataPointer;
	sourceDataOriginal[1] = 0;
	

	MemoryBlock data(noSourceSamples * sizeof(float));	
	float *sourceDataMixedMono = (float*)data.getData();

	sourceData = new float*[2];
	sourceData[0] = sourceDataMixedMono;
	sourceData[1] = 0;
	
	// reads the file into the array
	reader->read((int**)sourceDataOriginal,
				 0,
				 noSourceSamples);
	
	// convert data to float
	unsigned long int max = 0;
	for (int i = 0; i < noSourceSamples; i++)
	{
		if (abs(initialDataPointer[i]) > max)
			max = abs(initialDataPointer[i]);
	}
	double oneOverMax = 1.0 / max;
	for (int i = 0; i < noSourceSamples; i++)
	{
		sourceDataMixedMono[i] = initialDataPointer[i] * oneOverMax;
	}

	
	buildLowpassedWave(data);
	buildBandpassedWave(data);
	buildHighpassedWave(data);
	
	
	//=============================================================
	lowResCacheHighest = new float[noCacheSamples];
	lowResCacheLowest = new float[noCacheSamples];

	for (int i = 0; i < noCacheSamples; i++)
	{
		const int sourceStart = i * orginalSamplesPerThumbnailSample;
		const int sourceEnd = sourceStart + orginalSamplesPerThumbnailSample;		
		
//		float lowestLeft, highestLeft, lowestRight, highestRight;
//		
//		reader->readMaxLevels (sourceStart,
//							   sourceEnd - sourceStart,
//							   lowestLeft,
//							   highestLeft,
//							   lowestRight,
//							   highestRight);
		
		float lowest = 0.0f, highest = 0.0f;
		
		for ( int c = sourceStart; c < sourceEnd; c++)
		{
			if ( sourceDataMixedMono[c] < lowest )
				lowest = sourceDataMixedMono[c];
			else if ( sourceDataMixedMono[c] > highest )
				highest = sourceDataMixedMono[c];
		}
		lowResCacheHighest[i] = highest;
		lowResCacheLowest[i] = lowest;
	}
	
	return true;
}

void TriBandAudioThumbnail::buildLowpassedWave(MemoryBlock sourceData)
{
	float *originalWaveform = (float*)sourceData.getData();

	// filter the copy of the source data
	filter.makeLowPass(reader->sampleRate, 100);
	filter.reset();
	filter.processSamples(originalWaveform, sourceData.getSize()/sizeof(float));

	lowpassedCacheHighest = new float[noCacheSamples];
	lowpassedCacheLowest = new float[noCacheSamples];
	
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
		
		lowpassedCacheHighest[i] = highest;
		lowpassedCacheLowest[i] = lowest;
	}
}

void TriBandAudioThumbnail::buildBandpassedWave(MemoryBlock sourceData)
{
	float *originalWaveform = (float*)sourceData.getData();
	
	// filter the copy of the source data
	filter.makeBandPass(reader->sampleRate, 2000, 4);
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
	filter.makeHighPass(reader->sampleRate, 3500);
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
	int centreY = y + (h * 0.5f);
	float scale = h * 0.5f;
	
	const Rectangle clip (g.getClipBounds());
//	g.setColour(Colours::green);
//	for (int i = 0; i < noCacheSamples; i++)
//	{
//		g.drawLine(x + i, centreY + (lowResCacheHighest[i] * scale),
//				   x + i, centreY + (lowResCacheLowest[i] * scale));
//		
//		if ((x + i) >= clip.getRight())
//			break;
//	}
	
	// draw lowpassed wave
	g.setColour(Colours::red);
	for (int i = 0; i < noCacheSamples; i++)
	{
		g.drawLine(x + i, centreY + (lowpassedCacheHighest[i] * scale),
				   x + i, centreY + (lowpassedCacheLowest[i] * scale));
		
		if ((x + i) >= clip.getRight())
			break;
	}
	
	// draw bandpassed wave
	g.setColour(Colours::blue);
	for (int i = 0; i < noCacheSamples; i++)
	{
		g.drawLine(x + i, centreY + (bandpassedCacheHighest[i] * scale),
				   x + i, centreY + (bandpassedCacheLowest[i] * scale));
		
		if ((x + i) >= clip.getRight())
			break;
	}
	
	// draw highpassed wave
	g.setColour(Colours::green.withBrightness(0.8));
	for (int i = 0; i < noCacheSamples; i++)
	{
		g.drawLine(x + i, centreY + (highpassedCacheHighest[i] * scale),
				   x + i, centreY + (highpassedCacheLowest[i] * scale));
		
		if ((x + i) >= clip.getRight())
			break;
	}
}
