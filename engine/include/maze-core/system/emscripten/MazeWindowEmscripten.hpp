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
#if (!defined(_MazeWindowEmscripten_hpp_))
#define _MazeWindowEmscripten_hpp_


//////////////////////////////////////////
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include <emscripten/html5.h>


//////////////////////////////////////////
namespace Maze
{
    ///////////////////////////////////////
    MAZE_USING_SHARED_PTR(WindowEmscripten);
    MAZE_USING_SHARED_PTR(WindowParamsEmscripten);


    //////////////////////////////////////////
    // Class WindowParamsEmscripten
    //
    //////////////////////////////////////////
    class MAZE_CORE_API WindowParamsEmscripten
        : public WindowParams
    {
    public:

        //////////////////////////////////////////
        friend class WindowParams;

    public:


    protected:
        WindowParamsEmscripten();

        ////////////////////////////////////
        virtual bool init(WindowParamsPtr const& _windowParams);

    };

    
    //////////////////////////////////////////
    // Class WindowEmscripten
    //
    //////////////////////////////////////////
    class MAZE_CORE_API WindowEmscripten
        : public Window
        , public MultiDelegateCallbackReceiver
    {
    private:
        
        //////////////////////////////////////////
        friend class Window;
        
    public:

        //////////////////////////////////////////
        virtual ~WindowEmscripten();
        
        //////////////////////////////////////////
        virtual bool isOpened() MAZE_OVERRIDE;
        
        ////////////////////////////////////
        virtual void setClientSize(Vec2DU const& _size) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2DU getClientSize() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2DU getFullSize() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setPosition(Vec2DS const& _position) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2DS getPosition() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void close() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool getFocused() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setFocused(bool _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void maximizeFullscreen() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual DisplayPtr const& getRelatedDisplay() MAZE_OVERRIDE;


    protected:
        
        //////////////////////////////////////////
        WindowEmscripten();
        
        //////////////////////////////////////////
        virtual bool init(WindowParamsPtr const& _params) MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual bool updateTitle() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual bool updateWindowMode() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool updateMinimized() MAZE_OVERRIDE;

        
        
        //////////////////////////////////////////
        void resize(U32 _width, U32 _height);
        
        
        
        //////////////////////////////////////////
        static EM_BOOL ProcessFullscreenChangeCallback( 
            int _eventType,
            EmscriptenFullscreenChangeEvent const* _event,
            void* _userData);
        
        //////////////////////////////////////////
        static EM_BOOL ProcessContextLostCallback( 
            int _eventType,
            void const* _reserved,
            void* _userData);
        
        //////////////////////////////////////////
        static EM_BOOL ProcessContextRestoredCallback( 
            int _eventType,
            void const* _reserved,
            void* _userData);
            
        //////////////////////////////////////////
        static EM_BOOL ProcessResizeCallback( 
            int _eventType,
            EmscriptenUiEvent const* _uiEvent,
            void* _userData);
            
        //////////////////////////////////////////
        static EM_BOOL ProcessFullscreenStrategyCanvasResizedCallback(
            int _eventType,
            void const* _reserved,
            void* _userData);
            
        private:
            Vec2DU m_clientSize;
            
            Vec2DU m_clientSizeBeforeFullscreen;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeWindowEmscripten_hpp_
//////////////////////////////////////////
