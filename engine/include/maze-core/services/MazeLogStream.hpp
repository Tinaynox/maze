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
#if (!defined(_MazeLogStream_hpp_))
#define _MazeLogStream_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include "maze-core/utils/MazeSingleton.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include <ostream>
#include <iostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class LogStream
    //
    //////////////////////////////////////////
    class MAZE_CORE_API LogStream
        : public std::ostream
    {
    public:
        
        ////////////////////////////////////
        LogStream(S32 _priority);
        
    protected:
        
        //////////////////////////////////////////
        // Class LogStreamBuffer
        //
        //////////////////////////////////////////
        class LogStreamBuffer
            : public std::streambuf
        {
        public:
            
            //////////////////////////////////////////
            LogStreamBuffer(S32 _priority);
            
            //////////////////////////////////////////
            ~LogStreamBuffer();
            

        private:
            
            //////////////////////////////////////////
            virtual int overflow(int _character) override;
            
            //////////////////////////////////////////
            virtual int sync() override;

            //////////////////////////////////////////
            virtual std::streamsize xsputn(
                const char* _Ptr,
                std::streamsize _Count) override
            {
                std::streamsize result = std::streambuf::xsputn(_Ptr, _Count);

                if (result)
                {
                    sync();
                }

                return result;
            }
            
        protected:
            S32 m_priority;
        };
        
    protected:
        LogStreamBuffer m_buffer;
    };
    

    //////////////////////////////////////////
    namespace Debug
    {
        //////////////////////////////////////////
        MAZE_CORE_API extern LogStream log;
        MAZE_CORE_API extern LogStream logwarn;
        MAZE_CORE_API extern LogStream logerr;

    } // namespace Debug
    //////////////////////////////////////////

    
    //////////////////////////////////////////
    template < 
        class _CharT,
        class _Traits>
    inline std::basic_ostream< _CharT, _Traits >& endl(std::basic_ostream<_CharT, _Traits>& _os)
    {
        return std::endl<_CharT, _Traits>(_os);
    }
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLogServiceBase_hpp_
//////////////////////////////////////////
