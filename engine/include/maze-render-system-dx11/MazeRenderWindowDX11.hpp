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
#if (!defined(_MazeRenderWindowDX11_hpp_))
#define _MazeRenderWindowDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeRenderWindowParams.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderWindowDX11);
    class RenderSystemDX11;


    //////////////////////////////////////////
    // Class RenderWindowDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API RenderWindowDX11
        : public RenderWindow
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderWindowDX11, RenderWindow);

    public:

        //////////////////////////////////////////
        virtual ~RenderWindowDX11();

        //////////////////////////////////////////
        static RenderWindowDX11Ptr Create(
            RenderSystemDX11* _renderSystem,
            RenderWindowParams const& _params);


        //////////////////////////////////////////
        RenderSystemDX11* getRenderSystemDX11Raw() const;


        //////////////////////////////////////////
        virtual void swapBuffers() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool processRenderTargetWillSet() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processRenderTargetSet() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processRenderTargetWillReset() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setVSync(S32 _vsync) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool isReadyToRender() const MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderWindowDX11();

        //////////////////////////////////////////
        using RenderWindow::init;

        //////////////////////////////////////////
        bool init(
            RenderSystemDX11* _renderSystem,
            RenderWindowParams const& _params);

        //////////////////////////////////////////
        virtual void notifyWindowSizeChanged(Window* _window) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool createSwapChain(S32 _antialiasingLevel);

        //////////////////////////////////////////
        bool createBackBufferViews();

        //////////////////////////////////////////
        void destroyBackBufferViews();

        //////////////////////////////////////////
        void destroySwapChain();

    protected:
        RenderSystemDX11* m_renderSystemDX11 = nullptr;

        IDXGISwapChain* m_swapChain = nullptr;
        ID3D11RenderTargetView* m_renderTargetView = nullptr;
        ID3D11Texture2D* m_depthStencilTexture = nullptr;
        ID3D11DepthStencilView* m_depthStencilView = nullptr;

        S32 m_antialiasingLevel = 0;
        S32 m_vsync = 1;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderWindowDX11_hpp_
//////////////////////////////////////////
