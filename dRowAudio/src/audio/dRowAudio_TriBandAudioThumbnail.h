/*
 *  dRowAudio_TriBandAudioThumbnail.h
 *  dRowAudio
 *
 *  Created by David Rowland on 11/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _TRIBANDAUDIOTHUMBNAIL_H_
#define _TRIBANDAUDIOTHUMBNAIL_H_

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>

class AudioFilePlayer;

class TriBandAudioThumbnail : public Timer,
							  public ChangeBroadcaster
{
public:
	TriBandAudioThumbnail (const int sourceSamplesPerThumbnailSample,
						   AudioFormatManager& formatManagerToUse);
	
    /** Destructor. */
    ~TriBandAudioThumbnail ();
	
    //==============================================================================
    /** Specifies the file or stream that contains the audio file.
	 
	 For a file, just call
	 @code
	 setSource (new FileInputSource (file))
	 @endcode
	 
	 You can pass a zero in here to clear the thumbnail.
	 
	 The source that is passed in will be deleted by this object when it is no
	 longer needed
	 */
    void setSource (InputSource* const newSource);
	
	void setSource (AudioFilePlayer* const newFilePlayer);

	//==============================================================================
    /** Returns the number of channels in the file.
	 */
    int getNumChannels() const throw();
	
    /** Returns the length of the audio file.
	 */
    double getTotalLength() const throw();
		
    /** Renders the waveform shape for a channel.
	 
	 The waveform will be drawn within  the specified rectangle, where startTime
	 and endTime specify the times within the audio file that should be positioned
	 at the left and right edges of the rectangle.
	 
	 The waveform will be scaled vertically so that a full-volume sample will fill
	 the rectangle vertically, but you can also specify an extra vertical scale factor
	 with the verticalZoomFactor parameter.
	 */
    void drawChannel (Graphics& g,
                      int x, int y, int w, int h,
                      double startTime,
                      double endTime,
                      int channelNum,
                      const float verticalZoomFactor);
	
    /** Returns true if the low res preview is fully generated.
	 */
    bool isFullyLoaded() const throw();
	
    /** @internal */
	bool buildWaveform();
	
    //==============================================================================
    /** @internal */
    void timerCallback();
	
    //==============================================================================
	
	
private:
	AudioFormatManager& formatManagerToUse;
	InputSource* source;
	CriticalSection readerLock;
	AudioFormatReader* reader;
	
	AudioFormatReader* createReader() const;
	bool initialiseFromAudioFile (AudioFormatReader& reader);


	int orginalSamplesPerThumbnailSample, noChannels, noSourceSamples, noCacheSamples;
	double sampleRate;
	int noSamplesCached;
	
	float **sourceData, *sourceDataMixedMono;
	float *lowResCacheHighest, *lowResCacheLowest;
	float *lowpassedCacheHighest, *lowpassedCacheLowest;
	float *bandpassedCacheHighest, *bandpassedCacheLowest;
	float *highpassedCacheHighest, *highpassedCacheLowest;

	BiquadFilter filter;
	
	/// Sets up the filters to be used
	void setupFilters ();
	void buildLowpassedWave(MemoryBlock originalWaveform);
	void buildBandpassedWave(MemoryBlock originalWaveform);
	void buildHighpassedWave(MemoryBlock originalWaveform);
	
	void findMaxMin(float* sourceStartSampleHighest, float* sourceStartSampleLowest, int sourceNumSamples,
					float* destBufferHighest, float* destBufferLowest, int destBufferSize);
};

#endif //_TRIBANDAUDIOTHUMBNAIL_H_