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
#if (!defined(_MazeSettingsManager_hpp_))
#define _MazeSettingsManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/settings/MazeSettings.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SettingsManager);


    //////////////////////////////////////////
    // Class SettingsManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SettingsManager
    {
    public:

        //////////////////////////////////////////
        virtual ~SettingsManager();

        //////////////////////////////////////////
        static void Initialize(SettingsManagerPtr& _settingsManager, String const& _projectName);


        //////////////////////////////////////////
        static inline SettingsManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline SettingsManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        template <class TSettings>
        inline void registerSettings()
        {
            MetaClass* metaClass = TSettings::GetMetaClass();

            m_settings[metaClass] = SharedPtr<TSettings>(new TSettings());
        }

        //////////////////////////////////////////
        template <class TSettings>
        inline SharedPtr<TSettings> getSettings() const
        {
            static SharedPtr<TSettings> nullPointer;

            MetaClass* metaClass = TSettings::GetMetaClass();

            auto const it = m_settings.find(metaClass);
            if (it == m_settings.end())
                return nullPointer;

            return std::static_pointer_cast<TSettings>(it->second);
        }

        //////////////////////////////////////////
        bool loadSettings();

        //////////////////////////////////////////
        bool saveSettings();

    protected:

        //////////////////////////////////////////
        SettingsManager();

        //////////////////////////////////////////
        virtual bool init(String const& _projectName);

        //////////////////////////////////////////
        Pair<MetaClass* const, SettingsPtr>* getSettings(String const& _settingsClassName);

    private:
        static SettingsManager* s_instance;

        String m_settingsFileFullPath;

        Map<MetaClass*, SettingsPtr> m_settings;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSettingsManager_hpp_
//////////////////////////////////////////
