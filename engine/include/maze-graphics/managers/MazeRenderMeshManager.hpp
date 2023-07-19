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
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMeshManager);
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(Texture);
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(ByteBuffer);
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Enum BuiltinRenderMeshType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_6_API(MAZE_GRAPHICS_API, BuiltinRenderMeshType,
        Quad,
        Cube,
        Sphere,
        Cone,
        Cylinder,
        Torus);


    //////////////////////////////////////////
    // Struct RenderMeshLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderMeshLibraryData
    {
        //////////////////////////////////////////
        RenderMeshLibraryData(
            RenderMeshPtr const& _renderMesh = nullptr,
            AssetFilePtr const& _assetFile = nullptr)
            : renderMesh(_renderMesh)
            , assetFile(_assetFile)
        {}

        RenderMeshPtr renderMesh;
        AssetFilePtr assetFile;
    };


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
        static RenderMeshManagerPtr const& GetCurrentInstancePtr();


        //////////////////////////////////////////
        RenderMeshLibraryData const* getRenderMeshLibraryData(HashedCString _renderMeshName);

        //////////////////////////////////////////
        RenderMeshLibraryData const* getRenderMeshLibraryData(String const& _assetFileName) { return getRenderMeshLibraryData(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        RenderMeshLibraryData const* getRenderMeshLibraryData(CString _assetFileName) { return getRenderMeshLibraryData(MAZE_HASHED_CSTRING(_assetFileName)); }


        //////////////////////////////////////////
        inline RenderMeshPtr const& getDefaultQuadMesh() const { return getBuiltinRenderMesh(BuiltinRenderMeshType::Quad); }

        //////////////////////////////////////////
        inline RenderMeshPtr const& getDefaultCubeMesh() const { return getBuiltinRenderMesh(BuiltinRenderMeshType::Cube); }

        //////////////////////////////////////////
        inline RenderMeshPtr const& getDefaultSphereMesh() const { return getBuiltinRenderMesh(BuiltinRenderMeshType::Sphere); }


        //////////////////////////////////////////
        inline RenderMeshPtr const& getBuiltinRenderMesh(BuiltinRenderMeshType _renderMeshType) const { return m_builtinRenderMeshes[_renderMeshType]; }

        //////////////////////////////////////////
        RenderMeshPtr const& createBuiltinRenderMesh(BuiltinRenderMeshType _renderMeshType);

        //////////////////////////////////////////
        RenderMeshPtr const& ensureBuiltinRenderMesh(BuiltinRenderMeshType _renderMeshType);

        //////////////////////////////////////////
        void createBuiltinRenderMeshes();


        //////////////////////////////////////////
        RenderMeshPtr const& getRenderMesh(HashedCString _assetFileName);

        //////////////////////////////////////////
        inline RenderMeshPtr const& getRenderMesh(String const& _assetFileName) { return getRenderMesh(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        inline RenderMeshPtr const& getRenderMesh(CString _assetFileName) { return getRenderMesh(MAZE_HASHED_CSTRING(_assetFileName)); }

        //////////////////////////////////////////
        RenderMeshPtr const& getRenderMesh(AssetFilePtr const& _assetFile);


        //////////////////////////////////////////
        RenderMeshLibraryData* addRenderMeshToLibrary(RenderMeshPtr const& _renderMesh);

        //////////////////////////////////////////
        void removeRenderMeshFromLibrary(HashedCString _renderMeshName);

        //////////////////////////////////////////
        inline void removeRenderMeshFromLibrary(CString _renderMeshName) { removeRenderMeshFromLibrary(_renderMeshName); }

        //////////////////////////////////////////
        inline void removeRenderMeshFromLibrary(String const& _renderMeshName) { removeRenderMeshFromLibrary(_renderMeshName); }


        //////////////////////////////////////////
        Vector<RenderMeshPtr> getRenderMeshesSorted();


        //////////////////////////////////////////
        void loadAllAssetRenderMeshes();

        //////////////////////////////////////////
        void unloadAssetRenderMeshes(Set<String> const& _tags);


    protected:

        //////////////////////////////////////////
        RenderMeshManager();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        void notifyRenderSystemInited();
    
    protected:
        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        RenderMeshPtr m_builtinRenderMeshes[BuiltinRenderMeshType::MAX];

        StringKeyMap<RenderMeshLibraryData> m_renderMeshesLibrary;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderMeshManager_hpp_
//////////////////////////////////////////
