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
#if (!defined(_MazeRenderWindow_hpp_))
#define _MazeRenderWindow_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindowParams.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderWindow);
    MAZE_USING_SHARED_PTR(Window);


    //////////////////////////////////////////
    // Class RenderWindow
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderWindow
        : public RenderTarget
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderWindow, RenderTarget);

    public:

        //////////////////////////////////////////
        virtual ~RenderWindow();

        //////////////////////////////////////////
        static RenderWindowPtr Create(RenderWindowParams const& _params);

        //////////////////////////////////////////
        inline WindowPtr const& getWindow() const { return m_window; }

        //////////////////////////////////////////
        inline Window* getWindowRaw() const { return m_window.get(); }

        //////////////////////////////////////////
        virtual Vec2DU getRenderTargetSize() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void setVSync(int _vsync) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void endDraw() MAZE_OVERRIDE;


        //////////////////////////////////////////
        bool getFocused() const;

        //////////////////////////////////////////
        void close();

    protected:

        //////////////////////////////////////////
        RenderWindow();

        //////////////////////////////////////////
        using RenderTarget::init;

        //////////////////////////////////////////
        virtual bool init(RenderWindowParams const& _params);

        //////////////////////////////////////////
        void destroySystemWindow();

        //////////////////////////////////////////
        virtual WindowPtr fetchSystemWindow(WindowParamsPtr const& _params);

        //////////////////////////////////////////
        virtual bool setupSystemWindow(WindowParamsPtr const& _params);
    
        //////////////////////////////////////////
        virtual void notifyWindowCreated(Window* _window);

        //////////////////////////////////////////
        virtual void notifyWindowPositionChanged(Window* _window);

        //////////////////////////////////////////
        virtual void notifyWindowSizeChanged(Window* _window);

        //////////////////////////////////////////
        virtual void notifyWindowWillClose(Window* _window);

        //////////////////////////////////////////
        virtual void notifyWindowClosed(Window* _window);

        //////////////////////////////////////////
        virtual void notifyWindowFocusChanged(Window* _window);

    protected:

        WindowPtr m_window;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderWindow_hpp_
//////////////////////////////////////////
