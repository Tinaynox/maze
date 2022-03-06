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
#if (!defined(_MazeSpriteRenderer2D_hpp_))
#define _MazeSpriteRenderer2D_hpp_


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
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(CanvasRenderer);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_4(SpriteRenderMode,
        Simple,
        Sliced,
        Tiled,
        Custom);


    //////////////////////////////////////////
    // Class SpriteRenderer2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API SpriteRenderer2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SpriteRenderer2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SpriteRenderer2D);

        //////////////////////////////////////////
        friend class Entity;

        //////////////////////////////////////////
        using CustomRenderCallback = std::function<MeshPtr(SpriteRenderer2D*)>;

    public:

        //////////////////////////////////////////
        virtual ~SpriteRenderer2D();

        //////////////////////////////////////////
        static SpriteRenderer2DPtr Create(RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        inline Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        inline CanvasRendererPtr const& getCanvasRenderer() const { return m_canvasRenderer; }

            
        //////////////////////////////////////////
        inline SpritePtr const& getSprite() const { return m_sprite; }

        //////////////////////////////////////////
        void setSprite(SpritePtr const& _sprite);

        //////////////////////////////////////////
        void setSprite(HashedString const& _spriteName);


        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial() const { return m_material; }

        //////////////////////////////////////////
        void setMaterial(MaterialPtr const& _material);
            

        //////////////////////////////////////////
        void setColor(ColorU32 _color);

        //////////////////////////////////////////
        inline void setColor(U8 _r, U8 _g, U8 _b) { setColor({_r, _g, _b}); }

        //////////////////////////////////////////
        inline void setColor(U8 _r, U8 _g, U8 _b, U8 _a) { setColor({ _r, _g, _b, _a }); }

        //////////////////////////////////////////
        ColorU32 getColor() const { return m_color; }            


        //////////////////////////////////////////
        void setRenderMode(SpriteRenderMode renderMode);

        //////////////////////////////////////////
        inline SpriteRenderMode getRenderMode() const { return m_renderMode; }


        //////////////////////////////////////////
        void updateMaterial();

        //////////////////////////////////////////
        void updateMesh();


        //////////////////////////////////////////
        MeshRendererPtr const& getMeshRenderer() const { return m_meshRenderer; }


        //////////////////////////////////////////
        void setCustomRenderCallback(CustomRenderCallback _value);

        //////////////////////////////////////////
        inline CustomRenderCallback getCustomRenderCallback() const { return m_customRenderCallback; }

    protected:

        //////////////////////////////////////////
        SpriteRenderer2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            ECSWorld* _world,
            EntityCopyData _copyData = EntityCopyData()) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifySpriteDataChanged(Sprite* _sprite);

    protected:
        RenderSystem* m_renderSystem;

        Transform2DPtr m_transform;
        MeshRendererPtr m_meshRenderer;
        CanvasRendererPtr m_canvasRenderer;

        SpritePtr m_sprite;
        MaterialPtr m_material;
        ColorU32 m_color;

        SpriteRenderMode m_renderMode;

        CustomRenderCallback m_customRenderCallback;
    };


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS_SERIALIZATION(SpriteRenderMode);


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSpriteRenderer2D_hpp_
//////////////////////////////////////////
