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
#if (!defined(_MazeLight3D_hpp_))
#define _MazeLight3D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeColorF128.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Light3D);
    MAZE_USING_SHARED_PTR(RenderMask);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(MAZE_GRAPHICS_API, Light3DType,
        Directional,
        Spot,
        Point);


    //////////////////////////////////////////
    // Class Light3D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Light3D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Light3D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Light3D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Light3D();

        //////////////////////////////////////////
        static Light3DPtr Create(RenderSystem* _renderSystem = nullptr);

        
        //////////////////////////////////////////
        inline void setColor(ColorF128 const& _color) { m_color = _color; }

        //////////////////////////////////////////
        inline void setColor(ColorU32 _color)
        {
            setColor(
                (F32)_color.r / 255.0f,
                (F32)_color.g / 255.0f,
                (F32)_color.b / 255.0f,
                (F32)_color.a / 255.0f);
        }

        //////////////////////////////////////////
        inline void setColor(F32 _r, F32 _g, F32 _b, F32 _a = 1.0f)
        {
            setColor(ColorF128(_r, _g, _b, _a));
        }

        //////////////////////////////////////////
        inline void setColor(Vec3F const& _value)
        {
            setColor(_value.x, _value.y, _value.z);
        }

        //////////////////////////////////////////
        inline void setColor(Vec4F const& _value)
        {
            setColor(_value.x, _value.y, _value.z, _value.w);
        }

        //////////////////////////////////////////
        inline void setColor(U8 _r, U8 _g, U8 _b, U8 _a = 255)
        {
            setColor(
                (F32)_r / 255.0f,
                (F32)_g / 255.0f,
                (F32)_b / 255.0f,
                (F32)_a / 255.0f);
        }

        //////////////////////////////////////////
        inline ColorF128 const& getColor() const { return m_color; }


        //////////////////////////////////////////
        inline void setLightType(Light3DType _lightType) { m_lightType = _lightType; }

        //////////////////////////////////////////
        inline Light3DType getLightType() const { return m_lightType; }


        //////////////////////////////////////////
        inline Transform3DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        inline RenderMaskPtr const& getRenderMask() const { return m_renderMask; }


        //////////////////////////////////////////
        inline void setShadowCast(Bool _value) { m_shadowCast = _value; }

        //////////////////////////////////////////
        inline Bool getShadowCast() const { return m_shadowCast; }

        //////////////////////////////////////////
        inline void setShadowCastSize(F32 _value) { m_shadowCastSize = _value; }

        //////////////////////////////////////////
        inline F32 getShadowCastSize() const { return m_shadowCastSize; }

        //////////////////////////////////////////
        inline void setShadowCastNearZ(F32 _value) { m_shadowCastNearZ = _value; }

        //////////////////////////////////////////
        inline F32 getShadowCastNearZ() const { return m_shadowCastNearZ; }

        //////////////////////////////////////////
        inline void setShadowCastFarZ(F32 _value) { m_shadowCastFarZ = _value; }

        //////////////////////////////////////////
        inline F32 getShadowCastFarZ() const { return m_shadowCastFarZ; }

    protected:

        //////////////////////////////////////////
        Light3D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        bool init(
            Component* _component,
            EntityCopyData _copyData) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        RenderSystem* m_renderSystem;

        Transform3DPtr m_transform;
        RenderMaskPtr m_renderMask;

        Light3DType m_lightType;
        ColorF128 m_color;

        bool m_shadowCast = false;
        F32 m_shadowCastSize = 10.0f;
        F32 m_shadowCastNearZ = 1.0f;
        F32 m_shadowCastFarZ = 50.0f;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLight3D_hpp_
//////////////////////////////////////////
