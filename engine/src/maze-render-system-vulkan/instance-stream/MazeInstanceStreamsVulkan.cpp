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
#include "MazeRenderSystemVulkanHeader.hpp"
#include "maze-render-system-vulkan/instance-stream/MazeInstanceStreamsVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-graphics/config/MazeGraphicsConfig.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Same total per-frame capacity as RenderSystemVulkan::c_instanceStreamCapacity;
    // unlike DX11's 32-instance-per-draw-call UBO-array cap, SSBOs aren't
    // size-constrained the same way, so this backend allows large batches in
    // a single draw call (mirrors OpenGL's texture-buffer instancing mode)
    static U32 const c_instancesPerDrawCallVulkan = 16384u;
    static U32 const c_instancesPerDrawVulkan = 16384u * 2u;


    //////////////////////////////////////////
    // Class InstanceStreamModelMatrixVulkan
    //
    //////////////////////////////////////////
    InstanceStreamModelMatrixVulkanPtr InstanceStreamModelMatrixVulkan::Create(RenderSystemVulkan* _renderSystem)
    {
        InstanceStreamModelMatrixVulkanPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InstanceStreamModelMatrixVulkan, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool InstanceStreamModelMatrixVulkan::init(RenderSystemVulkan* _renderSystem)
    {
        if (!InstanceStreamModelMatrix::init())
            return false;

        m_renderSystem = _renderSystem;

        m_maxInstancesPerDrawCall = c_instancesPerDrawCallVulkan;
        m_maxInstancesPerDraw = c_instancesPerDrawVulkan;
        m_data.resize(m_maxInstancesPerDraw);

        return true;
    }

    //////////////////////////////////////////
    void InstanceStreamModelMatrixVulkan::prepareForRender(S32 _instancesCount, S32 _slotOffset)
    {
        U32 frameIndex = m_renderSystem->getCurrentFrameIndex();
        F32* dst = (F32*)m_renderSystem->getModelMatricesStreamMapped(frameIndex);
        if (!dst)
            return;

        // Expand each TMat (stored as 4 rows of 3 - see MazeTMat.hpp; the 4
        // rows ARE the 4 columns of the equivalent 4x4 matrix, matching the
        // DX11 HLSL 'float3 u_modelMatrices[N*4]' convention) into a full
        // std430 column-major mat4 (64 bytes/instance) to match the
        // 'mat4 u_modelMatrices[]' SSBO declaration in MazeCommonShaderHeader.mzglslvk
        for (S32 i = 0; i < _instancesCount; ++i)
        {
            TMat const& m = m_data[m_dataOffset + i];
            F32* out = dst + (Size)(_slotOffset + i) * 16;

            Vec3F c0 = m.getRow(0);
            Vec3F c1 = m.getRow(1);
            Vec3F c2 = m.getRow(2);
            Vec3F c3 = m.getRow(3);

            out[0] = c0.x; out[1] = c0.y; out[2] = c0.z; out[3] = 0.0f;
            out[4] = c1.x; out[5] = c1.y; out[6] = c1.z; out[7] = 0.0f;
            out[8] = c2.x; out[9] = c2.y; out[10] = c2.z; out[11] = 0.0f;
            out[12] = c3.x; out[13] = c3.y; out[14] = c3.z; out[15] = 1.0f;
        }
    }

    //////////////////////////////////////////
    void InstanceStreamModelMatrixVulkan::processDrawBegin()
    {
    }


    //////////////////////////////////////////
    // Class InstanceStreamColorVulkan
    //
    //////////////////////////////////////////
    InstanceStreamColorVulkanPtr InstanceStreamColorVulkan::Create(RenderSystemVulkan* _renderSystem)
    {
        InstanceStreamColorVulkanPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InstanceStreamColorVulkan, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool InstanceStreamColorVulkan::init(RenderSystemVulkan* _renderSystem)
    {
        if (!InstanceStreamColor::init())
            return false;

        m_renderSystem = _renderSystem;

        m_maxInstancesPerDrawCall = c_instancesPerDrawCallVulkan;
        m_maxInstancesPerDraw = c_instancesPerDrawVulkan;
        m_data.resize(m_maxInstancesPerDraw);

        return true;
    }

    //////////////////////////////////////////
    void InstanceStreamColorVulkan::prepareForRender(S32 _instancesCount, S32 _slotOffset)
    {
        U32 frameIndex = m_renderSystem->getCurrentFrameIndex();
        Vec4F* dst = (Vec4F*)m_renderSystem->getColorsStreamMapped(frameIndex);
        if (!dst)
            return;

        memcpy(dst + _slotOffset, &m_data[m_dataOffset], sizeof(Vec4F) * (Size)_instancesCount);
    }

    //////////////////////////////////////////
    void InstanceStreamColorVulkan::processDrawBegin()
    {
    }


    //////////////////////////////////////////
    // Class InstanceStreamUVVulkan
    //
    //////////////////////////////////////////
    InstanceStreamUVVulkanPtr InstanceStreamUVVulkan::Create(
        S32 _index,
        RenderSystemVulkan* _renderSystem)
    {
        InstanceStreamUVVulkanPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InstanceStreamUVVulkan, object, init(_index, _renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool InstanceStreamUVVulkan::init(
        S32 _index,
        RenderSystemVulkan* _renderSystem)
    {
        if (!InstanceStreamUV::init(_index))
            return false;

        m_renderSystem = _renderSystem;

        m_maxInstancesPerDrawCall = c_instancesPerDrawCallVulkan;
        m_maxInstancesPerDraw = c_instancesPerDrawVulkan;
        m_data.resize(m_maxInstancesPerDraw);

        return true;
    }

    //////////////////////////////////////////
    void InstanceStreamUVVulkan::prepareForRender(S32 _instancesCount, S32 _slotOffset)
    {
        // Only UV channels 0 and 1 are backed by GPU storage in this backend -
        // channels 2+ (m_index >= 2) are accepted by the generic
        // InstanceStreamUV interface but silently not uploaded, a known scope
        // reduction vs DX11/GL's 8-channel support.
        //
        // Layout matches MazeInstanceStream.mzglslvk's 'UVStreamsBuffer':
        // one MazeUVStreamData{vec4 uv0; vec4 uv1;} struct PER INSTANCE
        // (interleaved), not two concatenated per-channel regions - so
        // channel 0 writes to the first vec4 of each 32-byte instance slot
        // and channel 1 writes to the second, with a stride of 2 vec4s.
        if (m_index < 0 || m_index > 1)
            return;

        U32 frameIndex = m_renderSystem->getCurrentFrameIndex();
        Vec4F* base = (Vec4F*)m_renderSystem->getUVStreamMapped(frameIndex);
        if (!base)
            return;

        Vec4F* dst = base + (Size)(_slotOffset) * 2 + m_index;
        for (S32 i = 0; i < _instancesCount; ++i)
            dst[(Size)i * 2] = m_data[m_dataOffset + i];
    }

    //////////////////////////////////////////
    void InstanceStreamUVVulkan::processDrawBegin()
    {
    }


} // namespace Maze
//////////////////////////////////////////
