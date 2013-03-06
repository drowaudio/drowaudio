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

#ifndef __DROWAUDIO_BIQUADFILTER_H__
#define __DROWAUDIO_BIQUADFILTER_H__

//==============================================================================
/** A Biquad filter.
 
	This filter is a subclass of the Juce IIR filter but uses
	some additional methods to give more filter designs.
 */
class BiquadFilter : public IIRFilter
{
public:
    //==============================================================================
	/** Performs the filter operation on the given set of int samples.
	 */
    void processSamples (float* samples,
                         int numSamples) noexcept;
	
	/** Performs the filter operation on the given set of int samples.
	 */
    void processSamples (int* samples,
                         int numSamples) noexcept;
	
	/**	Makes the filter a Low-pass filter.
	 */
	void makeLowPass (const double sampleRate,
                      const double frequency,
                      const double Q) noexcept;
	
	/**	Makes the filter a High-pass filter.
	 */
	void makeHighPass (const double sampleRate,
                       const double frequency,
                       const double Q) noexcept;
	
	/**	Makes the filter a Band-pass filter.
	 */
	void makeBandPass (const double sampleRate,
                       const double frequency,
                       const double Q) noexcept;
	
	/**	Makes the filter a Band-stop filter.
	 */
	void makeBandStop (const double sampleRate,
                       const double frequency,
                       const double Q) noexcept;
	
	/**	Makes the filter a peak/notch filter. This type of filter
		adds or subtracts from the unfiltered signal.
	 */
	void makePeakNotch (const double sampleRate,
                        const double frequency,
                        const double Q,
                        const float gainFactor) noexcept;	
	
	/**	Makes the filter an Allpass filter.
		This type of filter has a complex phase response so will give a comb 
		filtered effect when combined with an unfilterd copy of the signal.
	 */
	void makeAllpass(const double sampleRate,
					   const double frequency,
					   const double Q) noexcept;
	
    /** Makes this filter duplicate the set-up of another one.
	 */
    void copyCoefficientsFrom (const BiquadFilter& other) noexcept;
	
	/** Makes this filter duplicate the set-up of another one.
	 */
    void copyOutputsFrom (const BiquadFilter& other) noexcept;
	
private:
    //==============================================================================
	JUCE_LEAK_DETECTOR (BiquadFilter);
};


//==============================================================================
/**	Primitive class to store the set-up info of a BiquadFilter
 */
class BiquadFilterSetup
{
public:
	enum FilterType
    {
		Lowpass = 0,
		Bandpass,
		Highpass,
		NoFilter
	};
	
	BiquadFilterSetup (FilterType filterType, double filterCf, double filterQ = 0.5)
	{
		type = filterType;
		cf = filterCf;
		q = filterQ;
	}
	
	void setUpFilter (BiquadFilter& filter, double sampleRate)
	{
		if (type == Lowpass)
			filter.makeLowPass (sampleRate, cf, q);
		else if (type == Bandpass)
			filter.makeBandPass (sampleRate, cf, q);
		else if (type == Highpass)
			filter.makeHighPass (sampleRate, cf, q);
		else if (type == NoFilter)
			filter.makeInactive();
		
		filter.reset();
	}
	
	FilterType type;
	double cf, q;
};

#endif //__DROWAUDIO_BIQUADFILTER_H__