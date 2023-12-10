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
        static MeshPtr Create();

        //////////////////////////////////////////
        virtual ~Mesh();


        //////////////////////////////////////////
        void clear();


        //////////////////////////////////////////
        void setRenderDrawTopology(RenderDrawTopology _renderDrawTopology);

        //////////////////////////////////////////
        void setIndices(U32* _indices, Size _count);

        //////////////////////////////////////////
        void setPositions(Vec3F* _positions, Size _count);

        //////////////////////////////////////////
        void setNormals(Vec3F* _normals, Size _count);

        //////////////////////////////////////////
        void setColors(Vec4F* _colors, Size _count);
        
        //////////////////////////////////////////
        void setTexCoords(S32 _uvIndex, Vec2F* _data, Size _count);

        //////////////////////////////////////////
        void setTexCoords(S32 _uvIndex, Vec4F* _data, Size _count);


        //////////////////////////////////////////
        SubMeshPtr const& getSubMesh(Size _index) const;

        //////////////////////////////////////////
        inline Size getSubMeshesCount() const { return m_subMeshes.size(); }


        //////////////////////////////////////////
        void addSubMesh(SubMeshPtr const& _subMesh);


        //////////////////////////////////////////
        void scale(F32 _scale);

        //////////////////////////////////////////
        void mergeSubMeshes();

    protected:

        //////////////////////////////////////////
        Mesh();

        //////////////////////////////////////////
        virtual bool init();    
    

        //////////////////////////////////////////
        SubMeshPtr const& ensureSubMesh();

    protected:
        FastVector<SubMeshPtr> m_subMeshes;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMesh_hpp_
//////////////////////////////////////////
