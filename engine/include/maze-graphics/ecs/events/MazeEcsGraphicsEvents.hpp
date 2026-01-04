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
#if (!defined(_MazeEcsGraphicsEvents_hpp_))
#define _MazeEcsGraphicsEvents_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"
#include "maze-core/math/MazeRect2.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/ecs/MazeLightingSettings.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/config/MazeGraphicsConfig.hpp"
#include "maze-graphics/MazeFrustum.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct DefaultPassParams
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API DefaultPassParams
    {
        S32 renderMask = 0;
        TMat cameraTransform = TMat::c_identity;
        TMat viewMatrix = TMat::c_identity;
        Mat4F projectionMatrix = Mat4F::c_identity;
        Frustum cameraFrustum;
        Rect2F viewport = Rect2F(0.0f, 0.0f, 1.0f, 1.0f);
        F32 nearZ = 0.001f;
        F32 farZ = 100.0f;
        F32 fieldOfViewY = Math::DegreesToRadians(30.0f);
        bool clearColorFlag = true;
        ColorU32 clearColor = ColorU32::c_green;
        bool clearDepthFlag = true;
        bool clearSkyBoxFlag = false;
        bool drawFlag = true;
        bool clipViewport = true;
        LightingSettings const* lightingSettings = nullptr;
        Vec4F mainLightColor = Vec4F::c_zero;
        Vec3F mainLightDirection = Vec3F::c_negativeUnitY;
        Mat4F mainLightViewProjectionMatrix = Mat4F::c_identity;
        Texture2DPtr mainLightShadowMap; // #TODO: Replace with ResourceId?
        S32 lightsCount = 0;
        Vec4F lightsPosRadius[MAZE_DYNAMIC_LIGHTS_MAX];
        Vec3F lightsColor[MAZE_DYNAMIC_LIGHTS_MAX];
    };

    //////////////////////////////////////////
    // Struct ShadowPassParams
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API ShadowPassParams
    {
        S32 renderMask = 0;
        TMat mainLightTransform = TMat::c_identity;
        TMat viewMatrix = TMat::c_identity;
        Mat4F mainLightProjectionMatrix = Mat4F::c_identity;
        Frustum mainLightFrustum;
        F32 nearZ = 0.001f;
        F32 farZ = 100.0f;
    };


    //////////////////////////////////////////
    class MAZE_GRAPHICS_API IRenderUnitDrawer;


    //////////////////////////////////////////
    // Struct RenderUnit
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderUnit
    {
        //////////////////////////////////////////
        RenderUnit(
            RenderPass* _renderPass = nullptr,
            Vec3F const& _worldPosition = Vec3F::c_zero,
            IRenderUnitDrawer* _drawer = nullptr,
            S32 _index = -1,
            U64 _userData = 0u,
            S32 _sortIndex = -1)
            : renderPass(_renderPass)
            , worldPosition(_worldPosition)
            , drawer(_drawer)
            , index(_index)
            , userData(_userData)
            , sortIndex(_sortIndex)
        {}

        RenderPass* renderPass;
        Vec3F worldPosition;
        IRenderUnitDrawer* drawer = nullptr;
        S32 index = 0;
        U64 userData = 0u;
        S32 sortIndex = -1;

        // Calculating values
        F32 sqrDistanceToCamera = 0.0f;
        U64 sortKey = 0u;
    };


    //////////////////////////////////////////
    class MAZE_GRAPHICS_API IRenderUnitDrawer
    {
    public:

        //////////////////////////////////////////
        virtual void drawDefaultPass(
            RenderQueuePtr const& _renderQueue,
            DefaultPassParams const& _params,
            RenderUnit const& _renderUnit) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void drawShadowPass(
            RenderQueuePtr const& _renderQueue,
            ShadowPassParams const& _params,
            RenderUnit const& _renderUnit) MAZE_ABSTRACT;
    };


    //////////////////////////////////////////
    // Class Render3DDefaultPrePassEvent
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Render3DDefaultPrePassEvent
        : public GenericEvent<Render3DDefaultPrePassEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Render3DDefaultPrePassEvent, Event);

    public:

        //////////////////////////////////////////
        inline Render3DDefaultPrePassEvent(
            RenderTarget* _renderTarget = nullptr,
            DefaultPassParams const* _passParams = nullptr)
            : m_renderTarget(_renderTarget)
            , m_passParams(_passParams)
        {}

        //////////////////////////////////////////
        inline RenderTarget* getRenderTarget() const { return m_renderTarget; }

        //////////////////////////////////////////
        inline DefaultPassParams const* getPassParams() const { return m_passParams; }

    private:
        RenderTarget* m_renderTarget = nullptr;
        DefaultPassParams const* m_passParams;
    };


    //////////////////////////////////////////
    // Class Render3DDefaultPassGatherRenderUnitsEvent
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Render3DDefaultPassGatherRenderUnitsEvent
        : public GenericEvent<Render3DDefaultPassGatherRenderUnitsEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Render3DDefaultPassGatherRenderUnitsEvent, Event);

    public:

        //////////////////////////////////////////
        inline Render3DDefaultPassGatherRenderUnitsEvent(
            RenderTarget* _renderTarget = nullptr,
            DefaultPassParams const* _passParams = nullptr,
            Vector<RenderUnit>* _renderUnits = nullptr)
            : m_renderTarget(_renderTarget)
            , m_passParams(_passParams)
            , m_renderUnits(_renderUnits)
        {}

        //////////////////////////////////////////
        inline RenderTarget* getRenderTarget() const { return m_renderTarget; }

        //////////////////////////////////////////
        inline DefaultPassParams const* getPassParams() const { return m_passParams; }

        //////////////////////////////////////////
        inline Vector<RenderUnit>* getRenderUnits() const { return m_renderUnits; }

    private:
        RenderTarget* m_renderTarget = nullptr;
        DefaultPassParams const* m_passParams;
        Vector<RenderUnit>* m_renderUnits;
    };


    //////////////////////////////////////////
    // Class Render3DShadowPassGatherRenderUnitsEvent
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Render3DShadowPassGatherRenderUnitsEvent
        : public GenericEvent<Render3DShadowPassGatherRenderUnitsEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Render3DShadowPassGatherRenderUnitsEvent, Event);

    public:

        //////////////////////////////////////////
        inline Render3DShadowPassGatherRenderUnitsEvent(
            RenderBuffer* _shadowBuffer = nullptr,
            ShadowPassParams const* _passParams = nullptr,
            Vector<RenderUnit>* _renderUnits = nullptr)
            : m_shadowBuffer(_shadowBuffer)
            , m_passParams(_passParams)
            , m_renderUnits(_renderUnits)
        {}

        //////////////////////////////////////////
        inline RenderBuffer* getShadowBuffer() const { return m_shadowBuffer; }

        //////////////////////////////////////////
        inline ShadowPassParams const* getPassParams() const { return m_passParams; }

        //////////////////////////////////////////
        inline Vector<RenderUnit>* getRenderUnits() const { return m_renderUnits; }

    private:
        RenderBuffer* m_shadowBuffer = nullptr;
        ShadowPassParams const* m_passParams;
        Vector<RenderUnit>* m_renderUnits;
    };


    //////////////////////////////////////////
    // Class Render3DDefaultPostPassEvent
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Render3DDefaultPostPassEvent
        : public GenericEvent<Render3DDefaultPostPassEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Render3DDefaultPostPassEvent, Event);

    public:

        //////////////////////////////////////////
        inline Render3DDefaultPostPassEvent(
            RenderTarget* _renderTarget = nullptr,
            DefaultPassParams const* _passParams = nullptr)
            : m_renderTarget(_renderTarget)
            , m_passParams(_passParams)
        {}

        //////////////////////////////////////////
        inline RenderTarget* getRenderTarget() const { return m_renderTarget; }

        //////////////////////////////////////////
        inline DefaultPassParams const* getPassParams() const { return m_passParams; }

    private:
        RenderTarget* m_renderTarget = nullptr;
        DefaultPassParams const* m_passParams;
    };



    //////////////////////////////////////////
    // Class Render2DPostUpdateEvent
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Render2DPostUpdateEvent
        : public GenericEvent<Render2DPostUpdateEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Render2DPostUpdateEvent, Event);

    public:

        //////////////////////////////////////////
        inline Render2DPostUpdateEvent(
            F32 _dt = 0.0f)
            : m_dt(_dt)
        {}

        //////////////////////////////////////////
        inline F32 getDt() const { return m_dt; }

    private:
        F32 m_dt = 0.0f;
    };


    //////////////////////////////////////////
    MAZE_DECLARE_SIMPLE_GENERIC_EVENT_API(MAZE_GRAPHICS_API, SpriteRenderer2DMaterialUpdatedEvent);

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsGraphicsEvents_hpp_
//////////////////////////////////////////
