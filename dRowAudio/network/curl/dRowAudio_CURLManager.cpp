/*
  ==============================================================================

    CURLManager.cpp
    Created: 18 May 2011 12:13:07am
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

juce_ImplementSingleton (CURLManager);

CURLManager::CURLManager()
    : TimeSliceThread("cURL Thread")
{
	CURLcode result = curl_global_init (CURL_GLOBAL_ALL);
    
	if (result != CURLE_OK)
    {
		DBG("Error loading cURL - " << curl_easy_strerror (result));
    }
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

END_JUCE_NAMESPACE