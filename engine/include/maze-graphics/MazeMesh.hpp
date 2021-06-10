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
#if (!defined(_MazeMesh_hpp_))
#define _MazeMesh_hpp_


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
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(SubMesh);
    

    //////////////////////////////////////////
    // Class Mesh
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Mesh
        : public SharedObject<Mesh>
    {
    public:

        //////////////////////////////////////////
        static MeshPtr Create(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        virtual ~Mesh();


        //////////////////////////////////////////
        inline RenderSystem* getRenderSystem() const { return m_renderSystem; }


        //////////////////////////////////////////
        void clear();


        //////////////////////////////////////////
        void setRenderDrawTopology(RenderDrawTopology _renderDrawTopology);

        //////////////////////////////////////////
        void setIndices(U32* _indices, Size _count);

        //////////////////////////////////////////
        void setPositions(Vec3DF* _positions, Size _count);

        //////////////////////////////////////////
        void setNormals(Vec3DF* _normals, Size _count);

        //////////////////////////////////////////
        void setColors(Vec4DF* _colors, Size _count);
        
        //////////////////////////////////////////
        void setTexCoords(S32 _uvIndex, Vec2DF* _data, Size _count);

        //////////////////////////////////////////
        void setTexCoords(S32 _uvIndex, Vec4DF* _data, Size _count);


        //////////////////////////////////////////
        SubMeshPtr const& getSubMesh(Size _index);

        //////////////////////////////////////////
        inline Size getSubMeshesCount() { return m_subMeshes.size(); }


        //////////////////////////////////////////
        void addSubMesh(SubMeshPtr const& _subMesh);

    protected:

        //////////////////////////////////////////
        Mesh();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem);    
    

        //////////////////////////////////////////
        SubMeshPtr const& ensureSubMesh();

    protected:
        RenderSystem* m_renderSystem;

        FastVector<SubMeshPtr> m_subMeshes;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMesh_hpp_
//////////////////////////////////////////
