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

#if DROWAUDIO_UNIT_TESTS


//==============================================================================
class AudioUtilityUnitTests  : public UnitTest
{
public:
    AudioUtilityUnitTests() : UnitTest ("AudioUtilityUnitTests") {}
    
    void runTest()
    {
        beginTest ("AudioUtilityUnitTests");
        
        
        expectEquals (semitonesToPitchRatio (12.0), 2.0);
        expectEquals (semitonesToPitchRatio (-12.0), 0.5);
        expectEquals (semitonesToPitchRatio (-24.0), 0.25);

        expectEquals (pitchRatioToSemitones (2.0), 12.0);
        expectEquals (pitchRatioToSemitones (0.5), -12.0);
        expectEquals (pitchRatioToSemitones (0.25), -24.0);
    }
};

static AudioUtilityUnitTests audioUtilityUnitTests;

//==============================================================================
class AudioSampleBufferUnitTests  : public UnitTest
{
public:
    AudioSampleBufferUnitTests() : UnitTest ("AudioSampleBufferUnitTests") {}
    
    void runTest()
    {
        beginTest ("AudioSampleBufferUnitTests");
        
        {
            AudioSampleBuffer buffer (1, 512);
            expect (isAudioSampleBuffer (buffer.getArrayOfChannels(), getNumBytesForAudioSampleBuffer (buffer)));
        }

        {
            AudioSampleBuffer buffer (6, 1024);
            expect (isAudioSampleBuffer (buffer.getArrayOfChannels(), getNumBytesForAudioSampleBuffer (buffer)));
        }
        
        {
            AudioSampleBuffer buffer (3, 256);

            MemoryInputStream stream (buffer.getArrayOfChannels(), getNumBytesForAudioSampleBuffer (buffer), false);
            unsigned int numChannels;
            int64 numSamples;
            expect (isAudioSampleBuffer (stream, numChannels, numSamples));
            expectEquals ((int) numChannels, 3);
            expectEquals ((int) numSamples, 256);
        }
    }
};

static AudioSampleBufferUnitTests audioSampleBufferUnitTests;

//==============================================================================


#endif // DROWAUDIO_UNIT_TESTS