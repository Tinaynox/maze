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
#if (!defined(_MazeVertexArrayObjectOpenGL_hpp_))
#define _MazeVertexArrayObjectOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeVertex.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(VertexArrayObjectOpenGL);
    MAZE_USING_SHARED_PTR(VertexBufferObjectOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    

    //////////////////////////////////////////
    // Class VertexArrayObjectOpenGLScopeBind
    //
    //////////////////////////////////////////
    class VertexArrayObjectOpenGLScopeBind
    {
    public:

        //////////////////////////////////////////    
        VertexArrayObjectOpenGLScopeBind(
            ContextOpenGL* _context,
            RenderSystemOpenGL* _rs,
            MZGLuint _newVAO);

        //////////////////////////////////////////    
        VertexArrayObjectOpenGLScopeBind(VertexArrayObjectOpenGL* _newVAO);

        //////////////////////////////////////////
        VertexArrayObjectOpenGLScopeBind(VertexArrayObjectOpenGL const& _newVAO);

        //////////////////////////////////////////
        inline VertexArrayObjectOpenGLScopeBind(VertexArrayObjectOpenGLPtr const& _newVAO)
            : VertexArrayObjectOpenGLScopeBind(_newVAO.get())
        {}

        //////////////////////////////////////////
        ~VertexArrayObjectOpenGLScopeBind();

    private:
        ContextOpenGL* m_context;
        MZGLuint m_prevVAO;
    };


    //////////////////////////////////////////
    // Class VertexArrayObjectOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API VertexArrayObjectOpenGL
        : public VertexArrayObject
    {
    public:

        //////////////////////////////////////////
        virtual ~VertexArrayObjectOpenGL();

        //////////////////////////////////////////
        static VertexArrayObjectOpenGLPtr Create(
            RenderSystem* _renderSystem,
            ContextOpenGL* _contextOpenGL);

        
        //////////////////////////////////////////
        inline ContextOpenGL* getContextOpenGL() const { return m_context; }

        //////////////////////////////////////////
        inline RenderSystemOpenGL* getRenderSystemOpenGLRaw() const { return m_renderSystem->castRaw<RenderSystemOpenGL>(); }


        //////////////////////////////////////////
        virtual void setIndices(
            U8 const* _indicesData,
            VertexAttributeType _indicesType,
            Size _indicesCount) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setVerticesData(
            U8 const* _data,
            VertexAttributeDescription _description,
            Size _verticesCount) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void bind();

        //////////////////////////////////////////
        inline MZGLuint getGLVAO() const { return m_glVAO; }

        //////////////////////////////////////////
        inline MZGLuint getGLEBO() const { return m_glEBO; }

    protected:

        //////////////////////////////////////////
        VertexArrayObjectOpenGL();

        //////////////////////////////////////////
        using VertexArrayObject::init;

        //////////////////////////////////////////
        virtual bool init(
            RenderSystem* _renderSystem,
            ContextOpenGL* _contextOpenGL);


        //////////////////////////////////////////
        void generateGLObjects();

        //////////////////////////////////////////
        void deleteGLObjects();

        //////////////////////////////////////////
        VertexBufferObjectOpenGLPtr const& ensureVBO(VertexAttributeSemantic _semantic);
    

        //////////////////////////////////////////
        void notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void setContextOpenGL(ContextOpenGL* _contextOpenGL);

    protected:
        ContextOpenGL* m_context;

        MZGLuint m_glVAO;
        MZGLuint m_glEBO;


        VertexBufferObjectOpenGLPtr m_vbos[(Size)VertexAttributeSemantic::MAX];
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVertexArrayObjectOpenGL_hpp_
//////////////////////////////////////////
