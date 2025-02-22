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
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeSprite.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Sprite);
    MAZE_USING_MANAGED_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(MeshRendererInstanced);
    MAZE_USING_SHARED_PTR(CanvasRenderer);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(MAZE_GRAPHICS_API, SpriteRenderMode,
        Simple,
        Sliced,
        Tiled);


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
        enum class Flags : U8
        {
            None = 0,

            MeshDataDirty = MAZE_BIT(0),
            ColorDirty = MAZE_BIT(1),
            ModelMatricesDirty = MAZE_BIT(2),
            UV0Dirty = MAZE_BIT(3),
            MaterialDirty = MAZE_BIT(4),
        };

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
        inline MeshRendererInstancedPtr const& getMeshRenderer() const { return m_meshRenderer; }


        //////////////////////////////////////////
        inline U8 getFlags() const { return m_flags; }

        //////////////////////////////////////////
        inline void setFlags(U8 _flags) { m_flags = _flags; }

        //////////////////////////////////////////
        inline void setFlag(Flags _flag, bool _value)
        {
            if (_value)
                setFlags(m_flags | static_cast<U8>(_flag));
            else
                setFlags(m_flags & ~static_cast<U8>(_flag));
        }

        //////////////////////////////////////////
        inline bool getFlag(Flags flag) const { return (m_flags & static_cast<U8>(flag)) != 0; }

        //////////////////////////////////////////
        inline void enableFlag(Flags _flag) { setFlag(_flag, true); }

        //////////////////////////////////////////
        inline void disableFlag(Flags _flag) { setFlag(_flag, false); }

            
        //////////////////////////////////////////
        inline SpriteAssetRef const& getSpriteRef() const { return m_spriteRef; }

        //////////////////////////////////////////
        void setSpriteRef(SpriteAssetRef const& _sprite);

        //////////////////////////////////////////
        inline SpritePtr const& getSprite() const { return m_spriteRef.getSprite(); }

        //////////////////////////////////////////
        void setSprite(SpritePtr const& _sprite) { setSpriteRef(SpriteAssetRef(_sprite)); }

        //////////////////////////////////////////
        void setSprite(HashedString const& _spriteName);



        //////////////////////////////////////////
        inline MaterialAssetRef const& getMaterialRef() const { return m_materialRef; }

        //////////////////////////////////////////
        void setMaterialRef(MaterialAssetRef const& _material);

        //////////////////////////////////////////
        inline MaterialPtr const& getMaterial() const { return m_materialRef.getMaterial(); }

        //////////////////////////////////////////
        inline void setMaterial(MaterialPtr const& _material) { setMaterialRef(MaterialAssetRef(_material)); }



        //////////////////////////////////////////
        void setMaterial(CString _materialName);

        //////////////////////////////////////////
        void setMaterialRefCopy(MaterialAssetRef const& _material);

        //////////////////////////////////////////
        void setMaterialCopy(MaterialPtr const& _material);

        //////////////////////////////////////////
        void setMaterialCopy(CString _materialName);
            

        //////////////////////////////////////////
        void setColor(ColorU32 _color);

        //////////////////////////////////////////
        inline void setColor(U8 _r, U8 _g, U8 _b) { setColor(ColorU32{_r, _g, _b}); }

        //////////////////////////////////////////
        inline void setColor(U8 _r, U8 _g, U8 _b, U8 _a) { setColor(ColorU32{ _r, _g, _b, _a }); }

        //////////////////////////////////////////
        inline void setColorAlpha(U8 _a) { setColor(ColorU32{ m_color.r, m_color.g, m_color.b, _a }); }

        //////////////////////////////////////////
        inline ColorU32 getColor() const { return m_color; }            


        //////////////////////////////////////////
        void setRenderMode(SpriteRenderMode renderMode);

        //////////////////////////////////////////
        inline SpriteRenderMode getRenderMode() const { return m_renderMode; }


        //////////////////////////////////////////
        inline bool getPixelPerfect() const { return m_pixelPerfect; }

        //////////////////////////////////////////
        void setPixelPerfect(bool _value);

        

        //////////////////////////////////////////
        void prepareForRender();
        

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
            EntityCopyData _copyData = EntityCopyData()) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifySpriteDataChanged(Sprite* _sprite);


        //////////////////////////////////////////
        void updateMeshData();

        //////////////////////////////////////////
        void updateMaterial();

        //////////////////////////////////////////
        void updateMeshRendererColors();

        //////////////////////////////////////////
        void updateMeshRendererUV0s();

        //////////////////////////////////////////
        void updateMeshRendererModelMatrices();

    protected:
        RenderSystem* m_renderSystem = nullptr;

        Transform2DPtr m_transform;
        MeshRendererInstancedPtr m_meshRenderer;
        CanvasRendererPtr m_canvasRenderer;

    protected:
        SpriteAssetRef m_spriteRef;
        MaterialAssetRef m_materialRef;
        ColorU32 m_color = ColorU32::c_white;

        SpriteRenderMode m_renderMode = SpriteRenderMode::Simple;

        bool m_pixelPerfect = false;

    private:
        U8 m_flags = 0u;
        Vector<TMat> m_localMatrices;
        Vector<Vec4F> m_localColors;
        Vector<Vec4F> m_localUV0s;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSpriteRenderer2D_hpp_
//////////////////////////////////////////
