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

#ifndef _DROWAUDIOFILTER_H_
#define _DROWAUDIOFILTER_H_

#include "Parameters.h"

#define filterMult1 0.962894248608534f
#define filterMult2 0.962894248608534f
#define filterMult3 0.953722334004024f
#define filterMult4 0.953586497890295f
#define filterMult5 0.941740412979351f
#define filterMult6 0.930305403288958f
#define filterMult7 0.939393939393939f

#define allpassMult1 0.498207885304659f
#define allpassMult2 0.793165467625899f
#define allpassMult3 0.773242630385488f
#define allpassMult4 0.659824046920821f

//#define filterMult1 0.932615816565278f
//#define filterMult2 0.926743602609132f
//#define filterMult3 0.962100703844071f
//
//#define filterMult4 0.218908272369162f
//#define filterMult5 0.326478149100257f
//#define filterMult6 0.338582677165354f

//==============================================================================
/**
    A simple plugin filter that just applies a gain change to the audio
    passing through it.

*/
class DRowAudioFilter  : public AudioProcessor,
                         public ChangeBroadcaster
{
public:
    //==============================================================================
    DRowAudioFilter();
    ~DRowAudioFilter();

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
	// Custom Methods
	void setupParams();
	void updateFilters();
	void updateParameters();
	
	// AU Compatibility Methods
	double getScaledParameter(int index);
	void setScaledParameter(int index, float newValue);
	void setScaledParameterNotifyingHost(int index, float newValue);
	double getParameterMin(int index);
	double getParameterMax(int index);
	double getParameterDefault(int index);
	ParameterUnit getParameterUnit(int index);
	double getParameterStep(int index);
	double getParameterSkewFactor(int index);
	void smoothParameters();
	dRowParameter* getParameterPointer(int index);

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

    //==============================================================================
    juce_UseDebuggingNewOperator

	float RMSLeft;
	float RMSRight;
	float peakLeft;
	float peakRight;
	
private:
	
	double currentSampleRate;
	
	dRowParameter params[noParams];
	
	CombFilter preDelayFilterL, preDelayFilterR;
	LBCF combFilter1L, combFilter1R, combFilter2L, combFilter2R, combFilter3L, combFilter3R, combFilter4L, combFilter4R,
		 combFilter5L, combFilter5R, combFilter6L, combFilter6R, combFilter7L, combFilter7R, combFilter8L, combFilter8R;
	AllpassFilter allpassFilter1L, allpassFilter1R, allpassFilter2L, allpassFilter2R,
				  allpassFilter3L, allpassFilter3R, allpassFilter4L, allpassFilter4R;
	
	IIRFilter lowEQL, lowEQR, highEQL, highEQR;
	
	void setupFilter(LBCF &filter, float fbCoeff, float delayTime, float filterCf);

//	IIRFilter filter1_L;
//	IIRFilter filter1_R;
//	IIRFilter filter2_L;
//	IIRFilter filter2_R;
};

#endif //_DROWAUDIOFILTER_H_
