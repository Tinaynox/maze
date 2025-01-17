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
        Mat4F projectionMatrix = Mat4F::c_identity;
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
        LightingSettingsPtr lightingSettings;
    };


    //////////////////////////////////////////
    class MAZE_GRAPHICS_API IRenderUnitDrawer;


    //////////////////////////////////////////
    // Struct RenderUnit
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderUnit
    {
        /*
        //////////////////////////////////////////
        RenderUnit()
        {
            memset(uvStreams, 0, sizeof(uvStreams));
        }

        //////////////////////////////////////////
        RenderUnit(
            RenderPassPtr const& _renderPass,
            VertexArrayObjectPtr const& _vao,
            Vec3F const& _worldPosition,
            S32 _count,
            TMat const* _modelMatricies,
            Vec4F const* _colorStream,
            Vec4F const* _uvs[MAZE_UV_CHANNELS_MAX])
            : renderPass(_renderPass)
            , vao(_vao)
            , worldPosition(_worldPosition)
            , count(_count)
            , modelMatricies(_modelMatricies)
            , colorStream(_colorStream)
        {
            memcpy(uvStreams, _uvs, sizeof(uvStreams));
        }

        //////////////////////////////////////////
        RenderUnit(
            RenderPassPtr const& _renderPass,
            VertexArrayObjectPtr const& _vao,
            Vec3F const& _worldPosition,
            S32 _count,
            TMat const* _modelMatricies,
            Vec4F const* _colorStream = nullptr,
            Vec4F const* _uv0 = nullptr)
            : renderPass(_renderPass)
            , vao(_vao)
            , worldPosition(_worldPosition)
            , count(_count)
            , modelMatricies(_modelMatricies)
            , colorStream(_colorStream)
        {
            memset(uvStreams, 0, sizeof(uvStreams));
            uvStreams[0] = _uv0;
        }
        */

        //////////////////////////////////////////
        RenderUnit(
            RenderPassPtr const& _renderPass = RenderPassPtr(),
            Vec3F const& _worldPosition = Vec3F::c_zero,
            IRenderUnitDrawer* _drawer = nullptr,
            S32 _index = -1,
            U64 _userData = 0u)
            : renderPass(_renderPass)
            , worldPosition(_worldPosition)
            , drawer(_drawer)
            , index(_index)
            , userData(_userData)
        {}

        RenderPassPtr renderPass;
        Vec3F worldPosition;
        IRenderUnitDrawer* drawer = nullptr;
        S32 index = 0;
        U64 userData = 0u;

        F32 sqrDistanceToCamera = 0.0f;

        /*
        VertexArrayObjectPtr vao;
        S32 count = 0;
        TMat const* modelMatricies = nullptr;
        Vec4F const* colorStream = nullptr;
        Vec4F const* uvStreams[MAZE_UV_CHANNELS_MAX] = { nullptr };
        */
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

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsGraphicsEvents_hpp_
//////////////////////////////////////////
