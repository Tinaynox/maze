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
#include "maze-core/managers/android/MazeSystemManagerAndroid.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/managers/android/MazeInputManagerAndroid.hpp"
#include "maze-core/managers/android/MazeWindowManagerAndroid.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/system/android/MazeWindowAndroid.hpp"


//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    // Class SystemManagerAndroid
    //
    //////////////////////////////////////////
    SystemManagerAndroid::SystemManagerAndroid()
        : m_androidApp(nullptr)
        , m_androidAppInited(false)
        , m_androidAppFocused(false)
        , m_androidAppPaused(false)
    {
        m_deviceCategory = DeviceCategory::Phone;
    }

    //////////////////////////////////////////
    SystemManagerAndroid::~SystemManagerAndroid()
    {
    }

    //////////////////////////////////////////
    bool SystemManagerAndroid::initApplication()
    {
        for (Maze::S8 const* commandLineArgument : m_commandLineArguments)
        {
            if (StringHelper::IsStartsWith(commandLineArgument, "--android_app"))
            {
                Vector<ConstSpan<Char>> words;
                StringHelper::SplitWords(commandLineArgument, words, '=');

                m_androidApp = reinterpret_cast<android_app*>(StringHelper::StringToU64(words[1].getPtr()));

                break;
            }
        }

        MAZE_ERROR_RETURN_VALUE_IF(!m_androidApp, false, "--android_app is not set!");
        
        
        m_androidApp->onAppCmd = OnEngineCommand;
        m_androidApp->onInputEvent = OnEngineInput;
        m_androidApp->userData = this;
        
        
        while (true)
        {
            // Read all pending events.
            int ident;
            int events;
            struct android_poll_source* source = nullptr;

            while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
            {

                // Process this event.
                if (source != nullptr)
                {
                    source->process(m_androidApp, source);
                }

                // Check if we are exiting.
                if (m_androidApp->destroyRequested != 0)
                {
                    Debug::Log("Activity destroyed - exit!");
                    return false;
                }
            }

            if (m_androidAppInited && !m_androidAppPaused && getNativeWindow() != nullptr)
            {
                // Application frame
                processApplicationFrame();
            }
        }
        
        //processApplicationInit();
        return true;
    }

    //////////////////////////////////////////
    bool SystemManagerAndroid::init(Vector<Maze::S8 const*> const& _commandLineArguments)
    {
        if (!SystemManagerUnix::init(_commandLineArguments))
            return false;

        return true;
    }
    
    //////////////////////////////////////////
    ANativeWindow* SystemManagerAndroid::getNativeWindow() const
    {
        return m_androidApp->window;
    }
    
    //////////////////////////////////////////
    void SystemManagerAndroid::OnEngineCommand(struct android_app* _app, int32_t _cmd)
    {
        SystemManagerAndroid* system = reinterpret_cast< SystemManagerAndroid* >(_app->userData);

        switch (_cmd)
        {
            case APP_CMD_SAVE_STATE:

                break;
            case APP_CMD_INIT_WINDOW:

                // The window is being shown, get it ready.
                if (_app->window != nullptr)
                {
                    system->setAndroidAppInited( true );
                }

                system->eventAndroidNativeWindowInited();

                break;
            case APP_CMD_TERM_WINDOW:
                system->eventAndroidNativeWindowWillClose();

                break;
                
            case APP_CMD_GAINED_FOCUS:
                system->setAndroidAppFocused(true);

                break;
                
            case APP_CMD_LOST_FOCUS:
                system->setAndroidAppFocused(false);
                break;
                
            case APP_CMD_PAUSE:
                system->setAndroidAppPaused(true);

                break;
                
            case APP_CMD_RESUME:
                system->setAndroidAppPaused(false);

                break;
        }
    }
    
    //////////////////////////////////////////
    int32_t SystemManagerAndroid::OnEngineInput(android_app* _android, AInputEvent* _event)
    {
        InputManagerAndroid* inputManager = InputManager::GetInstancePtr()->castRaw<InputManagerAndroid>();
        WindowManagerAndroid* windowManager = WindowManager::GetInstancePtr()->castRaw<WindowManagerAndroid>();
        
        WindowAndroidPtr const& windowAndroid =  windowManager->getWindow();
        
        S32 eventType = AInputEvent_getType(_event);
        
        switch (eventType)
        {
            case AINPUT_EVENT_TYPE_MOTION:
            {
                S32 action = (S32)(AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction(_event));

                S32 actionId  = AMotionEvent_getAction(_event);

                S32 pointerIndex = (actionId & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                S32 pointerId = AMotionEvent_getPointerId(_event, pointerIndex);

                S32 x = (AMotionEvent_getX(_event, pointerIndex));
                S32 y = windowAndroid->getClientSize().y - (AMotionEvent_getY(_event, pointerIndex));
                S32 sourceId = AInputEvent_getSource(_event);
                S32 deviceId = AInputEvent_getDeviceId(_event);
                S32 pointerCount = AMotionEvent_getPointerCount(_event);

                if (pointerId >= 4)
                    return 1;

                switch (action)
                {
                    case AMOTION_EVENT_ACTION_DOWN:
                    {
                        InputEventTouchData eventData;
                        eventData.type = InputEventTouchType::Press;
                        eventData.index = pointerId;
                        eventData.x = x;
                        eventData.y = y;
                        eventData.window = windowAndroid.get();
                        inputManager->generateInputEvent(eventData);
                        break;
                    }
                    case AMOTION_EVENT_ACTION_UP:
                    {
                        InputEventTouchData eventData;
                        eventData.type = InputEventTouchType::Release;
                        eventData.index = pointerId;
                        eventData.x = x;
                        eventData.y = y;
                        eventData.window = windowAndroid.get();
                        inputManager->generateInputEvent(eventData);
                        break;
                    }
                    case AMOTION_EVENT_ACTION_MOVE:
                    {
                        for (S32 i = 0; i < pointerCount; ++i)
                        {
                            InputEventTouchData eventData;
                            eventData.type = InputEventTouchType::Move;
                            eventData.index = i;
                            eventData.x = (AMotionEvent_getX(_event, i));
                            eventData.y = windowAndroid->getClientSize().y - AMotionEvent_getY(_event, i);
                            eventData.window = windowAndroid.get();
                            inputManager->generateInputEvent(eventData);
                        }

                        break;
                    }
                    case AMOTION_EVENT_ACTION_CANCEL:
                    {
                        InputEventTouchData eventData;
                        eventData.type = InputEventTouchType::Cancel;
                        eventData.index = pointerId;
                        eventData.x = x;
                        eventData.y = y;
                        eventData.window = windowAndroid.get();
                        inputManager->generateInputEvent(eventData);

                        break;
                    }

                    case AMOTION_EVENT_ACTION_OUTSIDE:
                    {

                        break;
                    }

                    case AMOTION_EVENT_ACTION_POINTER_DOWN:
                    {
                        InputEventTouchData eventData;
                        eventData.type = InputEventTouchType::Press;
                        eventData.index = pointerId;
                        eventData.x = x;
                        eventData.y = y;
                        eventData.window = windowAndroid.get();
                        inputManager->generateInputEvent(eventData);
                        break;
                    }

                    case AMOTION_EVENT_ACTION_POINTER_UP:
                    {
                        InputEventTouchData eventData;
                        eventData.type = InputEventTouchType::Release;
                        eventData.index = pointerId;
                        eventData.x = x;
                        eventData.y = y;
                        eventData.window = windowAndroid.get();
                        inputManager->generateInputEvent(eventData);
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case AINPUT_EVENT_TYPE_KEY:
            {
                S32 action = AKeyEvent_getAction(_event);

                S32 keyCode = AKeyEvent_getKeyCode(_event);
                S32 metaState = AKeyEvent_getMetaState(_event);
                
                break;
            }
        }
        
        return 0;
    }

    //////////////////////////////////////////
    void SystemManagerAndroid::setAndroidAppInited(bool _value)
    {
        if (m_androidAppInited == _value)
            return;

        m_androidAppInited = _value;

        if (m_androidAppInited)
        {
            processApplicationInit();
            eventAndroidAppInited();
        }
    }

    //////////////////////////////////////////
    void SystemManagerAndroid::setAndroidAppFocused(bool _value)
    {
        if (m_androidAppFocused == _value)
            return;

        m_androidAppFocused = _value;

        eventAndroidAppFocusChanged();
    }

    //////////////////////////////////////////
    void SystemManagerAndroid::setAndroidAppPaused(bool _value)
    {
        if (m_androidAppPaused == _value)
            return;

        m_androidAppPaused = _value;

        eventAndroidAppPauseChanged();
    }

    //////////////////////////////////////////
    ANativeActivity* SystemManagerAndroid::getNativeActivity() const
    {
        MAZE_ERROR_RETURN_VALUE_IF(!m_androidApp, nullptr, "android_app is null!");
        return m_androidApp->activity;
    }

    //////////////////////////////////////////
    AAssetManager* SystemManagerAndroid::getAssetManager() const
    {
        ANativeActivity* activity = getNativeActivity();
        MAZE_ERROR_RETURN_VALUE_IF(!activity, nullptr, "ANativeActivity is null!");
        return m_androidApp->activity->assetManager;
    }

    //////////////////////////////////////////
    JavaVM* SystemManagerAndroid::getJavaVM() const
    {
        ANativeActivity* activity = getNativeActivity();
        MAZE_ERROR_RETURN_VALUE_IF(!activity, nullptr, "ANativeActivity is null!");
        return activity->vm;
    }

    //////////////////////////////////////////
    JNIEnv* SystemManagerAndroid::getJNIEnv() const
    {
        ANativeActivity* activity = getNativeActivity();
        MAZE_ERROR_RETURN_VALUE_IF(!activity, nullptr, "ANativeActivity is null!");
        return activity->env;
    }

    ///////////////////////////////////////
    void SystemManagerAndroid::callActivityMethod(String const& _methodName)
    {
        Debug::log << "SystemManager_android::callActivityMethod: " << _methodName << endl;

        ANativeActivity* activity = getNativeActivity();
        MAZE_ERROR_RETURN_IF(!activity, "ANativeActivity is null!");

        JNIEnv* jniEnv = getJNIEnv();
        MAZE_ERROR_RETURN_IF(!jniEnv, "JNIEnv is null!");

        getJavaVM()->AttachCurrentThread(&jniEnv, nullptr);

        jobject activityObject = getAndroidApp()->activity->clazz;
        jclass activityClass = jniEnv->GetObjectClass(activityObject);

        jmethodID methodId = jniEnv->GetMethodID(
                activityClass,
                _methodName.c_str(),
                "()V" );

        MAZE_ERROR_RETURN_IF(!methodId, "Undefined activitiy method: %s", _methodName.c_str());

        jniEnv->CallVoidMethod(activityObject, methodId);

        getJavaVM()->DetachCurrentThread();
    }
    
    ///////////////////////////////////////
    void SystemManagerAndroid::callActivityMethod(String const& _methodName, CString _param0)
    {
        Debug::log << "SystemManager_android::callActivityMethod: " << _methodName << endl;

        ANativeActivity* activity = getNativeActivity();
        MAZE_ERROR_RETURN_IF(!activity, "ANativeActivity is null!");

        JNIEnv* jniEnv = getJNIEnv();
        MAZE_ERROR_RETURN_IF(!jniEnv, "JNIEnv is null!");

        getJavaVM()->AttachCurrentThread(&jniEnv, nullptr);

        jobject activityObject = getAndroidApp()->activity->clazz;
        jclass activityClass = jniEnv->GetObjectClass(activityObject);

        jmethodID methodId = jniEnv->GetMethodID(
                activityClass,
                _methodName.c_str(),
                "(Ljava/lang/String;)V" );

        MAZE_ERROR_RETURN_IF(!methodId, "Undefined activitiy method: %s", _methodName.c_str());

        jstring javaString = jniEnv->NewStringUTF(_param0);
        jniEnv->CallVoidMethod(activityObject, methodId, javaString);
        jniEnv->DeleteLocalRef(javaString);

        getJavaVM()->DetachCurrentThread();
    }

    ///////////////////////////////////////
    Vector<String> SystemManagerAndroid::callActivityMethodStringVector(String const& _methodName)
    {
        Debug::log << "SystemManager_android::callActivityMethodStringVector: " << _methodName << endl;

        ANativeActivity* activity = getNativeActivity();
        MAZE_ERROR_RETURN_VALUE_IF(!activity, Vector< String >(), "ANativeActivity is null!");

        JNIEnv* jniEnv = getJNIEnv();
        MAZE_ERROR_RETURN_VALUE_IF(!jniEnv, Vector< String >(), "JNIEnv is null!");

        getJavaVM()->AttachCurrentThread(&jniEnv, nullptr);

        jobject activityObject = getAndroidApp()->activity->clazz;
        jclass activityClass = jniEnv->GetObjectClass(activityObject);

        jmethodID methodId = jniEnv->GetMethodID(
                activityClass,
                _methodName.c_str(),
                "()[Ljava/lang/String;");

        MAZE_ERROR_RETURN_VALUE_IF(!methodId, Vector<String>(), "Undefined activitiy method: %s", _methodName.c_str());

        Vector<String> result;
        jobjectArray methodResult = (jobjectArray)jniEnv->CallObjectMethod(activityObject, methodId);
        jsize resultSize = jniEnv->GetArrayLength( methodResult );

        for (jsize i = 0; i < resultSize; ++i)
        {
            jobject arrayElement = jniEnv->GetObjectArrayElement(methodResult, i);
            jstring arrayElementString = (jstring)arrayElement;
            char const* stringChars = jniEnv->GetStringUTFChars(arrayElementString, nullptr);
            result.emplace_back(stringChars);
            jniEnv->ReleaseStringUTFChars(arrayElementString, stringChars);
            jniEnv->DeleteLocalRef(arrayElement);
        }

        getJavaVM()->DetachCurrentThread();

        return result;
    }


    ///////////////////////////////////////
    String SystemManagerAndroid::callActivityMethodString(String const& _methodName)
    {
        Debug::log << "SystemManager_android::callActivityMethodString: " << _methodName << endl;

        ANativeActivity* activity = getNativeActivity();
        MAZE_ERROR_RETURN_VALUE_IF(!activity, String(), "ANativeActivity is null!");

        JNIEnv* jniEnv = getJNIEnv();
        MAZE_ERROR_RETURN_VALUE_IF(!jniEnv, String(), "JNIEnv is null!");

        getJavaVM()->AttachCurrentThread(&jniEnv, nullptr);

        jobject activityObject = getAndroidApp()->activity->clazz;
        jclass activityClass = jniEnv->GetObjectClass(activityObject);

        jmethodID methodId = jniEnv->GetMethodID(
                activityClass,
                _methodName.c_str(),
                "()Ljava/lang/String;");

        MAZE_ERROR_RETURN_VALUE_IF(!methodId, String(), "Undefined activitiy method: %s", _methodName.c_str());

        String result;
        jstring methodResult = (jstring)jniEnv->CallObjectMethod(activityObject, methodId);

        char const* stringChars = jniEnv->GetStringUTFChars(methodResult, nullptr);
        result = stringChars;
        jniEnv->ReleaseStringUTFChars(methodResult, stringChars);

        getJavaVM()->DetachCurrentThread();

        return std::move(result);
    }

    ///////////////////////////////////////
    S32 SystemManagerAndroid::callActivityMethodInteger(String const& _methodName)
    {
        Debug::log << "SystemManager_android::callActivityMethodInteger: " << _methodName << endl;

        ANativeActivity* activity = getNativeActivity();
        MAZE_ERROR_RETURN_VALUE_IF(!activity, 0, "ANativeActivity is null!");

        JNIEnv* jniEnv = getJNIEnv();
        MAZE_ERROR_RETURN_VALUE_IF(!jniEnv, 0, "JNIEnv is null!");

        getJavaVM()->AttachCurrentThread(&jniEnv, nullptr);

        jobject activityObject = getAndroidApp()->activity->clazz;
        jclass activityClass = jniEnv->GetObjectClass(activityObject);

        jmethodID methodId = jniEnv->GetMethodID(
                activityClass,
                _methodName.c_str(),
                "()I");

        MAZE_ERROR_RETURN_VALUE_IF(!methodId, 0, "Undefined activitiy method: %s", _methodName.c_str());

        jint methodResult = jniEnv->CallIntMethod(activityObject, methodId);

        getJavaVM()->DetachCurrentThread();

        return static_cast<S32>(methodResult);
    }

    ///////////////////////////////////////
    bool SystemManagerAndroid::callActivityMethodBoolean(String const& _methodName)
    {
        Debug::log << "SystemManager_android::callActivityMethodInteger: " << _methodName << endl;

        ANativeActivity* activity = getNativeActivity();
        MAZE_ERROR_RETURN_VALUE_IF(!activity, 0, "ANativeActivity is null!");

        JNIEnv* jniEnv = getJNIEnv();
        MAZE_ERROR_RETURN_VALUE_IF(!jniEnv, 0, "JNIEnv is null!");

        getJavaVM()->AttachCurrentThread(&jniEnv, nullptr);

        jobject activityObject = getAndroidApp()->activity->clazz;
        jclass activityClass = jniEnv->GetObjectClass(activityObject);

        jmethodID methodId = jniEnv->GetMethodID(
                activityClass,
                _methodName.c_str(),
                "()Z");

        MAZE_ERROR_RETURN_VALUE_IF(!methodId, 0, "Undefined activitiy method: %s", _methodName.c_str());

        jboolean methodResult = jniEnv->CallBooleanMethod(activityObject, methodId);

        getJavaVM()->DetachCurrentThread();

        return static_cast<S32>(methodResult);
    }


} // namespace Maze
//////////////////////////////////////////
