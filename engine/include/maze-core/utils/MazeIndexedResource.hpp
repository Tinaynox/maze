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
#if (!defined(_MazeIndexedResource_hpp_))
#define _MazeIndexedResource_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeIdWithGeneration.hpp"
#include "maze-core/containers/MazeFastVector.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    using ResourceId = IdWithGeneration<S32, 8>;
    ResourceId const c_invalidResourceId = ResourceId(-1);


    //////////////////////////////////////////
    template <typename T>
    class IndexedResource
    {
    public:

        //////////////////////////////////////////
        MAZE_FORCEINLINE IndexedResource();

        //////////////////////////////////////////
        MAZE_FORCEINLINE ~IndexedResource();

        //////////////////////////////////////////
        MAZE_FORCEINLINE ResourceId getResourceId() const { return m_resourceId; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE static T* GetResource(ResourceId _id);

        //////////////////////////////////////////
        MAZE_FORCEINLINE static T* GetResource(S32 _id) { return GetResource(ResourceId(_id)); }

        //////////////////////////////////////////
        MAZE_FORCEINLINE static T* GetResourceUnsafe(ResourceId _id);

        //////////////////////////////////////////
        MAZE_FORCEINLINE static T* GetResourceUnsafe(S32 _id) { return GetResourceUnsafe(ResourceId(_id)); }

    private:

        //////////////////////////////////////////
        MAZE_FORCEINLINE static ResourceId GenerateNewResourceId(T* _ptr);

        //////////////////////////////////////////
        MAZE_FORCEINLINE static void ReleaseResourceId(ResourceId _id);

    private:
        static FastVector<T*> s_resources;
        static Stack<ResourceId> s_freeResourceIndices;

    protected:
        ResourceId m_resourceId;
    };

    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#define MAZE_IMPLEMENT_INDEXED_RESOURCE(DClass)                               \
    FastVector<DClass*> DClass::s_resources = FastVector<DClass*>();          \
    Stack<ResourceId> DClass::s_freeResourceIndices = Stack<ResourceId>();


//////////////////////////////////////////
#include "MazeIndexedResource.inl"


//////////////////////////////////////////
#endif // _MazeIndexedResource_hpp_
