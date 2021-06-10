//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeCoreHeader.hpp"
#import "maze-core/system/osx/MazeWindowDelegateOSX.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeWindowManager)
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeWindow)


//////////////////////////////////////////
// Class WindowDelegateOSX
//
//////////////////////////////////////////
@implementation WindowDelegateOSX

//////////////////////////////////////////
- (void)initialise
{
    [self setDelegate:self];
    
    // #TODO:
    //[self registerForDraggedTypes: [NSArray arrayWithObjects: NSFilenamesPboardType, nil] ];
}

//////////////////////////////////////////
- (void)setWindow:(Maze::WindowOSX*)_window
{
    m_window = _window;
}

//////////////////////////////////////////
- (void)windowDidResize:(NSNotification*)_notification
{
    m_window->processWindowDelegateOSXDidResize();
}

//////////////////////////////////////////
- (void)windowDidMove:(NSNotification*)_notification
{
    m_window->processWindowDelegateOSXDidMove();
}


//////////////////////////////////////////
- (void)windowWillClose:(NSNotification*)_notification
{
    m_window->processWindowDelegateOSXWillClose();
}

//////////////////////////////////////////
- (void)windowDidBecomeKey:(NSNotification*)_notification
{
    m_window->processWindowDelegateOSXDidBecomeKey();
}

//////////////////////////////////////////
- (void)windowDidResignKey:(NSNotification*)_notification
{
    m_window->processWindowDelegateOSXDidResignKey();
}

//////////////////////////////////////////
- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)_sender
{
    
    NSPasteboard* pboard;
    
    NSDragOperation sourceDragMask;
    
    sourceDragMask = [_sender draggingSourceOperationMask];
    pboard = [_sender draggingPasteboard];
    
    
    if ([[pboard types] containsObject:NSColorPboardType])
    {
        if (sourceDragMask & NSDragOperationGeneric)
        {
            return NSDragOperationGeneric;
        }
    }
    
    if ([[pboard types] containsObject:NSFilenamesPboardType])
    {
        
        if (sourceDragMask & NSDragOperationLink)
        {
            return NSDragOperationLink;
        }
        else 
        if (sourceDragMask & NSDragOperationCopy)
        {
            return NSDragOperationCopy;
        }
    }
    
    return NSDragOperationNone;
    
}

//////////////////////////////////////////
- (BOOL)performDragOperation:(id <NSDraggingInfo>)_sender
{
    
    NSPasteboard* pboard;
    NSDragOperation sourceDragMask;
    

    sourceDragMask = [_sender draggingSourceOperationMask];
    pboard = [_sender draggingPasteboard];
    
    
    if ([[pboard types] containsObject:NSFilenamesPboardType])
    {
        // #TODO:
        // NSArray* files = [pboard propertyListForType:NSFilenamesPboardType];

        // Depending on the dragging source and modifier keys,
        // the file data may be copied or linked
        if (sourceDragMask & NSDragOperationLink)
        {
            /*
            for (NSString* item in files)
                m_windowManager->processFileDragAndDrop([item UTF8String]);
            */
        }
        else
        {
            //[self addDataFromFiles:files];
        }
        
    }
    
    return YES;
    
}

//////////////////////////////////////////
- (IBAction)eventMainMenuItemClicked:(id)_sender
{
    // #TODO:
    /*
    NSMenuItem* item = _sender;
    m_windowManager->processMainMenuItemClicked([item tag]);
    */
}


@end
