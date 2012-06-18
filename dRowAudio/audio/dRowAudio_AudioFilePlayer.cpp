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



AudioFilePlayer::AudioFilePlayer()
    : bufferingTimeSliceThread ("Shared Buffering Thread"),
      formatManager (new AudioFormatManager(), true),
      currentMemoryBlock (nullptr)
{
    bufferingTimeSliceThread.startThread (3);
    
    audioTransportSource = new AudioTransportSource();
    audioTransportSource->addChangeListener (this);
    masterSource = audioTransportSource;
    
	formatManager->registerBasicFormats();
    //formatManager->registerFormat (new AudioSampleBufferAudioFormat(), false);
}

AudioFilePlayer::~AudioFilePlayer()
{
	audioTransportSource->setSource (nullptr);
    audioTransportSource->removeChangeListener (this);
}

//==============================================================================
void AudioFilePlayer::start()
{
    audioTransportSource->start();

    listeners.call (&Listener::playerStoppedOrStarted, this);
}

void AudioFilePlayer::stop()
{
    audioTransportSource->stop();
    
    listeners.call (&Listener::playerStoppedOrStarted, this);
}

void AudioFilePlayer::startFromZero()
{
	if (audioFormatReaderSource == nullptr)
        return;
	
	audioTransportSource->setPosition (0.0);
	audioTransportSource->start();
    
    listeners.call (&Listener::playerStoppedOrStarted, this);
}

void AudioFilePlayer::pause()
{
	if (audioTransportSource->isPlaying())
		audioTransportSource->stop();
	else
		audioTransportSource->start();
    
    listeners.call (&Listener::playerStoppedOrStarted, this);
}

void AudioFilePlayer::setPosition (double newPosition, bool /*ignoreAnyLoopPoints*/)
{
    audioTransportSource->setPosition (newPosition);
}

//==============================================================================
bool AudioFilePlayer::setFile (const File& newFile)
{
    currentMemoryBlock = nullptr;
    memoryInputStream = nullptr;
    
    file = newFile;
    
    return setSourceWithReader (formatManager->createReaderFor (file));
}

bool AudioFilePlayer::setMemoryInputStream (MemoryInputStream* newMemoryInputStream)
{
    file = File::nonexistent;
    memoryInputStream = newMemoryInputStream;
    
    return setSourceWithReader (formatManager->createReaderFor (memoryInputStream));
}

bool AudioFilePlayer::setMemoryBlock (MemoryBlock* inputBlock)
{
    file = File::nonexistent;
    currentMemoryBlock = inputBlock;
    
    if (currentMemoryBlock != nullptr)
        memoryInputStream = new MemoryInputStream (*currentMemoryBlock, false);
    else 
        memoryInputStream = nullptr;
    
    return setMemoryInputStream (memoryInputStream);
}

MemoryInputStream* AudioFilePlayer::getInputStream()
{   
    if (memoryInputStream != nullptr)
        return new MemoryInputStream (memoryInputStream->getData(), memoryInputStream->getDataSize(), false);
    else if (currentMemoryBlock != nullptr)
        return new MemoryInputStream (*currentMemoryBlock, false);

    return nullptr;
}

bool AudioFilePlayer::sourceIsMemoryBlock()
{
    return file == File::nonexistent;
}

void AudioFilePlayer::setAudioFormatManager (AudioFormatManager* newManager,  bool deleteWhenNotNeeded)
{
    OptionalScopedPointer<AudioFormatManager> newFormatManager (newManager, deleteWhenNotNeeded);
	formatManager = newFormatManager;
}

//==============================================================================
void AudioFilePlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    if (masterSource != nullptr)
        masterSource->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void AudioFilePlayer::releaseResources()
{
    if (masterSource != nullptr)
        masterSource->releaseResources();
}

void AudioFilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    if (masterSource != nullptr)
        masterSource->getNextAudioBlock (bufferToFill);
}

void AudioFilePlayer::setLooping (bool shouldLoop)
{   
    if (audioFormatReaderSource != nullptr)
        audioFormatReaderSource->setLooping (shouldLoop); 
}

void AudioFilePlayer::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == audioTransportSource)
        listeners.call (&Listener::playerStoppedOrStarted, this);
}

//==============================================================================
void AudioFilePlayer::addListener (AudioFilePlayer::Listener* const listener)
{
    listeners.add (listener);
}

void AudioFilePlayer::removeListener (AudioFilePlayer::Listener* const listener)
{
    listeners.remove (listener);
}

//==============================================================================
bool AudioFilePlayer::setSourceWithReader (AudioFormatReader* reader)
{
    bool shouldBeLooping = isLooping();
	audioTransportSource->setSource (nullptr);

	if (reader != nullptr)
	{										
		// we SHOULD let the AudioFormatReaderSource delete the reader for us..
		audioFormatReaderSource = new AudioFormatReaderSource (reader, true);
        audioTransportSource->setSource (audioFormatReaderSource,
                                         32768,
                                         &bufferingTimeSliceThread);
        
        if (shouldBeLooping)
            audioFormatReaderSource->setLooping (true);
        
		// let our listeners know that we have loaded a new file
		audioTransportSource->sendChangeMessage();
        listeners.call (&Listener::fileChanged, this);

		return true;
	}
	
    audioTransportSource->sendChangeMessage();
    listeners.call (&Listener::fileChanged, this);

    return false;    
}

