//////////////////////////////////////////
//
// Maze RenderSystemGL3
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
#if (!defined(_MazeRenderSystemOpenGL3Plugin_hpp_))
#define _MazeRenderSystemOpenGL3Plugin_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Header.hpp"
#include "maze-core/system/MazePlugin.hpp"
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL3Plugin);
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL3);


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallRenderSystemOpenGL3Plugin(RenderSystemOpenGLConfig const& _config = RenderSystemOpenGLConfig());

    //////////////////////////////////////////
    void UninstallRenderSystemOpenGL3Plugin();

#endif


    //////////////////////////////////////////
    // Class RenderSystemOpenGL3Plugin
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL3_API RenderSystemOpenGL3Plugin
        : public Plugin
        , public std::enable_shared_from_this<RenderSystemOpenGL3Plugin>
    {
    public:
        //////////////////////////////////////////
        static constexpr CString const c_libraryName = "maze-render-system-opengl3";

    public:

        //////////////////////////////////////////
        virtual ~RenderSystemOpenGL3Plugin();

        //////////////////////////////////////////
        static RenderSystemOpenGL3PluginPtr Create(RenderSystemOpenGLConfig const& _config = RenderSystemOpenGLConfig());

        //////////////////////////////////////////
        virtual String const& getName() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void install() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void uninstall() MAZE_OVERRIDE;


        //////////////////////////////////////////
        RenderSystemOpenGLConfig const& getConfig() const { return m_config; }

        //////////////////////////////////////////
        void setConfig(RenderSystemOpenGLConfig const& _config) { m_config = _config; }

    protected:

        //////////////////////////////////////////
        RenderSystemOpenGL3Plugin();

        //////////////////////////////////////////
        bool init(RenderSystemOpenGLConfig const& _config);

    protected:
        RenderSystemOpenGL3WPtr m_renderSystem;

        RenderSystemOpenGLConfig m_config;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderSystemOpenGL3Plugin_hpp_
//////////////////////////////////////////
