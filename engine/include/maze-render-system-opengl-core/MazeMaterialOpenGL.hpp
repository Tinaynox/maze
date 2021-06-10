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
#if (!defined(_MazeMaterialOpenGL_hpp_))
#define _MazeMaterialOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MaterialOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    

    //////////////////////////////////////////
    // Class MaterialOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API MaterialOpenGL
        : public Material
    {
    public:

        //////////////////////////////////////////
        using MaterialDeleter = std::function<void(MaterialOpenGL* _ptr)>;

    public:

        //////////////////////////////////////////
        virtual ~MaterialOpenGL();

        //////////////////////////////////////////
        static MaterialOpenGLPtr Create(
            RenderSystem* _renderSystem,
            ContextOpenGL* _contextOpenGL,
            MaterialDeleter const& _deleter = DefaultDelete<MaterialOpenGL>());

        //////////////////////////////////////////
        static MaterialOpenGLPtr Create(
            MaterialOpenGLPtr const& _material,
            MaterialDeleter const& _deleter = DefaultDelete<MaterialOpenGL>());


        //////////////////////////////////////////
        inline RenderSystemOpenGL* getRenderSystemOpenGL() const { return m_renderSystem->castRaw<RenderSystemOpenGL>(); }

        //////////////////////////////////////////
        RenderSystemOpenGL* getRenderSystemOpenGLRaw()
        {
            return m_renderSystem->castRaw<RenderSystemOpenGL>();
        }

        //////////////////////////////////////////
        virtual MaterialPtr createCopy() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        MaterialOpenGL();

        //////////////////////////////////////////
        using Material::init;

        //////////////////////////////////////////
        virtual bool init(
            RenderSystem* _renderSystem,
            ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        virtual bool init(MaterialOpenGLPtr const& _material);


        //////////////////////////////////////////
        virtual void set(MaterialPtr const& _material) MAZE_OVERRIDE;


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
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMaterialOpenGL_hpp_
//////////////////////////////////////////
