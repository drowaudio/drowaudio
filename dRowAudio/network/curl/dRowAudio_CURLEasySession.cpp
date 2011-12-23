/*
  ==============================================================================

    CURLEasySession.cpp
    Created: 18 May 2011 12:50:44am
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

CURLEasySession::CURLEasySession()
:	handle      (nullptr),
	remotePath  (String::empty),
	progress    (1.0)
{
	handle = CURLManager::getInstance()->createEasyCurlHandle();

	enableFullDebugging(true);
	curl_easy_setopt(handle, CURLOPT_NOPROGRESS, false);
}

CURLEasySession::CURLEasySession(String localPath,
								 String remotePath,
								 bool upload,
								 String username,
								 String password)
{
	handle = CURLManager::getInstance()->createEasyCurlHandle();
	enableFullDebugging (true);
	curl_easy_setopt (handle, CURLOPT_NOPROGRESS, false);

	setLocalFile (File (localPath));
	setRemotePath (remotePath);
	setUserNameAndPassword (username, password);
	beginTransfer (upload);
}

CURLEasySession::~CURLEasySession()
{
	CURLManager::getInstance()->removeTimeSliceClient (this);
	if (CURLManager::getInstance()->getNumClients() == 0)
		CURLManager::getInstance()->stopThread (1000);
	
	CURLManager::getInstance()->cleanUpEasyCurlHandle (handle);
}

//==============================================================================
void CURLEasySession::setInputStream (InputStream* newInputStream)
{
    localFile = File::nonexistent;
    inputStream = newInputStream;
}

void CURLEasySession::setLocalFile (File newLocalFile)
{
	localFile = newLocalFile;
    inputStream = localFile.createInputStream();
}

void CURLEasySession::setRemotePath (String newRemotePath)
{
	remotePath = newRemotePath;

    if (remotePath.getLastCharacters (1) == "/")
        remotePath  = remotePath << localFile.getFileName();

	curl_easy_setopt (handle, CURLOPT_URL, remotePath.toUTF8().getAddress());
}

void CURLEasySession::setUserNameAndPassword (String username, String password)
{
	userNameAndPassword = username << ":" << password;
	curl_easy_setopt (handle, CURLOPT_USERPWD, userNameAndPassword.toUTF8().getAddress());
}

//==============================================================================
String CURLEasySession::getCurrentWorkingDirectory()
{
	char url[500];
	
	CURLcode res = curl_easy_getinfo (handle, CURLINFO_EFFECTIVE_URL, url);
	
	if (res == CURLE_OK)
		return String (url);
	else
		return String::empty;
}

StringArray CURLEasySession::getDirectoryListing()
{
    String remoteUrl (remotePath.upToLastOccurrenceOf ("/", true, false));
	curl_easy_setopt (handle, CURLOPT_URL, remoteUrl.toUTF8().getAddress());
    
	directoryContentsList.setSize (0);
	
	curl_easy_setopt (handle, CURLOPT_PROGRESSFUNCTION, 0L);
	curl_easy_setopt (handle, CURLOPT_UPLOAD, 0L);
	curl_easy_setopt (handle, CURLOPT_DIRLISTONLY, 1L);
	curl_easy_setopt (handle, CURLOPT_WRITEDATA, this);
	curl_easy_setopt (handle, CURLOPT_WRITEFUNCTION, directoryListingCallback);
    
	// perform  the tranfer
	progress = 0.0;
	CURLcode result = curl_easy_perform (handle);
	reset();	
	
	if (result == CURLE_OK)
	{
		StringArray list;
		list.addLines (directoryContentsList.toString().trim());
        
		return list;
	}
	else 
    {
		DBG("*** error during directory listing");
		return StringArray (curl_easy_strerror (result));
	}				
}

//==============================================================================
void CURLEasySession::enableFullDebugging (bool shouldEnableFullDebugging)
{
	curl_easy_setopt (handle, CURLOPT_VERBOSE, shouldEnableFullDebugging ? 1L : 0L);
}

void CURLEasySession::beginTransfer (bool transferIsUpload, bool performOnBackgroundThread)
{
	isUpload = transferIsUpload;
    shouldStopTransfer = false;
    
    if (performOnBackgroundThread)
    {
        CURLManager::getInstance()->addTimeSliceClient (this);
        CURLManager::getInstance()->startThread();
    }
    else
    {
        performTransfer (isUpload);
    }
}

void CURLEasySession::stopTransfer()
{
    shouldStopTransfer = true;
}

void CURLEasySession::reset()
{
	curl_easy_reset (handle);
	curl_easy_setopt (handle, CURLOPT_URL, remotePath.toUTF8().getAddress());
	curl_easy_setopt (handle, CURLOPT_USERPWD, userNameAndPassword.toUTF8().getAddress());
	curl_easy_setopt (handle, CURLOPT_NOPROGRESS, false);
}

//==============================================================================
void CURLEasySession::addListener (CURLEasySession::Listener* const listener)
{
    listeners.add (listener);
}

void CURLEasySession::removeListener (CURLEasySession::Listener* const listener)
{
    listeners.remove (listener);
}

//==============================================================================
int CURLEasySession::useTimeSlice()
{
	DBG("time slice started");
	
    performTransfer (isUpload);

	DBG("time slice ended");

	return -1;
}

//==============================================================================
size_t CURLEasySession::writeCallback (void* sourcePointer, size_t blockSize, size_t numBlocks, CURLEasySession* session)
{
	if (session != nullptr)
	{
		if (session->outputStream->failedToOpen())
		{
			DBG("Error opening file for writing");
			return -1; /* failure, can't open file to write */ 
		}
		
		session->outputStream->write (sourcePointer, blockSize * numBlocks);
		return blockSize * numBlocks;
	}
	
	return -1;
}

size_t CURLEasySession::readCallback (void* destinationPointer, size_t blockSize, size_t numBlocks, CURLEasySession* session)
{
	if (session != nullptr)
	{
		if (&(session->inputStream) == nullptr)
		{
			DBG("Error opening file for reading");
			return -1; /* failure, can't open file to read */ 
		}
		
		return session->inputStream->read (destinationPointer, blockSize * numBlocks);
	}
	
	return -1;
}

size_t CURLEasySession::directoryListingCallback (void* sourcePointer, size_t blockSize, size_t numBlocks, CURLEasySession* session)
{
	if (session != nullptr)
	{
		session->directoryContentsList.append (sourcePointer, (int) (blockSize * numBlocks));

		return blockSize * numBlocks;
	}
	
	return -1;	
}

int CURLEasySession::internalProgressCallback (CURLEasySession* session, double dltotal, double dlnow, double ultotal, double ulnow)
{
	session->progress = session->isUpload ? (ulnow / session->inputStream->getTotalLength()) : (dlnow / dltotal);
	
    session->listeners.call (&CURLEasySession::Listener::transferProgressUpdate, session);
    
	return (int) session->shouldStopTransfer;
}

//==============================================================================
CURLcode CURLEasySession::performTransfer (bool transferIsUpload)
{
	curl_easy_setopt (handle, CURLOPT_URL, remotePath.toUTF8().getAddress());
    curl_easy_setopt (handle, CURLOPT_UPLOAD, (long) isUpload);
    curl_easy_setopt (handle, CURLOPT_PROGRESSDATA, this);
	curl_easy_setopt (handle, CURLOPT_PROGRESSFUNCTION, internalProgressCallback);
    
    if (isUpload == true)
	{
		// sets the pointer to be passed to the read callback
		curl_easy_setopt (handle, CURLOPT_READDATA, this);
		curl_easy_setopt (handle, CURLOPT_READFUNCTION, readCallback);
        inputStream->setPosition (0);
	}
	else
	{
		// sets the pointer to be passed to the write callback
		curl_easy_setopt (handle, CURLOPT_WRITEDATA, this);
		curl_easy_setopt (handle, CURLOPT_WRITEFUNCTION, writeCallback);
        
		// create local file to recieve transfer
		if (localFile.existsAsFile())
			localFile = localFile.getNonexistentSibling();
        
        outputStream = localFile.createOutputStream();
	}
    
	//perform the transfer
	progress = 0.0;
	CURLcode result = curl_easy_perform (handle);
	
	// delete the streams to flush the buffers
	outputStream = nullptr;
	
	if (result != CURLE_OK)
    {
        DBG("*** error during transfer"<<String (curl_easy_strerror (result)));
    }
    
    return result;
}

END_JUCE_NAMESPACE