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
#pragma once
#if (!defined(_MazeApplicationDelegateIOS_hpp_))
#define _MazeApplicationDelegateIOS_hpp_


//////////////////////////////////////////
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>


//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    class SystemManagerIOS;

    //////////////////////////////////////////
    void EnsureApplicationForeground();
    
    //////////////////////////////////////////
    bool IsApplicationForeground();
    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
// Class ApplicationDelegateIOS
//
//////////////////////////////////////////
@interface ApplicationDelegateIOS
    : UIResponder<UIApplicationDelegate>
{
    Maze::SystemManagerIOS* m_systemManager;
    NSTimer* m_updateTimer;
    CGFloat m_scaleFactor;
    CADisplayLink* m_displayLink;
    UIBackgroundTaskIdentifier m_backgroundTaskIdentifier;
    bool m_applicationForeground;
    UIApplication* m_application;
    NSDictionary* m_launchOptions;
    
    
    Maze::F64 m_frameTime;
    
    Maze::MultiDelegate<UIApplication*, NSURL*, NSDictionary<UIApplicationOpenURLOptionsKey, id>*, BOOL&> eventApplicationOpenURL;
    Maze::MultiDelegate<> eventApplicationDidEnterBackground;
    Maze::MultiDelegate<> eventApplicationWillEnterForeground;
}
////////////////////////////////////
+(ApplicationDelegateIOS*) GetInstance;

////////////////////////////////////////
-(void)doFrame:(id)_data;

////////////////////////////////////
- (void)updateTimerCallback:(id)_sender;

////////////////////////////////////
- (void)shutdown;

////////////////////////////////////
- (void)applicationWillTerminate:(NSNotification*)_notification;

////////////////////////////////////////
-(void)setDisplayLinkFrameInterval:(NSInteger)_frameInterval;

////////////////////////////////////////
-(void)ensureForeground;

////////////////////////////////////////
-(bool)getApplicationForeground;

////////////////////////////////////////
-(UIApplication*)getApplication;

////////////////////////////////////////
-(NSDictionary*)getLaunchOptions;

////////////////////////////////////
@property (retain) NSTimer* m_updateTimer;
@property (nonatomic) CGFloat m_scaleFactor;
@property (nonatomic) Maze::MultiDelegate<UIApplication*, NSURL*, NSDictionary<UIApplicationOpenURLOptionsKey, id>*, BOOL&>& eventApplicationOpenURL;
@property (nonatomic) Maze::MultiDelegate<>& eventApplicationDidEnterBackground;
@property (nonatomic) Maze::MultiDelegate<>& eventApplicationWillEnterForeground;

@end

#endif // _MazeApplicationDelegateIOS_h_
//////////////////////////////////////////
