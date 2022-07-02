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
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AbstractTextRenderer2D, Component);

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
        virtual void setText(String const& _text) MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline void setTextFormatted(CString _text, ...)
        {
            String newText;
            MAZE_FORMAT_VA_STRING(_text, newText);
            setText(newText);
        }

        //////////////////////////////////////////
        virtual String const& getText() const MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual void setColor(ColorU32 _color) MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline void setColor(U8 _r, U8 _g, U8 _b)
        {
            setColor({ _r, _g, _b });
        }

        //////////////////////////////////////////
        inline void setColor(U8 _r, U8 _g, U8 _b, U8 _a)
        {
            setColor({ _r, _g, _b, _a });
        }

        //////////////////////////////////////////
        virtual ColorU32 getColor() const MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual HorizontalAlignment2D getHorizontalAlignment() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void setHorizontalAlignment(HorizontalAlignment2D _horizontalAlignment) MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual VerticalAlignment2D getVerticalAlignment() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void setVerticalAlignment(VerticalAlignment2D _verticalAlignment) MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual Vec2DF getTextEnd(Size _rowIndex = 0) MAZE_ABSTRACT;

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
            ECSWorld* _world,
            EntityCopyData _copyData) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        RenderSystem* m_renderSystem = nullptr;

        Transform2DPtr m_transform;
        CanvasRendererPtr m_canvasRenderer;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAbstractTextRenderer2D_hpp_
//////////////////////////////////////////
