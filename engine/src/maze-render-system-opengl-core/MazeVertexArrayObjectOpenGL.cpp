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
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexBufferObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/utils/MazeProfiler.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class VertexArrayObjectOpenGLScopeBind
    //
    //////////////////////////////////////////    
    VertexArrayObjectOpenGLScopeBind::VertexArrayObjectOpenGLScopeBind(
        ContextOpenGL* _context,
        RenderSystemOpenGL* _rs,
        MZGLuint _newVAO)
        : m_context(_context)
        , m_prevVAO(0)
    {
        m_prevVAO = m_context->getBindedVertexArrayObject();
        m_context->bindVertexArrayObject(_newVAO);
    }

    //////////////////////////////////////////
    VertexArrayObjectOpenGLScopeBind::VertexArrayObjectOpenGLScopeBind(VertexArrayObjectOpenGL* _newVAO)
        : VertexArrayObjectOpenGLScopeBind(
            _newVAO->getContextOpenGL(),
            _newVAO->getRenderSystemOpenGLRaw(),
            _newVAO->getGLVAO())
    {}

    //////////////////////////////////////////
    VertexArrayObjectOpenGLScopeBind::VertexArrayObjectOpenGLScopeBind(VertexArrayObjectOpenGL const& _newVAO)
        : VertexArrayObjectOpenGLScopeBind(
            _newVAO.getContextOpenGL(),
            _newVAO.getRenderSystemOpenGLRaw(),
            _newVAO.getGLVAO())
    {}

    //////////////////////////////////////////
    VertexArrayObjectOpenGLScopeBind::~VertexArrayObjectOpenGLScopeBind()
    {
        if (m_context)
        {
            m_context->bindArrayBuffer(m_prevVAO);
        }
    }


    //////////////////////////////////////////
    // Class VertexArrayObjectOpenGL
    //
    //////////////////////////////////////////
    VertexArrayObjectOpenGL::VertexArrayObjectOpenGL()
        : m_context(nullptr)
        , m_glVAO(0)
        , m_glEBO(0)
    {
    }

    //////////////////////////////////////////
    VertexArrayObjectOpenGL::~VertexArrayObjectOpenGL()
    {

        setContextOpenGL(nullptr);
    }

    //////////////////////////////////////////
    VertexArrayObjectOpenGLPtr VertexArrayObjectOpenGL::Create(
        RenderSystem* _renderSystem,
        ContextOpenGL* _contextOpenGL)
    {
        VertexArrayObjectOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(VertexArrayObjectOpenGL, object, init(_renderSystem, _contextOpenGL));
        return object;
    }

    //////////////////////////////////////////
    bool VertexArrayObjectOpenGL::init(
        RenderSystem* _renderSystem,
        ContextOpenGL* _contextOpenGL)
    {
        if (!VertexArrayObject::init(_renderSystem))
            return false;

        if (!_contextOpenGL)
            return false;

        setContextOpenGL(_contextOpenGL);
        
        generateGLObjects();

        return true;
    }

    //////////////////////////////////////////
    void VertexArrayObjectOpenGL::generateGLObjects()
    {
        if (m_glVAO)
        {
            deleteGLObjects();
        }

        ContextOpenGLScopeBind contextScopedBind(m_context);

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        MAZE_GL_CALL(mzglGenVertexArrays(1, &m_glVAO));
        MAZE_GL_CALL(mzglGenBuffers(1, &m_glEBO));
    }

    //////////////////////////////////////////
    void VertexArrayObjectOpenGL::deleteGLObjects()
    {
        if (!m_context || !m_context->isValid())
        {
            m_glEBO = 0;
            m_glVAO = 0;
            return;
        }

        ContextOpenGLScopeBind contextScopedBind(m_context);
        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        MAZE_GL_CALL(mzglDeleteBuffers(1, &m_glEBO));
        m_glEBO = 0;

        MAZE_GL_CALL(mzglDeleteVertexArrays(1, &m_glVAO));
        m_glVAO = 0;

    }

    //////////////////////////////////////////
    void VertexArrayObjectOpenGL::setIndices(
        U8 const* _indicesData,
        VertexAttributeType _indicesType,
        Size _indicesCount)
    {
        if (!m_context->isValid())
            return;

        ContextOpenGLScopeBind contextScopedBind(m_context);
        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        VertexArrayObjectOpenGLScopeBind vaoScopedBind(this);

        Size bytesPerIndex = GetVertexAttributeTypeSize(_indicesType);

        m_context->bindElementArrayBuffer(m_glEBO);
        MAZE_GL_CALL(
            mzglBufferData(
                MAZE_GL_ELEMENT_ARRAY_BUFFER,
                (MZGLsizeiptr)(bytesPerIndex * _indicesCount),
                _indicesData,
                MAZE_GL_DYNAMIC_DRAW));

        m_context->bindVertexArrayObject(0);

        m_indicesCount = _indicesCount;
        m_indicesType = _indicesType;
    }

    //////////////////////////////////////////
    void VertexArrayObjectOpenGL::setVerticesData(
        U8 const* _data,
        VertexAttributeDescription _description,
        Size _verticesCount)
    {
        if (!m_context->isValid())
            return;

        ContextOpenGLScopeBind contextScopedBind(m_context);
        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        VertexArrayObjectOpenGLScopeBind vaoScopedBind(this);

        VertexAttributeSemantic vertexElementSemantic = _description.semantic;

        VertexBufferObjectOpenGLPtr const& vbo = ensureVBO(vertexElementSemantic);
        VertexBufferObjectOpenGLScopeBind vboScopedBind(vbo);

        Size bytesPerType = GetVertexAttributeTypeSize(_description.type);
        Size bytesPerVertexData = bytesPerType * _description.count;

        vbo->upload(_data, bytesPerVertexData * _verticesCount);

        MZGLuint vertexAttrib = static_cast<MZGLuint>(vertexElementSemantic);
        MZGLenum typeOpenGL = GetVertexAttributeTypeOpenGL(_description.type);

        MAZE_GL_CALL(
            mzglVertexAttribPointer(
                vertexAttrib,
                (MZGLint)_description.count,
                typeOpenGL,
                _description.normalized ? MAZE_GL_TRUE : MAZE_GL_FALSE,
                (MZGLsizei)_description.stride,
                (void*)0));
        MAZE_GL_CALL(mzglEnableVertexAttribArray(vertexAttrib));

        m_context->bindVertexArrayObject(0);
    }

    //////////////////////////////////////////
    void VertexArrayObjectOpenGL::bind()
    {
        ContextOpenGLScopeBind contextScopedBind(m_context);
        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        m_context->bindVertexArrayObject(m_glVAO);
    }

    //////////////////////////////////////////
    VertexBufferObjectOpenGLPtr const& VertexArrayObjectOpenGL::ensureVBO(VertexAttributeSemantic _semantic)
    {
        VertexBufferObjectOpenGLPtr& vbo = m_vbos[(Size)_semantic];
        if (!vbo)
        {
            vbo = VertexBufferObjectOpenGL::Create(
                getRenderSystemOpenGLRaw(),
                GPUByteBufferAccessType::Default,
                true,
                getContextOpenGL());
        }

        return vbo;
    }

    //////////////////////////////////////////
    void VertexArrayObjectOpenGL::notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_context = nullptr;
        m_glVAO = 0;
        m_glEBO = 0;

        for (VertexAttributeSemantic    attribSemantic = VertexAttributeSemantic(0);
            attribSemantic < VertexAttributeSemantic::MAX;
            attribSemantic = VertexAttributeSemantic((S32)attribSemantic + 1))
        {
            m_vbos[(Size)attribSemantic].reset();
        }
    }

    //////////////////////////////////////////
    void VertexArrayObjectOpenGL::notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_glVAO = 0;
        m_glEBO = 0;

        for (VertexAttributeSemantic    attribSemantic = VertexAttributeSemantic(0);
                                        attribSemantic < VertexAttributeSemantic::MAX;
                                        attribSemantic = VertexAttributeSemantic((S32)attribSemantic + 1))
        {
            m_vbos[(Size)attribSemantic].reset();
        }
    }

    //////////////////////////////////////////
    void VertexArrayObjectOpenGL::notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL)
    {
        generateGLObjects();

        if (m_subMeshCopy)
        {
            setMesh(m_subMeshCopy);
        }
    }

    //////////////////////////////////////////
    void VertexArrayObjectOpenGL::setContextOpenGL(ContextOpenGL* _contextOpenGL)
    {
        if (m_context == _contextOpenGL)
            return;

        if (m_context)
        {
            deleteGLObjects();

            m_context->eventDestroyed.unsubscribe(this);
            m_context->eventGLContextWillBeDestroyed.unsubscribe(this);
            m_context->eventGLContextSetup.unsubscribe(this);
        }

        m_context = _contextOpenGL;

        if (m_context)
        {
            m_context->eventDestroyed.subscribe(this, &VertexArrayObjectOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventDestroyed.updateDelegatesList();
            m_context->eventGLContextWillBeDestroyed.subscribe(this, &VertexArrayObjectOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextWillBeDestroyed.updateDelegatesList();
            m_context->eventGLContextSetup.subscribe(this, &VertexArrayObjectOpenGL::notifyContextOpenGLContextSetup);
            m_context->eventGLContextSetup.updateDelegatesList();
        }
    }

#if MAZE_DEBUG
    //////////////////////////////////////////
    void VertexArrayObjectOpenGL::debug()
    {
        Debug::Log("Querying VAO state:");
        S32 vao, eabb, eabbs, mva, isOn(1), vaabb;
        mzglGetIntegerv(MAZE_GL_VERTEX_ARRAY_BINDING, &vao);
        mzglGetIntegerv(MAZE_GL_ELEMENT_ARRAY_BUFFER_BINDING, &eabb);
        mzglGetBufferParameteriv(MAZE_GL_ELEMENT_ARRAY_BUFFER, MAZE_GL_BUFFER_SIZE, &eabbs);

        Debug::Log("  Binded VAO: %d (Current: %d)", vao, (S32)m_glVAO);
        Debug::Log("  Binded EBO: %d (Current: %d), size=%d", eabb, (S32)m_glEBO, eabbs);

        mzglGetIntegerv(MAZE_GL_MAX_VERTEX_ATTRIBS, &mva);
        for (S32 i = 0; i < mva; ++i)
        {
            mzglGetVertexAttribiv(i, MAZE_GL_VERTEX_ATTRIB_ARRAY_ENABLED, &isOn);
            if (isOn)
            {
                mzglGetVertexAttribiv(i, MAZE_GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vaabb);
                Debug::Log("  attrib #%d(%s): VBO=%d", i, GetVertexAttributeName(VertexAttributeSemantic(i)), vaabb);
            }
        }
    }
#endif

} // namespace Maze
//////////////////////////////////////////
