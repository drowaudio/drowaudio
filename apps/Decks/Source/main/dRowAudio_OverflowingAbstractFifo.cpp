/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-10 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#include "dRowAudio_OverflowingAbstractFifo.h"


//==============================================================================
OverflowingAbstractFifo::OverflowingAbstractFifo (const int capacity) throw()
    : bufferSize (capacity)
{
    jassert (bufferSize > 0);
}

OverflowingAbstractFifo::~OverflowingAbstractFifo() {}

int OverflowingAbstractFifo::getTotalSize() const throw()            { return bufferSize; }
int OverflowingAbstractFifo::getFreeSpace() const throw()            { return bufferSize - getNumReady(); }

int OverflowingAbstractFifo::getNumReady() const throw()
{
    const int vs = validStart.get();
    const int ve = validEnd.get();
    return ve >= vs ? (ve - vs) : (bufferSize - (vs - ve));
}

void OverflowingAbstractFifo::reset() throw()
{
    validEnd = 0;
    validStart = 0;
}

void OverflowingAbstractFifo::setTotalSize (int newSize) throw()
{
    jassert (newSize > 0);
    reset();
    bufferSize = newSize;
}

//==============================================================================
void OverflowingAbstractFifo::prepareToWrite (int numToWrite, int& startIndex1, int& blockSize1, int& startIndex2, int& blockSize2) const throw()
{
    const int vs = validStart.get();
    const int ve = validEnd.value;

    const int freeSpace = ve >= vs ? (bufferSize - (ve - vs)) : (vs - ve);
    numToWrite = jmin (numToWrite, freeSpace - 1);

    if (numToWrite <= 0)
    {
        startIndex1 = 0;
        startIndex2 = 0;
        blockSize1 = 0;
        blockSize2 = 0;
    }
    else
    {
        startIndex1 = ve;
        startIndex2 = 0;
        blockSize1 = jmin (bufferSize - ve, numToWrite);
        numToWrite -= blockSize1;
        blockSize2 = numToWrite <= 0 ? 0 : jmin (numToWrite, vs);
    }
}

//void OverflowingAbstractFifo::prepareToWriteOverflowing (int numToWrite, int& startIndex1, int& blockSize1, int& startIndex2, int& blockSize2) const throw()
//{
//    const int vs = validStart.get();
//    const int ve = validEnd.value;
//	
//    if (numToWrite <= 0)
//    {
//        startIndex1 = 0;
//        startIndex2 = 0;
//        blockSize1 = 0;
//        blockSize2 = 0;
//    }
//    else
//    {
//        startIndex1 = ve;
//        startIndex2 = 0;
//        blockSize1 = jmin (bufferSize - ve, numToWrite);
//        numToWrite -= blockSize1;
//        blockSize2 = numToWrite <= 0 ? 0 : jmin (numToWrite, vs);
//    }
//}

void OverflowingAbstractFifo::finishedWrite (int numWritten) throw()
{
    jassert (numWritten >= 0 && numWritten < bufferSize);
    int newEnd = validEnd.value + numWritten;
    if (newEnd >= bufferSize)
        newEnd -= bufferSize;

    validEnd = newEnd;
}

void OverflowingAbstractFifo::prepareToRead (int numWanted, int& startIndex1, int& blockSize1, int& startIndex2, int& blockSize2) const throw()
{
    const int vs = validStart.value;
    const int ve = validEnd.get();

    const int numReady = ve >= vs ? (ve - vs) : (bufferSize - (vs - ve));
    numWanted = jmin (numWanted, numReady);

    if (numWanted <= 0)
    {
        startIndex1 = 0;
        startIndex2 = 0;
        blockSize1 = 0;
        blockSize2 = 0;
    }
    else
    {
        startIndex1 = vs;
        startIndex2 = 0;
        blockSize1 = jmin (bufferSize - vs, numWanted);
        numWanted -= blockSize1;
        blockSize2 = numWanted <= 0 ? 0 : jmin (numWanted, ve);
    }
}

void OverflowingAbstractFifo::finishedRead (int numRead) throw()
{
    jassert (numRead >= 0 && numRead <= bufferSize);

    int newStart = validStart.value + numRead;
    if (newStart >= bufferSize)
        newStart -= bufferSize;

    validStart = newStart;
}

//==============================================================================
