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
// subject to the folloIOSg restrictions:
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
#include "maze-core/helpers/ios/MazeThreadHelperIOS.hpp"
#include "maze-core/helpers/MazeThreadHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#import <Foundation/Foundation.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static pthread_key_t s_poolKey;
    static pthread_once_t s_initOnceToken = PTHREAD_ONCE_INIT;
    
    
    //////////////////////////////////////////
    namespace ThreadHelper
    {
        //////////////////////////////////////////
        static void CreateNewAutoreleasePool()
        {
            NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
            pthread_setspecific(s_poolKey, pool);
        }
        
        //////////////////////////////////////////
        static void DestroyAutoreleasePool(void* data)
        {
            NSAutoreleasePool* pool = (NSAutoreleasePool*)data;
            [pool drain];
        }
        
        //////////////////////////////////////////
        static void CreateAutoreleasePoolKey()
        {
            pthread_key_create(&s_poolKey, DestroyAutoreleasePool);
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API void EnsureThreadHasAutoreleasePool()
        {
            pthread_once(&s_initOnceToken, CreateAutoreleasePoolKey);
            if (pthread_getspecific( s_poolKey ) == NULL)
            {
                CreateNewAutoreleasePool();
            }
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API void DrainThreadAutoreleasePool()
        {
            void* data = pthread_getspecific(s_poolKey);
            assert(data != NULL);
            
            // Drain the pool but keep it alive by creating a new one
            DestroyAutoreleasePool(data);
            CreateNewAutoreleasePool();
        }

    } // namespace ThreadHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
