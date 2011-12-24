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

#include "../../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_TwoPoleTwoZeroFilter.h"


TwoPoleTwoZeroFilter::TwoPoleTwoZeroFilter() throw()
{
}

TwoPoleTwoZeroFilter::~TwoPoleTwoZeroFilter() throw()
{
}

void TwoPoleTwoZeroFilter::processSamples(float* const pfSamples, const int numSamples) throw()
{
	for (int i = 0; i < numSamples; ++i)
	{
		float out = (a0 * pfSamples[i]) + (a1 * x1) + (a2 * x2) + (b1 * y1) + (b2 * y2);
		x2 = x1;
		x1 = pfSamples[i];
		y2 = y1;
		y1 = out;
		pfSamples[i] = out;
	}
}

void TwoPoleTwoZeroFilter::makeLowPassButterworth(const double sampleRate, const double frequency) throw()
{
	double c = 1;
	double g = 1;
	double p = root2;
	
	double f = (c * frequency) / sampleRate;
	double w0 = tan(double_Pi * f);
	double k1 = p * w0;
	double k2 = squareNumber(g * w0);
	
	a0 = k2 / (1.0 + k1 + k2);
	a1 = 2.0 * a0;
	a2 = a0;
	b1 = 2.0 * a0 * ((1.0 / k2) - 1.0);
	b2 = 1.0 - (a0 + a1 + a2 + b1);
}

void TwoPoleTwoZeroFilter::makeHighPassButterworth(const double sampleRate, const double frequency) throw()
{
	double c = 1;
	double g = 1;
	double p = root2;
	
	double f = 0.5 * c * (frequency / sampleRate);
	double w0 = tan(double_Pi * f);
	double k1 = p * w0;
	double k2 = squareNumber(g * w0);
		
	a0 = k2 / (1.0 + k1 + k2);
	a1 = -(2.0 * a0);
	a2 = a0;
	b1 = -(2.0 * a0 * ((1.0 / k2) - 1.0));
	b2 = 1.0 - (a0 + a1 + a2 + b1);	
}

void TwoPoleTwoZeroFilter::makeLowPassCriticallyDamped(const double sampleRate, const double frequency) throw()
{
	double c = pow((pow(2,0.5) - 1), -0.5);
	double g = 1;
	double p = 2;
	
	double f = (c * frequency) / sampleRate;
	double w0 = tan(double_Pi * f);
	double k1 = p * w0;
	double k2 = squareNumber(g * w0);
	
	a0 = k2 / (1.0 + k1 + k2);
	a1 = 2.0 * a0;
	a2 = a0;
	b1 = 2.0 * a0 * ((1.0 / k2) - 1.0);
	b2 = 1.0 - (a0 + a1 + a2 + b1);
}

void TwoPoleTwoZeroFilter::makeHighPassCriticallyDamped(const double sampleRate, const double frequency) throw()
{
	double c = pow((pow(2,0.5) - 1), 0.5);
	double g = 1;
	double p = 2;
	
	double f = 0.5 * c * (frequency / sampleRate);
	double w0 = tan(double_Pi * f);
	double k1 = p * w0;
	double k2 = squareNumber(g * w0);
	
	a0 = k2 / (1.0 + k1 + k2);
	a1 = -(2.0 * a0);
	a2 = a0;
	b1 = -(2.0 * a0 * ((1.0 / k2) - 1.0));
	b2 = 1.0 - (a0 + a1 + a2 + b1);	
}

void TwoPoleTwoZeroFilter::makeLowPassBessel(const double sampleRate, const double frequency) throw()
{
	double c = ((pow(1.25,0.5)-0.5),-0.5) / root3;
	double g = 3;
	double p = 3;
	
	double f = (c * frequency) / sampleRate;
	double w0 = tan(double_Pi * f);
	double k1 = p * w0;
	double k2 = squareNumber(g * w0);
	
	a0 = k2 / (1.0 + k1 + k2);
	a1 = 2.0 * a0;
	a2 = a0;
	b1 = 2.0 * a0 * ((1.0 / k2) - 1.0);
	b2 = 1.0 - (a0 + a1 + a2 + b1);
}

void TwoPoleTwoZeroFilter::makeHighPassBessel(const double sampleRate, const double frequency) throw()
{
	double c = root3 * ((pow(1.25,0.5)-0.5),0.5);
	double g = 3;
	double p = 3;
	
	double f = 0.5 * c * (frequency / sampleRate);
	double w0 = tan(double_Pi * f);
	double k1 = p * w0;
	double k2 = squareNumber(g * w0);
	
	a0 = k2 / (1.0 + k1 + k2);
	a1 = -(2.0 * a0);
	a2 = a0;
	b1 = -(2.0 * a0 * ((1.0 / k2) - 1.0));
	b2 = 1.0 - (a0 + a1 + a2 + b1);	
}

END_DROWAUDIO_NAMESPACE