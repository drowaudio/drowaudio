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

#if JUCE_IOS

} // namespace drow
using drow::AudioPicker;

//==============================================================================
@interface JuceUIAudioPicker : MPMediaPickerController <MPMediaPickerControllerDelegate,
                                                        UIPopoverControllerDelegate>
{
@private
    AudioPicker* owner;
    UIPopoverController* popover;
}

@property (nonatomic, assign) UIPopoverController* popover;

@end

@implementation JuceUIAudioPicker

@synthesize popover;

- (id) initWithOwner: (AudioPicker*) owner_
{
    if ((self = [super init]) != nil)
    {
        owner = owner_;

        self.delegate = self;
    }
    
    return self;
}

- (void) dealloc
{
    [super dealloc];
}

- (void) mediaPicker: (MPMediaPickerController*) mediaPicker didPickMediaItems: (MPMediaItemCollection*) mediaItemCollection
{
    owner->sendAudioPickerFinishedMessage (mediaPicker, mediaItemCollection);
}

- (void) mediaPickerDidCancel: (MPMediaPickerController*) mediaPicker
{
    self.delegate = nil;
    owner->sendAudioPickerCancelledMessage (mediaPicker);
}

- (void) setPopover: (UIPopoverController*) newPopover
{
    popover = newPopover;
    popover.delegate = self;
}

- (void) popoverControllerDidDismissPopover: (UIPopoverController*) popoverController
{
    [popoverController release];
}

+ (UIViewController*) topLevelViewController
{
    UIResponder* responder = ((UIView*) [[UIApplication sharedApplication].keyWindow.subviews objectAtIndex: 0]).nextResponder;
    
    if ([responder isKindOfClass: [UIViewController class]])
    {
        return (UIViewController*) responder;
    }
    
    return nil;
}
@end

namespace drow {

//==============================================================================
AudioPicker::AudioPicker()
{
}

AudioPicker::~AudioPicker()
{
}

//==============================================================================
void AudioPicker::show (bool allowMultipleSelection, Rectangle<int> areaToPointTo)
{
    UIViewController* controller = [JuceUIAudioPicker topLevelViewController];
    
    if (controller != nil)
    {
        JuceUIAudioPicker* audioPicker = [[JuceUIAudioPicker alloc] initWithOwner: this];
        
        if (audioPicker != nil)
        {
            [audioPicker retain];
            audioPicker.allowsPickingMultipleItems = allowMultipleSelection;
            
            if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
            {
                [controller presentViewController: audioPicker animated: YES completion: nil];
            }
            else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
            {
                UIPopoverController* popover = [[UIPopoverController alloc] initWithContentViewController: audioPicker];
                popover.popoverContentSize = CGSizeMake (320.0f, 480.0f);
                
                audioPicker.popover = popover;
                
                CGRect fromFrame = CGRectMake (controller.view.center.x - 160.0f, controller.view.center.y, 320.0f, 480.0f);
                
                if (! areaToPointTo.isEmpty())
                {
                    fromFrame = CGRectMake (areaToPointTo.getX(), areaToPointTo.getY(), areaToPointTo.getWidth(), areaToPointTo.getHeight());
                }
                
                [popover presentPopoverFromRect: fromFrame
                                         inView: controller.view 
                       permittedArrowDirections: UIPopoverArrowDirectionAny 
                                       animated: YES];
            }
        }
        else
        {
            UIAlertView* alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                            message: @"Could not load iPod library" 
                                                           delegate: nil
                                                  cancelButtonTitle: @"Ok" 
                                                  otherButtonTitles: nil];
            [alert show];
        }
    }
}

//==============================================================================
String AudioPicker::mpMediaItemToAvassetUrl (void* mpMediaItem)
{
    MPMediaItem* item = (MPMediaItem*) mpMediaItem;
    NSURL* location = [item valueForProperty: MPMediaItemPropertyAssetURL];
    
    return [location.absoluteString UTF8String];
}

//==============================================================================
void AudioPicker::addListener (Listener* const newListener)
{
    listeners.add (newListener);
}

void AudioPicker::removeListener (Listener* const listener)
{
    listeners.remove (listener);
}

//==============================================================================
void AudioPicker::sendAudioPickerFinishedMessage (void* picker, void* info)
{
    if (picker != nil)
    {
        JuceUIAudioPicker* audioPicker = (JuceUIAudioPicker*) picker;
        [audioPicker dismissViewControllerAnimated: YES completion: nil];
        [audioPicker release];

        MPMediaItemCollection* mediaItemCollection = (MPMediaItemCollection*) info;

        Array<void*> pickedMPMediaItems;
        for (int i = 0; i < mediaItemCollection.count; ++i)
        {
            MPMediaItem* mediaItem = [mediaItemCollection.items objectAtIndex: i];
            pickedMPMediaItems.add (mediaItem);
        }
        
        listeners.call (&Listener::audioPickerFinished, pickedMPMediaItems);
    }
}

void AudioPicker::sendAudioPickerCancelledMessage (void* picker)
{
    if (picker != nil)
    {
        JuceUIAudioPicker* audioPicker = (JuceUIAudioPicker*) picker;
        [audioPicker dismissViewControllerAnimated: YES completion: nil];
        [audioPicker release];

        listeners.call (&Listener::audioPickerCancelled);
    }
}

#endif