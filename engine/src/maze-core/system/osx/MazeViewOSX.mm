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
#import "maze-core/system/osx/MazeViewOSX.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeInputManager)
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeWindow)


//////////////////////////////////////////
// ViewOSX
//
//////////////////////////////////////////
@implementation ViewOSX


//////////////////////////////////////////
- (id)initWithFrame:(NSRect)_f;
{
    self = [super initWithFrame: _f];
    
    if (self)
    {
        
        [[NSNotificationCenter defaultCenter] addObserver: self
                                              selector: @selector(_surfaceNeedsUpdate:)
                                              name: NSViewGlobalFrameDidChangeNotification
                                              object: self];
        
        [self populateKeyConversion];
        
        m_lastLShiftState = false;
        m_lastRShiftState = false;
        m_lastLControlState = false;
        m_lastRControlState = false;
        m_lastLAlternateState = false;
        m_lastRAlternateState = false;
        m_lastLCommandState = false;
        m_lastRCommandState = false;
    }
    
    
    return self;
}

//////////////////////////////////////////
- (void)setWindow:(Maze::WindowOSX*)_window
{
    m_window = _window;
}

//////////////////////////////////////////
-(void)drawRect:(NSRect)_dirtyRect
{
    if (m_window)
    {
        if (m_window->getParams()->flags & Maze::WindowStyleFlags::Background)
        {
            [[NSColor blackColor] setFill];
            NSRectFill(_dirtyRect);
        }
        else
        {
            [[NSColor clearColor] set];
            NSRectFill(_dirtyRect);
        }
    }
}

//////////////////////////////////////////
- (void)mouseDown:(NSEvent*)_event
{
    NSPoint pos = [_event locationInWindow];
    
    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    Maze::InputEventMouseData eventData;
    eventData.type = Maze::InputEventMouseType::ButtonDown;
    eventData.x = pos.x;
    eventData.y =  pos.y;
    eventData.buttonId = 0;
    inputManager->generateInputEvent( eventData );
}

//////////////////////////////////////////
- (void)mouseUp:(NSEvent *)_event
{
    NSPoint pos = [_event locationInWindow];
    
    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    Maze::InputEventMouseData eventData;
    eventData.type = Maze::InputEventMouseType::ButtonUp;
    eventData.x = pos.x;
    eventData.y = pos.y;
    eventData.buttonId = 0;
    inputManager->generateInputEvent( eventData );
}

//////////////////////////////////////////
- (void)rightMouseDown: (NSEvent*)_event
{
    NSPoint pos = [_event locationInWindow];
    
    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    Maze::InputEventMouseData eventData;
    eventData.type = Maze::InputEventMouseType::ButtonDown;
    eventData.x = pos.x;
    eventData.y = pos.y;
    eventData.buttonId = 1;
    inputManager->generateInputEvent( eventData );
}

//////////////////////////////////////////
- (void)rightMouseUp:(NSEvent*)_event
{
    NSPoint pos = [_event locationInWindow];
    
    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    Maze::InputEventMouseData eventData;
    eventData.type = Maze::InputEventMouseType::ButtonUp;
    eventData.x = pos.x;
    eventData.y = pos.y;
    eventData.buttonId = 1;
    inputManager->generateInputEvent( eventData );
}

//////////////////////////////////////////
-(void)mouseMoved:(NSEvent*)_event
{
    NSPoint pos = [_event locationInWindow];
    
    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    Maze::InputEventMouseData eventData;
    eventData.type = Maze::InputEventMouseType::Move;
    eventData.x = pos.x;
    eventData.y = pos.y;
    inputManager->generateInputEvent( eventData );
}

//////////////////////////////////////////
- (void)mouseDragged:(NSEvent*)_event
{
    NSPoint pos = [_event locationInWindow];
    
    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    Maze::InputEventMouseData eventData;
    eventData.type = Maze::InputEventMouseType::Move;
    eventData.x = pos.x;
    eventData.y = pos.y;
    eventData.buttonId = 0;
    inputManager->generateInputEvent(eventData);
}

//////////////////////////////////////////
- (void)rightMouseDragged:(NSEvent *)_event
{
    NSPoint pos = [_event locationInWindow];
    
    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    Maze::InputEventMouseData eventData;
    eventData.type = Maze::InputEventMouseType::Move;
    eventData.x = pos.x;
    eventData.y = pos.y;
    eventData.buttonId = 1;
    inputManager->generateInputEvent(eventData);
}

//////////////////////////////////////////
- (void)populateKeyConversion
{
    keyConversion.insert(std::pair<int, int>(53,   (int)Maze::KeyCode::Escape));
    keyConversion.insert(std::pair<int, int>(122,  (int)Maze::KeyCode::F1));
    keyConversion.insert(std::pair<int, int>(120,  (int)Maze::KeyCode::F2));
    keyConversion.insert(std::pair<int, int>(99,   (int)Maze::KeyCode::F3));
    keyConversion.insert(std::pair<int, int>(118,  (int)Maze::KeyCode::F4));
    keyConversion.insert(std::pair<int, int>(96,   (int)Maze::KeyCode::F5));
    keyConversion.insert(std::pair<int, int>(97,   (int)Maze::KeyCode::F6));
    keyConversion.insert(std::pair<int, int>(98,   (int)Maze::KeyCode::F7));
    keyConversion.insert(std::pair<int, int>(100,  (int)Maze::KeyCode::F8));
    keyConversion.insert(std::pair<int, int>(101,  (int)Maze::KeyCode::F9));
    keyConversion.insert(std::pair<int, int>(109,  (int)Maze::KeyCode::F10));
    keyConversion.insert(std::pair<int, int>(103,  (int)Maze::KeyCode::F11));
    keyConversion.insert(std::pair<int, int>(111,  (int)Maze::KeyCode::F12));
    keyConversion.insert(std::pair<int, int>(105,  (int)Maze::KeyCode::PrintScreen));
    keyConversion.insert(std::pair<int, int>(107,  (int)Maze::KeyCode::ScrollLock));
    keyConversion.insert(std::pair<int, int>(113,  (int)Maze::KeyCode::Pause));
    
    keyConversion.insert(std::pair<int, int>(50, (int)Maze::KeyCode::Grave));
    keyConversion.insert(std::pair<int, int>(18, (int)Maze::KeyCode::Number1));
    keyConversion.insert(std::pair<int, int>(19, (int)Maze::KeyCode::Number2));
    keyConversion.insert(std::pair<int, int>(20, (int)Maze::KeyCode::Number3));
    keyConversion.insert(std::pair<int, int>(21, (int)Maze::KeyCode::Number4));
    keyConversion.insert(std::pair<int, int>(23, (int)Maze::KeyCode::Number5));
    keyConversion.insert(std::pair<int, int>(22, (int)Maze::KeyCode::Number6));
    keyConversion.insert(std::pair<int, int>(26, (int)Maze::KeyCode::Number7));
    keyConversion.insert(std::pair<int, int>(28, (int)Maze::KeyCode::Number8));
    keyConversion.insert(std::pair<int, int>(25, (int)Maze::KeyCode::Number9));
    keyConversion.insert(std::pair<int, int>(29, (int)Maze::KeyCode::Number0));
    keyConversion.insert(std::pair<int, int>(27, (int)Maze::KeyCode::Minus));
    keyConversion.insert(std::pair<int, int>(24, (int)Maze::KeyCode::Equals));
    keyConversion.insert(std::pair<int, int>(51, (int)Maze::KeyCode::Backspace));
    
    keyConversion.insert(std::pair<int, int>(48, (int)Maze::KeyCode::Tab));
    keyConversion.insert(std::pair<int, int>(12, (int)Maze::KeyCode::Q));
    keyConversion.insert(std::pair<int, int>(13, (int)Maze::KeyCode::W));
    keyConversion.insert(std::pair<int, int>(14, (int)Maze::KeyCode::E));
    keyConversion.insert(std::pair<int, int>(15, (int)Maze::KeyCode::R));
    keyConversion.insert(std::pair<int, int>(17, (int)Maze::KeyCode::T));
    keyConversion.insert(std::pair<int, int>(16, (int)Maze::KeyCode::Y));
    keyConversion.insert(std::pair<int, int>(32, (int)Maze::KeyCode::U));
    keyConversion.insert(std::pair<int, int>(34, (int)Maze::KeyCode::I));
    keyConversion.insert(std::pair<int, int>(31, (int)Maze::KeyCode::O));
    keyConversion.insert(std::pair<int, int>(35, (int)Maze::KeyCode::P));
    keyConversion.insert(std::pair<int, int>(33, (int)Maze::KeyCode::LBracket));
    keyConversion.insert(std::pair<int, int>(30, (int)Maze::KeyCode::RBracket));
    keyConversion.insert(std::pair<int, int>(42, (int)Maze::KeyCode::Backslash));
    
    
    keyConversion.insert(std::pair<int, int>(57, (int)Maze::KeyCode::CapsLock));
    keyConversion.insert(std::pair<int, int>(0, (int)Maze::KeyCode::A));
    keyConversion.insert(std::pair<int, int>(1, (int)Maze::KeyCode::S));
    keyConversion.insert(std::pair<int, int>(2, (int)Maze::KeyCode::D));
    keyConversion.insert(std::pair<int, int>(3, (int)Maze::KeyCode::F));
    keyConversion.insert(std::pair<int, int>(5, (int)Maze::KeyCode::G));
    keyConversion.insert(std::pair<int, int>(4, (int)Maze::KeyCode::H));
    keyConversion.insert(std::pair<int, int>(38, (int)Maze::KeyCode::J));
    keyConversion.insert(std::pair<int, int>(40, (int)Maze::KeyCode::K));
    keyConversion.insert(std::pair<int, int>(37, (int)Maze::KeyCode::L));
    keyConversion.insert(std::pair<int, int>(41, (int)Maze::KeyCode::Semicolon));
    keyConversion.insert(std::pair<int, int>(39, (int)Maze::KeyCode::Apostrophe));
    keyConversion.insert(std::pair<int, int>(36, (int)Maze::KeyCode::Enter));
    
    
    keyConversion.insert(std::pair<int, int>(6, (int)Maze::KeyCode::Z));
    keyConversion.insert(std::pair<int, int>(7, (int)Maze::KeyCode::X));
    keyConversion.insert(std::pair<int, int>(8, (int)Maze::KeyCode::C));
    keyConversion.insert(std::pair<int, int>(9, (int)Maze::KeyCode::V));
    keyConversion.insert(std::pair<int, int>(11, (int)Maze::KeyCode::B));
    keyConversion.insert(std::pair<int, int>(45, (int)Maze::KeyCode::N));
    keyConversion.insert(std::pair<int, int>(46, (int)Maze::KeyCode::M));
    keyConversion.insert(std::pair<int, int>(43, (int)Maze::KeyCode::Comma));
    keyConversion.insert(std::pair<int, int>(47, (int)Maze::KeyCode::Period));
    keyConversion.insert(std::pair<int, int>(44, (int)Maze::KeyCode::Slash));
    
    
    keyConversion.insert(std::pair<int, int>(49, (int)Maze::KeyCode::Space));
    keyConversion.insert(std::pair<int, int>(110, (int)Maze::KeyCode::Apps));
    
    
    // Central
    keyConversion.insert(std::pair<int, int>(126, (int)Maze::KeyCode::Up));
    keyConversion.insert(std::pair<int, int>(125, (int)Maze::KeyCode::Down));
    keyConversion.insert(std::pair<int, int>(123, (int)Maze::KeyCode::Left));
    keyConversion.insert(std::pair<int, int>(124, (int)Maze::KeyCode::Right));

    
    keyConversion.insert(std::pair<int, int>(114, (int)Maze::KeyCode::Insert));
    keyConversion.insert(std::pair<int, int>(115, (int)Maze::KeyCode::Home));
    keyConversion.insert(std::pair<int, int>(116, (int)Maze::KeyCode::PageUp));
    keyConversion.insert(std::pair<int, int>(117, (int)Maze::KeyCode::Delete));
    keyConversion.insert(std::pair<int, int>(119, (int)Maze::KeyCode::End));
    keyConversion.insert(std::pair<int, int>(121, (int)Maze::KeyCode::PageDown));
    
    
    // Keypad
    keyConversion.insert(std::pair<int, int>(71, (int)Maze::KeyCode::NumLock));
    keyConversion.insert(std::pair<int, int>(82, (int)Maze::KeyCode::Numpad0));
    keyConversion.insert(std::pair<int, int>(83, (int)Maze::KeyCode::Numpad1));
    keyConversion.insert(std::pair<int, int>(84, (int)Maze::KeyCode::Numpad2));
    keyConversion.insert(std::pair<int, int>(85, (int)Maze::KeyCode::Numpad3));
    keyConversion.insert(std::pair<int, int>(86, (int)Maze::KeyCode::Numpad4));
    keyConversion.insert(std::pair<int, int>(87, (int)Maze::KeyCode::Numpad5));
    keyConversion.insert(std::pair<int, int>(88, (int)Maze::KeyCode::Numpad6));
    keyConversion.insert(std::pair<int, int>(89, (int)Maze::KeyCode::Numpad7));
    keyConversion.insert(std::pair<int, int>(91, (int)Maze::KeyCode::Numpad8));
    keyConversion.insert(std::pair<int, int>(92, (int)Maze::KeyCode::Numpad9));
    keyConversion.insert(std::pair<int, int>(69, (int)Maze::KeyCode::NumpadPlus));
    keyConversion.insert(std::pair<int, int>(78, (int)Maze::KeyCode::NumpadMinus));
    keyConversion.insert(std::pair<int, int>(65, (int)Maze::KeyCode::NumpadDecimal));
    keyConversion.insert(std::pair<int, int>(75, (int)Maze::KeyCode::NumpadDivide));
    keyConversion.insert(std::pair<int, int>(67, (int)Maze::KeyCode::NumpadMultiply));
    keyConversion.insert(std::pair<int, int>(76, (int)Maze::KeyCode::NumpadEnter));
    
    
    // Other keyboards
    keyConversion.insert(std::pair<int, int>(10, (int)Maze::KeyCode::MoreLess));
    
}

//////////////////////////////////////////
-(Maze::U32) collectInputEventKeyboardModifiers: (Maze::U32)_modifierFlags
{
    Maze::U32 modifiers = Maze::InputEventKeyboardModifiers::None;
    
    if (_modifierFlags & NSEventModifierFlagCapsLock)
        modifiers |= Maze::InputEventKeyboardModifiers::CapsLockOn;
    
    if (_modifierFlags & NSEventModifierFlagShift)
    {
        modifiers |= Maze::InputEventKeyboardModifiers::ShiftDown;
        
        if (_modifierFlags & 0x2)
            modifiers |= Maze::InputEventKeyboardModifiers::IsLeft;
        else
            modifiers |= Maze::InputEventKeyboardModifiers::IsRight;
            
    }
    
    if (_modifierFlags & NSEventModifierFlagControl)
    {
        modifiers |= Maze::InputEventKeyboardModifiers::ControlDown;
        
        if (_modifierFlags & 0x1)
            modifiers |= Maze::InputEventKeyboardModifiers::IsLeft;
        else
            modifiers |= Maze::InputEventKeyboardModifiers::IsRight;
    }
    
    if (_modifierFlags & NSEventModifierFlagOption)
    {
        modifiers |= Maze::InputEventKeyboardModifiers::AltDown;
        
        if (!(_modifierFlags & 0x40))
            modifiers |= Maze::InputEventKeyboardModifiers::IsLeft;
        else
            modifiers |= Maze::InputEventKeyboardModifiers::IsRight;
    }
    
    if (_modifierFlags & NSEventModifierFlagCommand)
    {
        modifiers |= Maze::InputEventKeyboardModifiers::CommandDown;
        
        if (!(_modifierFlags & 0x10))
            modifiers |= Maze::InputEventKeyboardModifiers::IsLeft;
        else
            modifiers |= Maze::InputEventKeyboardModifiers::IsRight;
    }
    
    // #TODO:
    //if (_modifierFlags & NSEventModifierFlagFunction)
    //{
    //    modifiers |= Maze::InputEventKeyboardModifiers::FnDown;
        
        //if (!(_modifierFlags & 0x4))
        //    modifiers |= Maze::InputEventKeyboardModifiers::IsLeft;
        //else
        //    modifiers |= Maze::InputEventKeyboardModifiers::IsRight;
    //}
    
    if (_modifierFlags & NSEventModifierFlagNumericPad)
        modifiers |= Maze::InputEventKeyboardModifiers::IsKeyPad;    
    
    
    return modifiers;
}

//////////////////////////////////////////
-(void)keyDown:(NSEvent*)_event
{
    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    Maze::InputEventKeyboardData eventData;
    
    
    eventData.type = Maze::InputEventKeyboardType::KeyDown;
    
    eventData.scanCode = [_event keyCode];
    eventData.virtualCode = eventData.scanCode;
    eventData.keyCode = (Maze::KeyCode)keyConversion[eventData.scanCode];
    eventData.modifiers = [self collectInputEventKeyboardModifiers: [_event modifierFlags]];
    
    if (_event.isARepeat)
        eventData.modifiers |= Maze::InputEventKeyboardModifiers::IsKeyRepeat;
    
    switch (eventData.keyCode)
    {
        case Maze::KeyCode::PrintScreen:
        case Maze::KeyCode::ScrollLock:
        case Maze::KeyCode::Pause:
        case Maze::KeyCode::CapsLock:
        case Maze::KeyCode::NumLock:
        case Maze::KeyCode::Insert:
        case Maze::KeyCode::Home:
        case Maze::KeyCode::PageUp:
        case Maze::KeyCode::Delete:
        case Maze::KeyCode::End:
        case Maze::KeyCode::PageDown:
        case Maze::KeyCode::Up:
        case Maze::KeyCode::Left:
        case Maze::KeyCode::Down:
        case Maze::KeyCode::Right:
        case Maze::KeyCode::Backspace:
        {
            break;
        }
            
        default:
        {
            const char* characters = [[_event characters] UTF8String];
            if (characters)
            {
                strcpy(eventData.textUtf8, characters);
            }
        }
    }

    inputManager->generateInputEvent(eventData);
    
    if (eventData.isCommandDown())
    {
        m_commandKeyCodes.insert(eventData.keyCode);
    }
}

//////////////////////////////////////////
-(void)keyUp:(NSEvent*)_event
{
    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    Maze::InputEventKeyboardData eventData;
    eventData.type = Maze::InputEventKeyboardType::KeyUp;
    eventData.scanCode = [_event keyCode];
    eventData.virtualCode = eventData.scanCode;
    eventData.keyCode = (Maze::KeyCode)keyConversion[eventData.scanCode];
    eventData.modifiers = [self collectInputEventKeyboardModifiers: [_event modifierFlags]];
    
    inputManager->generateInputEvent( eventData );
    
}

//////////////////////////////////////////
- (void)resetCursorRects
{
    [super resetCursorRects];
    
    if (m_window)
    {
        // #TODO:
        /*
        const Maze::WindowCursorPtr& windowCursor = m_window->getParams().cursor;
        if (windowCursor)
        {
            [self addCursorRect:[self bounds] cursor: windowCursor->castRaw<Maze::WindowCursorOSX>()->getNSCursor()];
        }
        else
        {
            [self discardCursorRects];
        }
        */
    }
}

//////////////////////////////////////////
-(void)flagsChanged:(NSEvent*)event
{
    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    Maze::InputEventKeyboardData eventData;
    eventData.modifiers = [self collectInputEventKeyboardModifiers: [event modifierFlags]];
    
    
    if (  (event.modifierFlags & NSEventModifierFlagShift)
       && (event.modifierFlags & 0x2)
       && (m_lastLShiftState == false))
    {
        m_lastLShiftState = true;
        eventData.type = Maze::InputEventKeyboardType::KeyDown;
        eventData.keyCode = Maze::KeyCode::LShift;
        inputManager->generateInputEvent(eventData);
    }
    else
    if (  (event.modifierFlags & NSEventModifierFlagShift)
       && !(event.modifierFlags & 0x2)
       && (m_lastRShiftState == false))
    {
        m_lastRShiftState = true;
        eventData.type = Maze::InputEventKeyboardType::KeyDown;
        eventData.keyCode = Maze::KeyCode::RShift;
        inputManager->generateInputEvent(eventData);
    }
    else
    if (     !(event.modifierFlags & NSEventModifierFlagShift)
        &&   ((m_lastLShiftState == true) || (m_lastRShiftState == true)))
    {
        eventData.type = Maze::InputEventKeyboardType::KeyUp;
        
        if (m_lastLShiftState)
        {
            eventData.keyCode = Maze::KeyCode::LShift;
            inputManager->generateInputEvent(eventData);
        }
        
        if (m_lastRShiftState)
        {
            eventData.keyCode = Maze::KeyCode::RShift;
            inputManager->generateInputEvent(eventData);
        }
        
        m_lastLShiftState = false;
        m_lastRShiftState = false;
    }
    else
    // #TODO:
    /*
    if (    (event.modifierFlags & NSEventModifierFlagFunction)
       &&   (m_lastLFnState == false) )
    {
        m_lastLFnState = true;
        eventData.type = Maze::InputEventKeyboardType::KeyDown;
        eventData.keyCode = (int)Maze::KeyCode::LFn;
        inputManager->generateInputEvent( eventData );
    }
    else
    if (    !(event.modifierFlags & NSEventModifierFlagFunction)
       &&   (( m_lastLFnState == true ) || ( m_lastRFnState == true )) )
    {
        eventData.type = Maze::InputEventKeyboardType::KeyUp;
        
        if (m_lastLFnState)
        {
            eventData.keyCode = (int)Maze::KeyCode::LFn;
            inputManager->generateInputEvent( eventData );
        }
        
        if (m_lastRFnState)
        {
            eventData.keyCode = (int)Maze::KeyCode::RFn;
            inputManager->generateInputEvent( eventData );
        }
        
        m_lastLFnState = false;
        m_lastRFnState = false;
    }
    else
    */
    if (  (event.modifierFlags & NSEventModifierFlagControl)
       && (event.modifierFlags & 0x1)
       && (m_lastLControlState == false))
    {
        m_lastLControlState = true;
        eventData.type = Maze::InputEventKeyboardType::KeyDown;
        eventData.keyCode = Maze::KeyCode::LControl;
        inputManager->generateInputEvent(eventData);
    }
    else
    if (  (event.modifierFlags & NSEventModifierFlagControl)
       && !(event.modifierFlags & 0x1)
       && (m_lastRControlState == false))
    {
        m_lastRControlState = true;
        eventData.type = Maze::InputEventKeyboardType::KeyDown;
        eventData.keyCode = Maze::KeyCode::RControl;
        inputManager->generateInputEvent(eventData);
    }
    else
    if (  !(event.modifierFlags & NSEventModifierFlagControl)
       && ((m_lastLControlState == true) || (m_lastRControlState == true)))
    {
        eventData.type = Maze::InputEventKeyboardType::KeyUp;
        
        if (m_lastLControlState)
        {
            eventData.keyCode = Maze::KeyCode::LControl;
            inputManager->generateInputEvent(eventData);
        }
        
        if (m_lastRControlState)
        {
            eventData.keyCode = Maze::KeyCode::RControl;
            inputManager->generateInputEvent(eventData);
        }
        
        m_lastLControlState = false;
        m_lastRControlState = false;
    }
    else
    if (  (event.modifierFlags & NSEventModifierFlagOption)
       && !(event.modifierFlags & 0x40)
       && (m_lastLAlternateState == false))
    {
        m_lastLAlternateState = true;
        eventData.type = Maze::InputEventKeyboardType::KeyDown;
        eventData.keyCode = Maze::KeyCode::LAlt;
        inputManager->generateInputEvent(eventData);
    }
    else
    if (  (event.modifierFlags & NSEventModifierFlagOption)
       && (event.modifierFlags & 0x40)
       && (m_lastRAlternateState == false))
    {
        m_lastRAlternateState = true;
        eventData.type = Maze::InputEventKeyboardType::KeyDown;
        eventData.keyCode = Maze::KeyCode::RAlt;
        inputManager->generateInputEvent(eventData);
    }
    else
    if (  !(event.modifierFlags & NSEventModifierFlagOption)
       && ((m_lastLAlternateState == true) || (m_lastRAlternateState == true)))
    {
        eventData.type = Maze::InputEventKeyboardType::KeyUp;
        
        if (m_lastLAlternateState)
        {
            eventData.keyCode = Maze::KeyCode::LAlt;
            inputManager->generateInputEvent(eventData);
        }
        
        if (m_lastRAlternateState)
        {
            eventData.keyCode = Maze::KeyCode::RAlt;
            inputManager->generateInputEvent(eventData);
        }
        
        m_lastLAlternateState = false;
        m_lastRAlternateState = false;
    }
    else
    if (  (event.modifierFlags & NSEventModifierFlagCommand)
       && !(event.modifierFlags & 0x10)
       && (m_lastLCommandState == false))
    {
        m_lastLCommandState = true;
        eventData.type = Maze::InputEventKeyboardType::KeyDown;
        eventData.keyCode = Maze::KeyCode::LCommand;
        inputManager->generateInputEvent(eventData);
    }
    else
    if (  (event.modifierFlags & NSEventModifierFlagCommand)
       && (event.modifierFlags & 0x10)
       && (m_lastRCommandState == false))
    {
        m_lastRCommandState = true;
        eventData.type = Maze::InputEventKeyboardType::KeyDown;
        eventData.keyCode = Maze::KeyCode::RCommand;
        inputManager->generateInputEvent(eventData);
    }
    else
    if (  !(event.modifierFlags & NSEventModifierFlagCommand)
       && ((m_lastLCommandState == true) || (m_lastRCommandState == true)))
    {
        eventData.type = Maze::InputEventKeyboardType::KeyUp;
        
        for (Maze::KeyCode commandKeyCode : m_commandKeyCodes)
        {
            eventData.keyCode = commandKeyCode;
            inputManager->generateInputEvent(eventData);
        }
        m_commandKeyCodes.clear();
        
                
        if (m_lastLCommandState)
        {
            eventData.keyCode = Maze::KeyCode::LCommand;
            inputManager->generateInputEvent(eventData);
        }
        
        if (m_lastRCommandState)
        {
            eventData.keyCode = Maze::KeyCode::RCommand;
            inputManager->generateInputEvent(eventData);
        }
        
        m_lastLCommandState = false;
        m_lastRCommandState = false;
    }
    
}

//////////////////////////////////////////
- (void)scrollWheel:(NSEvent*)_event
{
   
    Maze::InputManager* inputManager = Maze::InputManager::GetInstancePtr();
    Maze::InputEventMouseData eventData;
    eventData.type = Maze::InputEventMouseType::Wheel;
    eventData.x = 0;
    eventData.y = 0;
    eventData.z = [_event deltaY] * 50;
    inputManager->generateInputEvent(eventData);
}

@end
