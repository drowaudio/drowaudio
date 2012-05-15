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

#if DROWAUDIO_USE_CURL



juce_ImplementSingleton (CURLManager);

CURLManager::CURLManager()
    : TimeSliceThread ("cURL Thread")
{
	CURLcode result = curl_global_init (CURL_GLOBAL_ALL);
    
	jassert (result == CURLE_OK);
}

CURLManager::~CURLManager()
{
	curl_global_cleanup();
}

const StringArray CURLManager::getSupportedProtocols()
{
	StringArray supportedProtocols (getCURLVersionInfoData()->protocols);
	
	return supportedProtocols;
}

void CURLManager::cleanUpEasyCurlHandle (CURL* handle)
{
	curl_easy_cleanup (handle);
	handle = nullptr;
}



#endif