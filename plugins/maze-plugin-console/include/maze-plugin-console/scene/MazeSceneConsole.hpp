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
#if (!defined(_MazeSceneConsole_hpp_))
#define _MazeSceneConsole_hpp_


//////////////////////////////////////////
#include "maze-plugin-console/MazeConsoleHeader.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLineRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSimpleLineRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeSystemTextEditBox2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneConsole);
    


    //////////////////////////////////////////
    // Class SceneConsole
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CONSOLE_API SceneConsole
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneConsole, ECSRenderScene);

    public:

        //////////////////////////////////////////
        static SceneConsolePtr Create(RenderTargetPtr const& _renderTarget);
    
        //////////////////////////////////////////
        virtual ~SceneConsole();


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        SceneConsole();

        //////////////////////////////////////////
        virtual bool init(RenderTargetPtr const& _renderTarget);


        //////////////////////////////////////////
        void create2D();


        //////////////////////////////////////////
        void updateSize();

        //////////////////////////////////////////
        void notifyLogChanged(String const& _log);

        //////////////////////////////////////////
        void notifyTextInput(SystemTextEditBox2D* _edit);

        //////////////////////////////////////////
        void notifyTextChanged(SystemTextEditBox2D* _edit, String const& _text);

        //////////////////////////////////////////
        void notifyBackgroundClick(CursorInputEvent const& _event);

        //////////////////////////////////////////
        void notifyRenderTargetResized(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void updateLogText();

        //////////////////////////////////////////
        void updateHintText();

        //////////////////////////////////////////
        void completeCommand();

        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _keyboardData);

    protected:
        bool m_firstFrame = true;

        CanvasPtr m_canvas;

        SpriteRenderer2DPtr m_background;
        UIElement2DPtr m_backgroundElement;
        SystemTextRenderer2DPtr m_consoleText;
        SystemTextEditBox2DPtr m_edit;
        SystemTextRenderer2DPtr m_hintText;


        S32 m_lastCommandIndex = -1;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSceneConsole_hpp_
//////////////////////////////////////////