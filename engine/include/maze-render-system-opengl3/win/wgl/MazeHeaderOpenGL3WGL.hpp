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
#if (!defined(_MazeHeaderOpenGL3WGL_hpp_))
#define _MazeHeaderOpenGL3WGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Header.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Platform.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"


#define MAZE_WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT 0x20A9

#define MAZE_WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB 0x20A9

#define MAZE_WGL_SAMPLES_ARB 0x2042
#define MAZE_WGL_SAMPLE_BUFFERS_ARB 0x2041

#define MAZE_WGL_ACCELERATION_ARB 0x2003
#define MAZE_WGL_ACCUM_ALPHA_BITS_ARB 0x2021
#define MAZE_WGL_ACCUM_BITS_ARB 0x201D
#define MAZE_WGL_ACCUM_BLUE_BITS_ARB 0x2020
#define MAZE_WGL_ACCUM_GREEN_BITS_ARB 0x201F
#define MAZE_WGL_ACCUM_RED_BITS_ARB 0x201E
#define MAZE_WGL_ALPHA_BITS_ARB 0x201B
#define MAZE_WGL_ALPHA_SHIFT_ARB 0x201C
#define MAZE_WGL_AUX_BUFFERS_ARB 0x2024
#define MAZE_WGL_BLUE_BITS_ARB 0x2019
#define MAZE_WGL_BLUE_SHIFT_ARB 0x201A
#define MAZE_WGL_COLOR_BITS_ARB 0x2014
#define MAZE_WGL_DEPTH_BITS_ARB 0x2022
#define MAZE_WGL_DOUBLE_BUFFER_ARB 0x2011
#define MAZE_WGL_DRAW_TO_BITMAP_ARB 0x2002
#define MAZE_WGL_DRAW_TO_WINDOW_ARB 0x2001
#define MAZE_WGL_FULL_ACCELERATION_ARB 0x2027
#define MAZE_WGL_GENERIC_ACCELERATION_ARB 0x2026
#define MAZE_WGL_GREEN_BITS_ARB 0x2017
#define MAZE_WGL_GREEN_SHIFT_ARB 0x2018
#define MAZE_WGL_NEED_PALETTE_ARB 0x2004
#define MAZE_WGL_NEED_SYSTEM_PALETTE_ARB 0x2005
#define MAZE_WGL_NO_ACCELERATION_ARB 0x2025
#define MAZE_WGL_NUMBER_OVERLAYS_ARB 0x2008
#define MAZE_WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#define MAZE_WGL_NUMBER_UNDERLAYS_ARB 0x2009
#define MAZE_WGL_PIXEL_TYPE_ARB 0x2013
#define MAZE_WGL_RED_BITS_ARB 0x2015
#define MAZE_WGL_RED_SHIFT_ARB 0x2016
#define MAZE_WGL_SHARE_ACCUM_ARB 0x200E
#define MAZE_WGL_SHARE_DEPTH_ARB 0x200C
#define MAZE_WGL_SHARE_STENCIL_ARB 0x200D
#define MAZE_WGL_STENCIL_BITS_ARB 0x2023
#define MAZE_WGL_STEREO_ARB 0x2012
#define MAZE_WGL_SUPPORT_GDI_ARB 0x200F
#define MAZE_WGL_SUPPORT_OPENGL_ARB 0x2010
#define MAZE_WGL_SWAP_COPY_ARB 0x2029
#define MAZE_WGL_SWAP_EXCHANGE_ARB 0x2028
#define MAZE_WGL_SWAP_LAYER_BUFFERS_ARB 0x2006
#define MAZE_WGL_SWAP_METHOD_ARB 0x2007
#define MAZE_WGL_SWAP_UNDEFINED_ARB 0x202A
#define MAZE_WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define MAZE_WGL_TRANSPARENT_ARB 0x200A
#define MAZE_WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define MAZE_WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define MAZE_WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define MAZE_WGL_TRANSPARENT_RED_VALUE_ARB 0x2037
#define MAZE_WGL_TYPE_COLORINDEX_ARB 0x202C
#define MAZE_WGL_TYPE_RGBA_ARB 0x202B

#define MAZE_WGL_DRAW_TO_PBUFFER_ARB 0x202D
#define MAZE_WGL_MAX_PBUFFER_HEIGHT_ARB 0x2030
#define MAZE_WGL_MAX_PBUFFER_PIXELS_ARB 0x202E
#define MAZE_WGL_MAX_PBUFFER_WIDTH_ARB 0x202F
#define MAZE_WGL_PBUFFER_HEIGHT_ARB 0x2035
#define MAZE_WGL_PBUFFER_LARGEST_ARB 0x2033
#define MAZE_WGL_PBUFFER_LOST_ARB 0x2036
#define MAZE_WGL_PBUFFER_WIDTH_ARB 0x2034

#define MAZE_WGL_CONTEXT_DEBUG_BIT_ARB 0x00000001
#define MAZE_WGL_CONTEXT_FLAGS_ARB 0x2094
#define MAZE_WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define MAZE_WGL_CONTEXT_LAYER_PLANE_ARB 0x2093
#define MAZE_WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define MAZE_WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define MAZE_WGL_ERROR_INVALID_VERSION_ARB 0x2095

#define MAZE_WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define MAZE_WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define MAZE_WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define MAZE_WGL_ERROR_INVALID_PROFILE_ARB 0x2096


#endif // _MazeHeaderOpenGL3WGL_hpp_
//////////////////////////////////////////