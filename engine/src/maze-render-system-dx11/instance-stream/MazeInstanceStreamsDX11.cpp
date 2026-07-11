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
#include "MazeRenderSystemDX11Header.hpp"
#include "maze-render-system-dx11/instance-stream/MazeInstanceStreamsDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeStateMachineDX11.hpp"
#include "maze-render-system-dx11/MazeShaderDX11.hpp"
#include "maze-graphics/MazeShaderUniform.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/config/MazeGraphicsConfig.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Instance streams work via constant buffer arrays (32 instances per draw call)
    static U32 const c_instancesPerDrawCallDX11 = 32u;
    static U32 const c_instancesPerDrawDX11 = 16384u * 2u;

    //////////////////////////////////////////
    static HashedCString const c_colorsStreamUniformName = MAZE_HASHED_CSTRING("u_colorsStream");

    //////////////////////////////////////////
    static HashedCString const c_uvStreamUniformNames[MAZE_UV_CHANNELS_MAX] =
    {
        MAZE_HASHED_CSTRING("u_uv0Stream"),
        MAZE_HASHED_CSTRING("u_uv1Stream"),
        MAZE_HASHED_CSTRING("u_uv2Stream"),
        MAZE_HASHED_CSTRING("u_uv3Stream"),
        MAZE_HASHED_CSTRING("u_uv4Stream"),
        MAZE_HASHED_CSTRING("u_uv5Stream"),
        MAZE_HASHED_CSTRING("u_uv6Stream"),
        MAZE_HASHED_CSTRING("u_uv7Stream")
    };


    //////////////////////////////////////////
    // Class InstanceStreamModelMatrixDX11
    //
    //////////////////////////////////////////
    InstanceStreamModelMatrixDX11Ptr InstanceStreamModelMatrixDX11::Create(RenderSystemDX11* _renderSystem)
    {
        InstanceStreamModelMatrixDX11Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InstanceStreamModelMatrixDX11, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool InstanceStreamModelMatrixDX11::init(RenderSystemDX11* _renderSystem)
    {
        if (!InstanceStreamModelMatrix::init())
            return false;

        m_renderSystem = _renderSystem;

        m_maxInstancesPerDrawCall = c_instancesPerDrawCallDX11;
        m_maxInstancesPerDraw = c_instancesPerDrawDX11;
        m_data.resize(m_maxInstancesPerDraw);

        return true;
    }

    //////////////////////////////////////////
    void InstanceStreamModelMatrixDX11::prepareForRender(S32 _instancesCount)
    {
        ShaderDX11* shader = m_renderSystem->getStateMachine()->getCurrentShader();
        if (!shader)
            return;

        if (shader->getModelMatricesUniform())
            shader->getModelMatricesUniform()->upload(&m_data[m_dataOffset], _instancesCount);
    }

    //////////////////////////////////////////
    void InstanceStreamModelMatrixDX11::processDrawBegin()
    {
    }


    //////////////////////////////////////////
    // Class InstanceStreamColorDX11
    //
    //////////////////////////////////////////
    InstanceStreamColorDX11Ptr InstanceStreamColorDX11::Create(RenderSystemDX11* _renderSystem)
    {
        InstanceStreamColorDX11Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InstanceStreamColorDX11, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool InstanceStreamColorDX11::init(RenderSystemDX11* _renderSystem)
    {
        if (!InstanceStreamColor::init())
            return false;

        m_renderSystem = _renderSystem;

        m_maxInstancesPerDrawCall = c_instancesPerDrawCallDX11;
        m_maxInstancesPerDraw = c_instancesPerDrawDX11;
        m_data.resize(m_maxInstancesPerDraw);

        return true;
    }

    //////////////////////////////////////////
    void InstanceStreamColorDX11::prepareForRender(S32 _instancesCount)
    {
        ShaderDX11* shader = m_renderSystem->getStateMachine()->getCurrentShader();
        if (!shader)
            return;

        ShaderUniformPtr const& colorsStreamUniform = shader->getUniform(c_colorsStreamUniformName);
        if (colorsStreamUniform)
            colorsStreamUniform->upload(&m_data[m_dataOffset], _instancesCount);
    }

    //////////////////////////////////////////
    void InstanceStreamColorDX11::processDrawBegin()
    {
    }


    //////////////////////////////////////////
    // Class InstanceStreamUVDX11
    //
    //////////////////////////////////////////
    InstanceStreamUVDX11Ptr InstanceStreamUVDX11::Create(
        S32 _index,
        RenderSystemDX11* _renderSystem)
    {
        InstanceStreamUVDX11Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InstanceStreamUVDX11, object, init(_index, _renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool InstanceStreamUVDX11::init(
        S32 _index,
        RenderSystemDX11* _renderSystem)
    {
        if (!InstanceStreamUV::init(_index))
            return false;

        m_renderSystem = _renderSystem;

        m_maxInstancesPerDrawCall = c_instancesPerDrawCallDX11;
        m_maxInstancesPerDraw = c_instancesPerDrawDX11;
        m_data.resize(m_maxInstancesPerDraw);

        return true;
    }

    //////////////////////////////////////////
    void InstanceStreamUVDX11::prepareForRender(S32 _instancesCount)
    {
        ShaderDX11* shader = m_renderSystem->getStateMachine()->getCurrentShader();
        if (!shader)
            return;

        ShaderUniformPtr const& uvStreamUniform = shader->getUniform(c_uvStreamUniformNames[m_index]);
        if (uvStreamUniform)
            uvStreamUniform->upload(&m_data[m_dataOffset], _instancesCount);
    }

    //////////////////////////////////////////
    void InstanceStreamUVDX11::processDrawBegin()
    {
    }


} // namespace Maze
//////////////////////////////////////////
