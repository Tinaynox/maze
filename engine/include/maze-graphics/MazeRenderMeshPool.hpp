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
#if (!defined(_MazeRenderMeshPool_hpp_))
#define _MazeRenderMeshPool_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeRenderDrawTopology.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/containers/MazeFastVector.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(RenderSystem);
    

    //////////////////////////////////////////
    // Class RenderMeshPool
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderMeshPool
    {
    public:

        //////////////////////////////////////////
        RenderMeshPool(RenderSystem* _renderSystem);

        //////////////////////////////////////////
        ~RenderMeshPool();

        //////////////////////////////////////////
        RenderMeshPtr createRenderMeshFromPool(S32 _subMeshCount = 1);
    
        //////////////////////////////////////////
        void clear();

    protected:

        //////////////////////////////////////////
        void releaseRenderMesh(
            RenderMesh* _ptr,
            S32 _key);

    protected:
        RenderSystem* m_renderSystem;

        Mutex m_mutex;

        Map<S32, Stack<RenderMesh*>> m_renderMeshPools;
        S32 m_activeMeshes;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderMeshPool_hpp_
//////////////////////////////////////////
