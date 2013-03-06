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



//==============================================================================
namespace
{
    const char* const audioSampleBufferAudioFormatName = "AudioSampleBuffer format stream";
}

//==============================================================================
class AudioSampleBufferReader : public AudioFormatReader
{
public:
    AudioSampleBufferReader (InputStream* const inp)
    : AudioFormatReader (inp, TRANS (audioSampleBufferAudioFormatName)),
    ok (false)
    {
        usesFloatingPointData = true;
        
        if (inp != nullptr)
        {
            ok = isAudioSampleBuffer (*inp, numChannels, lengthInSamples);
            sampleRate = 44100;
            bitsPerSample = 32;
        }
    }
    
    ~AudioSampleBufferReader()
    {
    }
    
    //==============================================================================
    bool readSamples (int** destSamples, int numDestChannels, int startOffsetInDestBuffer,
                      int64 startSampleInFile, int numSamples)
    {
        jassert (destSamples != nullptr);
        const int64 samplesAvailable = lengthInSamples - startSampleInFile;
        
        if (samplesAvailable < numSamples)
        {
            for (int i = numDestChannels; --i >= 0;)
                if (destSamples[i] != nullptr)
                    zeromem (destSamples[i] + startOffsetInDestBuffer, sizeof (int) * numSamples);
            
            numSamples = (int) samplesAvailable;
        }
        
        if (numSamples <= 0)
            return true;
        
        while (numSamples > 0)
        {
            const int numThisTime = jmin (8192, numSamples);
            const int numBytes = numThisTime * sizeof (float);
            
            for (int c = (int) numChannels; --c >= 0;)
            {
                const int64 pos = sampleToReadPosition (c, startSampleInFile);
                input->setPosition (pos);
                
                if (destSamples[c] != nullptr)
                {
                    if (c < (int) numChannels)
                        input->read (destSamples[c] + startOffsetInDestBuffer, numBytes);
                    else
                        zeromem (destSamples[c] + startOffsetInDestBuffer, numBytes);
                }
            }
            
            startOffsetInDestBuffer += numThisTime;
            numSamples -= numThisTime;
        }
        
        return true;
    }
    
    bool ok;
    
private:
    int64 sampleToReadPosition (int channel, int64 samplePosition)
    {
        const size_t startPosition = (numChannels + 1) * sizeof (float*);
        const int64 channelStartByte = startPosition + (channel * lengthInSamples * sizeof (float));
        const int64 sampleStartByte = channelStartByte + (samplePosition * sizeof (float));
        
        return sampleStartByte;
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSampleBufferReader);
};

//==============================================================================
AudioSampleBufferAudioFormat::AudioSampleBufferAudioFormat()
: AudioFormat (TRANS (audioSampleBufferAudioFormatName), StringArray())
{
}

AudioSampleBufferAudioFormat::~AudioSampleBufferAudioFormat() {}

Array<int> AudioSampleBufferAudioFormat::getPossibleSampleRates()    { return Array<int>(); }
Array<int> AudioSampleBufferAudioFormat::getPossibleBitDepths()      { return Array<int>(); }

bool AudioSampleBufferAudioFormat::canDoStereo()     { return true; }
bool AudioSampleBufferAudioFormat::canDoMono()       { return true; }

//==============================================================================
AudioFormatReader* AudioSampleBufferAudioFormat::createReaderFor (InputStream* sourceStream,
                                                                  bool deleteStreamIfOpeningFails)
{
    ScopedPointer<AudioSampleBufferReader> r (new AudioSampleBufferReader (sourceStream));
    
    if (r->ok)
        return r.release();
    
    if (! deleteStreamIfOpeningFails)
        r->input = nullptr;
    
    return nullptr;
}

AudioFormatWriter* AudioSampleBufferAudioFormat::createWriterFor (OutputStream* /*streamToWriteTo*/,
                                                                  double /*sampleRateToUse*/,
                                                                  unsigned int /*numberOfChannels*/,
                                                                  int /*bitsPerSample*/,
                                                                  const StringPairArray& /*metadataValues*/,
                                                                  int /*qualityOptionIndex*/)
{
    jassertfalse; // not yet implemented!
    return nullptr;
}

