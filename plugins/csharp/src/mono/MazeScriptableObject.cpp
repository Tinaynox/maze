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
#include "MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/mono/MazeScriptableObject.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/mono/MazeScriptInstance.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"
#include "maze-plugin-csharp/managers/MazeMonoSerializationManager.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScriptableObject
    // 
    //////////////////////////////////////////
    MAZE_IMPLEMENT_INDEXED_RESOURCE(ScriptableObject);
    
    //////////////////////////////////////////
    ScriptableObject::ScriptableObject()
    {
        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &ScriptableObject::notifyEvent);
    }
    
    //////////////////////////////////////////
    ScriptableObject::~ScriptableObject()
    {
        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->unsubscribeEvent<MonoShutdownEvent>(this);
    }

    //////////////////////////////////////////
    ScriptableObjectPtr ScriptableObject::Create()
    {
        ScriptableObjectPtr obj;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(ScriptableObject, obj, init());
        return obj;
    }

    //////////////////////////////////////////
    bool ScriptableObject::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void ScriptableObject::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<MonoShutdownEvent>::UID())
        {
            
        }
    }

    //////////////////////////////////////////
    bool ScriptableObject::loadFromAssetFile(AssetFilePtr const& _assetFile)
    {
        MAZE_PROFILE_EVENT("ScriptableObject::loadFromAssetFile");

        MAZE_ERROR_RETURN_VALUE_IF(!_assetFile, false, "Asset File is null!");

        MAZE_LOG("Loading ScriptableObject: %s...", _assetFile->getFileName().toUTF8().c_str());
        Timer timer;

        DataBlock dataBlock;
        ByteBufferPtr byteBuffer = _assetFile->readAsByteBuffer();
        dataBlock.loadFromByteBuffer(*byteBuffer.get());
        bool result = loadFromDataBlock(dataBlock);

        F32 msTime = F32(timer.getMicroseconds()) / 1000.0f;
        MAZE_LOG(result ? "ScriptableObject %s loaded for %.1fms." : "%s scriptable object failed to load for %.1fms.", _assetFile->getFileName().toUTF8().c_str(), msTime);

        return result;
    }

    //////////////////////////////////////////
    bool ScriptableObject::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        HashedCString className = _dataBlock.getHashedCString(MAZE_HCS("_class"));
        MAZE_ERROR_RETURN_VALUE_IF(className.empty(), false, "ScriptableObject - no class name!");

        ScriptClassPtr const& scriptClass =  MonoEngine::GetScriptableObjectSubClass(className);
        MAZE_ERROR_RETURN_VALUE_IF(!scriptClass, false, "ScriptableObject class '%s' is not found!", className.str);

        m_scriptInstance = scriptClass->instantiate();
        MAZE_ERROR_RETURN_VALUE_IF(!m_scriptInstance, false, "Failed to instantiate ScriptableObject with class '%s'", className.str);
        
        m_scriptInstance->setPropertyValue(MAZE_HCS("ResourceId"), getResourceId());

        setData(_dataBlock);
        return true;
    }

    //////////////////////////////////////////
    void ScriptableObject::setData(DataBlock const& _dataBlock)
    {
        if (m_scriptInstance && m_scriptInstance->getMonoClass())
        {
            MonoHelper::IterateAllFields(m_scriptInstance->getMonoClass(),
                [&](ScriptFieldPtr const& _field)
            {
                if (_field->isStatic())
                    return;

                MonoSerializationManager::GetInstancePtr()->loadFieldFromDataBlock(
                    nullptr,
                    *m_scriptInstance.get(),
                    _field,
                    _dataBlock);
            });

            MonoHelper::IterateSerializableProperties(m_scriptInstance->getMonoClass(),
                [&](ScriptPropertyPtr const& _prop)
            {
                if (_prop->isStaticGetter())
                    return;

                MonoSerializationManager::GetInstancePtr()->loadPropertyFromDataBlock(
                    nullptr,
                    *m_scriptInstance.get(),
                    _prop,
                    _dataBlock);
            });
        }
    }

} // namespace Maze
//////////////////////////////////////////
