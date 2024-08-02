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
#if (!defined(_MazeEditorSceneSettings_hpp_))
#define _MazeEditorSceneSettings_hpp_


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
    MAZE_USING_SHARED_PTR(EditorSceneSettings);

    //////////////////////////////////////////
    // Class EditorSceneSettings
    //
    //////////////////////////////////////////
    class EditorSceneSettings
        : public Settings
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorSceneSettings, Settings);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(EditorSceneSettings);

    public:

        //////////////////////////////////////////
        EditorSceneSettings();

        //////////////////////////////////////////
        virtual ~EditorSceneSettings();


        //////////////////////////////////////////
        void setDebugLightEnabled(bool _value) { m_debugLightEnabled = _value; }

        //////////////////////////////////////////
        inline bool getDebugLightEnabled() const { return m_debugLightEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getDebugLightEnabledChangedEvent() { return m_debugLightEnabled.eventValueChanged; }

        //////////////////////////////////////////
        inline void switchDebugLightEnabled() { setDebugLightEnabled(!getDebugLightEnabled()); }


        //////////////////////////////////////////
        void setDebugAxesEnabled(bool _value) { m_debugAxesEnabled = _value; }

        //////////////////////////////////////////
        inline bool getDebugAxesEnabled() const { return m_debugAxesEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getDebugAxesEnabledChangedEvent() { return m_debugAxesEnabled.eventValueChanged; }

        //////////////////////////////////////////
        inline void switchDebugAxesEnabled() { setDebugAxesEnabled(!getDebugAxesEnabled()); }


        //////////////////////////////////////////
        void setDebugGridEnabled(bool _value) { m_debugGridEnabled = _value; }

        //////////////////////////////////////////
        inline bool getDebugGridEnabled() const { return m_debugGridEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getDebugGridEnabledChangedEvent() { return m_debugGridEnabled.eventValueChanged; }

        //////////////////////////////////////////
        inline void switchDebugGridEnabled() { setDebugGridEnabled(!getDebugGridEnabled()); }


    protected:

        ObservableValue<bool> m_debugLightEnabled = true;
        ObservableValue<bool> m_debugAxesEnabled = true;
        ObservableValue<bool> m_debugGridEnabled = true;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorSceneSettings_hpp_
//////////////////////////////////////////
