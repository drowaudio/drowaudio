/*
  ==============================================================================

  This file is part of the dRowAudio JUCE module
  Copyright 2004-13 by dRowAudio.

  ------------------------------------------------------------------------------

  dRowAudio is provided under the terms of The MIT License (MIT):

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ==============================================================================
*/

#ifndef DROWAUDIO_COLUMNFILEBROWSERLOOKANDFEEL_H
#define DROWAUDIO_COLUMNFILEBROWSERLOOKANDFEEL_H

//==================================================================================
/** The LookAndFeel of the ColumnFileBrowser.
    This is not intended for public use.
 */
class ColumnFileBrowserLookAndFeel : public juce::LookAndFeel_V2
{
public:
    //==================================================================================
    ColumnFileBrowserLookAndFeel()
    {
        setColour (juce::ListBox::backgroundColourId, juce::Colour::greyLevel (0.2f));
        setColour (juce::DirectoryContentsDisplayComponent::highlightColourId, juce::Colour::greyLevel (0.9f));
        setColour (juce::DirectoryContentsDisplayComponent::textColourId, juce::Colour::greyLevel (0.9f));

        // scrollbars
        setColour (juce::ScrollBar::backgroundColourId, juce::Colour::greyLevel (0.5f));
        setColour (juce::ScrollBar::thumbColourId, juce::Colour::greyLevel (0.8f));
        setColour (juce::ScrollBar::trackColourId, juce::Colour::greyLevel (0.3f));
    }

    ~ColumnFileBrowserLookAndFeel() override
    {
    }

    void layoutFileBrowserComponent (juce::FileBrowserComponent& browserComp,
                                     juce::DirectoryContentsDisplayComponent* fileListComponent,
                                     juce::FilePreviewComponent* /*previewComp*/,
                                     juce::ComboBox* /*currentPathBox*/,
                                     juce::TextEditor* /*filenameBox*/,
                                     juce::Button* /*goUpButton*/) override
    {
        int w = browserComp.getWidth();
        int x = 2;
        int y = 2;

        juce::Component* const listAsComp = dynamic_cast <juce::Component*> (fileListComponent);
        listAsComp->setBounds (x, y, w, browserComp.getHeight() - y);// - bottomSectionHeight);

        y = listAsComp->getBottom();
    }

    void drawFileBrowserRow (juce::Graphics& g, int width, int height,
                             const juce::File& /*file*/, const juce::String& filename, juce::Image* icon,
                             const juce::String& fileSizeDescription, const juce::String& fileTimeDescription,
                             bool isDirectory, bool isItemSelected, int /*itemIndex*/,
                             juce::DirectoryContentsDisplayComponent&) override
    {
        if (isItemSelected)
            g.fillAll (findColour (juce::DirectoryContentsDisplayComponent::highlightColourId));
        //        else if ((itemIndex % 2) != 0)
        //            g.fillAll (findColour(ListBox::backgroundColourId).withBrightness(0.95f));
        //        else if ((itemIndex % 2) == 0)
        //            g.fillAll (browserColumnRow.withAlpha(0.05f));

        //        g.setColour (findColour (DirectoryContentsDisplayComponent::textColourId));
        //        if (isItemSelected)
        //            g.setColour (findColour (DirectoryContentsDisplayComponent::textColourId).contrasting(1.0f));
        //        g.setFont (height * 0.7f);
        //
        //        Image im;
        //        if (icon != 0 && icon->isValid())
        //            im = *icon;
        //
        //        if (im.isNull())
        //            im = isDirectory ? getDefaultFolderImage()
        //            : getDefaultDocumentFileImage();
        //
        //        const int x = 32;
        //
        //        if (im.isValid())
        //        {
        //            g.drawImageWithin (im, 2, 2, x - 4, height - 4,
        //                               RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
        //                               false);
        //        }
        const int x = 32;
        g.setColour (juce::Colours::black);

        if (icon != nullptr && icon->isValid())
        {
            g.drawImageWithin (*icon, 2, 2, x - 4, height - 4,
                               juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize,
                               false);
        }
        else
        {
            const juce::Drawable* d = isDirectory ? getDefaultFolderImage()
            : getDefaultDocumentFileImage();

            if (d != nullptr)
                d->drawWithin (g, juce::Rectangle<float> (2.0f, 2.0f, x - 4.0f, height - 4.0f),
                               juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize, 1.0f);
        }

        g.setColour (findColour (juce::DirectoryContentsDisplayComponent::textColourId));
        if (isItemSelected)
            g.setColour (findColour (juce::DirectoryContentsDisplayComponent::highlightColourId).contrasting(1.0f));

        g.setFont (height * 0.7f);

        if (width > 450 && ! isDirectory)
        {
            const int sizeX = juce::roundToInt (width * 0.7f);
            const int dateX = juce::roundToInt (width * 0.8f);

            g.drawFittedText (filename,
                              x, 0, sizeX - x, height,
                              juce::Justification::centredLeft, 1);

            g.setFont (height * 0.5f);
            g.setColour (juce::Colours::darkgrey);

            if (! isDirectory)
            {
                g.drawFittedText (fileSizeDescription,
                                  sizeX, 0, dateX - sizeX - 8, height,
                                  juce::Justification::centredRight, 1);

                g.drawFittedText (fileTimeDescription,
                                  dateX, 0, width - 8 - dateX, height,
                                  juce::Justification::centredRight, 1);
            }
        }
        else
        {
            g.drawFittedText (filename,
                              x, 0, (int) (width - x - (height * 0.7f)), height,
                              juce::Justification::centredLeft, 1);

        }

        // draw directory triangles
        if (isDirectory)
        {
            int diameter = juce::roundToInt (height * 0.5f);
            juce::Path p;
            p.addTriangle (width - (height * 0.2f), height * 0.5f,
                           (float) (width - diameter), height * 0.3f,
                           (float) (width - diameter), height * 0.7f);
            g.setColour (findColour (juce::ScrollBar::thumbColourId));
            g.fillPath (p);

            g.setColour (juce::Colour (0x80000000));
            g.strokePath (p, juce::PathStrokeType (0.5f));
        }
    }

    //============================================================
    void drawScrollbarButton (juce::Graphics& g,
                              juce::ScrollBar& scrollbar,
                              int width, int height,
                              int buttonDirection,
                              bool /*isScrollbarVertical*/,
                              bool /*isMouseOverButton*/,
                              bool isButtonDown) override
    {
        juce::Path p;

        if (buttonDirection == 0)
            p.addTriangle (width * 0.5f, height * 0.2f,
                           width * 0.1f, height * 0.7f,
                           width * 0.9f, height * 0.7f);
        else if (buttonDirection == 1)
            p.addTriangle (width * 0.8f, height * 0.5f,
                           width * 0.3f, height * 0.1f,
                           width * 0.3f, height * 0.9f);
        else if (buttonDirection == 2)
            p.addTriangle (width * 0.5f, height * 0.8f,
                           width * 0.1f, height * 0.3f,
                           width * 0.9f, height * 0.3f);
        else if (buttonDirection == 3)
            p.addTriangle (width * 0.2f, height * 0.5f,
                           width * 0.7f, height * 0.1f,
                           width * 0.7f, height * 0.9f);

        if (isButtonDown)
            g.setColour (scrollbar.findColour (juce::ScrollBar::thumbColourId).contrasting (0.2f));
        else
            g.setColour (scrollbar.findColour (juce::ScrollBar::thumbColourId));

        g.fillPath (p);

        g.setColour (juce::Colour (0x80000000));
        g.strokePath (p, juce::PathStrokeType (0.5f));
    }

    void drawScrollbar (juce::Graphics& g,
                        juce::ScrollBar& scrollbar,
                        int x, int y,
                        int width, int height,
                        bool isScrollbarVertical,
                        int thumbStartPosition,
                        int thumbSize,
                        bool /*isMouseOver*/,
                        bool /*isMouseDown*/) override
    {
        g.fillAll (scrollbar.findColour (juce::ScrollBar::backgroundColourId));

        juce::Path slotPath, thumbPath;

        float gx1 = 0.0f, gy1 = 0.0f, gx2 = 0.0f, gy2 = 0.0f;

        if (isScrollbarVertical)
        {
            slotPath.addRoundedRectangle ((float) x,
                                          (float) y,
                                          (float) width,
                                          (float) height,
                                          width * 0.5f);

            if (thumbSize > 0)
                thumbPath.addRoundedRectangle ((float) x,
                                               (float) thumbStartPosition,
                                               (float) width,
                                               (float) thumbSize,
                                               width * 0.5f);
            gx1 = (float) x;
            gx2 = x + width * 0.7f;
        }
        else
        {
            slotPath.addRoundedRectangle ((float) x,
                                          (float) y,
                                          (float) width,
                                          (float) height,
                                          height * 0.5f);

            if (thumbSize > 0)
                thumbPath.addRoundedRectangle ((float) thumbStartPosition,
                                               (float) y,
                                               (float) thumbSize,
                                               (float) height,
                                               height * 0.5f);
            gy1 = (float) y;
            gy2 = y + height * 0.7f;
        }

        const juce::Colour thumbColour (scrollbar.findColour (juce::ScrollBar::thumbColourId));
        juce::Colour trackColour1, trackColour2;

        trackColour1 = scrollbar.findColour (juce::ScrollBar::trackColourId);
        trackColour2 = trackColour1.overlaidWith (juce::Colour (0x19000000));

        g.setGradientFill (juce::ColourGradient (trackColour1, gx1, gy1,
                                           trackColour2, gx2, gy2, false));
        g.fillPath (slotPath);

        if (isScrollbarVertical)
        {
            gx1 = x + width * 0.6f;
            gx2 = (float) x + width;
        }
        else
        {
            gy1 = y + height * 0.6f;
            gy2 = (float) y + height;
        }

        g.setGradientFill (juce::ColourGradient (juce::Colours::transparentBlack,gx1, gy1,
                                           juce::Colour (0x19000000), gx2, gy2, false));
        g.fillPath (slotPath);

        g.setColour (thumbColour);
        g.fillPath (thumbPath);

        //    g.setGradientFill (ColourGradient (Colour (0x10000000), gx1, gy1,
        //                                       Colours::transparentBlack, gx2, gy2, false));
        //
        //    g.saveState();
        //
        //    if (isScrollbarVertical)
        //        g.reduceClipRegion (x + width / 2, y, width, height);
        //    else
        //        g.reduceClipRegion (x, y + height / 2, width, height);
        //
        //    g.fillPath (thumbPath);
        //    g.restoreState();

        //    g.setColour (Colour (0x4c000000));
        //    g.setColour (thumbColour.brighter(0.5));
        //    g.strokePath (thumbPath, PathStrokeType (1.0f));
    }

    //============================================================
    void drawCornerResizer (juce::Graphics& g,
                            int w, int h,
                            bool /*isMouseOver*/,
                            bool /*isMouseDragging*/) override
    {
        const float lineThickness = 1.0f;//jmin (w, h) * 0.075f;
        const float xGap = w / 3.0f;
        const float yGap = h * 0.25f;

        g.setColour (findColour (juce::ScrollBar::backgroundColourId));
        g.fillAll();

        g.setColour (findColour (juce::ScrollBar::thumbColourId));
        g.drawLine (xGap + lineThickness * 0.5f, yGap, xGap + lineThickness * 0.5f, h - yGap, lineThickness);
        g.drawLine ((2 * xGap) - lineThickness * 0.5f, yGap, (2 * xGap) - lineThickness * 0.5f, h - yGap, lineThickness);
    }
};

#endif //DROWAUDIO_COLUMNFILEBROWSERLOOKANDFEEL_H
