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



GraphicalComponent::GraphicalComponent()
    : paused (false),
	  needToProcess (true),
      sleepTime (5),
	  numSamples (0)
{
	samples.malloc (numSamples);

	startTimer (30);
}

GraphicalComponent::~GraphicalComponent()
{
}

int GraphicalComponent::useTimeSlice()
{
	if (paused)
    {
		return sleepTime;
	}
	else
	{
		if (needToProcess)
        {
			process();
            needToProcess = false;
            
            return sleepTime;
        }
        
		return sleepTime;
	}
}

void GraphicalComponent::copySamples (const float *values, int numSamples_)
{
		// allocate new memory only if needed
		if (numSamples != numSamples_)
        {
			numSamples = numSamples_;
			samples.malloc (numSamples);
		}
		
		// lock whilst copying
		ScopedLock sl (lock);
		memcpy (samples, values, numSamples * sizeof (float));
		
		needToProcess = true;
}

void GraphicalComponent::copySamples (float **values, int numSamples_, int numChannels)
{
	// allocate new memory only if needed
	if (numSamples != numSamples_) 
    {
		numSamples = numSamples_;
		samples.malloc (numSamples);
	}
	
	// lock whilst copying
	ScopedLock sl (lock);

	if (numChannels == 1)
    {
		memcpy (samples, values[0], numSamples * sizeof (float));
	}
	// this is quicker than the generic method below
	else if (numChannels == 2) 
    {
		for (int i = 0; i < numSamples; i++)
        {
			samples[i] = (fabsf (values[0][i]) > fabsf (values[1][i])) ? values[0][i] : values[1][i];
		}
	}
	else
    {
		samples.clear (numSamples);
		for (int c = 0; c < numChannels; c++) 
        {
			for (int i = 0; i < numSamples; i++) 
            {
				if (fabsf (values[c][i]) > samples[i])
                {
					samples[i] = values[c][i];
				}
			}			
		}
	}
	
	needToProcess = true;
}

