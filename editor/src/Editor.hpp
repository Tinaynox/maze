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
#if (!defined(_Editor_hpp_))
#define _Editor_hpp_


//////////////////////////////////////////
#include "maze-engine/MazeEngine.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-core/system/MazeInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Editor);
    MAZE_USING_SHARED_PTR(UIManager);
    MAZE_USING_SHARED_PTR(EditorManager);
    

    //////////////////////////////////////////
    enum class EditorMainRenderWindowState
    {
        None = 0,
        ReadyToCreate,
        Created,
        Error
    };


    //////////////////////////////////////////
    // Class Editor
    //
    //////////////////////////////////////////
    class Editor 
        : public Engine
    {
    public:

        //////////////////////////////////////////
        virtual ~Editor();

        //////////////////////////////////////////
        static EditorPtr Create(EngineConfig const& _config);

    
        //////////////////////////////////////////
        RenderWindowPtr const& getMainRenderWindow() const { return m_mainRenderWindow;}



        //////////////////////////////////////////
        static inline Editor* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline Editor& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        inline EditorManagerPtr const& getEditorManager() const { return m_editorManager; }

        //////////////////////////////////////////
        void loadCoreEditorAssets();

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventMainRenderWindowCreated;
        MultiDelegate<> eventCoreEditorResourcesLoaded;

    protected:

        //////////////////////////////////////////
        Editor();

        //////////////////////////////////////////
        bool init(EngineConfig const& _config);

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyFrame();

        //////////////////////////////////////////
        bool createMainRenderWindow();

        //////////////////////////////////////////
        bool createMainRenderWindowAndGoToFirstSceneNow();

        //////////////////////////////////////////
        void processReadyToCreateWindowAndGoToSplash();

        //////////////////////////////////////////
        virtual bool initMainManagers() MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool initEditorManagers();

        //////////////////////////////////////////
        bool loadPlugins();


        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _data);

        //////////////////////////////////////////
        void notifyCoreEditorResourcesLoaded();

    protected:
        static Editor* s_instance;

        EditorMainRenderWindowState m_mainRenderWindowState;
        RenderWindowPtr m_mainRenderWindow;

        EditorManagerPtr m_editorManager;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _Editor_hpp_
//////////////////////////////////////////
