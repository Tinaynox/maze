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
#if (!defined(_MazeGizmoToolsManager_hpp_))
#define _MazeGizmoToolsManager_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-editor-tools/gizmo-tools/MazeGizmoToolConfig.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GizmoToolsManager);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(GizmoTool);
    

    //////////////////////////////////////////
    // Class GizmoToolsManager
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API GizmoToolsManager
        : public Updatable
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~GizmoToolsManager();

        //////////////////////////////////////////
        static void Initialize(GizmoToolsManagerPtr& _gizmosManager);


        //////////////////////////////////////////
        static inline GizmoToolsManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline GizmoToolsManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual S32 getUpdatableOrder() const MAZE_OVERRIDE { return 20100; }

    protected:

        //////////////////////////////////////////
        GizmoToolsManager();

        //////////////////////////////////////////
        virtual bool init();

        ////////////////////////////////////////// 
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    
        //////////////////////////////////////////
        void notifyCanvasChanged(CanvasPtr const& _canvas);

        //////////////////////////////////////////
        void notifyCanvasWillBeChanged(CanvasPtr const& _canvas);

        //////////////////////////////////////////
        void subscribeCanvas(CanvasPtr const& _canvas);

        //////////////////////////////////////////
        void unsubscribeCanvas(CanvasPtr const& _canvas);

        //////////////////////////////////////////
        void notifyCursorMoveIn(Vec2F const& _positionOS, CursorInputEvent& _event);

        //////////////////////////////////////////
        void notifyCursorPressIn(Vec2F const& _positionOS, CursorInputEvent& _event);

        //////////////////////////////////////////
        void notifyCursorReleaseIn(Vec2F const& _positionOS, CursorInputEvent& _event);

        //////////////////////////////////////////
        void notifyCursorReleaseOut(CursorInputEvent& _event);

        //////////////////////////////////////////
        void processCursorMove(Vec2F const& _positionOS);

        //////////////////////////////////////////
        void processCursorPressIn(Vec2F const& _positionOS);

        //////////////////////////////////////////
        void processCursorRelease();

        //////////////////////////////////////////
        void notifySelectedGizmoToolChanged(GizmoToolType const& _tool);

        //////////////////////////////////////////
        void updateSelectedGizmoTool();

    protected:
        static GizmoToolsManager* s_instance;

        GizmoToolPtr m_gizmoTool;

        Vec2F m_cursorPos = Vec2F::c_zero;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGizmoToolsManager_hpp_
//////////////////////////////////////////
