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
#if (!defined(_ExampleSettings_hpp_))
#define _ExampleSettings_hpp_


//////////////////////////////////////////
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/utils/MazeObservableValue.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ExampleSettings);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_5(ExampleWaterRenderMode,
        Default,
        Reflection,
        Refraction,
        FloorDepth,
        WaterDepth);


    //////////////////////////////////////////
    // Class ExampleSettings
    //
    //////////////////////////////////////////
    class ExampleSettings
        : public Settings
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ExampleSettings, Settings);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ExampleSettings);

    public:

        //////////////////////////////////////////
        ExampleSettings();

        //////////////////////////////////////////
        virtual ~ExampleSettings();


        //////////////////////////////////////////
        void setDebugMenu(bool _value) { m_debugMenu = _value; }

        //////////////////////////////////////////
        inline bool getDebugMenu() const { return m_debugMenu.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getDebugMenuChangedEvent() { return m_debugMenu.eventValueChanged; }


        //////////////////////////////////////////
        void setParticlesEnabled(bool _value) { m_particlesEnabled = _value; }

        //////////////////////////////////////////
        inline bool getParticlesEnabled() const { return m_particlesEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getParticlesEnabledChangedEvent() { return m_particlesEnabled.eventValueChanged; }


        //////////////////////////////////////////
        void setWaterEnabled(bool _value) { m_waterEnabled = _value; }

        //////////////////////////////////////////
        inline bool getWaterEnabled() const { return m_waterEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getWaterEnabledChangedEvent() { return m_waterEnabled.eventValueChanged; }


        //////////////////////////////////////////
        void setBloomEnabled(bool _value) { m_bloomEnabled = _value; }

        //////////////////////////////////////////
        inline bool getBloomEnabled() const { return m_bloomEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getBloomEnabledChangedEvent() { return m_bloomEnabled.eventValueChanged; }


        //////////////////////////////////////////
        void setExampleWaterRenderMode(ExampleWaterRenderMode _value) { m_exampleWaterRenderMode = _value; }

        //////////////////////////////////////////
        inline ExampleWaterRenderMode getExampleWaterRenderMode() const { return m_exampleWaterRenderMode.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<ExampleWaterRenderMode const&>& getExampleWaterRenderModeChangedEvent() { return m_exampleWaterRenderMode.eventValueChanged; }


    protected:

        ObservableValue<bool> m_debugMenu = false;
        ObservableValue<bool> m_particlesEnabled = true;
        ObservableValue<bool> m_waterEnabled = true;
        ObservableValue<bool> m_bloomEnabled = true;
        ObservableValue<ExampleWaterRenderMode> m_exampleWaterRenderMode = ExampleWaterRenderMode::Default;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _ExampleSettings_hpp_
//////////////////////////////////////////