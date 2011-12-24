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

#ifndef __DROWAUDIO_XMLHELPERS_H__
#define __DROWAUDIO_XMLHELPERS_H__

namespace XmlHelpers {

static XmlElement* findXmlElementWithAttributeWithValue (XmlElement* element, const String &attributeName, const String &attributeValue)
{
	if (element == 0)
		return 0;
	
	if (element->hasAttribute(attributeName)) {
		if(element->compareAttribute(attributeName, attributeValue, true))
			return element;
	}
	
	XmlElement* child = element->getFirstChildElement();
	
	while (child != 0)
	{
		if (child->hasAttribute(attributeName)) {
			if(element->compareAttribute(attributeName, attributeValue, true))
				return element;
		}
		
		XmlElement* const found = findXmlElementWithAttributeWithValue (child, attributeName, attributeValue);
		
		if (found != 0)
			return found;
		
		child = child->getNextElement();
	}
	
	return 0;
}

static XmlElement* findXmlElementWithAttribute (XmlElement *element, const String &attributeName)
{
	if (element == 0)
		return 0;
	
	if (element->hasAttribute(attributeName))
		return element;
	
	XmlElement* child = element->getFirstChildElement();
	
	while (child != 0)
	{
		if (child->hasAttribute(attributeName)) {
			return element;
		}
		
		XmlElement* const found = findXmlElementWithAttribute (child, attributeName);
		
		if (found != 0)
			return found;
		
		child = child->getNextElement();
	}
	
	return 0;
}

static XmlElement* findXmlElementWithSubText (XmlElement *element, const String &subtext)
{
	if (element == 0)
		return 0;
	
	if (element->getAllSubText() == subtext)
		return element;
	
	XmlElement* child = element->getFirstChildElement();
	
	while (child != 0)
	{
		if (child->getAllSubText() == subtext) {
			return child;
		}
		
		XmlElement* const found = findXmlElementWithSubText (child, subtext);
		
		if (found != 0)
			return found;
	
		child = child->getNextElement();
	}
	
	return 0;
}

}



#endif  // __DROWAUDIO_XMLHELPERS_H__
