/*
  ==============================================================================


  ==============================================================================
*/

#ifndef _DROWAUDIO_DROWAUDIOFILTER__H_
#define _DROWAUDIO_DROWAUDIOFILTER__H_


#include <dRowAudio/dRowAudio.h>

//==============================================================================
class DRowAudioFilter  : public AudioProcessor,
                        public ChangeBroadcaster
{
public:
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
	
private:
	
};


#endif //_DROWAUDIO_DROWAUDIOFILTER__H_
