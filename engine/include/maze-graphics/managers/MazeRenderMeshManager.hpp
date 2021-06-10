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
#if (!defined(_MazeRenderMeshManager_hpp_))
#define _MazeRenderMeshManager_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMeshManager);
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(Texture);
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Class RenderMeshManager
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderMeshManager 
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~RenderMeshManager();

        //////////////////////////////////////////
        static void Initialize(RenderMeshManagerPtr& _renderMeshManager, RenderSystemPtr const& _renderSystem);


        //////////////////////////////////////////
        inline RenderMeshPtr const& getDefaultQuadMesh() const { return m_defaultQuadMesh; }

        //////////////////////////////////////////
        inline RenderMeshPtr const& getDefaultCubeMesh() const { return m_defaultCubeMesh; }

        //////////////////////////////////////////
        inline RenderMeshPtr const& getDefaultSphereMesh() const { return m_defaultSphereMesh; }


        //////////////////////////////////////////
        RenderMeshPtr const& getRenderMesh(String const& _assetFileName);

        //////////////////////////////////////////
        RenderMeshPtr const& getRenderMesh(AssetFilePtr const& _assetFile);


        //////////////////////////////////////////
        RenderMeshPtr const& addRenderMesh(RenderMeshPtr const& _renderMesh);

        //////////////////////////////////////////
        Vector<RenderMeshPtr> getRenderMeshesSorted();


        //////////////////////////////////////////
        void loadAllAssetRenderMeshes();

    protected:

        //////////////////////////////////////////
        RenderMeshManager();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        void notifyRenderSystemInited();
    
        //////////////////////////////////////////
        void createDefaultMeshes();

    protected:
        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        RenderMeshPtr m_defaultQuadMesh;
        RenderMeshPtr m_defaultCubeMesh;
        RenderMeshPtr m_defaultSphereMesh;

        UnorderedMap<String, RenderMeshPtr> m_renderMeshesByName;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderMeshManager_hpp_
//////////////////////////////////////////
