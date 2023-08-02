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
        virtual bool setLogErrorFile(Path const& _fullPath);

        //////////////////////////////////////////
        inline std::ofstream const& getLogErrorFile() const { return m_logErrorFile; }
           
        
        //////////////////////////////////////////
        void append(S32 _priority, CString _text, Size _size);
        
        //////////////////////////////////////////
        inline void append(CString _text, Size _size) { append(c_logPriority_Default, _text, _size); }

        //////////////////////////////////////////
        void append(S32 _priority, CWString _text, Size _size);
        
        //////////////////////////////////////////
        inline void append(CWString _text, Size _size) { append(c_logPriority_Default, _text, _size); }
        

        //////////////////////////////////////////
        void appendPrefix(S32 _priority);


        //////////////////////////////////////////
        void log(S32 _priority, CString _text, Size _size);

        //////////////////////////////////////////
        inline void log(CString _text, Size _size) { log(c_logPriority_Default, _text, _size); }

        //////////////////////////////////////////
        void log(S32 _priority, CWString _text, Size _size);

        //////////////////////////////////////////
        inline void log(CWString _text, Size _size) { log(c_logPriority_Default, _text, _size); }



        //////////////////////////////////////////
        void log(S32 _priority, CString _text);

        //////////////////////////////////////////
        inline void log(CString _text) { log(c_logPriority_Default, _text); }

        //////////////////////////////////////////
        void log(S32 _priority, CWString _text);

        //////////////////////////////////////////
        inline void log(CWString _text) { log(c_logPriority_Default, _text); }
        

        //////////////////////////////////////////
        inline bool getLastLogEndsWithEndline() const { return m_lastLogEndsWithEndline; }


        //////////////////////////////////////////
        void logFormatted(S32 _priority, CString _text, ...);

        //////////////////////////////////////////
        void logFormatted(CString _text, ...);

        //////////////////////////////////////////
        void logFormatted(S32 _priority, CWString _text, ...);

        //////////////////////////////////////////
        void logFormatted(CWString _text, ...);


        //////////////////////////////////////////
        void splitAndLog(S32 _priority, CString _text, Size _size, Char _separator = '\n');

        //////////////////////////////////////////
        inline void splitAndLog(String const& _text, Char _separator = '\n') { splitAndLog(c_logPriority_Default, _text.c_str(), _text.size(), _separator); }


        //////////////////////////////////////////
        void setPriorityMark(S32 _priority, Char _mark);

    public:

        //////////////////////////////////////////
        MultiDelegate<S32, CString, Size> eventLog;
        MultiDelegate<S32, CWString, Size> eventLogW;

        //////////////////////////////////////////
        MultiDelegate<Path const&> eventLogFileSet;
        MultiDelegate<Path const&> eventLogErrorFileSet;
        
    protected:
        
        //////////////////////////////////////////
        LogServiceBase();

        //////////////////////////////////////////
        void preparePrefix(S32 _priority);
        
        //////////////////////////////////////////
        void appendToDefaultStream(S32 _priority, CString _text, Size _size);

        //////////////////////////////////////////
        void appendToDefaultStream(S32 _priority, CWString _text, Size _size);

        //////////////////////////////////////////
        void appendToLogFile(S32 _priority, CString _text, Size _size);

        //////////////////////////////////////////
        void appendToLogFile(S32 _priority, CWString _text, Size _size);
        
    protected:
        std::ofstream m_logFile;
        String m_tempLogBuffer;

        std::ofstream m_logErrorFile;
        String m_tempLogErrorBuffer;


        Mutex m_mutex;
        FastVector<Char> m_priorityMarks;
        bool m_lastLogEndsWithEndline = true;

    private:
        String m_prefixBuffer;
    };
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLogServiceBase_hpp_
//////////////////////////////////////////
