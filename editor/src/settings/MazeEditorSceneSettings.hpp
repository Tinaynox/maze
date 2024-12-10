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
        void setPrefabDebugLightEnabled(bool _value) { m_prefabDebugLightEnabled = _value; }

        //////////////////////////////////////////
        inline bool getPrefabDebugLightEnabled() const { return m_prefabDebugLightEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getPrefabDebugLightEnabledChangedEvent() { return m_prefabDebugLightEnabled.eventValueChanged; }

        //////////////////////////////////////////
        inline void switchPrefabDebugLightEnabled() { setPrefabDebugLightEnabled(!getPrefabDebugLightEnabled()); }


        //////////////////////////////////////////
        void setPrefabDebugAxesEnabled(bool _value) { m_prefabDebugAxesEnabled = _value; }

        //////////////////////////////////////////
        inline bool getPrefabDebugAxesEnabled() const { return m_prefabDebugAxesEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getPrefabDebugAxesEnabledChangedEvent() { return m_prefabDebugAxesEnabled.eventValueChanged; }

        //////////////////////////////////////////
        inline void switchPrefabDebugAxesEnabled() { setPrefabDebugAxesEnabled(!getPrefabDebugAxesEnabled()); }


        //////////////////////////////////////////
        void setPrefabDebugGridEnabled(bool _value) { m_prefabDebugGridEnabled = _value; }

        //////////////////////////////////////////
        inline bool getPrefabDebugGridEnabled() const { return m_prefabDebugGridEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getPrefabDebugGridEnabledChangedEvent() { return m_prefabDebugGridEnabled.eventValueChanged; }

        //////////////////////////////////////////
        inline void switchPrefabDebugGridEnabled() { setPrefabDebugGridEnabled(!getPrefabDebugGridEnabled()); }




        //////////////////////////////////////////
        void setSceneDebugLightEnabled(bool _value) { m_sceneDebugLightEnabled = _value; }

        //////////////////////////////////////////
        inline bool getSceneDebugLightEnabled() const { return m_sceneDebugLightEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getSceneDebugLightEnabledChangedEvent() { return m_sceneDebugLightEnabled.eventValueChanged; }

        //////////////////////////////////////////
        inline void switchSceneDebugLightEnabled() { setSceneDebugLightEnabled(!getSceneDebugLightEnabled()); }


        //////////////////////////////////////////
        void setSceneDebugAxesEnabled(bool _value) { m_sceneDebugAxesEnabled = _value; }

        //////////////////////////////////////////
        inline bool getSceneDebugAxesEnabled() const { return m_sceneDebugAxesEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getSceneDebugAxesEnabledChangedEvent() { return m_sceneDebugAxesEnabled.eventValueChanged; }

        //////////////////////////////////////////
        inline void switchSceneDebugAxesEnabled() { setSceneDebugAxesEnabled(!getSceneDebugAxesEnabled()); }


        //////////////////////////////////////////
        void setSceneDebugGridEnabled(bool _value) { m_sceneDebugGridEnabled = _value; }

        //////////////////////////////////////////
        inline bool getSceneDebugGridEnabled() const { return m_sceneDebugGridEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getSceneDebugGridEnabledChangedEvent() { return m_sceneDebugGridEnabled.eventValueChanged; }

        //////////////////////////////////////////
        inline void switchSceneDebugGridEnabled() { setSceneDebugGridEnabled(!getSceneDebugGridEnabled()); }


    protected:

        ObservableValue<bool> m_prefabDebugLightEnabled = true;
        ObservableValue<bool> m_prefabDebugAxesEnabled = true;
        ObservableValue<bool> m_prefabDebugGridEnabled = true;
        ObservableValue<bool> m_sceneDebugLightEnabled = false;
        ObservableValue<bool> m_sceneDebugAxesEnabled = true;
        ObservableValue<bool> m_sceneDebugGridEnabled = true;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorSceneSettings_hpp_
//////////////////////////////////////////
