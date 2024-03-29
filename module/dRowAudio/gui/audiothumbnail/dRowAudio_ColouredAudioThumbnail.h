/*
  ==============================================================================

  This file is part of the dRowAudio JUCE module
  Copyright 2004-13 by dRowAudio.

  ------------------------------------------------------------------------------

  dRowAudio is provided under the terms of The MIT License (MIT):

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ==============================================================================
*/

#ifndef DROWAUDIO_COLOUREDAUDIOTHUMBNAIL_H
#define DROWAUDIO_COLOUREDAUDIOTHUMBNAIL_H

//==============================================================================
/**
    Makes it easy to quickly draw scaled views of the waveform shape of an
    audio file.

    To use this class, just create an AudioThumbNail class for the file you want
    to draw, call setSource to tell it which file or resource to use, then call
    drawChannel() to draw it.

    The class will asynchronously scan the wavefile to create its scaled-down view,
    so you should make your UI repaint itself as this data comes in. To do this, the
    AudioThumbnail is a ChangeBroadcaster, and will broadcast a message when its
    listeners should repaint themselves.

    The thumbnail stores an internal low-res version of the wave data, and this can
    be loaded and saved to avoid having to scan the file again.

    This version can draw multi coloured waveforms based on the frequency content
    of the wave, red being low, green mid and blue high. use drawColouredChannel
    to access this mode.

    @see AudioThumbnailCache
*/
class ColouredAudioThumbnail : public juce::AudioThumbnailBase
{
public:
    //==============================================================================
    /** Creates an audio thumbnail.

        @param sourceSamplesPerThumbnailSample  when creating a stored, low-res version
                        of the audio data, this is the scale at which it should be done. (This
                        number is the number of original samples that will be averaged for each
                        low-res sample)
        @param formatManagerToUse   the audio format manager that is used to open the file
        @param cacheToUse   an instance of an AudioThumbnailCache - this provides a background
                            thread and storage that is used to by the thumbnail, and the cache
                            object can be shared between multiple thumbnails
    */
    ColouredAudioThumbnail (int sourceSamplesPerThumbnailSample,
                            juce::AudioFormatManager& formatManagerToUse,
                            juce::AudioThumbnailCache& cacheToUse);

    /** Destructor. */
    ~ColouredAudioThumbnail();

    //==============================================================================
    /** Clears and resets the thumbnail. */
    void clear();

    /** Specifies the file or stream that contains the audio file.

        For a file, just call
        @code
        setSource (new FileInputSource (file))
        @endcode

        You can pass a zero in here to clear the thumbnail.
        The source that is passed in will be deleted by this object when it is no longer needed.
        @returns true if the source could be opened as a valid audio file, false if this failed for
        some reason.
    */
    bool setSource (juce::InputSource* newSource);

    /** Gives the thumbnail an AudioFormatReader to use directly.
        This will start parsing the audio in a background thread (unless the hash code
        can be looked-up successfully in the thumbnail cache). Note that the reader
        object will be held by the thumbnail and deleted later when no longer needed.
        The thumbnail will actually keep hold of this reader until you clear the thumbnail
        or change the input source, so the file will be held open for all this time. If
        you don't want the thumbnail to keep a file handle open continuously, you
        should use the setSource() method instead, which will only open the file when
        it needs to.
    */
    void setReader (juce::AudioFormatReader* newReader, juce::int64 hashCode);

    /** Resets the thumbnail, ready for adding data with the specified format.
        If you're going to generate a thumbnail yourself, call this before using addBlock()
        to add the data.
    */
    void reset (int numChannels, double sampleRate, juce::int64 totalSamplesInSource = 0);

    /** Adds a block of level data to the thumbnail.
        Call reset() before using this, to tell the thumbnail about the data format.
    */
    void addBlock (juce::int64 sampleNumberInSource, const juce::AudioSampleBuffer& newData,
                   int startOffsetInBuffer, int numSamples);

    //==============================================================================
    /** Reloads the low res thumbnail data from an input stream.

        This is not an audio file stream! It takes a stream of thumbnail data that would
        previously have been created by the saveTo() method.
        @see saveTo
    */
    bool loadFrom (juce::InputStream& input);

    /** Saves the low res thumbnail data to an output stream.

        The data that is written can later be reloaded using loadFrom().
        @see loadFrom
    */
    void saveTo (juce::OutputStream& output) const;

    //==============================================================================
    /** Returns the number of channels in the file. */
    int getNumChannels() const noexcept;

    /** Returns the length of the audio file, in seconds. */
    double getTotalLength() const noexcept;

    /** Draws the waveform for a channel.

        The waveform will be drawn within  the specified rectangle, where startTime
        and endTime specify the times within the audio file that should be positioned
        at the left and right edges of the rectangle.

        The waveform will be scaled vertically so that a full-volume sample will fill
        the rectangle vertically, but you can also specify an extra vertical scale factor
        with the verticalZoomFactor parameter.
    */
    void drawChannel (juce::Graphics& g,
                      const juce::Rectangle<int>& area,
                      double startTimeSeconds,
                      double endTimeSeconds,
                      int channelNum,
                      float verticalZoomFactor);

    /**    Draws the waveform for a channel with colour coded frequency information.
     */
    void drawColouredChannel (juce::Graphics& g, const juce::Rectangle<int>& area, double startTime,
                              double endTime, int channelNum, float verticalZoomFactor);

    /** Draws the waveforms for all channels in the thumbnail.

        This will call drawChannel() to render each of the thumbnail's channels, stacked
        above each other within the specified area.

        @see drawChannel
    */
    void drawChannels (juce::Graphics& g,
                       const juce::Rectangle<int>& area,
                       double startTimeSeconds,
                       double endTimeSeconds,
                       float verticalZoomFactor);

    /** Returns true if the low res preview is fully generated. */
    bool isFullyLoaded() const noexcept;

    /** Returns the number of samples that have been set in the thumbnail. */
    juce::int64 getNumSamplesFinished() const noexcept;

    /** Returns the number of samples per thumbnail sample being used.
     */
    //int32 getNumSamplesPerThumbnailSample()     {   return samplesPerThumbSample;   }

    /** Returns the highest level in the thumbnail.
        Note that because the thumb only stores low-resolution data, this isn't
        an accurate representation of the highest value, it's only a rough approximation.
    */
    float getApproximatePeak() const;

    /** Reads the approximate min and max levels from a section of the thumbnail.
        The lowest and highest samples are returned in minValue and maxValue, but obviously
        because the thumb only stores low-resolution data, these numbers will only be a rough
        approximation of the true values.
    */
    void getApproximateMinMax (double startTime, double endTime, int channelIndex,
                               float& minValue, float& maxValue) const noexcept;

    /** Returns the hash code that was set by setSource() or setReader(). */
    juce::int64 getHashCode() const;

private:
    //==============================================================================
    juce::AudioFormatManager& formatManagerToUse;
    juce::AudioThumbnailCache& cache;

    class LevelDataSource;
    struct MinMaxColourValue;
    class ThumbData;
    class CachedWindow;

    friend class LevelDataSource;
    friend class std::unique_ptr<LevelDataSource>;
    friend class ThumbData;
    friend class juce::OwnedArray<ThumbData>;
    friend class CachedWindow;
    friend class std::unique_ptr<CachedWindow>;

    std::unique_ptr<LevelDataSource> source;
    std::unique_ptr<CachedWindow> window;
    juce::OwnedArray<ThumbData> channels;

    juce::int32 samplesPerThumbSample;
    juce::int64 totalSamples, numSamplesFinished;
    juce::int32 numChannels;
    double sampleRate;
    juce::CriticalSection lock;

    //==============================================================================
    bool setDataSource (LevelDataSource* newSource);
    void setLevels (const MinMaxColourValue* const* values, int thumbIndex, int numChans, int numValues);
    void createChannels (int length);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColouredAudioThumbnail)
};


#endif   //DROWAUDIO_COLOUREDAUDIOTHUMBNAIL_H
