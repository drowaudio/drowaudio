/*
  ==============================================================================

    dRowAudio_XmlHelpers.h
    Created: 14 Mar 2011 2:17:41pm
    Author:  David Rowland

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
