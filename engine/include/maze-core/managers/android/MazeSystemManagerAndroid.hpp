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
#if (!defined(_MazeSystemManagerAndroid_hpp_))
#define _MazeSystemManagerAndroid_hpp_


//////////////////////////////////////////
#include "maze-core/managers/unix/MazeSystemManagerUnix.hpp"
#include <android_native_app_glue.h>


//////////////////////////////////////////
namespace Maze
{
    ///////////////////////////////////////
    MAZE_USING_SHARED_PTR(SystemManagerAndroid);


    //////////////////////////////////////////
    // Class SystemManagerAndroid
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SystemManagerAndroid
        : public SystemManagerUnix
    {
    private:

        //////////////////////////////////////////
        friend class SystemManager;

    public:

        //////////////////////////////////////////
        virtual ~SystemManagerAndroid();

        //////////////////////////////////////////
        virtual bool initApplication() MAZE_OVERRIDE;

        //////////////////////////////////////////
        inline android_app* getAndroidApp() const { return m_androidApp; }

        //////////////////////////////////////////
        inline bool getAndroidAppInited() const {return  m_androidAppInited; }

        //////////////////////////////////////////
        inline bool getAndroidAppFocused() const { return m_androidAppFocused; }

        //////////////////////////////////////////
        inline bool getAndroidAppPaused() const { return m_androidAppPaused; }

        //////////////////////////////////////////
        ANativeWindow* getNativeWindow() const;

        //////////////////////////////////////////
        ANativeActivity* getNativeActivity() const;

        //////////////////////////////////////////
        AAssetManager* getAssetManager() const;

        //////////////////////////////////////////
        JavaVM* getJavaVM() const;

        //////////////////////////////////////////
        JNIEnv* getJNIEnv() const;


        ///////////////////////////////////////
        void callActivityMethod(String const& _methodName);

        ///////////////////////////////////////
        Vector<String> callActivityMethodStringVector(String const& _methodName);

        ///////////////////////////////////////
        String callActivityMethodString(String const& _methodName);

        ///////////////////////////////////////
        S32 callActivityMethodInteger(String const& _methodName);

        ///////////////////////////////////////
        bool callActivityMethodBoolean(String const& _methodName);

    public:

        MultiDelegate<> eventAndroidAppInited;
        MultiDelegate<> eventAndroidAppFocusChanged;
        MultiDelegate<> eventAndroidAppPauseChanged;
        MultiDelegate<> eventAndroidNativeWindowInited;
        MultiDelegate<> eventAndroidNativeWindowWillClose;

    protected:

        //////////////////////////////////////////
        SystemManagerAndroid();

        //////////////////////////////////////////
        virtual bool init(Vector<Maze::S8 const*> const& _commandLineArguments) MAZE_OVERRIDE;
        
        
        //////////////////////////////////////////
        static void OnEngineCommand(struct android_app* _app, int32_t _cmd);

        //////////////////////////////////////////
        static int32_t OnEngineInput(android_app* _android, AInputEvent* _event);


        //////////////////////////////////////////
        void setAndroidAppInited(bool _value);

        //////////////////////////////////////////
        void setAndroidAppFocused(bool _value);

        //////////////////////////////////////////
        void setAndroidAppPaused(bool _value);

    protected:
    
        android_app* m_androidApp;
        bool m_androidAppInited;
        bool m_androidAppFocused;
        bool m_androidAppPaused;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemManagerAndroid_hpp_
//////////////////////////////////////////
