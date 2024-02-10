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
#if (!defined(_MazeVertexArrayObject_hpp_))
#define _MazeVertexArrayObject_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeVertex.hpp"
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
    MAZE_USING_SHARED_PTR(VertexArrayObject);
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(SubMesh);
    

    //////////////////////////////////////////
    // Class VertexArrayObject
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API VertexArrayObject
        : public SharedObject<VertexArrayObject>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~VertexArrayObject();

        //////////////////////////////////////////
        static VertexArrayObjectPtr Create(
            RenderSystem* _renderSystem,
            RenderTarget* _renderTarget = nullptr);


        //////////////////////////////////////////
        inline void setRenderDrawTopology(RenderDrawTopology _renderDrawTopology) { m_renderDrawTopology = _renderDrawTopology; }

        //////////////////////////////////////////
        inline RenderDrawTopology getRenderDrawTopology() const { return m_renderDrawTopology; }


        //////////////////////////////////////////
        virtual void setIndices(
            U8 const* _indicesData,
            VertexAttributeType _indicesType,
            Size _indicesCount) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void setVerticesData(
            U8 const* _data,
            VertexAttributeDescription _description,
            Size _verticesCount) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual SubMeshPtr readAsSubMesh() const MAZE_ABSTRACT;


        //////////////////////////////////////////
        void setMesh(SubMeshPtr const& _subMesh);


        //////////////////////////////////////////
        inline Size getIndicesCount() const { return m_indicesCount; }

        //////////////////////////////////////////
        inline VertexAttributeType getIndicesType() const { return m_indicesType; }

        //////////////////////////////////////////
        inline void clear() { m_indicesCount = 0; }
        

#if MAZE_DEBUG
        //////////////////////////////////////////
        inline String const& getDebugInfo() const { return m_debugInfo; }

        //////////////////////////////////////////
        inline void setDebugInfo(String const& _value) { m_debugInfo = _value; }

        //////////////////////////////////////////
        virtual void debug() MAZE_ABSTRACT;
#endif

    protected:

        //////////////////////////////////////////
        VertexArrayObject();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem);    
    
    protected:
        RenderSystem* m_renderSystem;

        RenderDrawTopology m_renderDrawTopology;

        VertexAttributeType m_indicesType;
        Size m_indicesCount;

#if MAZE_DEBUG
        String m_debugInfo;
#endif
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVertexArrayObject_hpp_
//////////////////////////////////////////
