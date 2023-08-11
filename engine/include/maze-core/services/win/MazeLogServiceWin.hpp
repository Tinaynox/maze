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
#if (!defined(_MazeLogServiceWin_hpp_))
#define _MazeLogServiceWin_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/services/MazeLogServiceBase.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Enum LogServiceWinColor
    //
    //////////////////////////////////////////
    enum class LogServiceWinColor
    {
        Black = 0,
        DarkBlue = 1,
        DarkGreen = 2,
        DarkCyan = 3,
        DarkRed = 4,
        DarkMagenta = 5,
        DarkYellow = 6,
        Gray = 7,
        DarkGray = 8,
        Blue = 9,
        Green = 10,
        Cyan = 11,
        Red = 12,
        Magenta = 13,
        Yellow = 14,
        White = 15
    };


    //////////////////////////////////////////
    // Class LogService
    //
    //////////////////////////////////////////
    class MAZE_CORE_API LogService
        : public LogServiceBase
        , public MultiDelegateCallbackReceiver
    {
    public:
        
        //////////////////////////////////////////
        LogService();
        
        //////////////////////////////////////////
        static inline LogService& GetInstance()
        {
            static LogService s_logService;
            return s_logService;
        }

        //////////////////////////////////////////
        static inline LogService* GetInstancePtr() { return &GetInstance(); }
        

        //////////////////////////////////////////
        void applyConsoleColor(U8 _consoleColor);

        //////////////////////////////////////////
        void setPriorityColor(S32 _priority, LogServiceWinColor _symbolColor, LogServiceWinColor _backgroundColor);


    protected:

        //////////////////////////////////////////
        virtual void appendToDefaultStream(S32 _priority, CString _text, Size _size) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void appendToDefaultStream(S32 _priority, CWString _text, Size _size) MAZE_OVERRIDE;

    protected:
        HANDLE m_consoleHandle = 0;
        U8 m_currentConsoleColor = 0;
        FastVector<U8> m_priorityColors;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLogServiceWin_hpp_
//////////////////////////////////////////
