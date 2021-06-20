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
#if (!defined(_MazeTerrainMesh3D_hpp_))
#define _MazeTerrainMesh3D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeAlignment2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(SubMesh);
    MAZE_USING_SHARED_PTR(TerrainMesh3D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(CanvasRenderer);


    //////////////////////////////////////////
    // Class TerrainMesh3D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API TerrainMesh3D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(TerrainMesh3D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(TerrainMesh3D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~TerrainMesh3D();

        //////////////////////////////////////////
        static TerrainMesh3DPtr Create(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        Transform3DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        MeshRendererPtr const& getMeshRenderer() const { return m_meshRenderer; }


        //////////////////////////////////////////
        inline Texture2DPtr const& getHeightMap() const { return m_heightMap; }

        //////////////////////////////////////////
        void setHeightMap(Texture2DPtr const& _value);

        //////////////////////////////////////////
        inline Vec2DU const& getCellsCount() const { return m_cellsCount; }

        //////////////////////////////////////////
        void setCellsCount(Vec2DU const& _value);

        //////////////////////////////////////////
        inline Vec2DF const& getSize() const { return m_size; }

        //////////////////////////////////////////
        void setSize(Vec2DF const& _value);

        //////////////////////////////////////////
        inline F32 getHeight() const { return m_height; }

        //////////////////////////////////////////
        void setHeight(F32 _value);


        //////////////////////////////////////////
        void clearMesh();

        //////////////////////////////////////////
        void buildMesh();

    protected:

        //////////////////////////////////////////
        TerrainMesh3D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            ECSWorld* _world,
            EntityCopyData _copyData) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        RenderSystem* m_renderSystem = nullptr;

        Transform3DPtr m_transform;
        MeshRendererPtr m_meshRenderer;

        Texture2DPtr m_heightMap;

        Vec2DU m_cellsCount = { 100, 100 };
        Vec2DF m_size = { 10.0f, 10.0f };
        F32 m_height = 1.0f;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTerrainMesh3D_hpp_
//////////////////////////////////////////
