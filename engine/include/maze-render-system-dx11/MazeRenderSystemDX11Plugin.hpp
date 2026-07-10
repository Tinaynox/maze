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
#if (!defined(_MazeRenderSystemDX11Plugin_hpp_))
#define _MazeRenderSystemDX11Plugin_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-core/system/MazePlugin.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystemDX11Plugin);
    MAZE_USING_SHARED_PTR(RenderSystemDX11);


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallRenderSystemDX11Plugin(RenderSystemDX11Config const& _config = RenderSystemDX11Config());

    //////////////////////////////////////////
    void UninstallRenderSystemDX11Plugin();

#endif


    //////////////////////////////////////////
    // Class RenderSystemDX11Plugin
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API RenderSystemDX11Plugin
        : public Plugin
        , public std::enable_shared_from_this<RenderSystemDX11Plugin>
    {
    public:
        //////////////////////////////////////////
        static constexpr CString const c_libraryName = "maze-render-system-dx11";

    public:

        //////////////////////////////////////////
        virtual ~RenderSystemDX11Plugin();

        //////////////////////////////////////////
        static RenderSystemDX11PluginPtr Create(RenderSystemDX11Config const& _config = RenderSystemDX11Config());

        //////////////////////////////////////////
        virtual String const& getName() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void install() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void uninstall() MAZE_OVERRIDE;


        //////////////////////////////////////////
        RenderSystemDX11Config const& getConfig() const { return m_config; }

        //////////////////////////////////////////
        void setConfig(RenderSystemDX11Config const& _config) { m_config = _config; }

    protected:

        //////////////////////////////////////////
        RenderSystemDX11Plugin();

        //////////////////////////////////////////
        bool init(RenderSystemDX11Config const& _config);

    protected:
        RenderSystemDX11WPtr m_renderSystem;

        RenderSystemDX11Config m_config;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderSystemDX11Plugin_hpp_
//////////////////////////////////////////
