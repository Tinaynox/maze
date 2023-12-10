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
#include "MazeCoreHeader.hpp"
#include "maze-core/system/emscripten/MazeWindowEmscripten.hpp"
#include "maze-core/managers/emscripten/MazeWindowManagerEmscripten.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class WindowParamsEmscripten
    //
    //////////////////////////////////////////
    WindowParamsEmscripten::WindowParamsEmscripten()
    {
    }

    ////////////////////////////////////
    bool WindowParamsEmscripten::init(WindowParamsPtr const& _windowParams)
    {
        if (!WindowParams::init(_windowParams))
            return false;

        WindowParamsEmscripten* windowParamsEmscripten = _windowParams->castRaw<WindowParamsEmscripten>();


        return true;
    }
    
    
    //////////////////////////////////////////
    static const CString c_canvasName = "canvas";
    
    //////////////////////////////////////////
    // Class WindowEmscripten
    //
    //////////////////////////////////////////
    WindowEmscripten::WindowEmscripten()
        : m_clientSize(Vec2U32::c_zero)
        , m_clientSizeBeforeFullscreen(Vec2U32::c_zero)
    {
        emscripten_set_fullscreenchange_callback(NULL, (void*)this, 1, &WindowEmscripten::ProcessFullscreenChangeCallback);
        emscripten_set_webglcontextlost_callback(NULL, (void*)this, 1, &WindowEmscripten::ProcessContextLostCallback);
        emscripten_set_webglcontextrestored_callback(NULL, (void*)this, 1, &WindowEmscripten::ProcessContextRestoredCallback);
        emscripten_set_resize_callback(NULL, (void*)this, 1, &WindowEmscripten::ProcessResizeCallback);
        
    }

    //////////////////////////////////////////
    WindowEmscripten::~WindowEmscripten()
    {
        emscripten_set_fullscreenchange_callback(NULL, NULL, 0, NULL);
        emscripten_set_webglcontextlost_callback(NULL, NULL, 0, NULL);
        emscripten_set_webglcontextrestored_callback(NULL, NULL, 0, NULL);
        emscripten_set_resize_callback(NULL, NULL, 0, NULL);
    }


    //////////////////////////////////////////
    bool WindowEmscripten::init(WindowParamsPtr const& _params)
    {
        if (!Window::init(_params))
            return false;
        
        S32 width = 0;
        S32 height = 0;
        S32 isFullscreen = 0;        
        emscripten_get_canvas_element_size(c_canvasName, &width, &height);
        m_clientSize.x = width;
        m_clientSize.y = height;
        
        EmscriptenFullscreenChangeEvent fullscreenStatus;
        emscripten_get_fullscreen_status(&fullscreenStatus);
        isFullscreen = fullscreenStatus.isFullscreen;
        
        m_params->windowMode = isFullscreen ? WindowMode::Fullscreen
                                            : WindowMode::Windowed;
        
        EmscriptenFullscreenStrategy strategy;
        strategy.scaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF;
        strategy.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
        strategy.canvasResizedCallback = WindowEmscripten::ProcessFullscreenStrategyCanvasResizedCallback;
        strategy.canvasResizedCallbackUserData = this;
        emscripten_enter_soft_fullscreen(c_canvasName, &strategy);
        
        
        processWindowCreated();
        
        return true;
    }
    
    //////////////////////////////////////////
    bool WindowEmscripten::isOpened()
    {
        
        return true;
    }
    
    ////////////////////////////////////
    void WindowEmscripten::setClientSize(Vec2U32 const& _size)
    {
        resize(_size.x, _size.y);
    }

    //////////////////////////////////////////
    Vec2U32 WindowEmscripten::getClientSize()
    {
        return m_clientSize;
    }

    //////////////////////////////////////////
    Vec2U32 WindowEmscripten::getFullSize()
    {
        return getClientSize();
    }

    //////////////////////////////////////////
    void WindowEmscripten::setPosition(Vec2S32 const& _position)
    {
    }

    //////////////////////////////////////////
    Vec2S32 WindowEmscripten::getPosition()
    {
        return Vec2U32::c_zero;
    }

    //////////////////////////////////////////
    void WindowEmscripten::close()
    {
        
    }

    //////////////////////////////////////////
    bool WindowEmscripten::getFocused()
    {
        return true;
    }


    //////////////////////////////////////////
    void WindowEmscripten::setFocused(bool _value)
    {

    }

    //////////////////////////////////////////
    void WindowEmscripten::maximizeFullscreen()
    {
        if (m_params->windowMode != WindowMode::Fullscreen)
        {
            EmscriptenFullscreenChangeEvent fullscreenStatus;
            emscripten_get_fullscreen_status(&fullscreenStatus);
            
        
            m_params->windowMode = WindowMode::Fullscreen;
            m_clientSizeBeforeFullscreen = m_clientSize;
            resize(fullscreenStatus.screenWidth, fullscreenStatus.screenHeight);
            
            setFullscreen(true);
        }
    }

    //////////////////////////////////////////
    DisplayPtr const& WindowEmscripten::getRelatedDisplay()
    {
        static DisplayPtr nullPointer;

        WindowManager* windowManager = WindowManager::GetInstancePtr();
        if (!windowManager || windowManager->getDisplays().empty())
            return nullPointer;

        return windowManager->getDisplays().front();
    }
    
    //////////////////////////////////////////
    bool WindowEmscripten::updateTitle()
    {
        return false;
    }
    
    //////////////////////////////////////////
    bool WindowEmscripten::updateWindowMode()
    {
        if (m_params->windowMode == WindowMode::Fullscreen)
        {
            if (emscripten_request_fullscreen(NULL, 1) == EMSCRIPTEN_RESULT_SUCCESS)
            {
                Debug::log << "emscripten_request_fullscreen: " << "SUCCESS" << endl;
            }
            else
            {
                Debug::log << "emscripten_request_fullscreen: " << "FAILED" << endl;
            }
                
        }
        else
        {
            if (emscripten_exit_fullscreen() == EMSCRIPTEN_RESULT_SUCCESS)
            {
                Debug::log << "emscripten_exit_fullscreen: " << "SUCCESS" << endl;
            }
            else
            {
                Debug::log << "emscripten_exit_fullscreen: " << "FAILED" << endl;
            }    
        }
        
        return false;
    }

    //////////////////////////////////////////
    bool WindowEmscripten::updateMinimized()
    {
        return false;
    }
    
    //////////////////////////////////////////
    void WindowEmscripten::resize(U32 _width, U32 _height)
    {
        if (emscripten_set_canvas_element_size(c_canvasName, _width, _height) == EMSCRIPTEN_RESULT_SUCCESS)
        {
            processWindowSizeChanged();
        }
        else
        {
            MAZE_ERROR("Canvas resize failed!");
        }    
        
    }
    
    //////////////////////////////////////////
    EM_BOOL WindowEmscripten::ProcessFullscreenChangeCallback( 
        int _eventType,
        EmscriptenFullscreenChangeEvent const* _event,
        void* _userData)
    {       
        WindowEmscripten* window = static_cast<WindowEmscripten*>(_userData);
        
        
        if (_event->isFullscreen)
        {
            if (window->m_params->windowMode != WindowMode::Fullscreen)
            {
                Debug::log << "FULLSCREEN!" << endl;
                window->m_clientSizeBeforeFullscreen = window->m_clientSize;
                window->resize(_event->screenWidth, _event->screenHeight);
                window->setFullscreen(true);
            }
        }
        else
        {
            if (window->m_params->windowMode != WindowMode::Windowed)
            {
                Debug::log << "WINDOWED!" << endl;
                window->resize(window->m_clientSizeBeforeFullscreen.x, window->m_clientSizeBeforeFullscreen.y);
                window->setFullscreen(false);
            }
        }
                    
        return EMSCRIPTEN_RESULT_SUCCESS;
    }

    //////////////////////////////////////////
    EM_BOOL WindowEmscripten::ProcessContextLostCallback( 
        int _eventType,
        void const* _reserved,
        void* _userData)
    {
        Debug::log << "WindowEmscripten::ProcessContextLostCallback" << endl;
        
        WindowEmscripten* window = static_cast<WindowEmscripten*>(_userData);
        
        return EMSCRIPTEN_RESULT_SUCCESS;
    }

    //////////////////////////////////////////
    EM_BOOL WindowEmscripten::ProcessContextRestoredCallback( 
        int _eventType,
        void const* _reserved,
        void* _userData)
    {
        Debug::log << "WindowEmscripten::ProcessContextRestoredCallback" << endl;
        
        WindowEmscripten* window = static_cast<WindowEmscripten*>(_userData);
        
        return EMSCRIPTEN_RESULT_SUCCESS;
    }    
        
    //////////////////////////////////////////
    EM_BOOL WindowEmscripten::ProcessResizeCallback( 
        int _eventType,
        EmscriptenUiEvent const* _uiEvent,
        void* _userData)
    {       
        WindowEmscripten* window = static_cast<WindowEmscripten*>(_userData);
        
        window->m_clientSize.x = _uiEvent->windowInnerWidth;
        window->m_clientSize.y = _uiEvent->windowInnerHeight;
        window->processWindowSizeChanged();
        
        return EMSCRIPTEN_RESULT_SUCCESS;
    }
    
    //////////////////////////////////////////
    EM_BOOL WindowEmscripten::ProcessFullscreenStrategyCanvasResizedCallback(
        int _eventType,
        void const* _reserved,
        void* _userData)
    {
        WindowEmscripten* window = static_cast<WindowEmscripten*>(_userData);
    
        S32 width = 0;
        S32 height = 0;
        S32 isFullscreen = 0;        
        emscripten_get_canvas_element_size(c_canvasName, &width, &height);
             
        window->m_clientSize.x = width;
        window->m_clientSize.y = height;
        
        window->m_params->windowMode = isFullscreen ? WindowMode::Fullscreen
                                                    : WindowMode::Windowed;
        
        window->processWindowSizeChanged();
     
        return EMSCRIPTEN_RESULT_SUCCESS;
    }
    
    
} // namespace Maze
//////////////////////////////////////////
