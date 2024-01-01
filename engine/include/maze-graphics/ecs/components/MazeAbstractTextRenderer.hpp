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
#if (!defined(_MazeAbstractTextRenderer_hpp_))
#define _MazeAbstractTextRenderer_hpp_


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
    MAZE_USING_SHARED_PTR(AbstractTextRenderer);


    //////////////////////////////////////////
    // Class AbstractTextRenderer
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API AbstractTextRenderer
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AbstractTextRenderer, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(AbstractTextRenderer);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~AbstractTextRenderer();

        

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
            setColor(ColorU32{ _r, _g, _b });
        }

        //////////////////////////////////////////
        inline void setColor(U8 _r, U8 _g, U8 _b, U8 _a)
        {
            setColor(ColorU32{ _r, _g, _b, _a });
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
        virtual Vec2F getTextEnd(Size _rowIndex = 0) MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual F32 getLineSpacingScale() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void setLineSpacingScale(F32 _value) MAZE_ABSTRACT;

    protected:

        //////////////////////////////////////////
        AbstractTextRenderer();

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
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAbstractTextRenderer_hpp_
//////////////////////////////////////////
