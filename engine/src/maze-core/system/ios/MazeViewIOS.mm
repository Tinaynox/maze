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
#import "maze-core/system/ios/MazeViewIOS.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeInputManager)
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeWindow)


//////////////////////////////////////////
@interface ViewIOS()

@property (nonatomic) NSMutableArray* viewTouches;

@end


//////////////////////////////////////////
@implementation ViewIOS


//////////////////////////////////////////
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

//////////////////////////////////////////
- (id)initWithFrame:(CGRect)f  andScaleFactor:(CGFloat)_scaleFactor;
{
    self = [super initWithFrame:f];
    self.contentScaleFactor = _scaleFactor;


    self.contentScaleFactor = [[UIScreen mainScreen] scale];
    
    if (self)
    {
        MAZE_TODO;
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                                     [NSNumber numberWithBool:FALSE], 
                                                     kEAGLDrawablePropertyRetainedBacking,
                                                     kEAGLColorFormatRGBA8, 
                                                     kEAGLDrawablePropertyColorFormat,
                                                     nil];
        
        self.multipleTouchEnabled = true;
        self.userInteractionEnabled = true;
        
        
        self.viewTouches = [[NSMutableArray arrayWithCapacity:16] retain];
        
         self.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleBottomMargin |
                                 UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleRightMargin |
                                 UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleBottomMargin;
        
        
        // #TODO:
        // m_nullTextField = [[UITextField alloc] initWithFrame:CGRectZero];
        // m_nullTextField.delegate = self;
        // [self addSubview: m_nullTextField];
        
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                              selector:@selector(keyboardWillShow:)
                                              name:UIKeyboardWillShowNotification
                                              object:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                              selector:@selector(keyboardWillHide:)
                                              name:UIKeyboardWillHideNotification
                                              object:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                              selector:@selector(keyboardDidShow:)
                                              name:UIKeyboardDidShowNotification
                                              object:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                              selector:@selector(currentInputModeDidChange:)
                                              name:UITextInputCurrentInputModeDidChangeNotification
                                              object:nil];

    }
    
    return self;
}

//////////////////////////////////////////
- (UITextInputMode *) textInputMode 
{
    for (UITextInputMode* tim in [UITextInputMode activeInputModes]) 
    {
        if (    [tim.primaryLanguage isEqualToString:@"en_US"]
           ||   [tim.primaryLanguage isEqualToString:@"en-US"] ) 
            return tim;
    }
    return [super textInputMode];
}

//////////////////////////////////////////
- (void)keyboardWillShow:(NSNotification*)note 
{
    // NSDictionary* userInfo = [note userInfo];
    // CGSize kbSize = [[userInfo objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue].size;
    // NSLog(@"Keyboard Height: %f Width: %f", kbSize.height, kbSize.width);
}

//////////////////////////////////////////
- (void)keyboardWillHide:(NSNotification *)notification 
{
    
}

//////////////////////////////////////////
- (void)keyboardDidShow:(NSNotification *)notification
{
    // NSString* currentLanguage = [UITextInputMode currentInputMode].primaryLanguage;
}

//////////////////////////////////////////
- (void)currentInputModeDidChange:(NSNotification *)notification
{
    
}

//////////////////////////////////////////
- (void)drawRect:(CGRect)rect
{
    
}

//////////////////////////////////////////
- (void)layoutSubviews
{
    eventLayoutSubviews(self);
}

//////////////////////////////////////////
- (void)setWindow:(Maze::WindowIOS*)_window
{
    m_window = _window;
}


//////////////////////////////////////////
-(void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)_event
{
    for (UITouch* touch in touches)
    {
        NSUInteger index = [self.viewTouches indexOfObject:[NSNull null]];
        if (index != NSNotFound)
        {
            [self.viewTouches replaceObjectAtIndex:index withObject:touch];
        }
        else
        {
            [self.viewTouches addObject:touch];
            index = [self.viewTouches count] - 1;
        }
        
        CGPoint pos = [touch locationInView: self];
        
        
        Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
        Maze::InputEventTouchData eventData;
        eventData.type = Maze::InputEventTouchType::Press;
        eventData.index = static_cast<Maze::S32>(index);
        eventData.x = pos.x * self.contentScaleFactor;
        eventData.y = pos.y * self.contentScaleFactor;
        inputManager->generateInputEvent(eventData);
    }
    
}

//////////////////////////////////////////
-(void) touchesMoved:(NSSet*)_touches withEvent:(UIEvent*)_event
{
    for (UITouch* touch in _touches)
    {
        NSUInteger index = [self.viewTouches indexOfObject:touch];
        if (index != NSNotFound)
        {
            CGPoint pos = [touch locationInView: self];
            
            Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
            Maze::InputEventTouchData eventData;
            eventData.type = Maze::InputEventTouchType::Move;
            eventData.index = static_cast<Maze::S32>(index);
            eventData.x = pos.x * self.contentScaleFactor;
            eventData.y = pos.y * self.contentScaleFactor;
            inputManager->generateInputEvent(eventData);
            
        }
        
    }
        
}

//////////////////////////////////////////
-(void) touchesEnded:(NSSet*)_touches withEvent:(UIEvent*)_event
{
    for (UITouch* touch in _touches)
    {
        NSUInteger index = [self.viewTouches indexOfObject:touch];
        if (index != NSNotFound)
        {
            CGPoint pos = [touch locationInView: self];            
            
            Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
            Maze::InputEventTouchData eventData;
            eventData.type = Maze::InputEventTouchType::Release;
            eventData.index = static_cast<Maze::S32>(index);
            eventData.x = pos.x * self.contentScaleFactor;
            eventData.y = pos.y * self.contentScaleFactor;
            inputManager->generateInputEvent(eventData);
            
        
            
            [self.viewTouches replaceObjectAtIndex:index withObject:[NSNull null]];
        }
        
    }
}

//////////////////////////////////////////
-(void) touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)_event
{
    [self touchesEnded:touches withEvent:_event];
}

//////////////////////////////////////////
-(BOOL)canBecomeFirstResponder
{
    return true;
}

//////////////////////////////////////////
- (BOOL)hasText
{
    return FALSE;
}

//////////////////////////////////////////
- (void)deleteBackward
{
    MAZE_TODO;
    /*
    Maze::EventManagerPtr const& eventManager = Maze::Engine::GetInstancePtr()->getEventManager();
    
    Maze::SystemEventKeyboardData eventData;
    eventData.type = Maze::SystemEventKeyboardType::KeyDown;
    eventData.data = 0;
    eventData.keyCode = Maze::KeyCode::Backspace;
    strcpy(eventData.textUtf8, "\b");
    eventData.virtualCode = 0;
    eventManager->generateSystemEventKeyboard(eventData);
     */
       
}

//////////////////////////////////////////
- (void)insertText:(NSString*)_text
{
    // #TODO:
    /*
    Maze::EventManagerPtr const& eventManager = Maze::Engine::GetInstancePtr()->getEventManager();
    
    // Convert the NSString to UTF-8
    Maze::String utf8 = [_text UTF8String];
    
    Maze::String::iterator it = utf8.begin();
    utf8::next(it, utf8.end());
    
    do
    {
        Size symbols = it - utf8.begin();
        
        if (symbols > 0)
        {
            Maze::SystemEventKeyboardData eventData;
            eventData.type = Maze::SystemEventKeyboardType::KeyDown;
            eventData.data = 0;
            
            if ("\n" == utf8)
            {
                eventData.keyCode = Maze::KeyCode::Enter;
            }
            else
            {
                eventData.keyCode = Maze::KeyCode::None;
            }
            
            strcpy(eventData.textUtf8, utf8.substr(0, symbols).c_str());
            
            eventData.virtualCode = 0;
            eventManager->generateSystemEventKeyboard(eventData);
        }
        
        if (it != utf8.end())
            utf8::next(it, utf8.end());
    }
    while (it != utf8.end());
    */
}

//////////////////////////////////////////
-(void)showVirtualKeyboard
{
    [self becomeFirstResponder];
}

//////////////////////////////////////////
-(void)hideVirtualKeyboard
{
    [self resignFirstResponder];
}

@end
