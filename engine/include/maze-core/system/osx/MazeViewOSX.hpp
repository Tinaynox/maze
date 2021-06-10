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
#if (!defined(_MazeViewOSX_hpp_))
#define _MazeViewOSX_hpp_


//////////////////////////////////////////
#import <Cocoa/Cocoa.h>
#include "maze-core/system/MazeKeyCode.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/system/MazeKeyCode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    class WindowOSX;
    
} // namespace Maze
//////////////////////////////////////////





//////////////////////////////////////////
// Class ViewOSX
//
//////////////////////////////////////////
@interface ViewOSX
    : NSOpenGLView
{
@private
    Maze::Map<S32, S32> keyConversion; // #TODO: Rename with m_
    Maze::WindowOSX* m_window;
    
    bool m_lastLShiftState;
    bool m_lastRShiftState;
    bool m_lastLControlState;
    bool m_lastRControlState;
    bool m_lastLAlternateState;
    bool m_lastRAlternateState;
    bool m_lastLCommandState;
    bool m_lastRCommandState;
    std::set<Maze::KeyCode> m_commandKeyCodes;
}

- (id)initWithFrame:(NSRect)_f;
- (void)setWindow:(Maze::WindowOSX*)_window;
- (void)populateKeyConversion;
- (void)mouseDown:(NSEvent*)_event;
- (void)mouseUp:(NSEvent*)_event;
- (void)rightMouseDown:(NSEvent*)_event;
- (void)rightMouseUp:(NSEvent*)_event;
- (void)mouseMoved:(NSEvent*)_event;
- (void)mouseDragged:(NSEvent*)_event;
- (void)rightMouseDragged:(NSEvent*)_event;
- (void)keyDown:(NSEvent*)_event;
- (void)keyUp:(NSEvent*)_event;
- (void)resetCursorRects;
@end


#endif // _MazeViewOSX_h_
//////////////////////////////////////////
