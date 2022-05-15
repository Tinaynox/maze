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
#if (!defined(_MazeEditorToolsSettings_hpp_))
#define _MazeEditorToolsSettings_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeObservableValue.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
#include "maze-editor-tools/gizmo-tools/MazeGizmoToolConfig.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorToolsSettings);

    //////////////////////////////////////////
    // Class EditorToolsSettings
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorToolsSettings
        : public Settings
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorToolsSettings, Settings);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(EditorToolsSettings);

    public:

        //////////////////////////////////////////
        EditorToolsSettings();

        //////////////////////////////////////////
        virtual ~EditorToolsSettings();


        //////////////////////////////////////////
        void setActive(bool _value) { m_active = _value; }

        //////////////////////////////////////////
        inline bool getActive() const { return m_active.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getActiveChangedEvent() { return m_active.eventValueChanged; }

        //////////////////////////////////////////
        inline void switchActive() { setActive(!getActive()); }


        //////////////////////////////////////////
        void setPause(bool _value) { m_pause = _value; }

        //////////////////////////////////////////
        inline bool getPause() const { return m_pause.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getPauseChangedEvent() { return m_pause.eventValueChanged; }

        //////////////////////////////////////////
        inline void switchPause() { setPause(!getPause()); }


        //////////////////////////////////////////
        void setSelectedGizmoTool(GizmoToolType _value) { m_selectedGizmoTool = _value; }

        //////////////////////////////////////////
        inline GizmoToolType getSelectedGizmoTool() const { return m_selectedGizmoTool.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<GizmoToolType const&>& getSelectedGizmoToolChangedEvent() { return m_selectedGizmoTool.eventValueChanged; }

    protected:

        ObservableValue<bool> m_active = true;
        ObservableValue<bool> m_pause = false;
        ObservableValue<GizmoToolType> m_selectedGizmoTool = GizmoToolType::Translate;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorToolsSettings_hpp_
//////////////////////////////////////////
