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

void BiquadFilter::processSamples (float* const samples,
                                   const int numSamples) noexcept
{
    IIRFilter::processSamples (samples, numSamples);
}

void BiquadFilter::processSamples (int* const samples,
								   const int numSamples) noexcept
{
    const ScopedLock sl (processLock);
	
    if (active)
    {
        for (int i = 0; i < numSamples; ++i)
        {
            const int in = samples[i];
			
            int out = (int) (coefficients[0] * in)
			+ (int) (coefficients[1] * x1)
			+ (int) (coefficients[2] * x2)
			- (int) (coefficients[4] * y1)
			- (int) (coefficients[5] * y2);
			
            x2 = x1;
            x1 = (float) in;
            y2 = y1;
            y1 = (float) out;
            
            samples[i] = out;
        }
    }
}

void BiquadFilter::makeLowPass (const double sampleRate,
								const double frequency,
                                const double Q) noexcept
{
	double oneOverCurrentSampleRate = 1/sampleRate; 
	float w0 = (float) (2.0f * float_Pi * frequency * oneOverCurrentSampleRate);
	float cos_w0 = cos (w0);
	float sin_w0 = sin (w0);
	float alpha = sin_w0 / (2.0f * (float) Q);
    
	setCoefficients ((1.0f - cos_w0) * 0.5f, 
                     (1.0f - cos_w0), 
                     (1.0f - cos_w0) * 0.5f, 
                     (1.0f + alpha), 
                     -2.0f * cos_w0, 
                     (1.0f - alpha));
}

void BiquadFilter::makeHighPass (const double sampleRate,
                                 const double frequency,
                                 const double Q) noexcept
{
	double oneOverCurrentSampleRate = 1/sampleRate; 
	float w0 = (float) (2.0f * float_Pi * frequency * oneOverCurrentSampleRate);
	float cos_w0 = cos (w0);
	float sin_w0 = sin (w0);
	float alpha = sin_w0 / (2.0f * (float) Q);
    
	setCoefficients ((1.0f + cos_w0) * 0.5f, 
                     -(1.0f + cos_w0), 
                     (1.0f + cos_w0) * 0.5f, 
                     (1.0f + alpha), 
                     -2.0f * cos_w0, 
                     (1.0f - alpha));	
}

void BiquadFilter::makeBandPass(const double sampleRate,
                                const double frequency,
                                const double Q) noexcept
{
	const double qFactor = jlimit (0.00001, 1000.0, Q);
	const double oneOverCurrentSampleRate = 1/sampleRate;
    
	
	float w0 = (float) (2.0f * float_Pi * frequency * oneOverCurrentSampleRate);
	float cos_w0 = cos (w0);
	float sin_w0 = sin (w0);
	float alpha = sin_w0 / (2.0f * (float) qFactor);
    //	float alpha = sin_w0 * sinh( (log(2.0)/2.0) * bandwidth * w0/sin_w0 );
	
	setCoefficients (alpha, 
					 0.0f, 
					 -alpha, 
                     1.0f + alpha, 
                     -2.0f * cos_w0, 
                     1.0f - alpha);
}

void BiquadFilter::makeBandStop(const double sampleRate,
                                const double frequency,
                                const double Q) noexcept
{
	const double qFactor = jlimit(0.00001, 1000.0, Q);
	const double oneOverCurrentSampleRate = 1/sampleRate;
	
	
	float w0 = (float) (2.0f * float_Pi * frequency * oneOverCurrentSampleRate);
	float cos_w0 = cos (w0);
	float sin_w0 = sin (w0);
	float alpha = (float) (sin_w0 / (2 * qFactor));
	
	setCoefficients (1.0f, 
					 -2*cos_w0, 
					 1.0f, 
					 1.0f + alpha, 
					 -2.0f * cos_w0, 
					 1.0f - alpha);
}

void BiquadFilter::makePeakNotch (const double sampleRate,
								  const double centreFrequency,
								  const double Q,
								  const float gainFactor) noexcept
{
    jassert (sampleRate > 0);
    jassert (Q > 0);
	
    const double A = jmax (0.0f, gainFactor);
    const double omega = (double_Pi * 2.0 * jmax (centreFrequency, 2.0)) / sampleRate;
    const double alpha = 0.5 * sin (omega) / Q;
    const double c2 = -2.0 * cos (omega);
    const double alphaTimesA = alpha * A;
    const double alphaOverA = alpha / A;
	
    setCoefficients (1.0 + alphaTimesA,
                     c2,
                     1.0 - alphaTimesA,
                     1.0 + alphaOverA,
                     c2,
                     1.0 - alphaOverA);
}

void BiquadFilter::makeAllpass (const double sampleRate,
                                const double frequency,
                                const double Q) noexcept
{
	const double qFactor = jlimit(0.00001, 1000.0, Q);
	const double oneOverCurrentSampleRate = 1/sampleRate;
	
	
	float w0 = (float) (2.0f * float_Pi * frequency * oneOverCurrentSampleRate);
	float cos_w0 = cos(w0);
	float sin_w0 = sin(w0);
	float alpha = (float) (sin_w0 / (2 * qFactor));
	
	setCoefficients(1.0f - alpha, 
					-2*cos_w0, 
					1.0f + alpha, 
					1.0f + alpha, 
					-2.0f * cos_w0, 
					1.0f - alpha);
}

void BiquadFilter::copyCoefficientsFrom (const BiquadFilter& other) noexcept
{
    const ScopedLock sl (processLock);
	
    memcpy (coefficients, other.coefficients, sizeof (coefficients));
    active = other.active;
}

void BiquadFilter::copyOutputsFrom (const BiquadFilter& other) noexcept
{
	x1 = other.x1;
	x2 = other.x2;
	y1 = other.y1;
	y2 = other.y2;
}
