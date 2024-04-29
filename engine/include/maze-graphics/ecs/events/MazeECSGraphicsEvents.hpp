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
#if (!defined(_MazeECSGraphicsEvents_hpp_))
#define _MazeECSGraphicsEvents_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/events/MazeECSEvents.hpp"
#include "maze-core/math/MazeRect2D.hpp"
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
        Mat4F cameraTransform = Mat4F::c_identity;
        Mat4F projectionMatrix = Mat4F::c_identity;
        Rect2DF viewport = Rect2DF(0.0f, 0.0f, 1.0f, 1.0f);
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
    // Struct RenderUnit
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderUnit
    {
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
            Mat4F const* _modelMatricies,
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
            Mat4F const* _modelMatricies,
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

        RenderPassPtr renderPass;
        VertexArrayObjectPtr vao;
        Vec3F worldPosition;
        S32 count = 0;
        Mat4F const* modelMatricies = nullptr;
        Vec4F const* colorStream = nullptr;
        Vec4F const* uvStreams[MAZE_UV_CHANNELS_MAX] = { nullptr };

        F32 sqrDistanceToCamera;
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


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeECSGraphicsEvents_hpp_
//////////////////////////////////////////
