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

#ifndef __DROWAUDIO_AUDIOSAMPLEBUFFERAUDIOFORMAT_H__
#define __DROWAUDIO_AUDIOSAMPLEBUFFERAUDIOFORMAT_H__

//==============================================================================
/**
    Streams data from an AudioSampleBuffer.
 
    This reads from a stream that has been initialised from the AudioSampleBuffer
    method getArrayOfChannels(). The AudioSampleBuffer needs to stay in exisistance
    for the duration of this object and not be changed as the stream is unique to
    the memory layout of the buffer.
 
    @see AudioFormat
 */
class AudioSampleBufferAudioFormat :    public AudioFormat
{
public:
    //==============================================================================
    /** Creates a format object. */
    AudioSampleBufferAudioFormat();
    
    /** Destructor. */
    ~AudioSampleBufferAudioFormat();
    
    //==============================================================================
    Array<int> getPossibleSampleRates();
    Array<int> getPossibleBitDepths();
    bool canDoStereo();
    bool canDoMono();
    
    //==============================================================================
    AudioFormatReader* createReaderFor (InputStream* sourceStream,
                                        bool deleteStreamIfOpeningFails);
    
    AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
                                        double sampleRateToUse,
                                        unsigned int numberOfChannels,
                                        int bitsPerSample,
                                        const StringPairArray& metadataValues,
                                        int qualityOptionIndex);
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSampleBufferAudioFormat);
};


#endif // __DROWAUDIO_AUDIOSAMPLEBUFFERAUDIOFORMAT_H__