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

#if JUCE_IOS

BEGIN_JUCE_NAMESPACE

//==============================================================================
namespace
{
    const char* const AVAssetAudioFormatName = "AVAsset supported file";

    StringArray findFileExtensionsForCoreAudioCodecs()
    {
        StringArray extensionsArray;
        CFMutableArrayRef extensions = CFArrayCreateMutable (0, 0, 0);
        UInt32 sizeOfArray = sizeof (CFMutableArrayRef);

        if (AudioFileGetGlobalInfo (kAudioFileGlobalInfo_AllExtensions, 0, 0, &sizeOfArray, &extensions) == noErr)
        {
            const CFIndex numValues = CFArrayGetCount (extensions);

            for (CFIndex i = 0; i < numValues; ++i)
                extensionsArray.add ("." + String::fromCFString ((CFStringRef) CFArrayGetValueAtIndex (extensions, i)));
        }

        return extensionsArray;
    }
    
    String nsStringToJuce (NSString* s)
    {
        return CharPointer_UTF8 ([s UTF8String]);
    }
    
    NSString* juceStringToNS (const String& s)
    {
        return [NSString stringWithUTF8String: s.toUTF8()];
    }
}

//==============================================================================
class AVAssetAudioReader : public AudioFormatReader
{
public:
    AVAssetAudioReader (NSURL* assetURL)
        : AudioFormatReader (nullptr, TRANS (AVAssetAudioFormatName)),
          ok (false),
          lastReadPosition (0),
          fifoBuffer (2 * 8196 * 2)
    {
        @autoreleasepool {
            
        usesFloatingPointData = true;

        AVURLAsset* songAsset = [AVURLAsset URLAssetWithURL:assetURL options:nil];
        AVAssetTrack* avAssetTrack = [songAsset.tracks objectAtIndex:0];
        [avAssetTrack retain];
        
        NSError* status = nil;
        assetReader = [AVAssetReader assetReaderWithAsset:songAsset
                                                    error:&status];
        [assetReader retain];

        if (! status)
        {
            // fill in format information
            CMAudioFormatDescriptionRef formatDescription = (CMAudioFormatDescriptionRef)[avAssetTrack.formatDescriptions objectAtIndex:0];
            const AudioStreamBasicDescription* audioDesc = CMAudioFormatDescriptionGetStreamBasicDescription (formatDescription);
            
            numChannels = audioDesc->mChannelsPerFrame;
            bitsPerSample = audioDesc->mBitsPerChannel;
            sampleRate = audioDesc->mSampleRate;
            lengthInSamples = avAssetTrack.timeRange.duration.value;

            NSDictionary* outputSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                                            [NSNumber numberWithInt:kAudioFormatLinearPCM], AVFormatIDKey, 
//                                            [NSNumber numberWithFloat:44100.0], AVSampleRateKey,
//                                            [NSData dataWithBytes:&channelLayout length:sizeof(AudioChannelLayout)], AVChannelLayoutKey,
//                                            [NSNumber numberWithInt:16], AVLinearPCMBitDepthKey,
                                            [NSNumber numberWithBool:NO], AVLinearPCMIsNonInterleaved,
                                            [NSNumber numberWithBool:YES], AVLinearPCMIsFloatKey,
                                            [NSNumber numberWithInt:32], AVLinearPCMBitDepthKey,
//                                            [NSNumber numberWithBool:NO], AVLinearPCMIsBigEndianKey,
                                            nil];
            
            assetReaderOutput = [[AVAssetReaderTrackOutput assetReaderTrackOutputWithTrack:avAssetTrack
                                                                            outputSettings:outputSettings]
                                 retain];

            if ([assetReader canAddOutput:assetReaderOutput])
            {

                [assetReader addOutput:assetReaderOutput];

                startCMTime = CMTimeMake (lastReadPosition, sampleRate);
                playbackCMTimeRange = CMTimeRangeMake (startCMTime, kCMTimePositiveInfinity);
                assetReader.timeRange = playbackCMTimeRange;                
                NSLog(@"%@", CMTimeRangeCopyDescription(kCFAllocatorDefault, assetReader.timeRange));
                DBG("duration: "<<CMTimeGetSeconds(avAssetTrack.timeRange.duration));

                if (! [assetReader startReading])
                {
                    DBG("error starting reading");
                }

                ok = true;
            }
        }}
    }

    ~AVAssetAudioReader()
    {
        DBG("~AVAssetAudioReader()");
        [assetReader release];
        [assetReaderOutput release];
    }

    //==============================================================================
    bool readSamples (int** destSamples, int numDestChannels, int startOffsetInDestBuffer,
                      int64 startSampleInFile, int numSamples)
    {
        @autoreleasepool {
            jassert (destSamples != nullptr);
        jassert (numDestChannels == numChannels);
        
        // check if position has changed
        if (lastReadPosition != startSampleInFile)
        {
            DBG("need to change position");
            [assetReader cancelReading];

            lastReadPosition = startSampleInFile;
//            startCMTime = CMTimeMake (lastReadPosition, sampleRate);
            startCMTime.value = lastReadPosition;
//            playbackCMTimeRange = CMTimeRangeMake (startCMTime, kCMTimePositiveInfinity);
            //assetReader.timeRange = playbackCMTimeRange;
//            fifoBuffer.clear();
            if (! [assetReader startReading])
            {
                DBG("error starting reading");
            }
        }
        
//        for (int i = numDestChannels; --i >= 0;)
//            if (destSamples[i] != nullptr)
//                zeromem (destSamples[i] + startOffsetInDestBuffer, sizeof (int) * numSamples);

        if (fifoBuffer.getNumAvailable() < numSamples)
        {
            CMSampleBufferRef sampleRef = [assetReaderOutput copyNextSampleBuffer];

            if (sampleRef != NULL)
            {
                CMBlockBufferRef bufferRef = CMSampleBufferGetDataBuffer (sampleRef);
                size_t lengthAtOffset;
                size_t totalLength;
                char* dataPointer;
                CMBlockBufferGetDataPointer (bufferRef, 
                                             0, 
                                             &lengthAtOffset, 
                                             &totalLength, 
                                             &dataPointer);
                
                if (bufferRef != NULL)
                {
                    int samplesExpected = CMSampleBufferGetNumSamples (sampleRef);
                          
                    int numSamplesNeeded = fifoBuffer.getNumAvailable() + (samplesExpected * numChannels);
                    if (numSamplesNeeded > fifoBuffer.getSize())
                    {
                        DBG("enlarging fifo");
                        fifoBuffer.setSize (numSamplesNeeded);
                    }
                    
                    fifoBuffer.writeSamples ((float*) dataPointer, samplesExpected * numChannels);
                }
                else
                {
                    DBG("CMBlockBufferRef is NULL");
                }
            }
            else
            {
                DBG("CMSampleBufferRef is NULL");
            }
            
            CFRelease (sampleRef);
        }

        // deinterleave
        float tempBuffer [numChannels * numSamples];
        fifoBuffer.readSamples (tempBuffer, numChannels * numSamples);
        
        float deSamps[numChannels * numSamples];
        float* deinterleavedSamples[numChannels];
        for (int i = 0; i < numChannels; i++)
            deinterleavedSamples[i] = &deSamps[i * numSamples];

        AudioDataConverters::deinterleaveSamples (tempBuffer, deinterleavedSamples,
                                                  numSamples, numChannels);
            
        for (int i = 0; i < numChannels; i++)
            memcpy (destSamples[i] + startOffsetInDestBuffer, deinterleavedSamples[i], sizeof (float) * numSamples);
            
//        for (int c = 0; c < numChannels; c++)
//        {
//            for (int s = 0; s < numSamples * numChannels; s++)
//            {
//                destSamples[c][startOffsetInDestBuffer + s] = tempBuffer[s * numChannels];
//            }
//        }
        
        lastReadPosition += numSamples;
        }
        return true;
    }

    bool ok;

private:
    AVAssetReader* assetReader;
    AVAssetReaderTrackOutput* assetReaderOutput;
    CMTime startCMTime;
    CMTimeRange playbackCMTimeRange;
    int64 lastReadPosition;
//    MemoryBlock audioDataBlock;
//    int numFramesInBlock;
    FifoBuffer fifoBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AVAssetAudioReader);
};

//==============================================================================
AVAssetAudioFormat::AVAssetAudioFormat()
    : AudioFormat (TRANS (AVAssetAudioFormatName), findFileExtensionsForCoreAudioCodecs())
{
}

AVAssetAudioFormat::~AVAssetAudioFormat() {}

Array<int> AVAssetAudioFormat::getPossibleSampleRates()    { return Array<int>(); }
Array<int> AVAssetAudioFormat::getPossibleBitDepths()      { return Array<int>(); }

bool AVAssetAudioFormat::canDoStereo()     { return true; }
bool AVAssetAudioFormat::canDoMono()       { return true; }

//==============================================================================
AudioFormatReader* AVAssetAudioFormat::createReaderFor (String assetNSURLAsString)
{
    NSString* assetNSString = [NSString stringWithUTF8String:assetNSURLAsString.toUTF8()];
    NSURL* assetNSURL = [NSURL URLWithString:assetNSString];
    
    ScopedPointer<AVAssetAudioReader> r (new AVAssetAudioReader (assetNSURL));

    [assetNSString release];
    [assetNSURL release];
    
    if (r->ok)
        return r.release();

    return nullptr;
}

AudioFormatReader* AVAssetAudioFormat::createReaderFor (InputStream* sourceStream,
                                                        bool deleteStreamIfOpeningFails)
{
    jassertfalse;
    /*  Can't read from a stream, has to be from an AVURLAsset compatible string.
        see createReaderFor (String assetNSURLAsString).
     */
    return nullptr;
}

AudioFormatWriter* AVAssetAudioFormat::createWriterFor (OutputStream* streamToWriteTo,
                                                        double sampleRateToUse,
                                                        unsigned int numberOfChannels,
                                                        int bitsPerSample,
                                                        const StringPairArray& metadataValues,
                                                        int qualityOptionIndex)
{
    jassertfalse; // not yet implemented!
    return nullptr;
}

END_JUCE_NAMESPACE

#endif
