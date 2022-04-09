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
        inline String const& getName() const { return m_name; }

        //////////////////////////////////////////
        void setName(String const& _name);


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
        void clear();


        //////////////////////////////////////////
        String toString() const;

        //////////////////////////////////////////
        static RenderMeshPtr FromString(String const& _string);
        
#if MAZE_DEBUG
        //////////////////////////////////////////
        void updateVAODebugInfo();
#endif

    protected:

        //////////////////////////////////////////
        RenderMesh();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem);    
    

        //////////////////////////////////////////
        void clearVertexArrayObjects();


    protected:
        RenderSystem* m_renderSystem;

        String m_name;

        Vector<VertexArrayObjectPtr> m_vertexArrayObjects;
    };


    //////////////////////////////////////////
    MAZE_NOT_IMPLEMENTED_SERIALIZATION(RenderMesh);


    //////////////////////////////////////////
    inline void ValueToString(RenderMeshPtr const& _value, String& _data)
    {
        _data = _value ? _value->toString() : String();
    }
    inline void ValueFromString(RenderMeshPtr& _value, CString _data, Size _count)
    {
        _value = RenderMesh::FromString(String(_data, _count));
    }
    inline U32 GetValueSerializationSize(RenderMeshPtr const& _value)
    {
        MAZE_TODO;
        return 0;
    }
        inline void SerializeValue(RenderMeshPtr const& _value, U8* _data)
    {
        MAZE_TODO;
    }
    inline void DeserializeValue(RenderMeshPtr& _value, U8 const* _data)
    {
        MAZE_TODO;
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderMesh_hpp_
//////////////////////////////////////////
