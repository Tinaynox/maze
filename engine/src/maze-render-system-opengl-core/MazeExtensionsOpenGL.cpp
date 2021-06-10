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
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeExtensionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ExtensionsOpenGL
    //
    //////////////////////////////////////////
    ExtensionsOpenGL::ExtensionsOpenGL()
        : m_extensionsLoaded(false)
        , m_supportArbBufferStorage(false)
        , m_supportMultisample(false)
    {

    }

    //////////////////////////////////////////
    ExtensionsOpenGL::~ExtensionsOpenGL()
    {
    }

    //////////////////////////////////////////
    bool ExtensionsOpenGL::init(ContextOpenGL* _context)
    {
        m_context = _context;

        return true;
    }

    ////////////////////////////////////
    bool ExtensionsOpenGL::loadGLExtensions()
    {
        m_extensions.clear();

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        m_context->makeCurrentContext(true);


        if (   m_context->getMajorVersion() >= 3
            && mzglGetIntegerv != nullptr
            && mzglGetStringi != nullptr)
        {
            MZGLint n = 0;
            MZGLint i = 0;
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_NUM_EXTENSIONS, &n));
            for (i = 0; i < n; i++)
            {
                CString extension;
                MAZE_GL_CALL(extension = (CString)mzglGetStringi(MAZE_GL_EXTENSIONS, i));
                if (extension)
                    m_extensions.insert(extension);
            }
        }
        else
        if (mzglGetString != nullptr)
        {
            CString extList = nullptr;
            MAZE_GL_CALL(extList = (CString)mzglGetString(MAZE_GL_EXTENSIONS));

            if (extList)
            {
                Vector<String> words;
                StringHelper::SplitWords(extList, words);
                for (Size i = 0, in = words.size(); i < in; ++i)
                    if (!words[i].empty())
                        m_extensions.insert(words[i]);
            }
        }

        m_extensionsLoaded = true;

        saveCommonChecks();

        return true;
    }

    //////////////////////////////////////////
    void ExtensionsOpenGL::printGLExtensions()
    {
        Vector<String> sortedExtensions(m_extensions.begin(), m_extensions.end());
        std::sort(sortedExtensions.begin(), sortedExtensions.end());

        MAZE_LOG("GL_EXTENSIONS: ");
        for (String const& glExtension : sortedExtensions)
            MAZE_LOG("\t- %s", glExtension.c_str());
    }

    //////////////////////////////////////////
    void ExtensionsOpenGL::saveCommonChecks()
    {
        m_supportArbBufferStorage = m_context->hasMinVersion(4, 4) || hasGLExtension("GL_ARB_buffer_storage");
        m_supportMultisample = hasGLExtension("EXT_multisample_compatibility");
    }


} // namespace Maze
//////////////////////////////////////////
