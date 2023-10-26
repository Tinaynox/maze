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
#if (!defined(_MazeLogHelper_hpp_))
#define _MazeLogHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Debug
    {
        //////////////////////////////////////////
        MAZE_CORE_API extern void Log(CString _text, ...);
        MAZE_CORE_API extern void Log(CWString _text, ...);
        MAZE_CORE_API extern void LogMinor(CString _text, ...);
        MAZE_CORE_API extern void LogMinor(CWString _text, ...);
        MAZE_CORE_API extern void LogWarning(CString _text, ...);
        MAZE_CORE_API extern void LogWarning(CWString _text, ...);
        MAZE_CORE_API extern void LogError(CString _text, ...);
        MAZE_CORE_API extern void LogError(CWString _text, ...);

    } // namespace Debug
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#define MAZE_LOG(...) { Maze::Debug::Log(__VA_ARGS__); }

//////////////////////////////////////////
#define MAZE_LOG_LOW(...) { Maze::Debug::LogLow(__VA_ARGS__); }

//////////////////////////////////////////
#if (MAZE_ARCH == MAZE_ARCH_EMSCRIPTEN)
#    define MAZE_LOG_FUNCTION MAZE_PRETTY_FUNCTION
#else
#    define MAZE_LOG_FUNCTION __FUNCTION__
#endif

//////////////////////////////////////////
#define MAZE_LOG_WARNING(...)\
{\
    Maze::Debug::LogWarning("%s[%d]:", MAZE_LOG_FUNCTION, __LINE__);\
    Maze::Debug::LogWarning(__VA_ARGS__);\
}

//////////////////////////////////////////
#define MAZE_LOG_ERROR(...)\
{\
    Maze::Debug::LogError("%s[%d]:", MAZE_LOG_FUNCTION, __LINE__);\
    Maze::Debug::LogError(__VA_ARGS__);\
}

//////////////////////////////////////////
#define MAZE_WARNING_BP(...)\
{\
    MAZE_LOG_WARNING(__VA_ARGS__);\
    MAZE_DEBUG_BP;\
}

//////////////////////////////////////////
#define MAZE_WARNING(...) MAZE_LOG_WARNING(__VA_ARGS__);

//////////////////////////////////////////
#define MAZE_WARNING_BP_IF(__condition, ...)\
{\
    if (__condition)\
    {\
        MAZE_LOG_WARNING(__VA_ARGS__);\
    }\
}

//////////////////////////////////////////
#define MAZE_WARNING_IF(__condition, ...) { if (__condition) { MAZE_LOG_WARNING(__VA_ARGS__); } }

//////////////////////////////////////////
#define MAZE_WARNING_RETURN(...)\
{\
    MAZE_LOG_WARNING(__VA_ARGS__);\
    return;\
}

//////////////////////////////////////////
#define MAZE_WARNING_RETURN_IF(__condition, ...)\
{\
    if (__condition)\
    {\
        MAZE_LOG_WARNING(__VA_ARGS__);\
        return;\
    }\
}

//////////////////////////////////////////
#define MAZE_WARNING_RETURN_VALUE(__value, ...)\
{\
    MAZE_LOG_WARNING(__VA_ARGS__);\
    return __value;\
}

//////////////////////////////////////////
#define MAZE_WARNING_RETURN_VALUE_IF(__condition, __value, ...)\
{\
    if (__condition)\
    {\
        MAZE_LOG_WARNING(__VA_ARGS__);\
        return __value;\
    }\
}

//////////////////////////////////////////
#define MAZE_WARNING_CONTINUE(...)\
{\
    MAZE_LOG_WARNING(__VA_ARGS__);\
    continue;\
}

//////////////////////////////////////////
#define MAZE_WARNING_CONTINUE_IF(__condition, ...)\
{\
    if (__condition)\
    {\
        MAZE_LOG_WARNING(__VA_ARGS__);\
        continue;\
    }\
}

//////////////////////////////////////////
#if (MAZE_DEBUG)
#define MAZE_DEBUG_WARNING_BP(...)                                      MAZE_WARNING_BP(__VA_ARGS__)
#define MAZE_DEBUG_WARNING(...)                                         MAZE_WARNING(__VA_ARGS__)
#define MAZE_DEBUG_WARNING_BP_IF(__condition, ...)                      MAZE_WARNING_BP_IF(__condition, __VA_ARGS__)
#define MAZE_DEBUG_WARNING_IF(__condition, ...)                         MAZE_WARNING_IF(__condition, __VA_ARGS__)
#define MAZE_DEBUG_WARNING_RETURN(...)                                  MAZE_WARNING_RETURN(__VA_ARGS__)
#define MAZE_DEBUG_WARNING_RETURN_IF(__condition, ...)                  MAZE_WARNING_RETURN_IF(__condition, __VA_ARGS__)
#define MAZE_DEBUG_WARNING_RETURN_VALUE(__value, ...)                   MAZE_WARNING_RETURN_VALUE(__value, __VA_ARGS__)
#define MAZE_DEBUG_WARNING_RETURN_VALUE_IF(__condition, __value, ...)   MAZE_WARNING_RETURN_VALUE_IF(__condition, __value, __VA_ARGS__)
#define MAZE_DEBUG_WARNING_CONTINUE(...)                                MAZE_WARNING_CONTINUE(__VA_ARGS__)
#define MAZE_DEBUG_WARNING_CONTINUE_IF(__condition, ...)                MAZE_WARNING_CONTINUE_IF(__condition, __VA_ARGS__)
#else
#define MAZE_DEBUG_WARNING_BP(...)
#define MAZE_DEBUG_WARNING(...)
#define MAZE_DEBUG_WARNING_BP_IF(__condition, ...)                      
#define MAZE_DEBUG_WARNING_IF(__condition, ...)                         
#define MAZE_DEBUG_WARNING_RETURN(...)                                  
#define MAZE_DEBUG_WARNING_RETURN_IF(__condition, ...)                  
#define MAZE_DEBUG_WARNING_RETURN_VALUE(__value, ...)                   
#define MAZE_DEBUG_WARNING_RETURN_VALUE_IF(__condition, __value, ...)   
#define MAZE_DEBUG_WARNING_CONTINUE(...)                                
#define MAZE_DEBUG_WARNING_CONTINUE_IF(__condition, ...)                
#endif


//////////////////////////////////////////
#define MAZE_ERROR_BP(...)\
{\
    MAZE_LOG_ERROR(__VA_ARGS__);\
    MAZE_DEBUG_BP;\
}

//////////////////////////////////////////
#define MAZE_ERROR(...) \
{\
    MAZE_ERROR_BP(__VA_ARGS__);\
}

//////////////////////////////////////////
#define MAZE_ERROR_BP_IF(__condition, ...)\
{\
    if (__condition)\
    {\
        MAZE_LOG_ERROR(__VA_ARGS__);\
        MAZE_DEBUG_BP;\
    }\
}

//////////////////////////////////////////
#define MAZE_ERROR_IF(__condition, ...) MAZE_ERROR_BP_IF(__condition, __VA_ARGS__);

//////////////////////////////////////////
#define MAZE_ERROR_RETURN(...)\
{\
    MAZE_LOG_ERROR(__VA_ARGS__);\
    MAZE_DEBUG_BP;\
    return;\
}

//////////////////////////////////////////
#define MAZE_ERROR_RETURN_IF(__condition, ...)\
{\
    if (__condition)\
    {\
        MAZE_LOG_ERROR(__VA_ARGS__);\
        MAZE_DEBUG_BP;\
        return;\
    }\
}

//////////////////////////////////////////
#define MAZE_ERROR_RETURN_VALUE(__value, ...)\
{\
    MAZE_LOG_ERROR(__VA_ARGS__);\
    MAZE_DEBUG_BP;\
    return __value;\
}

//////////////////////////////////////////
#define MAZE_ERROR_RETURN_VALUE_IF(__condition, __value, ...)\
{\
    if (__condition)\
    {\
        MAZE_LOG_ERROR(__VA_ARGS__);\
        MAZE_DEBUG_BP;\
        return __value;\
    }\
}

//////////////////////////////////////////
#define MAZE_ERROR_CONTINUE(...)\
{\
    MAZE_LOG_ERROR(__VA_ARGS__);\
    MAZE_DEBUG_BP;\
    continue;\
}

//////////////////////////////////////////
#define MAZE_ERROR_CONTINUE_IF(__condition, ...)\
{\
    if (__condition)\
    {\
        MAZE_LOG_ERROR(__VA_ARGS__);\
        MAZE_DEBUG_BP;\
        continue;\
    }\
}


//////////////////////////////////////////
#define MAZE_FATAL(...) \
{\
    MAZE_LOG_ERROR(__VA_ARGS__);\
    MAZE_BP;\
}

//////////////////////////////////////////
#define MAZE_FATAL_IF(__condition, ...)\
{\
    if (__condition)\
    {\
        MAZE_FATAL(__VA_ARGS__);\
    }\
}


//////////////////////////////////////////
#define MAZE_NOT_IMPLEMENTED MAZE_ERROR("Not implemented!");
#define MAZE_NOT_IMPLEMENTED_RETURN MAZE_ERROR_RETURN("Not implemented!");
#define MAZE_NOT_IMPLEMENTED_RETURN_VALUE(__value) MAZE_ERROR_RETURN_VALUE(__value, "Not implemented!");
#define MAZE_NOT_IMPLEMENTED_CONTINUE MAZE_ERROR_CONTINUE("Not implemented!");


//////////////////////////////////////////
#if (MAZE_DEBUG)
#define MAZE_DEBUG_ERROR_BP(...)                                      MAZE_ERROR_BP(__VA_ARGS__)
#define MAZE_DEBUG_ERROR(...)                                         MAZE_ERROR(__VA_ARGS__)
#define MAZE_DEBUG_ERROR_BP_IF(__condition, ...)                      MAZE_ERROR_BP_IF(__condition, __VA_ARGS__)
#define MAZE_DEBUG_ERROR_IF(__condition, ...)                         MAZE_ERROR_IF(__condition, __VA_ARGS__)
#define MAZE_DEBUG_ERROR_RETURN(...)                                  MAZE_ERROR_RETURN(__VA_ARGS__)
#define MAZE_DEBUG_ERROR_RETURN_IF(__condition, ...)                  MAZE_ERROR_RETURN_IF(__condition, __VA_ARGS__)
#define MAZE_DEBUG_ERROR_RETURN_VALUE(__value, ...)                   MAZE_ERROR_RETURN_VALUE(__value, __VA_ARGS__)
#define MAZE_DEBUG_ERROR_RETURN_VALUE_IF(__condition, __value, ...)   MAZE_ERROR_RETURN_VALUE_IF(__condition, __value, __VA_ARGS__)
#define MAZE_DEBUG_ERROR_CONTINUE(...)                                MAZE_ERROR_CONTINUE(__VA_ARGS__)
#define MAZE_DEBUG_ERROR_CONTINUE_IF(__condition, ...)                MAZE_ERROR_CONTINUE_IF(__condition, __VA_ARGS__)
#else
#define MAZE_DEBUG_ERROR_BP(...)
#define MAZE_DEBUG_ERROR(...)
#define MAZE_DEBUG_ERROR_BP_IF(__condition, ...)                      
#define MAZE_DEBUG_ERROR_IF(__condition, ...)                         
#define MAZE_DEBUG_ERROR_RETURN(...)                                  
#define MAZE_DEBUG_ERROR_RETURN_IF(__condition, ...)                  
#define MAZE_DEBUG_ERROR_RETURN_VALUE(__value, ...)                   
#define MAZE_DEBUG_ERROR_RETURN_VALUE_IF(__condition, __value, ...)   
#define MAZE_DEBUG_ERROR_CONTINUE(...)                                
#define MAZE_DEBUG_ERROR_CONTINUE_IF(__condition, ...)                
#endif


//////////////////////////////////////////
#endif // _MazeLogHelper_hpp_
