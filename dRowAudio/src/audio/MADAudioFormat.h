#pragma once

#include <juce/juce.h>

/**
	This AudioFormat enables you to read and playback MP3 files using the MAD decoder.
	You must include _libmad_.c in you program to compile the MAD library.
 */
class MADAudioFormat : public AudioFormat
{
public:
	MADAudioFormat();
    ~MADAudioFormat();

	bool canHandleFile(const File& fileToTest);

    const Array<int> getPossibleSampleRates();
    const Array<int> getPossibleBitDepths();

    bool canDoStereo();
    bool canDoMono();
    bool isCompressed();

	void start();
	void stop();

    AudioFormatReader* createReaderFor(InputStream* sourceStream,
                                       const bool deleteStreamIfOpeningFails);


    AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
                                        double sampleRateToUse,
										unsigned int numberOfChannels,
										int bitsPerSample,
										const StringPairArray& metadataValues,
										int qualityOptionIndex);
};
