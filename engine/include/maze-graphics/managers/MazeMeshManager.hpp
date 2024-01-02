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
    MAZE_USING_SHARED_PTR(Texture);
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(ByteBuffer);
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Enum BuiltinMeshType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_6_API(MAZE_GRAPHICS_API, BuiltinMeshType,
        Quad,
        Cube,
        Sphere,
        Cone,
        Cylinder,
        Torus);


    //////////////////////////////////////////
    struct MeshLoaderProperties
    {
        F32 scale = 1.0f;
        bool mergeSubMeshes = false;
    };


    //////////////////////////////////////////
    using LoadMeshAssetFileFunction = bool(*)(
        AssetFile const& _file,
        Mesh& _mesh,
        MeshLoaderProperties const& _props);
    using LoadMeshByteBufferFunction = bool(*)(
        ByteBuffer const& _fileData,
        Mesh& _mesh,
        MeshLoaderProperties const& _props);
    using IsMeshAssetFileFunction = bool(*)(AssetFile const& _file);
    using IsMeshByteBufferFunction = bool(*)(ByteBuffer const& _fileData);


    //////////////////////////////////////////
    // Struct MeshLoaderData
    //
    //////////////////////////////////////////
    struct MeshLoaderData
    {
        //////////////////////////////////////////
        MeshLoaderData() = default;

        //////////////////////////////////////////
        MeshLoaderData(
            LoadMeshAssetFileFunction _loadMeshAssetFileFunc,
            LoadMeshByteBufferFunction _loadMeshByteBufferFunc,
            IsMeshAssetFileFunction _isMeshAssetFileFunc,
            IsMeshByteBufferFunction _isMeshByteBufferFunc)
            : loadMeshAssetFileFunc(_loadMeshAssetFileFunc)
            , loadMeshByteBufferFunc(_loadMeshByteBufferFunc)
            , isMeshAssetFileFunc(_isMeshAssetFileFunc)
            , isMeshByteBufferFunc(_isMeshByteBufferFunc)
        {}

        LoadMeshAssetFileFunction loadMeshAssetFileFunc = nullptr;
        LoadMeshByteBufferFunction loadMeshByteBufferFunc = nullptr;
        IsMeshAssetFileFunction isMeshAssetFileFunc = nullptr;
        IsMeshByteBufferFunction isMeshByteBufferFunc = nullptr;
    };


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
        static void Initialize(MeshManagerPtr& _meshManager);

        //////////////////////////////////////////
        static inline MeshManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline MeshManager& GetInstance() { return *s_instance; }

       
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



        //////////////////////////////////////////
        void registerMeshLoader(
            HashedCString _extension,
            MeshLoaderData const& _data)
        {
            m_meshLoaders.insert(_extension, _data);
            eventMeshLoaderAdded(_extension, _data);
        }

        //////////////////////////////////////////
        void clearMeshLoader(HashedCString _extension)
        {
            m_meshLoaders.erase(_extension);
        }

        //////////////////////////////////////////
        Vector<String> getMeshLoaderExtensions();


        //////////////////////////////////////////
        MeshPtr loadMesh(AssetFilePtr const& _assetFile);

    public:

        //////////////////////////////////////////
        MultiDelegate<HashedCString, MeshLoaderData const&> eventMeshLoaderAdded;

    protected:

        //////////////////////////////////////////
        MeshManager();

        //////////////////////////////////////////
        virtual bool init();
    
    protected:
        static MeshManager* s_instance;

        MeshPtr m_builtinMeshes[BuiltinMeshType::MAX];

        StringKeyMap<MeshLoaderData> m_meshLoaders;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshManager_hpp_
//////////////////////////////////////////
