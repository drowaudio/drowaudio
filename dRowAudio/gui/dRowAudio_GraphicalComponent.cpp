/*
  ==============================================================================

    dRowAudio_GraphicalComponent.cpp
    Created: 8 Jul 2010 2:13:51pm
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

GraphicalComponent::GraphicalComponent()
    : paused (false),
	  needToProcess (true),
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
		return false;
	}
	else
	{
		if (needToProcess)
        {
			process();
        }
        
		needToProcess = false;
		return true;
	}
}

void GraphicalComponent::copySamples (float *values, int numSamples)
{
		// allocate new memory only if needed
		if (numSamples != numSamples)
        {
			numSamples = numSamples;
			samples.malloc (numSamples);
		}
		
		// lock whilst copying
		ScopedLock sl (lock);
		memcpy (samples, values, numSamples * sizeof (float));
		
		needToProcess = true;
}

void GraphicalComponent::copySamples (float **values, int numSamples, int numChannels)
{
	// allocate new memory only if needed
	if (numSamples != numSamples) 
    {
		numSamples = numSamples;
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

END_JUCE_NAMESPACE