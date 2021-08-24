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
#if (!defined(_GraphicsSettings_hpp_))
#define _GraphicsSettings_hpp_


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
    MAZE_USING_SHARED_PTR(GameGraphicsSettings);

    //////////////////////////////////////////
    // Class GameGraphicsSettings
    //
    //////////////////////////////////////////
    class GameGraphicsSettings
        : public Settings
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(GameGraphicsSettings, Settings);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(GameGraphicsSettings);

    public:

        //////////////////////////////////////////
        GameGraphicsSettings();

        //////////////////////////////////////////
        virtual ~GameGraphicsSettings();


        //////////////////////////////////////////
        void setFullscreen(bool _fullscreen) { m_fullscreen = _fullscreen; }

        //////////////////////////////////////////
        inline bool getFullscreen() const { return m_fullscreen; }

        //////////////////////////////////////////
        inline void switchFullscreen() { setFullscreen(!getFullscreen()); }

        //////////////////////////////////////////
        inline auto& getFullscreenChangedEvent() { return m_fullscreen.eventValueChanged; }


        //////////////////////////////////////////
        void setVSync(int _value) { m_vsync = _value; }

        //////////////////////////////////////////
        inline int getVSync() const { return m_vsync; }

        //////////////////////////////////////////
        inline void switchVSync() { setVSync(!getVSync()); }

        //////////////////////////////////////////
        inline auto& getVSyncChangedEvent() { return m_vsync.eventValueChanged; }


        //////////////////////////////////////////
        void setPostProcessEnabled(bool _postProcessEnabled) { m_postProcessEnabled = _postProcessEnabled; }

        //////////////////////////////////////////
        inline bool getPostProcessEnabled() const { return m_postProcessEnabled; }

        //////////////////////////////////////////
        inline void switchPostProcessEnabled() { setPostProcessEnabled(!getPostProcessEnabled()); }

        //////////////////////////////////////////
        inline auto& getPostProcessEnabledChangedEvent() { return m_postProcessEnabled.eventValueChanged; }

    protected:
        ObservableValue<bool> m_fullscreen = false;
        ObservableValue<int> m_vsync = 1;
        ObservableValue<bool> m_postProcessEnabled = true;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGameGraphicsSettings_hpp_
//////////////////////////////////////////
