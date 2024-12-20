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
#if (!defined(_MazeTopBarController_hpp_))
#define _MazeTopBarController_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-ui/ecs/components/MazeLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-editor-tools/inspectors/MazeInspector.hpp"
#include "maze-editor-tools/gizmo-tools/MazeGizmoToolConfig.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(TopBarController);


    //////////////////////////////////////////
    // Class TopBarController
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API TopBarController
        : public Component
        , public MultiDelegateCallbackReceiver
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(TopBarController, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(TopBarController);

        //////////////////////////////////////////
        friend class Entity;


    public:

        //////////////////////////////////////////
        virtual ~TopBarController();

        //////////////////////////////////////////
        static TopBarControllerPtr Create(Canvas* _canvas);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

    protected:

        //////////////////////////////////////////
        TopBarController();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(Canvas* _canvas);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void notifySelectedGizmoToolChanged(GizmoToolType const& _tool);


        //////////////////////////////////////////
        void updateGizmoToolsButtons();

        //////////////////////////////////////////
        void updateUI();

        //////////////////////////////////////////
        void notifyPauseChanged(bool const& _value);

    protected:
        Canvas* m_canvas;

        Transform2DPtr m_transform;

        SpriteRenderer2DPtr m_bodyBackground;

        HorizontalLayout2DPtr m_leftLayout;
        ToggleButton2DPtr m_gizmoToolButtons[GizmoToolType::MAX];

        HorizontalLayout2DPtr m_layout;

        ToggleButton2DPtr m_pauseButton;
        ClickButton2DPtr m_stepButton;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTopBarController_hpp_
//////////////////////////////////////////
