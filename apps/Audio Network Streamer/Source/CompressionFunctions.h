/*
  ==============================================================================

    CompressionFunctions.h
    Created: 11 Jun 2011 7:06:03am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __COMPRESSIONFUNCTIONS_H_C18D49A6__
#define __COMPRESSIONFUNCTIONS_H_C18D49A6__

//==============================================================================
static void compressMemoryBlock(const MemoryBlock &sourceBlock, MemoryBlock &destinationBlock)
{
	MemoryOutputStream memoryStream(destinationBlock, true);
	GZIPCompressorOutputStream zipStream(&memoryStream);
	
	zipStream.write(sourceBlock.getData(), sourceBlock.getSize());
	zipStream.flush();
	
	//DBG("Original: "<<(int)sourceBlock.getSize()<<" - Comp: "<<(int)destinationBlock.getSize()<<" - Ratio: "<<(destinationBlock.getSize()/(float)sourceBlock.getSize()));
}

static void compressMemoryBlockWithSize(const void *sourceBlock, size_t sourceSizeBytes, MemoryBlock &destinationBlock)
{
	MemoryOutputStream memoryStream(destinationBlock, false);
	GZIPCompressorOutputStream zipStream(&memoryStream);
	
	zipStream.write(sourceBlock, sourceSizeBytes);
	zipStream.flush();
	
	//DBG("Original: "<<(int)sourceBlock.getSize()<<" - Comp: "<<(int)destinationBlock.getSize()<<" - Ratio: "<<(destinationBlock.getSize()/(float)sourceBlock.getSize()));
}

static void decompressMemoryBlock(const MemoryBlock &sourceBlock, MemoryBlock &destinationBlock, const int bufferSize =8192)
{
	MemoryInputStream memoryStream(sourceBlock, false);
	GZIPDecompressorInputStream zipStream(memoryStream);
		
	destinationBlock.ensureSize(bufferSize, false);
	void* currentPos = destinationBlock.getData();
	int64 totalSize = 0;
	while (! zipStream.isExhausted())
	{
		int64 bytesRead = zipStream.read(currentPos, bufferSize);
		totalSize += bytesRead;
		currentPos = addBytesToPointer(currentPos, bytesRead);
		destinationBlock.ensureSize(destinationBlock.getSize()+bufferSize, false);
	}
	destinationBlock.setSize(totalSize, false);
	
	DBG("Comp: "<<(int)sourceBlock.getSize()<<" - Original: "<<(int)destinationBlock.getSize()<<" - Ratio: "<<(destinationBlock.getSize()/(float)sourceBlock.getSize())<<"\n");
}

static void decompressMemory(const void *sourceBlock, size_t sourceBlockSize, MemoryBlock &destinationBlock, const int bufferSize =8192)
{
	MemoryInputStream memoryStream(sourceBlock, sourceBlockSize, false);
	GZIPDecompressorInputStream zipStream(memoryStream);
	
	destinationBlock.ensureSize(bufferSize, false);
	void* currentPos = destinationBlock.getData();
	int64 totalSize = 0;
	while (! zipStream.isExhausted())
	{
		int64 bytesRead = zipStream.read(currentPos, bufferSize);
		totalSize += bytesRead;
		currentPos = addBytesToPointer(currentPos, bytesRead);
		destinationBlock.ensureSize(destinationBlock.getSize()+bufferSize, false);
	}
	destinationBlock.setSize(totalSize, false);
	
	DBG("Comp: "<<(int)sourceBlockSize<<" - Original: "<<(int)destinationBlock.getSize()<<" - Ratio: "<<(destinationBlock.getSize()/(float)sourceBlockSize)<<"\n");
}

static void decompressMemoryWithKnownSize(const void *sourceBlock, size_t sourceBlockSize, MemoryBlock &destinationBlock, const int uncompressedSize)
{
	MemoryInputStream memoryStream(sourceBlock, sourceBlockSize, false);
	GZIPDecompressorInputStream zipStream(&memoryStream, false, false, uncompressedSize);
	
	destinationBlock.setSize(uncompressedSize, false);
	zipStream.read(destinationBlock.getData(), uncompressedSize);
		
	//DBG("Comp: "<<(int)sourceBlockSize<<" - Original: "<<(int)destinationBlock.getSize()<<" - Ratio: "<<(destinationBlock.getSize()/(float)sourceBlockSize)<<"\n");
}

static void serialiseAudio(MemoryBlock &serialisedBlock,
						   const int numChannels, const int numSamples, const float** channelData)
{
	serialisedBlock.append(&numChannels, sizeof(int));
	serialisedBlock.append(&numSamples, sizeof(int));
	if (numChannels > 0)
		serialisedBlock.append(channelData[0], numSamples * sizeof(float));
	if (numChannels > 1)
		serialisedBlock.append(channelData[1], numSamples * sizeof(float));	
}

static void deserialiseAudio(MemoryBlock &serialisedBlock,
							 int &numChannels, int &numSamples, float** channelData)
{
	numChannels = *static_cast<int*>(serialisedBlock.getData());
	numSamples = *static_cast<int*>(addBytesToPointer(serialisedBlock.getData(),sizeof(int)));
	
	if (numChannels > 0)
	{
		const int bytesPerChannel = (serialisedBlock.getSize()-(2*sizeof(int))) / numChannels;
		
		if (numChannels > 0)
			memcpy(channelData[0], (float*)addBytesToPointer(serialisedBlock.getData(),2*sizeof(int)), bytesPerChannel);
		if (numChannels > 1)
			memcpy(channelData[1], (float*)addBytesToPointer(serialisedBlock.getData(),2*sizeof(int)+bytesPerChannel), bytesPerChannel);
	}
}

#endif  // __COMPRESSIONFUNCTIONS_H_C18D49A6__
