/*
 *  DecksLookAndFeel.h
 *  Decks
 *
 *  Created by David Rowland on 04/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DECKSLOOKANDFEEL__H_
#define _DECKSLOOKANDFEEL__H_

#include "../JuceLibraryCode/JuceHeader.h"

class DecksLookAndFeel :	public LookAndFeel,
							public DeletedAtShutdown
{
public:
	juce_DeclareSingleton (DecksLookAndFeel, false);
	
	enum IconType {
		Stop,
		Play,
		Cue,
		Pause,
		Next,
		Previous,
		ShuffleForward,
		ShuffleBack,
		Eject,
		Cross,
		Add,
		Search,
		Power,
		Bypass,
		GoUp,
		Infinity,
		DownTriangle,
		Info,
		noIcons
	};
	
	DecksLookAndFeel(){}
	
	~DecksLookAndFeel()
	{
		DBG("DecksLookAndFeel deleted");
	}

	//============================================================
	int getSliderThumbRadius (Slider& slider);

	virtual void drawRotarySlider (Graphics& g,
								   int x, int y,
								   int width, int height,
								   float sliderPosProportional,
								   const float rotaryStartAngle,
								   const float rotaryEndAngle,
								   Slider& slider);
	
	virtual void drawLinearSliderThumb (Graphics& g,
									    int x, int y,
									    int width, int height,
									    float sliderPos,
									    float minSliderPos,
									    float maxSliderPos,
									    const Slider::SliderStyle style,
										Slider& slider);
	
	virtual void drawFaderKnob(Graphics& g,
							   const Slider::SliderStyle style,
							   const float x,
							   const float y,
							   const float width,
							   const float height);
	
	/** Draws the text for a TextButton. */
    virtual void drawButtonBackground (Graphics& g,
									   Button& button,
									   const Colour& backgroundColour,
									   bool isMouseOverButton,
									   bool isButtonDown);                 
	
	static void drawSquareButton (Graphics& g,
                                  const float x, const float y,
                                  const float width, const float height,
                                  const Colour& colour,
                                  const float outlineThickness,
                                  const float cornerSize) throw();
	//============================================================
	void drawScrollbarButton (Graphics& g,
							  ScrollBar& scrollbar,
							  int width, int height,
							  int buttonDirection,
							  bool isScrollbarVertical,
							  bool isMouseOverButton,
							  bool isButtonDown);
	
	void drawScrollbar (Graphics& g,
						ScrollBar& scrollbar,
						int x, int y,
						int width, int height,
						bool isScrollbarVertical,
						int thumbStartPosition,
						int thumbSize,
						bool isMouseOver,
						bool isMouseDown);

	//============================================================
	virtual Button* createDocumentWindowButton (int buttonType);
	
	virtual void drawDocumentWindowTitleBar (DocumentWindow& window,
                                             Graphics& g, int w, int h,
                                             int titleSpaceX, int titleSpaceW,
                                             const Image* icon,
                                             bool drawTitleTextOnLeft);
		
	//============================================================
	virtual void drawTableHeaderBackground (Graphics& g, TableHeaderComponent& header);
	
    virtual void drawTableHeaderColumn (Graphics& g, const String& columnName, int columnId,
                                        int width, int height,
                                        bool isMouseOver, bool isMouseDown,
                                        int columnFlags);

	//============================================================
	static DrawablePath createIcon (IconType icon, Colour colour);

	//============================================================
	
private:

};

class InvertedSliderLookAndFeel : public DecksLookAndFeel
{
public:
	
	juce_DeclareSingleton (InvertedSliderLookAndFeel, false);

//	virtual void drawLinearSliderThumb (Graphics& g,
//									    int x, int y,
//									    int width, int height,
//									    float sliderPos,
//									    float minSliderPos,
//									    float maxSliderPos,
//									    const Slider::SliderStyle style,
//										Slider& slider);
	
	virtual void drawFaderKnob(Graphics& g,
							   const Slider::SliderStyle style,
							   const float x,
							   const float y,
							   const float width,
							   const float height);
	
};

#endif //_DECKSLOOKANDFEEL__H_