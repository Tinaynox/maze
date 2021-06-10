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
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/system/ios/MazeApplicationDelegateIOS.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeSystemManager)
#include <mach/mach.h>
#include <mach/mach_time.h>


//////////////////////////////////////////
namespace
{
    ApplicationDelegateIOS* g_delegateInstance;
}


//////////////////////////////////////////
@implementation ApplicationDelegateIOS

@synthesize m_updateTimer;
@synthesize m_scaleFactor;
@synthesize eventApplicationOpenURL;
@synthesize eventApplicationDidEnterBackground;
@synthesize eventApplicationWillEnterForeground;


//////////////////////////////////////////
+(ApplicationDelegateIOS*) GetInstance
{
    return g_delegateInstance;
}

//////////////////////////////////////////
- (BOOL)application:(UIApplication*)_application didFinishLaunchingWithOptions:(NSDictionary*)_launchOptions
{
    m_application = _application;
    m_launchOptions = _launchOptions;
    
    g_delegateInstance = self;
    
    
    m_displayLink = [CADisplayLink displayLinkWithTarget:self
                                   selector:@selector(doFrame:)];
                                   
    [self setDisplayLinkFrameInterval: 1];
    
    m_frameTime = Maze::F64(m_displayLink.duration * m_displayLink.frameInterval);
    [m_displayLink addToRunLoop:[NSRunLoop mainRunLoop]
                   forMode:NSDefaultRunLoopMode];
    
    
    
    m_updateTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1.0f / 60.0f)
                             target:self
                             selector:@selector(updateTimerCallback:)
                             userInfo:nil
                             repeats:YES];
    
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
    [[UIApplication sharedApplication] setIdleTimerDisabled: YES];
    
    id data = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"NSHighResolutionCapable"];
    if (data && [data boolValue] && 1)
        m_scaleFactor = [[UIScreen mainScreen] scale];
    else
        m_scaleFactor = 1;
    
    m_applicationForeground = true;
    
    
    m_systemManager = Maze::SystemManager::GetInstancePtr()->castRaw<Maze::SystemManagerIOS>();
    m_systemManager->processApplicationInit();
    
    return YES;
}

//////////////////////////////////////////
- (BOOL)application:(UIApplication*)_app openURL:(NSURL*)_url options:(NSDictionary<UIApplicationOpenURLOptionsKey, id> *)_options
{
    BOOL handled = FALSE;
    eventApplicationOpenURL(_app, _url, _options, handled);
    return handled;
}

//////////////////////////////////////////
- (BOOL)application:(UIApplication *)_application
                openURL:(NSURL *)_url
                sourceApplication:(NSString *)_sourceApplication
                annotation:(id)_annotation
{
    return FALSE;
}

- (void)openURL:(NSURL*)url options:(NSDictionary<NSString *, id> *)_options
                            completionHandler:(void (^ __nullable)(BOOL success))_completion
{
    
}

//////////////////////////////////////////
- (void)applicationDidEnterBackground:(UIApplication*)_application
{
    m_applicationForeground = false;
    
    m_backgroundTaskIdentifier =
        [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:^
         {
             if (m_backgroundTaskIdentifier != UIBackgroundTaskInvalid)
             {
                 Maze::Debug::Log("Background task timeout.");
                 [[UIApplication sharedApplication] endBackgroundTask: m_backgroundTaskIdentifier];
                 m_backgroundTaskIdentifier = UIBackgroundTaskInvalid;
             }
         }];
    
    m_displayLink.paused = YES;
    
    Maze::Debug::Log("Application Did Enter Background.");
    eventApplicationDidEnterBackground();
}

//////////////////////////////////////////
- (void)applicationWillEnterForeground:(UIApplication*)_application
{
    Maze::Debug::Log("Application Will Enter Foreground.");
    
    eventApplicationWillEnterForeground();
    
    m_applicationForeground = true;
    
    m_displayLink.paused = NO;
    
    if (m_backgroundTaskIdentifier != UIBackgroundTaskInvalid)
    {
        [[UIApplication sharedApplication] endBackgroundTask: m_backgroundTaskIdentifier];
        m_backgroundTaskIdentifier = UIBackgroundTaskInvalid;
    }
}

//////////////////////////////////////////
-(void)setDisplayLinkFrameInterval:(NSInteger)_frameInterval
{
    m_displayLink.frameInterval = _frameInterval;
    m_frameTime = Maze::F64( m_displayLink.duration * m_displayLink.frameInterval );
}

//////////////////////////////////////////
-(void)ensureForeground
{
    // #TODO:
    /*
    while (!m_applicationForeground)
    {
        Maze::SleepCurrentThread(200);
    }
    */
}

//////////////////////////////////////////
-(bool)getApplicationForeground
{
    return m_applicationForeground;
}

//////////////////////////////////////////
-(UIApplication*)getApplication
{
    return m_application;
}

//////////////////////////////////////////
-(NSDictionary*)getLaunchOptions
{
    return m_launchOptions;
}

//////////////////////////////////////////
-(void)doFrame:(id)data
{
    if (!m_applicationForeground)
        return;
    
#if (1)
    static Maze::F64 s_timeBank = 0;
    
    s_timeBank -= m_frameTime;
    
    if (s_timeBank > 0)
        return;
    
    s_timeBank = 0;
    Maze::U64 start = mach_absolute_time();
    
    
    m_systemManager->processApplicationFrame();
    
    Maze::U64 end = mach_absolute_time();
    MAZE_DEBUG_BP_IF(end < start);
    Maze::U64 elapsed = end - start;
    
    static mach_timebase_info_data_t sTimebaseInfo;
    if (sTimebaseInfo.denom == 0)
        (void)mach_timebase_info(&sTimebaseInfo);
    
    s_timeBank = (elapsed * sTimebaseInfo.numer / sTimebaseInfo.denom) / 1000000000.0;
    
    if (s_timeBank > m_frameTime)
        s_timeBank = m_frameTime + fmod(s_timeBank, m_frameTime);
#else

    m_systemManager->notifyApplicationFrame();
    
#endif

}

//////////////////////////////////////////
- (void)updateTimerCallback:(id)sender
{
    if (m_applicationForeground)
        return;
    
    m_systemManager->processApplicationFrame();
}

//////////////////////////////////////////
- (void)shutdown
{
    exit(0);
}

//////////////////////////////////////////
- (void)applicationWillTerminate:(NSNotification*)_notification
{
    m_systemManager->processApplicationWillTerminate();
}

//////////////////////////////////////////
- (void)dealloc
{
    if (m_updateTimer)
    {
        [m_updateTimer invalidate];
        m_updateTimer = nil;
    }
    
    [super dealloc];
}

@end


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    void EnsureApplicationForeground()
    {
        [[ApplicationDelegateIOS GetInstance] ensureForeground];
    }
    
    //////////////////////////////////////////
    bool IsApplicationForeground()
    {
        return [[ApplicationDelegateIOS GetInstance] getApplicationForeground];
    }
}
