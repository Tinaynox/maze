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
#include "editor/EditorMode.hpp"


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
        void setEditorMode(EditorMode _value) { m_editorMode = _value; }

        //////////////////////////////////////////
        inline EditorMode getEditorMode() const { return m_editorMode.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<EditorMode const&>& getEditorModeChangedEvent() { return m_editorMode.eventValueChanged; }



        //////////////////////////////////////////
        void setAssetsFullPath(String _value) { m_assetsFullPath = _value; }

        //////////////////////////////////////////
        inline String getAssetsFullPath() const { return m_assetsFullPath.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<String const&>& getAssetsFullPathChangedEvent() { return m_assetsFullPath.eventValueChanged; }

    protected:

        ObservableValue<EditorMode> m_editorMode = EditorMode::None;
        ObservableValue<String> m_assetsFullPath;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorSettings_hpp_
//////////////////////////////////////////
