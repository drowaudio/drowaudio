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

#ifndef __DROWAUDIO_AUDIOTHUMBNAILIMAGE_H__
#define __DROWAUDIO_AUDIOTHUMBNAILIMAGE_H__

#include "../../audio/dRowAudio_AudioFilePlayer.h"

//==============================================================================	
/** A class to display the waveform of an audio file.
	
	This will load an audio file and display its waveform. All waveform rendering
    happens on a background thread. This will listen to changes in the
    AudioFilePlayer passed in and update the thumbnail accordingly.
    
    You can either get the whole image using getImage() or you can get a scaled
    section using getImageAtTime().
 
    You can also register as a listener to recive update when the sourc changes
    or new data has been generated.
 */
class AudioThumbnailImage : public Timer,
                            public TimeSliceClient,
                            public AudioFilePlayer::Listener
{
public:
    //==============================================================================	
    /** Creates the AudioThumbnailImage.
		The file player associated with the display must be passed in.
		To save on the number of threads in your program you can optionally pass in your own
		AudioThumbnailCache. If you pass in your own the caller is responsible for deleting it,
		if not the PositionableWaveform will create and delete its own when not needed.	 
	 */
	explicit AudioThumbnailImage (AudioFilePlayer* sourceToBeUsed,
                                  TimeSliceThread& backgroundThread,
                                  AudioThumbnailCache* cacheToUse = nullptr,
                                  AudioThumbnailBase* thumbnailToUse = nullptr,
                                  int sourceSamplesPerThumbnailSample = 512);
	
	/** Destructor. */
	~AudioThumbnailImage ();
	
    /** Sets the colour to use for the background.
     */
    void setBackgroundColour (Colour newBackgroundColour);
    
    /** Sets the colour to use for the waveform.
     */
    void setWaveformColour (Colour newWaveformColour);
    
	//====================================================================================
    /** Returns the whole waveform image.
     */
    const Image getImage()                          {   return waveformImage;   }
    
    /** Returns a section of the image at a given time for a given duration.
     */
    const Image getImageAtTime (double startTime, double duration);
    
    /** Sets the image resolution in lines per pixel.
        This will cause the waveform to be re-generated from the source.
     */
    void setResolution (double newResolution);
    
    /** Returns the AudioFilePlayer currently being used.
     */
    AudioFilePlayer* getAudioFilePlayer()           {   return filePlayer;      }
    
    /** Retuns the ammount of time that has been rendered.
     */
    double getTimeRendered()                        {   return lastTimeDrawn;   }
    
    /** Returns true if the Image has finished rendering;
     */
    bool hasFinishedLoading()                       {   return renderComplete;  }
    
    /** Returns the number of sourceSamplesPerThumbnailSample if set in the constructor.
     */
    int getNumSourceSamplesPerThumbnailSamples()    {   return sourceSamplesPerThumbnailSample; }
    
	//====================================================================================
	/** @internal */
    void resized ();
	
	/** @internal */
	void paint (Graphics &g);

	//====================================================================================
	/** @internal */
	void timerCallback ();
    
	/** @internal */
    int useTimeSlice();
    
	/** @internal */
	void fileChanged (AudioFilePlayer *player);
    
    //==============================================================================
    /** A class for receiving callbacks from an AudioThumbnailImage.
        These are called from the internal buffering thread so be sure to lock the
        message manager if you intend to do any graphical related stuff with the Image.
	 
        @see AudioThumbnailImage::addListener, AudioThumbnailImage::removeListener
	 */
    class  Listener
    {
    public:
        //==============================================================================
        /** Destructor. */
        virtual ~Listener() {}
		
        //==============================================================================
        /** Called when the source file changes and the image starts rendering.
		 */
        virtual void imageChanged (AudioThumbnailImage* /*audioThumbnailImage*/) {}
		
        /** Called when the the image is updated.
            This will be continuously called while thewaveform is being generated.
		 */
        virtual void imageUpdated (AudioThumbnailImage* /*audioThumbnailImage*/) {}
        
        /** Called when the the image has finished rendering.
            If you are using a scaled version of the Image it might be worth caching 
            your own copy to avoid having to rescale it each time.
         */
        virtual void imageFinished (AudioThumbnailImage* /*audioThumbnailImage*/) {}
    };
	
    /** Adds a listener to be notified of changes to the AudioThumbnailImage. */
    void addListener (Listener* listener);
	
    /** Removes a previously-registered listener. */
    void removeListener (Listener* listener);
	    
private:
	//==============================================================================
    void triggerWaveformRefresh();
	void refreshWaveform();
	
	AudioFilePlayer* filePlayer;
	double fileLength, oneOverFileLength, currentSampleRate, oneOverSampleRate;
	
	// thumbnail classes
    ReadWriteLock imageLock;
    TimeSliceThread& backgroundThread;
	OptionalScopedPointer<AudioThumbnailCache> audioThumbnailCache;
	OptionalScopedPointer<AudioThumbnailBase> audioThumbnail;
    int sourceSamplesPerThumbnailSample;
	
	Image waveformImage, tempSectionImage;
    Colour backgroundColour;
    Colour waveformColour;
    
    double lastTimeDrawn, resolution;
    bool renderComplete;
    
    ListenerList <Listener> listeners;

	//==============================================================================	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioThumbnailImage);
};

#endif  // __DROWAUDIO_AUDIOTHUMBNAILIMAGE_H__
