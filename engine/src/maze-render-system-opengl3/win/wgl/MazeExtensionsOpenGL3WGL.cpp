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
#include "MazeRenderSystemOpenGL3Header.hpp"


//////////////////////////////////////////
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)


//////////////////////////////////////////
#include "maze-render-system-opengl3/win/wgl/MazeExtensionsOpenGL3WGL.hpp"
#include "maze-render-system-opengl3/win/wgl/MazeContextOpenGL3WGL.hpp"
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ExtensionsOpenGL3WGL
    //
    //////////////////////////////////////////
    ExtensionsOpenGL3WGL::ExtensionsOpenGL3WGL()
    {
    }

    //////////////////////////////////////////
    ExtensionsOpenGL3WGL::~ExtensionsOpenGL3WGL()
    {
    }

    //////////////////////////////////////////
    ExtensionsOpenGL3WGLPtr ExtensionsOpenGL3WGL::Create(ContextOpenGL* _context)
    {
        ExtensionsOpenGL3WGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ExtensionsOpenGL3WGL, object, init(_context));
        return object;
    }

    //////////////////////////////////////////
    bool ExtensionsOpenGL3WGL::init(ContextOpenGL* _context)
    {
        if (!ExtensionsOpenGL3::init(_context))
            return false;

        m_context = _context;
        
        return true;
    }

    //////////////////////////////////////////
    bool ExtensionsOpenGL3WGL::loadGLExtensionsImpl()
    {
        MAZE_PROFILE_EVENT("ExtensionsOpenGL3WGL::loadGLExtensionsImpl");

        if (!ExtensionsOpenGL3::loadGLExtensionsImpl())
            return false;

        MAZE_LOG("Loading WGL Extensions...");

        if (!m_context)
        {
            MAZE_ERROR("Loading GL Extension error - null context!");
            return false;
        }

        m_context->makeCurrentContext(true);
        HDC deviceContext = m_context->cast<ContextOpenGL3WGL>()->getDeviceContext();
        if (!deviceContext)
        {
            MAZE_ERROR("Loading GL Extension error - null device context!");
            return false;
        }

        if (mzwglGetExtensionsString == nullptr)
        {
            MAZE_ERROR("Loading GL Extension error - null mzwglGetExtensionsString!");
            return false;
        }

        CString strExtList = reinterpret_cast<const char*>(mzwglGetExtensionsString(deviceContext));
        if (strExtList)
        {
            Vector<String> words;
            StringHelper::SplitWords((String)strExtList, words);
            for (Size i = 0, in = words.size(); i < in; ++i)
                if (!words[i].empty())
                    m_extensions.insert(std::move(words[i]));
        }

        return true;
    }



} // namespace Maze
//////////////////////////////////////////


#endif // (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
//////////////////////////////////////////
