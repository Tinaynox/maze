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
#include "MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeVertexBufferObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexBufferObjectMappingControllerOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeExtensionsOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include "maze-core/utils/MazeProfiler.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class VertexBufferObjectOpenGLScopeBind
    //
    //////////////////////////////////////////    
    VertexBufferObjectOpenGLScopeBind::VertexBufferObjectOpenGLScopeBind(
        RenderSystemOpenGL* _rs,
        MZGLuint _newVBO)
        : m_context(nullptr)
        , m_prevVBO(0)
    {
        ContextOpenGL* context = _rs->ensureCurrentContext();
        if (context)
        {
            m_prevVBO = context->getBindedArrayBuffer();

            context->bindArrayBuffer(_newVBO);

            m_context = context;
        }
    }

    //////////////////////////////////////////
    VertexBufferObjectOpenGLScopeBind::VertexBufferObjectOpenGLScopeBind(VertexBufferObjectOpenGL* _newVBO)
        : VertexBufferObjectOpenGLScopeBind(
            _newVBO->getRenderSystemOpenGLRaw(),
            _newVBO->getGLVBO())
    {}

    //////////////////////////////////////////
    VertexBufferObjectOpenGLScopeBind::VertexBufferObjectOpenGLScopeBind(VertexBufferObjectOpenGL const& _newVBO)
        : VertexBufferObjectOpenGLScopeBind(
            _newVBO.getRenderSystemOpenGLRaw(),
            _newVBO.getGLVBO())
    {}

    //////////////////////////////////////////
    VertexBufferObjectOpenGLScopeBind::~VertexBufferObjectOpenGLScopeBind()
    {
        if (m_context)
        {
            m_context->bindArrayBuffer(m_prevVBO);
        }
    }


    //////////////////////////////////////////
    // Class VertexBufferObjectOpenGL
    //
    //////////////////////////////////////////
    VertexBufferObjectOpenGL::VertexBufferObjectOpenGL()
        : m_context(nullptr)
        , m_glVBO(0)
        , m_sizeBytes(0)
        , m_mappingController(nullptr)
    {
    }

    //////////////////////////////////////////
    VertexBufferObjectOpenGL::~VertexBufferObjectOpenGL()
    {
        MAZE_SAFE_DELETE(m_mappingController);

        setContextOpenGL(nullptr);
    }

    //////////////////////////////////////////
    VertexBufferObjectOpenGLPtr VertexBufferObjectOpenGL::Create(
        RenderSystem* _renderSystem,
        GPUByteBufferAccessType::Enum _accessType,
        bool _singleMapping,
        ContextOpenGL* _contextOpenGL)
    {
        VertexBufferObjectOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(VertexBufferObjectOpenGL, object, init(
            _renderSystem,
            _accessType,
            _singleMapping,
            _contextOpenGL));
        return object;
    }

    //////////////////////////////////////////
    bool VertexBufferObjectOpenGL::init(
        RenderSystem* _renderSystem,
        GPUByteBufferAccessType::Enum _accessType,
        bool _singleMapping,
        ContextOpenGL* _contextOpenGL)
    {
        if (!VertexBufferObject::init(_renderSystem, _accessType))
            return false;

        setContextOpenGL(_contextOpenGL);
                    
        generateGLObjects();

        if (_accessType != GPUByteBufferAccessType::Immutable)
        {
            m_mappingController = MAZE_NEW_WITH_ARGS(VertexBufferObjectMappingControllerOpenGL, this, _singleMapping);
        }

        return true;
    }

    //////////////////////////////////////////
    void VertexBufferObjectOpenGL::generateGLObjects()
    {
        if (m_glVBO)
        {
            deleteGLObjects();
        }

        ContextOpenGLScopeBind contextScopedBind(m_context);

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        MAZE_GL_CALL(mzglGenBuffers(1, &m_glVBO));
    }

    //////////////////////////////////////////
    void VertexBufferObjectOpenGL::deleteGLObjects()
    {
        if (!m_context || m_context->getIsDestroyed() || !m_context->isValid())
        {
            m_glVBO = 0;
            return;
        }

        ContextOpenGLScopeBind contextScopedBind(m_context);
        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        MAZE_GL_CALL(mzglDeleteBuffers(1, &m_glVBO));
        m_glVBO = 0;
    }

    //////////////////////////////////////////
    void VertexBufferObjectOpenGL::resize(Size _bytes)
    {
        upload(nullptr, _bytes);
    }

    //////////////////////////////////////////
    void VertexBufferObjectOpenGL::upload(
        void const* _data,
        Size _bytes)
    {
        if (m_mappingController)
        {
            MAZE_ERROR_RETURN_IF(m_mappingController->isMapped(), "Mapped buffer cannot be changed!");
        }

        ExtensionsOpenGLPtr const& extensions = m_context->getExtensions();

        ContextOpenGLScopeBind contextScopedBind(m_context);
        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        VertexBufferObjectOpenGLScopeBind vboScopedBind(this);

        m_sizeBytes = _bytes;


        if (_data == nullptr)
        {
            bool isPersistentMappingEnabled = extensions->getSupportArbBufferStorage();

            if (isPersistentMappingEnabled)
            {
                MZGLbitfield flags = 0;
                if (getAccessType() >= GPUByteBufferAccessType::DynamicDefault)
                {
                    flags |= MAZE_GL_MAP_WRITE_BIT;

                    if (getAccessType() >= GPUByteBufferAccessType::DynamicPersistent)
                    {
                        flags |= MAZE_GL_MAP_PERSISTENT_BIT;

                        if (getAccessType() >= GPUByteBufferAccessType::DynamicPersistentCoherent)
                        {
                            flags |= MAZE_GL_MAP_COHERENT_BIT;
                        }
                    }
                }

                MAZE_GL_CALL(mzglBufferStorage(MAZE_GL_ARRAY_BUFFER, getSizeBytes(), 0, flags));
            }
            else
            {
                MAZE_GL_CALL(
                    mzglBufferData(
                        MAZE_GL_ARRAY_BUFFER,
                        getSizeBytes(),
                        0,
                        (getAccessType() == GPUByteBufferAccessType::Immutable ? MAZE_GL_STATIC_DRAW
                                                                               : MAZE_GL_DYNAMIC_DRAW)));

            }
        }
        else
        {
            MAZE_PROFILER_SCOPED_LOCK(BUFFDATA);
            MAZE_GL_CALL(
                mzglBufferData(
                    MAZE_GL_ARRAY_BUFFER,
                    _bytes,
                    _data,
                    m_accessType == GPUByteBufferAccessType::Immutable ? MAZE_GL_STATIC_DRAW
                                                                       : MAZE_GL_DYNAMIC_DRAW));
        }
    }

    //////////////////////////////////////////
    bool VertexBufferObjectOpenGL::findSuitableBlock(
        Size _sizeBytes,
        Size _alignment,
        Size& _outBestBlockIndex,
        bool& _outMatchingStride) const
    {
        Vector<VertexBufferObjectBlockOpenGL>::const_iterator it = m_freeBlocks.begin();
        Vector<VertexBufferObjectBlockOpenGL>::const_iterator end = m_freeBlocks.end();

        bool blockFound = false;

        while (it != end && !_outMatchingStride)
        {
            const VertexBufferObjectBlockOpenGL& block = *it;

            Size newOffset = ((block.offset + _alignment - 1) / _alignment) * _alignment;
            Size padding = newOffset - block.offset;

            if (_sizeBytes + padding <= block.size)
            {
                _outBestBlockIndex = it - m_freeBlocks.begin();

                blockFound = true;

                if (newOffset == block.offset)
                    _outMatchingStride = true;
            }

            ++it;
        }

        return blockFound;
    }

    //////////////////////////////////////////
    void VertexBufferObjectOpenGL::addFreeBlock(
        Size _bufferOffset,
        Size _sizeBytes)
    {
        m_freeBlocks.emplace_back(_bufferOffset, _sizeBytes);

        Vector<VertexBufferObjectBlockOpenGL>::iterator blockToMerge = m_freeBlocks.end() - 1;
        Vector<VertexBufferObjectBlockOpenGL>::iterator it = m_freeBlocks.begin();
        Vector<VertexBufferObjectBlockOpenGL>::iterator end = m_freeBlocks.end();

        while (it != end)
        {
            if (it->offset + it->size == blockToMerge->offset)
            {
                it->size += blockToMerge->size;
                Size idx = it - m_freeBlocks.begin();

                if (idx == m_freeBlocks.size() - 1)
                    idx = blockToMerge - m_freeBlocks.begin();

                StdHelper::EraseUnorderedVector(m_freeBlocks, blockToMerge);

                blockToMerge = m_freeBlocks.begin() + idx;
                it = m_freeBlocks.begin();
                end = m_freeBlocks.end();
            }
            else
            if (blockToMerge->offset + blockToMerge->size == it->offset)
            {
                blockToMerge->size += it->size;
                Size idx = blockToMerge - m_freeBlocks.begin();

                if (idx == m_freeBlocks.size() - 1)
                    idx = it - m_freeBlocks.begin();

                StdHelper::EraseUnorderedVector(m_freeBlocks, it);

                blockToMerge = m_freeBlocks.begin() + idx;
                it = m_freeBlocks.begin();
                end = m_freeBlocks.end();
            }
            else
            {
                ++it;
            }
        }
    }

    //////////////////////////////////////////
    void VertexBufferObjectOpenGL::addPaddingInfo(
        Size _offsetAfterPadding,
        Size _padding)
    {
        Vector<VertexBufferObjectBlockOpenGL::PaddingInfo >::iterator it =
            std::lower_bound(
                m_usedPadding.begin(),
                m_usedPadding.end(),
                _offsetAfterPadding,
                VertexBufferObjectBlockOpenGL::PaddingInfo());
        m_usedPadding.insert(it, VertexBufferObjectBlockOpenGL::PaddingInfo(_offsetAfterPadding, _padding));
    }

    //////////////////////////////////////////
    bool VertexBufferObjectOpenGL::erasePaddingInfo(
        Size _offsetAfterPadding,
        VertexBufferObjectBlockOpenGL::PaddingInfo& _outPaddingInfo)
    {
        Vector<VertexBufferObjectBlockOpenGL::PaddingInfo>::iterator it =
            std::lower_bound(
                m_usedPadding.begin(),
                m_usedPadding.end(),
                _offsetAfterPadding,
                VertexBufferObjectBlockOpenGL::PaddingInfo());

        if (it != m_usedPadding.end() && it->offsetAfterPadding == _offsetAfterPadding)
        {
            _outPaddingInfo = (*it);
            m_usedPadding.erase(it);
            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    void VertexBufferObjectOpenGL::notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_context = nullptr;
        m_glVBO = 0;
    }

    //////////////////////////////////////////
    void VertexBufferObjectOpenGL::notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_glVBO = 0;
    }

    //////////////////////////////////////////
    void VertexBufferObjectOpenGL::notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL)
    {
        generateGLObjects();
    }

    //////////////////////////////////////////
    void VertexBufferObjectOpenGL::setContextOpenGL(ContextOpenGL* _contextOpenGL)
    {
        if (m_context == _contextOpenGL)
            return;

        if (m_context)
        {
            deleteGLObjects();

            m_context->eventDestroyed.unsubscribe(this, &VertexBufferObjectOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventGLContextWillBeDestroyed.unsubscribe(this, &VertexBufferObjectOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextSetup.unsubscribe(this, &VertexBufferObjectOpenGL::notifyContextOpenGLContextSetup);
        }

        m_context = _contextOpenGL;

        if (m_context)
        {
            m_context->eventDestroyed.subscribe(this, &VertexBufferObjectOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventGLContextWillBeDestroyed.subscribe(this, &VertexBufferObjectOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextSetup.subscribe(this, &VertexBufferObjectOpenGL::notifyContextOpenGLContextSetup);
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
