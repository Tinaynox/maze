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
#if (!defined(_MazeLogServiceBase_hpp_))
#define _MazeLogServiceBase_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include "maze-core/system/MazePath.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    enum LogPriority
    {
        c_logPriority_Default = 0,
        c_logPriority_Warning = 1,
        c_logPriority_Error = 2,
        c_logPriority_Minor = 3
    };


    //////////////////////////////////////////
    // Class LogServiceBase
    //
    //////////////////////////////////////////
    class MAZE_CORE_API LogServiceBase
    {        
    public:
      
        //////////////////////////////////////////
        virtual ~LogServiceBase();
        
        
        //////////////////////////////////////////
        virtual bool setLogFile(Path const& _fullPath);
        
        //////////////////////////////////////////
        inline std::ofstream const& getLogFile() const { return m_logFile; }
           

        //////////////////////////////////////////
        void log(S32 _priority, CString _text);

        //////////////////////////////////////////
        inline void log(CString _text) { log(c_logPriority_Default, _text); }

        //////////////////////////////////////////
        void log(S32 _priority, CWString _text);

        //////////////////////////////////////////
        inline void log(CWString _text) { log(c_logPriority_Default, _text); }
        


        
        //////////////////////////////////////////
        void log(S32 _priority, CString _text, Size _size);
        
        //////////////////////////////////////////
        inline void log(CString _text, Size _size) { log(c_logPriority_Default, _text, _size); }

        //////////////////////////////////////////
        void log(S32 _priority, CWString _text, Size _size);
        
        //////////////////////////////////////////
        inline void log(CWString _text, Size _size) { log(c_logPriority_Default, _text, _size); }
        
        



        //////////////////////////////////////////
        void logFormatted(S32 _priority, CString _text, ...);

        //////////////////////////////////////////
        void logFormatted(CString _text, ...);

        //////////////////////////////////////////
        void logFormatted(S32 _priority, CWString _text, ...);

        //////////////////////////////////////////
        void logFormatted(CWString _text, ...);


    public:

        //////////////////////////////////////////
        MultiDelegate<S32, CString, Size> eventLog;
        
    protected:
        
        //////////////////////////////////////////
        LogServiceBase();
        
        
    protected:
        std::ofstream m_logFile;
        String m_tempLogBuffer;
        Mutex m_mutex;
    };
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLogServiceBase_hpp_
//////////////////////////////////////////
