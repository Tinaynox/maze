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
#if (!defined(_MazeAbstractTextRenderer2D_hpp_))
#define _MazeAbstractTextRenderer2D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeAlignment2D.hpp"
#include "maze-graphics/MazeSystemFont.hpp"
#include "maze-graphics/ecs/components/MazeAbstractTextRenderer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(SubMesh);
    MAZE_USING_SHARED_PTR(AbstractTextRenderer2D);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(CanvasRenderer);


    //////////////////////////////////////////
    // Class AbstractTextRenderer2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API AbstractTextRenderer2D
        : public AbstractTextRenderer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AbstractTextRenderer2D, AbstractTextRenderer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(AbstractTextRenderer2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~AbstractTextRenderer2D();


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        CanvasRendererPtr const& getCanvasRenderer() const { return m_canvasRenderer; }


        //////////////////////////////////////////
        inline Vec2F const& getBoundingSize() const { return m_boundingSize; }


        //////////////////////////////////////////
        virtual bool getPixelPerfect() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void setPixelPerfect(bool _value) MAZE_ABSTRACT;

    protected:

        //////////////////////////////////////////
        AbstractTextRenderer2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            EntityCopyData _copyData) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:

        Transform2DPtr m_transform;
        CanvasRendererPtr m_canvasRenderer;

        Vec2F m_boundingSize = Vec2F::c_zero;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAbstractTextRenderer2D_hpp_
//////////////////////////////////////////
