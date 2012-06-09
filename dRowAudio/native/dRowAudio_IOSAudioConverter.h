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

#ifndef __DROWAUDIO_IOSAUDIOCONVERTER__
#define __DROWAUDIO_IOSAUDIOCONVERTER__

#if JUCE_IOS || DOXYGEN

//==============================================================================
/**
    This class converts an iPod Library track to a PCM Wav format suitable for
    reading using a CoreAudioFormatReader.
    
    Because of iOS app sandboxing, we don't have direct access to the source
    file. This class will take an AVAssetUrl string and convert the source to an
    application-local file which can be freely modified.
 
    The easiest way to use this is to show an AudioPicker and convert a returned
    item to an AVAssetUrl using AudioPicker::mpMediaItemToAvassetUrl and pass
    this to the start conversion method. Either then continuosly check the
    progress and get the converted file or register youself as a Listener to be
    updated about the conversion process.
 
    @see AudioPicker, AudioPicker::mpMediaItemToAvassetUrl
 */
class IOSAudioConverter
{
public:
    //==============================================================================
    /** Creates an IOSAudioConverter.
     
        Initially this does nothing, use the startConversion method to convert a
        file.
     */
    IOSAudioConverter();
    
    /** Destructor.
     */
    ~IOSAudioConverter();

    /** Starts the conversion process.
     
        Register yourself as a listener to be notified of updates during the
        conversion process.

        @param  avAssetUrl          The AVAssetUrl in String format to convert. This
                                    is usually obtained from an AudioPicker and its
                                    AudioPicker::mpMediaItemToAvassetUrl method.
        @param  convertedFileName   A name for the converted file. This will have
                                    the extension .caf and if left blank will be
                                    called "convertedFile".
     */
    void startConversion (const String& avAssetUrl, const String& convertedFileName = String::empty);
    
    /** Cancels the current conversion.
        A file may be partially converted and can be obtained using the
        getConvertedFile method.
     */
    void cancel();
    
    /** Returns the most recently converted file.
     */
    File getConvertedFile()     {   return convertedFile;   }
    
    /** Returns the progress of the current conversion.
        @returns The current progress, a value between 0 and 1.
     */
    double getProgress()        {   return progress;    }
    
    //==============================================================================
    /** A class for receiving callbacks from a IOSAudioConverter.
     
        To be notified about updates during the conversion process, you can register
        an IOSAudioConverter::Listener object using IOSAudioConverter::addListener().
     
        @see IOSAudioConverter::addListener, IOSAudioConverter::removeListener
     */
    class JUCE_API  Listener
    {
    public:
        /** Destructor. */
        virtual ~Listener() {}
        
        //==============================================================================
        /** Called when a conversion has been started using the startConversion method.
         */
        virtual void conversionStarted() {}
        
        /** Called repeatedly during the conversion process.
         
            @param progress The current progress between 1 and 0.
         */
        virtual void conversionUpdated (double progress) {}

        /** Called when the conversion process has finished.
            
            @param convertedFile    The fully converted .caf file.
         */
        virtual void conversionFinished (const File& convertedFile)  {}
    };
    
    /**	Description
     
        @see removeListener
     */
    void addListener (Listener* newListener);
    
    /**	Description
     
        @see addListener
     */
    void removeListener (Listener* listener);

    //==============================================================================
    /** @internal */
    void sendConverstionStartedMessage (void* juceIOSAudioConverter);
    
    /** @internal */
    void sendConverstionUpdatedMessage (void* juceIOSAudioConverter);

    /** @internal */
    void sendConverstionFinishedMessage (void* juceIOSAudioConverter, void* convertedUrl);

private:
    //==============================================================================
    ListenerList<Listener> listeners;
    
    File convertedFile;
    double progress;

    void* currentAudioConverter;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IOSAudioConverter);
};


#endif
#endif // __DROWAUDIO_IOSAUDIOCONVERTER__