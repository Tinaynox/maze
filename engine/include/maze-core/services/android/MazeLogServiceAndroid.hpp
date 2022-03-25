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
#if (!defined(_MazeLogServiceAndroid_hpp_))
#define _MazeLogServiceAndroid_hpp_


//////////////////////////////////////////
#include "maze-core/services/unix/MazeLogServiceUnix.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class LogService
    //
    //////////////////////////////////////////
    class MAZE_CORE_API LogService
        : public LogServiceUnix
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

    protected:

        //////////////////////////////////////////
        void notifyLog(S32 _priority, CString _text, Size _size);


    protected:
        String m_androidBuffer;
        S32 m_prevLogPriority;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLogServiceAndroid_hpp_
//////////////////////////////////////////
