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
#if (!defined(_MazeMaterialOpenGLPool_hpp_))
#define _MazeMaterialOpenGLPool_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-graphics/MazeMaterial.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MaterialOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    

    //////////////////////////////////////////
    // Class MaterialOpenGLPool
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API MaterialOpenGLPool
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MaterialOpenGLPool(RenderSystem* _renderSystem);

        //////////////////////////////////////////
        ~MaterialOpenGLPool();

        //////////////////////////////////////////
        MaterialOpenGLPtr createMaterialOpenGL(ContextOpenGL* _contextOpenGL);
    
        //////////////////////////////////////////
        void clear();

    protected:

        //////////////////////////////////////////
        void releaseMaterialOpenGL(
            MaterialOpenGL* _ptr,
            ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyContextDestroyed(ContextOpenGL* _contextOpenGL);

    protected:
        RenderSystem* m_renderSystem;

        Mutex m_mutex;

        Map<ContextOpenGL*, Stack<MaterialOpenGL*>> m_materialPools;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMaterialOpenGLPool_hpp_
//////////////////////////////////////////
