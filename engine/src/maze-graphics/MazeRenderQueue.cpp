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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderCommands.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include <memory>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderQueueGPUBufferInfo
    //
    //////////////////////////////////////////
    RenderQueueGPUBufferInfo::RenderQueueGPUBufferInfo(
        GPUTextureBufferPtr _buffer,
        void* _mappedPointer)
        : buffer(_buffer)
        , mappedPointer(_mappedPointer)
        , mappedElementStart(U32_MAX)
        , mappedElementEnd(0)
    {
    }


    //////////////////////////////////////////
    // Class RenderQueue
    //
    //////////////////////////////////////////
    RenderQueue::RenderQueue()
        : m_renderTarget(nullptr)
        , m_currentRenderPass(nullptr)
        , m_lastDrawVAOInstancedCommand(nullptr)
        , m_maxInstancesPerDrawCall(0)
        , m_maxInstancesPerDraw(0)
        , m_drawCalls(0)
        , m_drawCallsLimit(0)
    {
    }

    //////////////////////////////////////////
    RenderQueue::~RenderQueue()
    {
    }

    //////////////////////////////////////////
    bool RenderQueue::init(RenderTarget* _renderTarget)
    {
        if (!_renderTarget)
            return false;

        m_renderTarget = _renderTarget;


        return true;
    }

    //////////////////////////////////////////
    void RenderQueue::clear()
    {
        m_currentRenderPass = nullptr;
        m_lastDrawVAOInstancedCommand = nullptr;
        m_renderCommandsBuffer.clear();
    }

    //////////////////////////////////////////
    void RenderQueue::addSelectRenderPassCommand(RenderPass* _renderPass)
    {
        if (m_currentRenderPass == _renderPass)
            return;
        // TODO
        /*
        else
        if (m_currentRenderPass && m_currentRenderPass->calculateCRC32() == _renderPass->calculateCRC32())
        {
            return;
        }
        */

        m_currentRenderPass = _renderPass;

        m_renderCommandsBuffer.createCommand<RenderCommandSetRenderPass>(_renderPass);
        m_lastDrawVAOInstancedCommand = nullptr;
    }

    //////////////////////////////////////////
    void RenderQueue::pushInstanceModelMatrix(
        TMat const& _modelMatrix)
    {
        S32 offset = m_instanceStreamModelMatrix->getOffset();

        MAZE_DEBUG_ERROR_RETURN_IF(offset >= (S32)m_instanceStreamModelMatrix->getDataSize(), "Index is out of bounds!");

        m_instanceStreamModelMatrix->setData(offset, _modelMatrix);
        m_instanceStreamModelMatrix->setOffset(++offset);
    }

    //////////////////////////////////////////
    void RenderQueue::pushInstanceModelMatricies(
        TMat const* _modelMatricies,
        S32 _count)
    {
        S32 offset = m_instanceStreamModelMatrix->getOffset();

        MAZE_DEBUG_ERROR_RETURN_IF(offset + _count > (S32)m_instanceStreamModelMatrix->getDataSize(), "Index is out of bounds!");

        m_instanceStreamModelMatrix->setData(offset, _modelMatricies, _count);
        m_instanceStreamModelMatrix->setOffset(offset + _count);
    }

    //////////////////////////////////////////
    void RenderQueue::pushInstanceColor(
        Vec4F const& _color)
    {
        S32 offset = m_instanceStreamColor->getOffset();

        MAZE_DEBUG_ERROR_RETURN_IF(offset >= (S32)m_instanceStreamColor->getDataSize(), "Index is out of bounds!");

        m_instanceStreamColor->setData(offset, _color);
        m_instanceStreamColor->setOffset(++offset);
    }

    //////////////////////////////////////////
    void RenderQueue::pushInstanceColors(
        Vec4F const* _colors,
        S32 _count)
    {
        S32 offset = m_instanceStreamColor->getOffset();

        MAZE_DEBUG_ERROR_RETURN_IF(offset + _count > (S32)m_instanceStreamColor->getDataSize(), "Index is out of bounds!");

        m_instanceStreamColor->setData(offset, _colors, _count);
        m_instanceStreamColor->setOffset(offset + _count);
    }

    //////////////////////////////////////////
    void RenderQueue::pushInstanceUV(
        S32 _index,
        Vec4F const& _uv)
    {
        S32 offset = m_instanceStreamUVs[_index]->getOffset();

        MAZE_DEBUG_ERROR_RETURN_IF(offset >= (S32)m_instanceStreamUVs[_index]->getDataSize(), "Index is out of bounds!");

        m_instanceStreamUVs[_index]->setData(offset, _uv);
        m_instanceStreamUVs[_index]->setOffset(++offset);
    }

    //////////////////////////////////////////
    void RenderQueue::pushInstanceUV(
        S32 _index,
        Vec4F const* _uvs,
        S32 _count)
    {
        S32 offset = m_instanceStreamUVs[_index]->getOffset();

        MAZE_DEBUG_ERROR_RETURN_IF(offset + _count > (S32)m_instanceStreamUVs[_index]->getDataSize(), "Index is out of bounds!");

        m_instanceStreamUVs[_index]->setData(offset, _uvs, _count);
        m_instanceStreamUVs[_index]->setOffset(offset + _count);
    }

} // namespace Maze
//////////////////////////////////////////
