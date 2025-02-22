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
#include "maze-render-system-opengl-core/MazeMaterialOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazePixelFormatOpenGL.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    // Class MaterialOpenGL
    //
    //////////////////////////////////////////
    MaterialOpenGL::MaterialOpenGL()
        : m_context(nullptr)
    {
    }

    //////////////////////////////////////////
    MaterialOpenGL::~MaterialOpenGL()
    {
        setContextOpenGL(nullptr);
    }

    //////////////////////////////////////////
    MaterialOpenGLPtr MaterialOpenGL::Create(
        RenderSystem* _renderSystem,
        ContextOpenGL* _contextOpenGL,
        MaterialDeleter const& _deleter)
    {
        MaterialOpenGLPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR_EX(MaterialOpenGL, object, _deleter, init(_renderSystem, _contextOpenGL));
        return object;
    }

    //////////////////////////////////////////
    MaterialOpenGLPtr MaterialOpenGL::Create(
        MaterialOpenGLPtr const& _material,
        MaterialDeleter const& _deleter)
    {
        MaterialOpenGLPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR_EX(MaterialOpenGL, object, _deleter, init(_material));
        return object;
    }


    //////////////////////////////////////////
    bool MaterialOpenGL::init(
        RenderSystem* _renderSystem,
        ContextOpenGL* _contextOpenGL)
    {
        if (!Material::init(_renderSystem))
            return false;

        setContextOpenGL(_contextOpenGL);

        return true;
    }

    //////////////////////////////////////////
    bool MaterialOpenGL::init(MaterialOpenGLPtr const& _material)
    {
        if (!Material::init(_material))
            return false;

        setContextOpenGL(_material->m_context);

        return true;
    }

    //////////////////////////////////////////
    MaterialPtr MaterialOpenGL::createCopy()
    {
        MaterialOpenGLPtr material = m_renderSystem->castRaw<RenderSystemOpenGL>()->getMaterialOpenGLPool().createMaterialOpenGL(m_context);
        if (!material)
            return nullptr;

        material->set(cast<MaterialOpenGL>());

        return material;
    }

    //////////////////////////////////////////
    void MaterialOpenGL::set(MaterialPtr const& _material)
    {
        Material::set(_material);

        setContextOpenGL(_material->castRaw<MaterialOpenGL>()->m_context);
    }

    //////////////////////////////////////////
    void MaterialOpenGL::notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_context = nullptr;
    }

    //////////////////////////////////////////
    void MaterialOpenGL::notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL)
    {

    }

    //////////////////////////////////////////
    void MaterialOpenGL::notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL)
    {

    }

    //////////////////////////////////////////
    void MaterialOpenGL::setContextOpenGL(ContextOpenGL* _contextOpenGL)
    {
        if (m_context == _contextOpenGL)
            return;

        if (m_context)
        {
            m_context->eventDestroyed.unsubscribe(this, &MaterialOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventGLContextWillBeDestroyed.unsubscribe(this, &MaterialOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextSetup.unsubscribe(this, &MaterialOpenGL::notifyContextOpenGLContextSetup);
        }

        m_context = _contextOpenGL;

        if (m_context)
        {
            m_context->eventDestroyed.subscribe(this, &MaterialOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventGLContextWillBeDestroyed.subscribe(this, &MaterialOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextSetup.subscribe(this, &MaterialOpenGL::notifyContextOpenGLContextSetup);
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
