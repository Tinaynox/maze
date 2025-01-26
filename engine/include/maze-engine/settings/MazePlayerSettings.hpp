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
#if (!defined(_MazePlayerSettings_hpp_))
#define _MazePlayerSettings_hpp_


//////////////////////////////////////////
#include "maze-engine/MazeEngineHeader.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/utils/MazeObservableValue.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PlayerSettings);


    //////////////////////////////////////////
    // Class PlayerSettings
    //
    //////////////////////////////////////////
    class MAZE_ENGINE_API PlayerSettings
        : public Settings
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PlayerSettings, Settings);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PlayerSettings);

    public:

        //////////////////////////////////////////
        PlayerSettings();

        //////////////////////////////////////////
        virtual ~PlayerSettings();


        //////////////////////////////////////////
        void setBloomEnabled(bool _value) { m_bloomEnabled = _value; }

        //////////////////////////////////////////
        inline bool getBloomEnabled() const { return m_bloomEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getBloomEnabledChangedEvent() { return m_bloomEnabled.eventValueChanged; }


    protected:
        ObservableValue<bool> m_bloomEnabled = true;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePlayerSettings_hpp_
//////////////////////////////////////////
