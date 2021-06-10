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
#if (!defined(_MazeGizmoBillboard3D_hpp_))
#define _MazeGizmoBillboard3D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeColorU32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Sprite);
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(GizmoBillboard3D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(CanvasRenderer);


    //////////////////////////////////////////
    // Class GizmoBillboard3D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API GizmoBillboard3D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(GizmoBillboard3D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(GizmoBillboard3D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~GizmoBillboard3D();

        //////////////////////////////////////////
        static GizmoBillboard3DPtr Create();


        //////////////////////////////////////////
        inline Transform3DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        inline MeshRendererPtr const& getMeshRenderer() const { return m_meshRenderer; }

    protected:

        //////////////////////////////////////////
        GizmoBillboard3D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            ECSWorld* _world,
            EntityCopyData _copyData) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        RenderSystem* m_renderSystem;

        Transform3DPtr m_transform;
        MeshRendererPtr m_meshRenderer;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGizmoBillboard3D_hpp_
//////////////////////////////////////////
