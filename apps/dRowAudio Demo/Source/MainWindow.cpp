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

#include "MainWindow.h"
#include "MainComponent.h"

//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::darkgrey,
                      DocumentWindow::allButtons)
{
    LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);
    LookAndFeel& laf = getLookAndFeel();
    laf.setColour (TextButton::buttonColourId, Colours::lightgrey);
    laf.setColour (TextButton::buttonOnColourId, Colours::grey);

    setContentOwned (new MainComponent, false);
    centreWithSize (800, 600);
    setVisible (true);
    setResizable (true, true);
}

MainAppWindow::~MainAppWindow()
{
}

void MainAppWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}
