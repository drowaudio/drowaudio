/*
 *  DefaultSettings.h
 *  Decks
 *
 *  Created by David Rowland on 15/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DECKS_DEFAULTSETTINGS__H_
#define _DECKS_DEFAULTSETTINGS__H_

namespace Defaults
{
	static const int noDecks = 4;
	
	namespace Mixer
	{
		namespace Channel
		{
			static const bool on = true;
			static const bool bypass = false;
			static const float gainMin = 0.0f;
			static const float gainMax = 2.0f;
			static const float gain = 1.0f;
			
			namespace HighEQ {
				static const int cf = 1300;
				static const float q = 1.5f;
				static const float gainMin = 0.05f;
				static const float gainMax = 2.0f;				
				static const float gain = 1.0f;
				static const int killCf = 1300;
				static const float killQ = 1.5;
				static const bool killOn = false;
			}
			
			namespace MidEQ {
				static const int cf = 1000;
				static const float q = 1.5f;
				static const float gainMin = 0.05f;
				static const float gainMax = 2.0f;				
				static const float gain = 1.0f;
				static const int killCf = 1000;
				static const float killQ = 1.5;
				static const bool killOn = false;
			}
			
			namespace LowEQ {
				static const int cf = 70;
				static const float q = 1.5f;
				static const float gainMin = 0.05f;
				static const float gainMax = 2.0f;				
				static const float gain = 1.0f;
				static const int killCf = 70;
				static const float killQ = 1.5;
				static const bool killOn = false;
			}

			static const float faderMin = 0.0f;
			static const float faderMax = 1.0f;
			static const float fader = 1.0f;
			static const bool cue = true;
			static const float fxSendMin = 0.0f;
			static const float fxSendMax = 1.0f;
			static const float fxASend = 0.0f;
			static const float fxBSend = 0.0f;
		}
		
		
		namespace XFader
		{
			static const float fader = 0.25f;
			static const int assignX = 0;
			static const int assignY = 0;
		}
		
		namespace Master
		{
			static const float gainMin = 0.0f;
			static const float gainMax = 2.0f;				
			static const float gain = 1.0f;
			static const bool cue = false;
			static const float faderCurveMin = 0.001f;
			static const float faderCurveMax = 1.0f;
			static const float faderCurve = 0.5f;
			static const float xFaderCurveMin = 0.001f;
			static const float xFaderCurveMax = 1.0f;
			static const float xFaderCurve = 0.5f;
		}
	}
	
}

#endif //_DECKS_DEFAULTSETTINGS__H_