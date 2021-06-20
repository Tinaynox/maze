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
#if (!defined(_Example_hpp_))
#define _Example_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-engine/MazeEngine.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Example);


    //////////////////////////////////////////
    enum class ExampleMainRenderWindowState
    {
        None = 0,
        ReadyToCreate,
        Created,
        Error
    };


    //////////////////////////////////////////
    // Class Example
    //
    //////////////////////////////////////////
    class Example
        : public Engine
    {
    public:

        //////////////////////////////////////////
        virtual ~Example();

        //////////////////////////////////////////
        static ExamplePtr Create(EngineConfig const& _config);
    

        //////////////////////////////////////////
        RenderWindowPtr const& getMainRenderWindow() const { return m_mainRenderWindow; }

        //////////////////////////////////////////
        Rect2DF const& getMainRenderWindowViewport() const { return m_mainRenderWindowViewport; }

        //////////////////////////////////////////
        inline Vec2DU getMainRenderWindowAbsoluteSize()
        {
            return Vec2DU(Vec2DF(getMainRenderWindow()->getRenderTargetSize()) * getMainRenderWindowViewport().size);
        }


        //////////////////////////////////////////
        static inline Example* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline Example& GetInstance() { return *s_instance; }

        //////////////////////////////////////////
        void openDebugEditor();

        //////////////////////////////////////////
        void closeDebugEditor();

        //////////////////////////////////////////
        void updateDebugEditor();

        //////////////////////////////////////////
        bool isMainWindowReadyToRender();

        //////////////////////////////////////////
        inline F32 getDebugEditorProgress() const { return m_debugEditorProgress; }

        //////////////////////////////////////////
        inline bool isDebugEditorProgress() const { return m_debugEditorProgress > 0.0f && m_debugEditorProgress < 1.0f; }

    public:

        //////////////////////////////////////////
        MultiDelegate<Rect2DF const&> eventMainRenderWindowViewportChanged;
        MultiDelegate<> eventMainRenderWindowCreated;
        MultiDelegate<> eventCoreGameResourcesLoaded;

    protected:

        //////////////////////////////////////////
        Example();

        //////////////////////////////////////////
        bool init(EngineConfig const& _config);

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyFrame();

        //////////////////////////////////////////
        bool createMainRenderWindow();

        //////////////////////////////////////////
        bool createMainRenderWindowAndGoToSplashNow();

        //////////////////////////////////////////
        void processReadyToCreateWindowAndGoToSplash();

        //////////////////////////////////////////
        virtual bool initMainManagers() MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool initExampleManagers();

        //////////////////////////////////////////
        bool loadPlugins();

        //////////////////////////////////////////
        void setMainRenderWindowViewport(Rect2DF const& _mainRenderWindowViewport);

        //////////////////////////////////////////
        void notifyDebuggerActiveChanged(bool _active);

        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _data);

        //////////////////////////////////////////
        void updateDebugEditorViewport();

    protected:
        static Example* s_instance;

        ExampleMainRenderWindowState m_mainRenderWindowState;
        RenderWindowPtr m_mainRenderWindow;
        Rect2DF m_mainRenderWindowViewport;


        F32 m_debugEditorProgress;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _Example_hpp_
//////////////////////////////////////////
