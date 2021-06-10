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
#include "maze-core/system/unix/MazeMutexUnix.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MutexUnix
    //
    //////////////////////////////////////////
    MutexUnix::MutexUnix()
    {
        pthread_mutexattr_t attributes;
        pthread_mutexattr_init(&attributes);
        pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_RECURSIVE);

        pthread_mutex_init(&m_mutex, &attributes);
    }

    //////////////////////////////////////////
    MutexUnix::~MutexUnix()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    //////////////////////////////////////////
    void MutexUnix::lock()
    {
        pthread_mutex_lock(&m_mutex);
    }

    //////////////////////////////////////////
    void MutexUnix::unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }

    //////////////////////////////////////////
    bool MutexUnix::tryLock()
    {
        return pthread_mutex_trylock(&m_mutex) == 0;
    }

} // namespace Maze
//////////////////////////////////////////
