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
#if (!defined(_MazeCamera3D_hpp_))
#define _MazeCamera3D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include "maze-core/math/MazeRay.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/ecs/MazeLightingSettings.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    // Class Camera3D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Camera3D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Camera3D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Camera3D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Camera3D();

        //////////////////////////////////////////
        static Camera3DPtr Create();


        //////////////////////////////////////////
        inline Transform3DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        inline void setRenderTarget(RenderTargetPtr const& _renderTarget) { m_renderTarget = _renderTarget; }

        //////////////////////////////////////////
        inline RenderTargetPtr const& getRenderTarget() const { return m_renderTarget; }


        //////////////////////////////////////////
        inline void setFOV(F32 _value) { m_fieldOfViewY = _value; }

        //////////////////////////////////////////
        inline F32 getFOV() const { return m_fieldOfViewY; }


        //////////////////////////////////////////
        inline void setNearZ(F32 _value) { m_nearZ = _value; }

        //////////////////////////////////////////
        inline F32 getNearZ() const { return m_nearZ; }


        //////////////////////////////////////////
        inline void setFarZ(F32 _value) { m_farZ = _value; }

        //////////////////////////////////////////
        inline F32 getFarZ() const { return m_farZ; }


        //////////////////////////////////////////
        inline void setViewport(Rect2DF const& _viewport) { m_viewport = _viewport; }

        //////////////////////////////////////////
        inline void setViewport(F32 _x, F32 _y, F32 _w, F32 _h) { setViewport(Rect2DF(_x, _y, _w, _h)); }

        //////////////////////////////////////////
        inline Rect2DF const& getViewport() const { return m_viewport; }

        //////////////////////////////////////////
        F32 getAspectRatio() const;


        //////////////////////////////////////////
        inline bool getClearColorFlag() const { return m_clearColorFlag; }

        //////////////////////////////////////////
        inline void setClearColorFlag(bool _clearColorFlag) { m_clearColorFlag = _clearColorFlag; }

        //////////////////////////////////////////
        inline ColorU32 const& getClearColor() const { return m_clearColor; }

        //////////////////////////////////////////
        inline void setClearColor(ColorU32 const& _clearColor) { m_clearColor = _clearColor; }


        //////////////////////////////////////////
        inline bool getClearDepthFlag() const { return m_clearDepthFlag; }

        //////////////////////////////////////////
        inline void setClearDepthFlag(bool _clearDepthFlag) { m_clearDepthFlag = _clearDepthFlag; }


        //////////////////////////////////////////
        inline bool getClearSkyBoxFlag() const { return m_clearSkyBoxFlag; }

        //////////////////////////////////////////
        inline void setClearSkyBoxFlag(bool _clearSkyBoxFlag) { m_clearSkyBoxFlag = _clearSkyBoxFlag; }


        //////////////////////////////////////////
        inline bool getDrawFlag() const { return m_drawFlag; }

        //////////////////////////////////////////
        inline void setDrawFlag(bool _value) { m_drawFlag = _value; }


        //////////////////////////////////////////
        inline S32 getRenderMask() const { return m_renderMask; }

        //////////////////////////////////////////
        inline void setRenderMask(S32 _renderMask) { m_renderMask = _renderMask; }


        //////////////////////////////////////////
        inline S32 getSortOrder() const { return m_sortOrder; }

        //////////////////////////////////////////
        inline void setSortOrder(S32 _sortOrder) { m_sortOrder = _sortOrder; }


        //////////////////////////////////////////
        LightingSettingsPtr const& getLightingSettings();


        //////////////////////////////////////////
        Vec2F convertWorldCoordsToViewportCoords(Vec3F const& _positionWS) const;

        //////////////////////////////////////////
        Vec3F convertViewportCoordsToWorldCoords(Vec2F const& _positionV) const;

        //////////////////////////////////////////
        Ray convertViewportCoordsToRay(Vec2F const& _positionV) const;

        //////////////////////////////////////////
        Mat4F calculateProjectionMatrix(RenderTargetPtr const& _renderTarget) const;

    protected:

        //////////////////////////////////////////
        Camera3D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        Transform3DPtr m_transform;

        RenderTargetPtr m_renderTarget;
        Rect2DF m_viewport = { 0.0f, 0.0f, 1.0f, 1.0f };

        F32 m_fieldOfViewY = Math::DegreesToRadians(60);
        F32 m_nearZ = 0.01f;
        F32 m_farZ = 200.0f;
            
        bool m_clearColorFlag = true;
        ColorU32 m_clearColor = ColorU32(99, 101, 140, 255);

        bool m_clearDepthFlag = true;

        bool m_clearSkyBoxFlag = false;

        bool m_drawFlag = true;

        S32 m_renderMask = ~S32(DefaultRenderMask::Gizmos);

        S32 m_sortOrder = 0;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCamera3D_hpp_
//////////////////////////////////////////
