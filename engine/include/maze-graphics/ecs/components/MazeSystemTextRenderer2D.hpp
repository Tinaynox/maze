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
#if (!defined(_MazeSystemTextRenderer2D_hpp_))
#define _MazeSystemTextRenderer2D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeAlignment2D.hpp"
#include "maze-graphics/MazeSystemFont.hpp"
#include "maze-graphics/ecs/components/MazeAbstractTextRenderer2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Material);
    MAZE_USING_MANAGED_SHARED_PTR(Mesh);
    MAZE_USING_MANAGED_SHARED_PTR(SubMesh);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(MeshRendererInstanced);
    MAZE_USING_SHARED_PTR(CanvasRenderer);


    //////////////////////////////////////////
    // Class SystemTextRenderer2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API SystemTextRenderer2D
        : public AbstractTextRenderer2D
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SystemTextRenderer2D, AbstractTextRenderer2D);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SystemTextRenderer2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~SystemTextRenderer2D();

        //////////////////////////////////////////
        static SystemTextRenderer2DPtr Create(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        inline SystemFontPtr const& getSystemFont() const { return m_systemFont; }

        //////////////////////////////////////////
        void setSystemFont(SystemFontPtr const& _systemFont);


        //////////////////////////////////////////
        virtual Vec2F getTextEnd(Size _rowIndex = 0) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2F getSymbolPosition(Size _symbolIndex) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Size getSymbolIndexAtPosition(Vec2F const& _position) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        SystemTextRenderer2D();

        //////////////////////////////////////////
        using Component::init;

        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            EntityCopyData _copyData) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void updateMeshDataNow() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void updateMaterial() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec4F calculateMeshColor() const MAZE_OVERRIDE;

    protected:
        SystemFontPtr m_systemFont;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemTextRenderer2D_hpp_
//////////////////////////////////////////
