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

#if JUCE_IOS

} // namespace drow
using drow::AudioPicker;

#include <MediaPlayer/MediaPlayer.h>

//==============================================================================
@interface JuceUIAudioPicker : MPMediaPickerController <MPMediaPickerControllerDelegate>
{
@private
    AudioPicker* owner;
}

@end

@implementation JuceUIAudioPicker

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

- (void) selectFile
{
    MPMediaPickerController *mediaPicker = [[MPMediaPickerController alloc] initWithMediaTypes: MPMediaTypeMusic];
    mediaPicker.delegate = self;
    mediaPicker.allowsPickingMultipleItems = YES;
    mediaPicker.prompt = @"Select Your Favourite Song!";
    [mediaPicker loadView];
    [self.navigationController presentViewController:mediaPicker animated:YES completion:nil]; 
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

+ (UIViewController*) topLevelViewController
{
    return [UIApplication sharedApplication].keyWindow.rootViewController;
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
    auto* controller = [JuceUIAudioPicker topLevelViewController];

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
                controller.modalPresentationStyle = UIModalPresentationPopover;
            
                CGRect fromFrame = CGRectMake (controller.view.center.x - 160.0f, 
                                               controller.view.center.y, 
                                               320.0f, 480.0f);

                if (! areaToPointTo.isEmpty())
                {
                    fromFrame = CGRectMake (areaToPointTo.getX(), areaToPointTo.getY(), 
                                            areaToPointTo.getWidth(), areaToPointTo.getHeight());
                }
                
                controller.popoverPresentationController.sourceView = controller.view;
                controller.popoverPresentationController.sourceRect = fromFrame;
                
                [controller presentViewController: audioPicker animated: YES completion: nil];
            }
        }
        else
        {            
            UIAlertController* alert = [UIAlertController
                                         alertControllerWithTitle:@"Error"
                                         message:@"Could not load music library"
                                         preferredStyle:UIAlertControllerStyleAlert];
            
            UIAlertAction* okAction = [UIAlertAction
                                        actionWithTitle:@"Ok"
                                        style:UIAlertActionStyleDefault
                                        handler:^(UIAlertAction* action) { /* ok button action */ }];
            
            [alert addAction:okAction];
            
            UIViewController *vc = [[[[UIApplication sharedApplication] delegate] window] rootViewController];
            [vc presentViewController:alert animated:YES completion:nil];
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
String AudioPicker::mpMediaItemToTitle (void* mpMediaItem)
{
    MPMediaItem* item = (MPMediaItem*) mpMediaItem;
    NSString* property = [item valueForProperty: MPMediaItemPropertyTitle];

    return CharPointer_UTF8 ([property UTF8String]);
}

String AudioPicker::mpMediaItemToArtist (void* mpMediaItem)
{
    MPMediaItem* item = (MPMediaItem*) mpMediaItem;
    NSString* property = [item valueForProperty: MPMediaItemPropertyArtist];

    return CharPointer_UTF8 ([property UTF8String]);
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
        for (uint64 i = 0; i < mediaItemCollection.count; ++i)
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
