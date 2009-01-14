/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-7 by Raw Material Software ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the
   GNU General Public License, as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later version.

   JUCE is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with JUCE; if not, visit www.gnu.org/licenses or write to the
   Free Software Foundation, Inc., 59 Temple Place, Suite 330,
   Boston, MA 02111-1307 USA

  ------------------------------------------------------------------------------

   If you'd like to release a closed-source product which uses JUCE, commercial
   licenses are also available: visit www.rawmaterialsoftware.com/juce for
   more information.

  ==============================================================================
*/

#ifndef DROWTREMOLOFILTER_H
#define DROWTREMOLOFILTER_H

#include "dRowParameter.h"

//==============================================================================
/**
    A simple plugin filter that just applies a gain change to the audio
    passing through it.
*/
class dRowTremoloFilter  : public AudioProcessor,
                           public ChangeBroadcaster,
						   public Timer
{
public:
    //==============================================================================
    dRowTremoloFilter();
    ~dRowTremoloFilter();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

	void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (const int channelIndex) const;
    const String getOutputChannelName (const int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;

    //==============================================================================
    int getNumPrograms()                                        { return 0; }
    int getCurrentProgram()                                     { return 0; }
    void setCurrentProgram (int index)                          { }
    const String getProgramName (int index)                     { return String::empty; }
    void changeProgramName (int index, const String& newName)   { }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    // this is kept up to date with the midi messages that arrive, and the UI component
    // registers with it so it can represent the incoming messages
    MidiKeyboardState keyboardState;

    // this keeps a copy of the last set of time info that was acquired during an audio
    // callback - the UI component will read this and display it.
    AudioPlayHead::CurrentPositionInfo lastPosInfo;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;

    //==============================================================================
	
	juce_UseDebuggingNewOperator

	double getScaledParameter(int index);
	void setScaledParameter(int index, float newValue);
	void setScaledParameterNotifyingHost(int index, float newValue);
	double getParameterMin(int index);
	double getParameterMax(int index);
	double getParameterDefault(int index);

	// buffer variables
	static const unsigned int tremoloBufferSize = 2000;
	float tremoloBuffer[tremoloBufferSize];
	float tremoloBuffer2[tremoloBufferSize];
	float fTremoloBufferPosition;
	
	dRowParameter* newGain;
	dRowParameter* newRate;
	dRowParameter* newDepth;
	dRowParameter* newShape;
	dRowParameter* newPhase;

private:
	float* sinLookupTable;
	
	// UI parameters
	float gain;
	float rate;
	float depth;
	float shape;
	float phase;
	
	double currentSampleRate;	
	float currentScalingFactor;
	float nextScalingFactor;	
	float currentShape;
	float currentDepth;
	float currentPhase;
	
	
	/** This will be used to check if parameters have changed and
		to trigger a buffer refill if necessary
	 */
	void timerCallback();
	
	/**	This refills a buffer with a given phase angle and the current
		parameters from the UI so as to be the most up to date even if
		they have changed since the timer callback
	 */
	void fillBuffer(float *bufferToFill, float phaseAngle);
};


#endif
