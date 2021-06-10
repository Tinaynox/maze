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
#if (!defined(_MazePreprocessor_Memory_hpp_))
#define _MazePreprocessor_Memory_hpp_


//////////////////////////////////////////
// Include
#include "MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/memory/MazeStdMemoryAllocators.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"


//////////////////////////////////////////
// Memory tracking
//
//////////////////////////////////////////
#if (MAZE_DEBUG_MEMORY)
#    define MAZE_NEW(__DClass) (__DClass*)::Maze::MemoryTrackerService::TrackAlloc(new __DClass(), sizeof(__DClass), Maze::ClassInfo<__DClass>::QualifiedName(), __FILE__, __LINE__, __FUNCTION__)
#    define MAZE_NEW_WITH_ARGS(__DClass, ...) (__DClass*)::Maze::MemoryTrackerService::TrackAlloc(new __DClass(__VA_ARGS__), sizeof(__DClass), Maze::ClassInfo<__DClass>::QualifiedName(), __FILE__, __LINE__, __FUNCTION__)
#    define MAZE_NEW_ARRAY(__DClass, __sz) (__DClass*)::Maze::MemoryTrackerService::TrackAlloc(new __DClass[__sz](), sizeof(__DClass) * __sz, Maze::ClassInfo<__DClass>::QualifiedName(), __FILE__, __LINE__, __FUNCTION__)
#    define MAZE_DELETE(__ptr) { ::Maze::MemoryTrackerService::TrackDealloc(__ptr); delete __ptr; }
#    define MAZE_DELETE_ARRAY(__ptr) { ::Maze::MemoryTrackerService::TrackDealloc(__ptr); delete[] __ptr; }
#else
#    define MAZE_NEW(__DClass) new __DClass()
#    define MAZE_NEW_WITH_ARGS(__DClass, ...) new __DClass(__VA_ARGS__)
#    define MAZE_NEW_ARRAY(__DClass, __sz) new __DClass[__sz]()
#    define MAZE_DELETE(__ptr) delete __ptr
#    define MAZE_DELETE_ARRAY(__ptr) delete[] __ptr
#endif


//////////////////////////////////////////
// Delete
//
//////////////////////////////////////////
#define MAZE_SAFE_DELETE(__object)              if (__object != nullptr) { MAZE_DELETE(__object); __object = nullptr; }

//////////////////////////////////////////
#define MAZE_SAFE_DELETE_ARRAY(__object)        if (__object != nullptr) { MAZE_DELETE_ARRAY(__object); __object = nullptr; }

//////////////////////////////////////////
#define MAZE_SAFE_DELETE_VECTOR(__obj)          for (Size i = 0; i < __obj.size(); ++i) \
                                                    MAZE_SAFE_DELETE(__obj[i]);         \
                                                __obj.clear();


//////////////////////////////////////////
// Create
//
//////////////////////////////////////////
#define MAZE_CREATE_AND_INIT(__DClass, __object, __init)    __object = MAZE_NEW(__DClass);               \
                                                            if (!___object->__init)                      \
                                                            {                                            \
                                                                MAZE_DELETE(___object);                  \
                                                                __object = nullptr;                      \
                                                            }

//////////////////////////////////////////
#define MAZE_CREATE_SHARED_PTR_EX(__DClass, __deleter)                          Maze::SharedPtr<__DClass>(MAZE_NEW(__DClass), __deleter, Maze::GetDefaultStdMemoryAllocator<__DClass>())
#define MAZE_CREATE_SHARED_PTR(__DClass)                                        MAZE_CREATE_SHARED_PTR_EX(__DClass, Maze::DefaultDelete<__DClass>())
#define MAZE_CREATE_SHARED_PTR_WITH_ARGS_EX(__DClass, __deleter, ...)           Maze::SharedPtr<__DClass>(MAZE_NEW_WITH_ARGS(__DClass, __VA_ARGS__), __deleter, Maze::GetDefaultStdMemoryAllocator<__DClass>())
#define MAZE_CREATE_SHARED_PTR_WITH_ARGS(__DClass, ...)                         MAZE_CREATE_SHARED_PTR_WITH_ARGS_EX(__DClass, Maze::DefaultDelete<__DClass>(), __VA_ARGS__)


//////////////////////////////////////////
#define MAZE_CREATE_AND_INIT_SHARED_PTR_EX(__DClass, __object, __deleter, __init)       __object = MAZE_CREATE_SHARED_PTR_EX(__DClass, __deleter);    \
                                                                                        if (!std::static_pointer_cast<__DClass>(__object)->__init)    \
                                                                                            __object.reset();
#define MAZE_CREATE_AND_INIT_SHARED_PTR(__DClass, __object, __init)                     MAZE_CREATE_AND_INIT_SHARED_PTR_EX(__DClass, __object, Maze::DefaultDelete<__DClass>(), __init)


//////////////////////////////////////////
#define MAZE_CREATE_AND_INIT_OS_OBJECT(__DClass, __object, __init)                      MAZE_CREATE_AND_INIT(MAZE_PLATFORM_OBJECT(__DClass), __object, __init)

//////////////////////////////////////////
#define MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR_EX(__DClass, __object, __deleter, __init)     __object = MAZE_CREATE_SHARED_PTR_EX(MAZE_PLATFORM_OBJECT(__DClass), __deleter);    \
                                                                                                if (!__object->__init)                                                              \
                                                                                                    __object.reset();
#define MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR(__DClass, __object, __init)                   MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR_EX(__DClass, __object, Maze::DefaultDelete<MAZE_PLATFORM_OBJECT(__DClass)>(), __init)


#endif // _MazePreprocessor_Memory_hpp_
//////////////////////////////////////////
