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



//==============================================================================
EnvelopeFollower::EnvelopeFollower()
    : envelope (0.0f),
      envAttack (1.0f), envRelease (1.0f)
{
}

EnvelopeFollower::~EnvelopeFollower()
{
}

//==============================================================================
void EnvelopeFollower::processEnvelope (const float* inputBuffer, float* outputBuffer, int numSamples) noexcept
{
    for (int i = 0; i < numSamples; ++i)
    {
        float envIn = fabsf (inputBuffer[i]);
        
        if (envelope < envIn)
            envelope += envAttack * (envIn - envelope);
        else if (envelope > envIn)
            envelope -= envRelease * (envelope - envIn);
        
        outputBuffer[i] = envelope;
    }
}

void EnvelopeFollower::setCoefficients (float attack, float release) noexcept
{
    envAttack = attack;
    envRelease = release;
}

