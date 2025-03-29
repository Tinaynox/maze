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
#if (!defined(_MazeEditorSettings_hpp_))
#define _MazeEditorSettings_hpp_


//////////////////////////////////////////
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeObservableValue.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorSettings);

    //////////////////////////////////////////
    // Class EditorSettings
    //
    //////////////////////////////////////////
    class EditorSettings
        : public Settings
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorSettings, Settings);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(EditorSettings);

    public:

        //////////////////////////////////////////
        EditorSettings();

        //////////////////////////////////////////
        virtual ~EditorSettings();



        //////////////////////////////////////////
        void setProjectFullPath(String _value) { m_projectFullPath = _value; }

        //////////////////////////////////////////
        inline String getProjectFullPath() const { return m_projectFullPath.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<String const&>& getProjectFullPathChangedEvent() { return m_projectFullPath.eventValueChanged; }


        //////////////////////////////////////////
        void setDebugInfoEnabled(bool _value) { m_debugInfoEnabled = _value; }

        //////////////////////////////////////////
        inline bool getDebugInfoEnabled() const { return m_debugInfoEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getDebugInfoEnabledChangedEvent() { return m_debugInfoEnabled.eventValueChanged; }

        //////////////////////////////////////////
        inline void switchDebugInfoEnabled() { setDebugInfoEnabled(!getDebugInfoEnabled()); }

    protected:
        ObservableValue<String> m_projectFullPath;

        ObservableValue<bool> m_debugInfoEnabled = true;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorSettings_hpp_
//////////////////////////////////////////
