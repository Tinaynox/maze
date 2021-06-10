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
// subject to the folloosxg restrictions:
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
#include "maze-core/helpers/osx/MazeWindowHelperOSX.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>
#import <AppKit/AppKit.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace WindowHelper
    {
        ////////////////////////////////////
        MAZE_CORE_API U32 GetModeBitsPerPixel(CGDisplayModeRef _mode)
        {
            U32 bpp = 0;
            
            // Compare encoding.
#if (__MAC_OS_X_VERSION_MAX_ALLOWED >= 1050)
            CFDictionaryRef dict = (CFDictionaryRef)*((int64_t *)_mode + 2);
            CFNumberRef num;
            if (   CFGetTypeID(dict) == CFDictionaryGetTypeID()
                && CFDictionaryGetValueIfPresent(dict, kCGDisplayBitsPerPixel, (const void**)&num))
            {
                CFNumberGetValue(num, kCFNumberSInt32Type, (void*)&bpp);
            }
#else
            CFStringRef pixEnc = CGDisplayModeCopyPixelEncoding(_mode);
            if (CFStringCompare(pixEnc, CFSTR(IO32BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
                bpp = 32;
            else if (CFStringCompare(pixEnc, CFSTR(IO16BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
                bpp = 16;
            else if (CFStringCompare(pixEnc, CFSTR(IO8BitIndexedPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
                bpp = 8;
                
            CFRelease(pixEnc);
#endif
            
            return bpp;
        }
        
        ////////////////////////////////////
        MAZE_CORE_API WindowVideoMode ConvertCGModeToGGMode(CGDisplayModeRef _cgmode)
        {
            WindowVideoMode mode(CGDisplayModeGetWidth(_cgmode), CGDisplayModeGetHeight(_cgmode), GetModeBitsPerPixel(_cgmode));
            F32 scaleFactor = 1.0f; // #TODO
            mode.width *= scaleFactor;
            mode.height *= scaleFactor;
            return mode;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API Vector<WindowVideoMode> GetDisplayFullscreenModes(U64 _displayHandle)
        {
            Vector<WindowVideoMode> modes;
            
            CFArrayRef cgmodes = CGDisplayCopyAllDisplayModes(_displayHandle, NULL);
            
            MAZE_ERROR_RETURN_VALUE_IF(cgmodes == nullptr, modes, "Couldn't get VideoMode for display.")
            
            
            WindowVideoMode desktop = GetDisplayCurrentMode(_displayHandle);
            
            CFIndex const modesCount = CFArrayGetCount(cgmodes);
            for (CFIndex i = 0; i < modesCount; i++)
            {
                CGDisplayModeRef cgmode = (CGDisplayModeRef)CFArrayGetValueAtIndex(cgmodes, i);
                
                WindowVideoMode mode = ConvertCGModeToGGMode(cgmode);
                
                if ((mode.width > desktop.width) || (mode.height > desktop.height))
                    continue;
                
                if (std::find( modes.begin(), modes.end(), mode ) == modes.end())
                    modes.push_back(mode);
            }
            
            CFRelease(cgmodes);
            
            return modes;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API Vector<WindowVideoMode> GetDisplayFullscreenModes(Display const& _display)
        {
            return GetDisplayFullscreenModes(_display.getHandle());
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API WindowVideoMode GetDisplayCurrentMode(U64 _displayHandle)
        {
            CGDisplayModeRef cgmode = CGDisplayCopyDisplayMode(_displayHandle);
            WindowVideoMode mode = ConvertCGModeToGGMode(cgmode);
            CGDisplayModeRelease(cgmode);
            
            return mode;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API WindowVideoMode GetDisplayCurrentMode(Display const& _display)
        {
            return GetDisplayCurrentMode(_display.getHandle());
        }

    } // namespace WindowHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
