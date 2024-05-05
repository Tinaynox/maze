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
#include "MazeCoreHeader.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeSystemManager)


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(DeviceCategory);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(SystemPopupType);


    //////////////////////////////////////////
    // Class SystemManager
    //
    //////////////////////////////////////////
    SystemManager* SystemManager::s_instance = nullptr;

    //////////////////////////////////////////
    SystemManager::SystemManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    SystemManager::~SystemManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void SystemManager::Initialize(
        SystemManagerPtr& _systemManager,
        Vector<Maze::S8 const*> const& _commandLineArguments)
    {
        MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR(SystemManager, _systemManager, init(_commandLineArguments));
    }

    //////////////////////////////////////////
    bool SystemManager::init(Vector<Maze::S8 const*> const& _commandLineArguments)
    {
        UpdateManager::Initialize(m_updateManager);
        if (!m_updateManager)
            return false;

        m_commandLineArguments = _commandLineArguments;
        parseCommandLineArguments();

        CString forceDeviceCategory = getCommandLineArgumentValue(MAZE_HCS("device-category"));
        if (forceDeviceCategory != nullptr)
        {
            m_deviceCategory = DeviceCategory::FromString(forceDeviceCategory);
        }

        return true;
    }

    //////////////////////////////////////////
    void SystemManager::processApplicationInit()
    {
        eventApplicationInit();
    }

    //////////////////////////////////////////
    void SystemManager::processApplicationFrame()
    {
        eventApplicationFrame();
    }

    //////////////////////////////////////////
    String SystemManager::getClipboardAsString()
    {
        MAZE_NOT_IMPLEMENTED;
        return String();
    }

    //////////////////////////////////////////
    void SystemManager::setClipboardString(String const& _text)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    bool SystemManager::hasCommandLineArgumentFlag(HashedCString _key)
    {
        return m_commandLineArgumentFlags.count(_key) > 0;
    }

    //////////////////////////////////////////
    CString SystemManager::getCommandLineArgumentValue(HashedCString _key)
    {
        auto it = m_commandLineArgumentValues.find(_key);
        if (it == m_commandLineArgumentValues.end())
            return nullptr;

        if (it->second.empty())
            return nullptr;

        return it->second[0];
    }

    //////////////////////////////////////////
    Vector<CString> const& SystemManager::getCommandLineArgumentValues(HashedCString _key)
    {
        static Vector<CString> nullValue;

        auto it = m_commandLineArgumentValues.find(_key);
        if (it == m_commandLineArgumentValues.end())
            return nullValue;

        return it->second;
    }

    //////////////////////////////////////////
    void SystemManager::parseCommandLineArguments()
    {
        m_commandLineArgumentFlags.clear();
        m_commandLineArgumentValues.clear();

        CString currentKey = nullptr;
        Vector<CString> currentValues;

        auto finishCurrentFlag =
            [&]()
            {
                if (!currentKey)
                    return;

                if (*currentKey != '\0')
                {
                    if (currentValues.empty())
                        m_commandLineArgumentFlags.insert(HashedCString(currentKey));
                    else
                        m_commandLineArgumentValues.insert(HashedCString(currentKey), std::move(currentValues));
                }

                currentKey = nullptr;
                currentValues.clear();
            };

        for (CString value : m_commandLineArguments)
        {
            if (value[0] == '-')
            {
                finishCurrentFlag();

                currentKey = value + 1;
                while (*currentKey == '-' && *currentKey != '\0') { ++currentKey; }
            }
            else
            {
                if (currentKey)
                    currentValues.push_back(value);
            }
        }

        finishCurrentFlag();
    }

    //////////////////////////////////////////
    bool SystemManager::isMobileDeviceCategory() const
    {
        return m_deviceCategory == DeviceCategory::Phone || m_deviceCategory == DeviceCategory::Pad;
    }


} // namespace Maze
//////////////////////////////////////////
