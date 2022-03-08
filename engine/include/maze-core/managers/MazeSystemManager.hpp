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
#if (!defined(_MazeSystemManager_hpp_))
#define _MazeSystemManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SystemManager);
    MAZE_USING_SHARED_PTR(UpdateManager);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_5_API(MAZE_CORE_API, DeviceCategory,
        Desktop,
        Pad,
        Phone,
        Console,
        WebBrowser);

    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(MAZE_CORE_API, SystemPopupType,
        Default,
        Warning,
        Error);


    //////////////////////////////////////////
    // Class SystemManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SystemManager
        : public SharedObject<SystemManager>
    {
    public:

        //////////////////////////////////////////
        virtual ~SystemManager();

        //////////////////////////////////////////
        static void Initialize(SystemManagerPtr& _systemManager, Vector<Maze::S8 const*> const& _commandLineArguments);

        //////////////////////////////////////////
        virtual bool initApplication() MAZE_ABSTRACT;
        
        //////////////////////////////////////////
        virtual void shutdownApplication() {}


        //////////////////////////////////////////
        virtual void processApplicationInit();

        //////////////////////////////////////////
        virtual void processApplicationFrame();


        //////////////////////////////////////////
        UpdateManagerPtr const& getUpdateManager() const { return m_updateManager; }

        //////////////////////////////////////////
        inline DeviceCategory const& getDeviceCategory() const { return m_deviceCategory; }

        //////////////////////////////////////////
        static inline SystemManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline SystemManager& GetInstance() { return *s_instance; }


        ////////////////////////////////////
        virtual String getClipboardAsString();

        ////////////////////////////////////
        virtual void setClipboardString(String const& _text);

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventApplicationInit;
        MultiDelegate<> eventApplicationFrame;

    protected:

        //////////////////////////////////////////
        SystemManager();

        //////////////////////////////////////////
        virtual bool init(Vector<Maze::S8 const*> const& _commandLineArguments);

    protected:
        static SystemManager* s_instance;

        Vector<S8 const*> m_commandLineArguments;

        UpdateManagerPtr m_updateManager;

        DeviceCategory m_deviceCategory;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemManager_hpp_
//////////////////////////////////////////
