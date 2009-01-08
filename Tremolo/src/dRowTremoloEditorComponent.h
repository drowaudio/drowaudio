/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-7 by Raw Material Software ltd.
   Modified 2008 by David Rowland - dRowAudio

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the
   GNU General Public License, as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later version.

   JUCE is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with JUCE; if not, visit www.gnu.org/licenses or write to the
   Free Software Foundation, Inc., 59 Temple Place, Suite 330,
   Boston, MA 02111-1307 USA

  ------------------------------------------------------------------------------

   If you'd like to release a closed-source product which uses JUCE, commercial
   licenses are also available: visit www.rawmaterialsoftware.com/juce for
   more information.

  ==============================================================================
*/

#ifndef DROWTREMOLOEDITOR_H
#define DROWTREMOLOEDITOR_H

#include "dRowTremoloFilter.h"
#include "dRowBufferView.h"


//==============================================================================
/**
    This is the Component that our filter will use as its UI.

    One or more of these is created by the dRowTremoloFilter::createEditor() method,
    and they will be deleted at some later time by the wrapper code.

    To demonstrate the correct way of connecting a filter to its UI, this
    class is a ChangeListener, and our demo filter is a ChangeBroadcaster. The
    editor component registers with the filter when it's created and deregisters
    when it's destroyed. When the filter's parameters are changed, it broadcasts
    a message and this editor responds by updating its display.
*/
class dRowTremoloEditorComponent   : public AudioProcessorEditor,
                              public ChangeListener,
                              public SliderListener
{
public:
    /** Constructor.

        When created, this will register itself with the filter for changes. It's
        safe to assume that the filter won't be deleted before this object is.
    */
    dRowTremoloEditorComponent (dRowTremoloFilter* const ownerFilter);

    /** Destructor. */
    ~dRowTremoloEditorComponent();

    //==============================================================================
    /** Our demo filter is a ChangeBroadcaster, and will call us back when one of
        its parameters changes.
    */
    void changeListenerCallback (void* source);
	
	/** Using the slider start and end points to start and stop the filter timer 
		would the most efficient but doesn't let automation update the graphs
	 */
    void sliderValueChanged (Slider*);

    //==============================================================================
    /** Standard Juce paint callback. */
    void paint (Graphics& g);

    /** Standard Juce resize callback. */
    void resized();


private:
    //==============================================================================
    dRowTremoloFilter* const ownerFilterGlobal;
	
	Label* titleLabel;
	
	Slider* gainSlider;
	Slider* rateSlider;
	Slider* depthSlider;
	Slider* shapeSlider;
	Slider* phaseSlider;
	
    Label* gainLabel;
	Label* rateLabel;
	Label* depthLabel;
	Label* shapeLabel;
	Label* phaseLabel;
	
	MidiKeyboardComponent* midiKeyboard;
	Label* infoLabel;
	
	dRowBufferView* bufferView1;
	dRowBufferView* bufferView2;
	
	Label* bufferView1Label;
    Label* bufferView2Label;
	
	// Binary resources:
    static const char* tremoloTitleImage_jpg;
    static const int tremoloTitleImage_jpgSize;
	Image* cachedTitleImage;
	
    TooltipWindow tooltipWindow;

    void updateParametersFromFilter();

    // handy wrapper method to avoid having to cast the filter to a dRowTremoloFilter
    // every time we need it..
    dRowTremoloFilter* getFilter() const throw()       { return (dRowTremoloFilter*) getAudioProcessor(); }
};


#endif
