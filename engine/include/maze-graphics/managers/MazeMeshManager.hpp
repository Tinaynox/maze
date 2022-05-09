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
#if (!defined(_MazeMeshManager_hpp_))
#define _MazeMeshManager_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MeshManager);
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(Texture);
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(ByteBuffer);
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Enum BuiltinMeshType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_5_API(MAZE_GRAPHICS_API, BuiltinMeshType,
        Quad,
        Cube,
        Sphere,
        Cone,
        Cylinder);


    //////////////////////////////////////////
    // Class MeshManager
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MeshManager 
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~MeshManager();

        //////////////////////////////////////////
        static void Initialize(MeshManagerPtr& _meshManager, RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        static MeshManagerPtr const& GetCurrentInstancePtr();


        //////////////////////////////////////////
        inline MeshPtr const& getDefaultQuadMesh() const { return getBuiltinMesh(BuiltinMeshType::Quad); }

        //////////////////////////////////////////
        inline MeshPtr const& getDefaultCubeMesh() const { return getBuiltinMesh(BuiltinMeshType::Cube); }

        //////////////////////////////////////////
        inline MeshPtr const& getDefaultSphereMesh() const { return getBuiltinMesh(BuiltinMeshType::Sphere); }


        //////////////////////////////////////////
        inline MeshPtr const& getBuiltinMesh(BuiltinMeshType _renderMeshType) const { return m_builtinMeshes[_renderMeshType]; }

        //////////////////////////////////////////
        MeshPtr const& createBuiltinMesh(BuiltinMeshType _renderMeshType);

        //////////////////////////////////////////
        MeshPtr const& ensureBuiltinMesh(BuiltinMeshType _renderMeshType);

        //////////////////////////////////////////
        void createBuiltinMeshes();


    protected:

        //////////////////////////////////////////
        MeshManager();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);
    
    protected:
        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        MeshPtr m_builtinMeshes[BuiltinMeshType::MAX];
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshManager_hpp_
//////////////////////////////////////////
