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
#import "maze-core/system/ios/MazeViewControllerIOS.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeInputManager)
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeWindow)


//////////////////////////////////////////
@implementation ViewControllerIOS


//////////////////////////////////////////
- (id)init
{
    self = [super init];
    
    if ([self respondsToSelector:@selector(setNeedsStatusBarAppearanceUpdate)])
    {
        // iOS 7
        [self performSelector:@selector(setNeedsStatusBarAppearanceUpdate)];
    }
    else
    {
        // iOS 6
        //[[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationSlide];
    }
    
    return self;
}


//////////////////////////////////////////
- (BOOL)prefersStatusBarHidden
{
    return YES;
}


//////////////////////////////////////////
- (void)setWindow:(Maze::WindowIOS*)_window
{
    m_window = _window;
}

//////////////////////////////////////////
- (void)loadView
{
    
}

//////////////////////////////////////////
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)_toInterfaceOrientation
{
    return (_toInterfaceOrientation & [self supportedInterfaceOrientations]);
}

//////////////////////////////////////////
- (BOOL)shouldAutorotate
{
    return YES;
}

//////////////////////////////////////////
- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)_fromInterfaceOrientation
{
    [UIView setAnimationsEnabled:YES];
}

//////////////////////////////////////////
- (NSUInteger)supportedInterfaceOrientations
{
    // #TODO:
    return UIInterfaceOrientationMaskLandscape /*| UIInterfaceOrientationMaskPortrait*/;
}

//////////////////////////////////////////
- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    
     [coordinator animateAlongsideTransition:^(id<UIViewControllerTransitionCoordinatorContext> context)
     {
         // UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
    
     } completion:^(id<UIViewControllerTransitionCoordinatorContext> context)
     {
        // [UIView setAnimationsEnabled:YES];
         m_window->processOrientationChanged();
         
     }];
    // [UIView setAnimationsEnabled:NO];
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
}


@end
