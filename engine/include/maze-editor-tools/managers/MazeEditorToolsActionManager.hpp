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
#if (!defined(_MazeEditorToolsActionManager_hpp_))
#define _MazeEditorToolsActionManager_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/utils/MazeObservableValue.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorAction.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorToolsActionManager);
    MAZE_USING_SHARED_PTR(EditorAction);


    //////////////////////////////////////////
    // Class EditorToolsActionManager
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorToolsActionManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~EditorToolsActionManager();

        //////////////////////////////////////////
        static void Initialize(EditorToolsActionManagerPtr& _manager);


        //////////////////////////////////////////
        static inline EditorToolsActionManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EditorToolsActionManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        void setCurrentHistoryIndex(S32 _index);

        //////////////////////////////////////////
        void applyAction(EditorActionPtr const& _action);

        //////////////////////////////////////////
        void undoAction();

        //////////////////////////////////////////
        void redoAction();

        ////////////////////////////////////
        void clearHistory();

        ////////////////////////////////////
        EditorActionPtr const& getLastEditorAction() const;

        ////////////////////////////////////
        template <typename TEditorAction>
        SharedPtr<TEditorAction> castLastEditorAction() const
        {
            EditorActionPtr const& lastAction = getLastEditorAction();
            if (lastAction)
                return lastAction->castSafe<TEditorAction>();

            return nullptr;
        }


        //////////////////////////////////////////
        inline U32 getLastChangeTimestamp() const { return m_lastChangeTimestamp.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<U32 const&>& getLastChangeTimestampChangedEvent() { return m_lastChangeTimestamp.eventValueChanged; }

    protected:

        //////////////////////////////////////////
        EditorToolsActionManager();

        //////////////////////////////////////////
        virtual bool init();

        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _data);

    protected:
        static EditorToolsActionManager* s_instance;

    private:

        Vector<EditorActionPtr> m_history;
        S32 m_currentHistoryIndex = -1;

        Size m_historyMaxCapacity = 100;

        bool m_historyIndexSetting = false;

        ObservableValue<U32> m_lastChangeTimestamp = 0u;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorToolsActionManager_hpp_
//////////////////////////////////////////
