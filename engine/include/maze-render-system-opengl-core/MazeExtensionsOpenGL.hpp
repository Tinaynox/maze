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
#if (!defined(_MazeExtensionsOpenGL_hpp_))
#define _MazeExtensionsOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ExtensionsOpenGL);
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);


    //////////////////////////////////////////
    // Class ExtensionsOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API ExtensionsOpenGL
        : public SharedObject<ExtensionsOpenGL>
    {
    public:

        //////////////////////////////////////////
        virtual ~ExtensionsOpenGL();

        //////////////////////////////////////////
        inline ContextOpenGL* getContext() const { return m_context; }

        //////////////////////////////////////////
        inline bool hasGLExtension(String const& _ext) { return m_extensions.count(_ext) != 0; }
        
        //////////////////////////////////////////
        virtual bool loadGLExtensions();

        //////////////////////////////////////////
        void printGLExtensions();


        //////////////////////////////////////////
        inline bool getSupportArbBufferStorage() const { return m_supportArbBufferStorage; }

        //////////////////////////////////////////
        inline bool getSupportMultisample() const { return m_supportMultisample; }

        //////////////////////////////////////////
        inline bool getSupportClipDistance() const { return m_supportClipDistance; }

    protected:

        //////////////////////////////////////////
        ExtensionsOpenGL();

        //////////////////////////////////////////
        virtual bool init(ContextOpenGL* _context);

        //////////////////////////////////////////
        void saveCommonChecks();
    
    protected:
        ContextOpenGL* m_context;

        Set<String> m_extensions;

        bool m_extensionsLoaded = false;

        bool m_supportArbBufferStorage = false;
        bool m_supportMultisample = false;
        bool m_supportClipDistance = false;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeExtensionsOpenGL_hpp_
//////////////////////////////////////////
