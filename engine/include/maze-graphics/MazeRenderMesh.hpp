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
#if (!defined(_MazeRenderMesh_hpp_))
#define _MazeRenderMesh_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeRenderDrawTopology.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/MazeObject.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/containers/MazeFastVector.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(VertexArrayObject);
    

    //////////////////////////////////////////
    // Class RenderMesh
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderMesh
        : public SharedObject<RenderMesh>
        , public Object
        , public IStringSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderMesh, Object);

        //////////////////////////////////////////
        using RenderMeshDeleter = std::function<void(RenderMesh* _ptr)>;

    public:

        //////////////////////////////////////////
        static RenderMeshPtr Create(
            RenderSystem* _renderSystem = nullptr,
            RenderMeshDeleter const& _deleter = DefaultDelete<RenderMesh>());

        //////////////////////////////////////////
        static RenderMeshPtr Create(
            AssetFilePtr const& _assetFile,
            RenderSystem* _renderSystem = nullptr,
            RenderMeshDeleter const& _deleter = DefaultDelete<RenderMesh>(),
            RenderTarget* _renderTarget = nullptr);

        //////////////////////////////////////////
        static RenderMeshPtr Create(
            String const& _assetFileName,
            RenderSystem* _renderSystem = nullptr,
            RenderMeshDeleter const& _deleter = DefaultDelete<RenderMesh>(),
            RenderTarget* _renderTarget = nullptr);

        //////////////////////////////////////////
        static RenderMeshPtr Create(
            MeshPtr const& _mesh,
            RenderSystem* _renderSystem = nullptr,
            RenderMeshDeleter const& _deleter = DefaultDelete<RenderMesh>(),
            RenderTarget* _renderTarget = nullptr);

        //////////////////////////////////////////
        virtual ~RenderMesh();



        //////////////////////////////////////////
        inline RenderSystem* getRenderSystem() const { return m_renderSystem; }


        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }

        //////////////////////////////////////////
        void setName(HashedString const& _name);


        //////////////////////////////////////////
        inline VertexArrayObjectPtr const& getVertexArrayObject() const { return getVertexArrayObject(0); }

        //////////////////////////////////////////
        inline VertexArrayObjectPtr const& getVertexArrayObject(Size _index) const { return m_vertexArrayObjects[_index]; }

        //////////////////////////////////////////
        inline Vector<VertexArrayObjectPtr> const& getVertexArrayObjects() const { return m_vertexArrayObjects; }

        //////////////////////////////////////////
        inline void setVertexArrayObjects(Vector<VertexArrayObjectPtr> const& _vertexArrayObjects) { m_vertexArrayObjects = _vertexArrayObjects; }


        //////////////////////////////////////////
        void setVertexArrayObject(VertexArrayObjectPtr const& _vao);


        //////////////////////////////////////////
        void loadFromAssetFile(
            String const& _assetFileName,
            RenderTarget* _renderTarget = nullptr);

        //////////////////////////////////////////
        void loadFromAssetFile(
            AssetFilePtr const& _assetFile,
            RenderTarget* _renderTarget = nullptr);

        //////////////////////////////////////////
        void loadFromMesh(
            MeshPtr const& _mesh,
            RenderTarget* _renderTarget = nullptr);


        //////////////////////////////////////////
        inline MeshPtr const& getMesh() const { return m_mesh; }


        //////////////////////////////////////////
        void clear();


        //////////////////////////////////////////
        inline void lock() { m_locked = true; }

        //////////////////////////////////////////
        inline void unlock() { m_locked = false; }



        //////////////////////////////////////////
        static RenderMeshPtr FromString(String const& _string);
        
#if MAZE_DEBUG
        //////////////////////////////////////////
        void updateVAODebugInfo();
#endif

    public:

        //////////////////////////////////////////
        virtual String toString() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderMesh();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem);    
    

        //////////////////////////////////////////
        void clearVertexArrayObjects();


    protected:
        bool m_locked = false;
        RenderSystem* m_renderSystem;

        MeshPtr m_mesh;
        HashedString m_name;

        Vector<VertexArrayObjectPtr> m_vertexArrayObjects;
    };


    //////////////////////////////////////////
    // Class RenderMeshAssetRef
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderMeshAssetRef
        : public IStringSerializable
        , public IDataBlockSerializable
    {
    public:

        //////////////////////////////////////////
        RenderMeshAssetRef(RenderMeshPtr const& _renderMesh = nullptr)
            : m_renderMesh(_renderMesh)
        {}


        //////////////////////////////////////////
        void setRenderMesh(RenderMeshPtr const& _renderMesh) { m_renderMesh = _renderMesh; }

        //////////////////////////////////////////
        inline RenderMeshPtr const& getRenderMesh() const { return m_renderMesh; }

        //////////////////////////////////////////
        virtual String toString() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline bool operator==(RenderMeshAssetRef const& _value) const { return m_renderMesh == _value.getRenderMesh(); }

        //////////////////////////////////////////
        inline bool operator!=(RenderMeshAssetRef const& _value) const { return m_renderMesh != _value.getRenderMesh(); }

    public:
        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE;

    private:
        RenderMeshPtr m_renderMesh;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderMesh_hpp_
//////////////////////////////////////////
