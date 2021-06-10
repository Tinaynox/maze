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
#include "maze-core/hash/MazeHashSuperFast.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include <fstream>


//////////////////////////////////////////
namespace Maze
{

    ////////////////////////////////////
    // Define
#if ((defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__))
#   define GET_16_BITS(d)   (*((uint16_t const*) (d)))
#else
#   define GET_16_BITS(d)   ((((uint32_t)(((uint8_t const*)(d))[1])) << 8)\
                            +(uint32_t)(((uint8_t const*)(d))[0]))
#endif

    
    //////////////////////////////////////////
    MAZE_CORE_API U32 SuperFastHashIncremental(Char const* _data, S32 _len, U32 _lastHash)
    {
        U32 hash = (U32)_lastHash;
        U32 tmp;
        S32 rem;

        if (_len <= 0 || _data == NULL) 
            return 0;

        rem = _len & 3;
        _len >>= 2;

        for (; _len > 0; --_len) 
        {
            hash += GET_16_BITS(_data);
            tmp = (GET_16_BITS(_data + 2) << 11) ^ hash;
            hash = (hash << 16) ^ tmp;
            _data += 2 * sizeof(uint16_t);
            hash  += hash >> 11;
        }

        switch (rem)
        {
            case 3: 
                hash += GET_16_BITS(_data);
                hash ^= hash << 16;
                hash ^= _data[sizeof(uint16_t)] << 18;
                hash += hash >> 11;
                break;

            case 2: 
                hash += GET_16_BITS(_data);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;

            case 1: 
                hash += *_data;
                hash ^= hash << 10;
                hash += hash >> 1;
                break;
        }

        hash ^= hash << 3;
        hash += hash >> 5;
        hash ^= hash << 4;
        hash += hash >> 17;
        hash ^= hash << 25;
        hash += hash >> 6;

        return (U32)hash;
    }

    
    //////////////////////////////////////////
    MAZE_CORE_API U32 CalculateSuperFastHash(std::ifstream& _file)
    {
        static S32 const c_incrementalReadBlock = 65536;

        S8* buffer = MAZE_NEW_ARRAY(S8, c_incrementalReadBlock);
        Size bytesRead = 0;

        if (!_file.good())
            return 0;

        auto fsize = _file.tellg();
        _file.seekg(0, std::ios::end);
        fsize = _file.tellg() - fsize;
        _file.seekg(0, std::ios::beg);
        
        U32 lastHash = (U32)fsize;

        while (true)
        {
            _file.read(buffer, c_incrementalReadBlock);
            bytesRead = static_cast<Size>(_file.gcount());

            if (!bytesRead)
                break;

            S8* text = buffer;
            lastHash = SuperFastHashIncremental(text, (S32)bytesRead, lastHash);
        }

        MAZE_DELETE_ARRAY(buffer);

        return lastHash;
    }
    

} // namespace Maze
//////////////////////////////////////////
