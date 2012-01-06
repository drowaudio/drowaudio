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

#ifndef __DROWAUDIO_CURLMANAGER_H__
#define __DROWAUDIO_CURLMANAGER_H__

#if DROWAUDIO_USE_CURL || DOXYGEN

#include "curl/include/curl/curl.h"

//==============================================================================
class CURLManager : public TimeSliceThread,
					public DeletedAtShutdown
{
public:
	//==============================================================================
	juce_DeclareSingleton (CURLManager, true);
	
	CURLManager();
	
	~CURLManager();
	
	//==============================================================================
	/**	Creates a new easy curl session handle.
		This simply creates the handle for you, it is the caller's responsibility
		to clean up when the handle is no longer needed. This can be done with
		cleanUpEasyCurlHandle().
	 */
	CURL* createEasyCurlHandle()				{	return curl_easy_init();	}
	
	/**	Cleans up an easy curl session for you.
		You can pass this a handle generated with createEasyCurlHandle() to clean
		up any resources associated with it. Be careful not to use the handle after
		calling this function as it will be a nullptr.
	 */
	void cleanUpEasyCurlHandle (CURL* handle);
	
	/**	Returns a list of the supported protocols.
	 */
	const StringArray getSupportedProtocols();

	/**	Returns a pointer to the version info data structure.
		@see curl_version_info_data
	 */
	curl_version_info_data* getCURLVersionInfoData()	{	return curl_version_info (CURLVERSION_NOW);	}
	
private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CURLManager);
};

#endif
#endif  // __DROWAUDIO_CURLMANAGER_H__