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

namespace XmlHelpers
{
    //==============================================================================
    /** Searches an XmlElement for an element with a given attribute name with
        the given attribute value.
     */
    static XmlElement* findXmlElementWithAttributeWithValue (XmlElement* element,
                                                             const String& attributeName,
                                                             const String& attributeValue)
    {
        if (element == nullptr)
            return nullptr;
        
        if (element->hasAttribute(attributeName)) {
            if(element->compareAttribute(attributeName, attributeValue, true))
                return element;
        }
        
        XmlElement* child = element->getFirstChildElement();
        
        while (child != nullptr)
        {
            if (child->hasAttribute(attributeName)) {
                if(element->compareAttribute(attributeName, attributeValue, true))
                    return element;
            }
            
            XmlElement* const found = findXmlElementWithAttributeWithValue (child, attributeName, attributeValue);
            
            if (found != nullptr)
                return found;
            
            child = child->getNextElement();
        }
        
        return nullptr;
    }

    //==============================================================================
    /** Searches an XmlElement for an element with a given attribute name.
     */
    static XmlElement* findXmlElementWithAttribute (XmlElement* element, const String& attributeName)
    {
        if (element == nullptr)
            return nullptr;
        
        if (element->hasAttribute(attributeName))
            return element;
        
        XmlElement* child = element->getFirstChildElement();
        
        while (child != nullptr)
        {
            if (child->hasAttribute(attributeName)) {
                return element;
            }
            
            XmlElement* const found = findXmlElementWithAttribute (child, attributeName);
            
            if (found != nullptr)
                return found;
            
            child = child->getNextElement();
        }
        
        return nullptr;
    }

    //==============================================================================
    /** Searches for an element with subtext that is an exact match.
     */
    static XmlElement* findXmlElementWithSubText (XmlElement* element, const String& subtext)
    {
        if (element == nullptr)
            return nullptr;
        
        if (element->getAllSubText() == subtext)
            return element;
        
        XmlElement* child = element->getFirstChildElement();
        
        while (child != nullptr)
        {
            if (child->getAllSubText() == subtext) {
                return child;
            }
            
            XmlElement* const found = findXmlElementWithSubText (child, subtext);
            
            if (found != nullptr)
                return found;
            
            child = child->getNextElement();
        }
        
        return nullptr;
    }
    
    //==============================================================================
    /** Searches for an element with subtext contains the given text.
     */
    static XmlElement* findXmlElementContainingSubText (XmlElement* element, const String& subtext)
    {
        if (element == nullptr || element->getFirstChildElement() == nullptr)
            return nullptr;
        
        if (element->getFirstChildElement()->isTextElement()
            && element->getFirstChildElement()->getText().contains (subtext))
            return element;
        
        XmlElement* child = element->getFirstChildElement();
        
        while (child != nullptr)
        {
            if (child->isTextElement()
                && child->getText().contains (subtext)) {
                return child;
            }
            
            XmlElement* const found = findXmlElementContainingSubText (child, subtext);
            
            if (found != nullptr)
                return found;
            
            child = child->getNextElement();
        }
        
        return nullptr;
    }
}

#endif  // __DROWAUDIO_XMLHELPERS_H__
