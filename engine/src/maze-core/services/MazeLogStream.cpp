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
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include <iostream>


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class LogStream
    //
    //////////////////////////////////////////
    LogStream::LogStream(S32 _priority)
        : std::ostream(&m_buffer)
        , m_buffer(_priority)
    {
        
    }
    
    //////////////////////////////////////////
    LogStream::LogStreamBuffer::LogStreamBuffer(S32 _priority)
        : m_priority(_priority)
    {
        static S32 const c_size = 256;
        char* buffer = MAZE_NEW_ARRAY(char, c_size);
        setp(buffer, buffer + c_size);
    }
    
    //////////////////////////////////////////
    LogStream::LogStreamBuffer::~LogStreamBuffer()
    {
        sync();
        MAZE_DELETE_ARRAY(pbase());
    }
    
    //////////////////////////////////////////
    int LogStream::LogStreamBuffer::overflow(int _character)
    {
        if ((_character != EOF) && (pptr() != epptr()))
        {
            // Valid character
            return sputc(static_cast<char>(_character));
        }
        else 
        if (_character != EOF)
        {
            // Not enough space in the buffer: synchronize output and try again
            sync();
            return overflow(_character);
        }
        else
        {
            // Invalid character: synchronize output
            return sync();
        }
    }
    
    //////////////////////////////////////////
    int LogStream::LogStreamBuffer::sync()
    {
        // Check if there is something into the write buffer
        if (pbase() != pptr())
        {
            // Print the contents of the write buffer into the standard error output
            Size size = static_cast<int>(pptr() - pbase());
            
            LogService::GetInstancePtr()->log(m_priority, pbase(), size);
            //fwrite(pbase(), 1, size, stdout);
            
            // Reset the pointer position to the beginning of the write buffer
            setp(pbase(), epptr());
        }
        
        return 0;
    }
    
    
    //////////////////////////////////////////
    namespace Debug
    {
        //////////////////////////////////////////
        MAZE_CORE_API LogStream log(c_logPriority_Default);
        MAZE_CORE_API LogStream logwarn(c_logPriority_Warning);
        MAZE_CORE_API LogStream logerr(c_logPriority_Error);

    } // namespace Debug
    //////////////////////////////////////////
    
    
} // namespace Maze 
//////////////////////////////////////////
