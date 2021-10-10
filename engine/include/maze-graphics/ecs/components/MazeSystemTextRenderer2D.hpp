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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SystemText);
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(SubMesh);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(CanvasRenderer);


    //////////////////////////////////////////
    // Class SystemTextRenderer2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API SystemTextRenderer2D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SystemTextRenderer2D, Component);

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
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        CanvasRendererPtr const& getCanvasRenderer() const { return m_canvasRenderer; }

        //////////////////////////////////////////
        MeshRendererPtr const& getMeshRenderer() const { return m_meshRenderer; }


        //////////////////////////////////////////
        void setText(String const& _text);

        //////////////////////////////////////////
        inline String const& getText() const { return m_text; }


        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial() const { return m_material; }

        //////////////////////////////////////////
        void setMaterial(MaterialPtr const& _material);
            

        //////////////////////////////////////////
        void setColor(ColorU32 _color);

        //////////////////////////////////////////
        inline ColorU32 getColor() const { return m_color; }            


        //////////////////////////////////////////
        inline SystemFontPtr const& getSystemFont() const { return m_systemFont; }

        //////////////////////////////////////////
        void setSystemFont(SystemFontPtr const& _systemFont);


        //////////////////////////////////////////
        inline U32 getFontSize() const { return m_fontSize; }

        //////////////////////////////////////////
        void setFontSize(U32 _fontSize) { m_fontSize = _fontSize; }


        //////////////////////////////////////////
        HorizontalAlignment2D getHorizontalAlignment() const { return m_horizontalAlignment; }

        //////////////////////////////////////////
        inline void setHorizontalAlignment(HorizontalAlignment2D _horizontalAlignment)
        {
            if (m_horizontalAlignment == _horizontalAlignment)
                return;

            m_horizontalAlignment = _horizontalAlignment;

            updateMesh();
        }


        //////////////////////////////////////////
        inline VerticalAlignment2D getVerticalAlignment() const { return m_verticalAlignment; }

        //////////////////////////////////////////
        inline void setVerticalAlignment(VerticalAlignment2D _verticalAlignment)
        {
            if (m_verticalAlignment == _verticalAlignment)
                return;

            m_verticalAlignment = _verticalAlignment;

            updateMesh();
        }


        //////////////////////////////////////////
        void updateMesh();


        //////////////////////////////////////////
        Vec2DF getTextEnd(Size _rowIndex = 0);

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
            ECSWorld* _world,
            EntityCopyData _copyData) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void updateMaterial();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void calculateTextData(
            S32& _rowsCount,
            S32& _maxColumnsCount,
            S32& _charsCount);

    protected:
        RenderSystem* m_renderSystem;

        Transform2DPtr m_transform;
        MeshRendererPtr m_meshRenderer;
        CanvasRendererPtr m_canvasRenderer;

        String m_text;

        MaterialPtr m_material;
        ShaderUniformVariantPtr m_colorUniform;
        ShaderUniformVariantPtr m_baseMapUniform;
        ShaderUniformVariantPtr m_baseMapTexelSizeUniform;

        ColorU32 m_color;

        SystemFontPtr m_systemFont;
        U32 m_fontSize;

        HorizontalAlignment2D m_horizontalAlignment;
        VerticalAlignment2D m_verticalAlignment;

        MeshPtr m_mesh;
        SubMeshPtr m_subMesh;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemTextRenderer2D_hpp_
//////////////////////////////////////////
