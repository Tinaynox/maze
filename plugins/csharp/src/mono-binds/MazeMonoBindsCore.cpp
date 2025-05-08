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
#include "maze-plugin-csharp/mono-binds/MazeMonoBindsCore.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp/ecs/events/MazeEcsCSharpEvents.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeTaskManager.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntityPrefabManager.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-graphics/ecs/MazeEcsAssetScene.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    inline void MazeLog(MonoString* _log)
    {
        Char* cstr = mono_string_to_utf8(_log);
        Debug::Log("MONO: %s", cstr);
        mono_free(cstr);
    }

    //////////////////////////////////////////
    inline void MazeLogWarning(MonoString* _log)
    {
        Char* cstr = mono_string_to_utf8(_log);
        Debug::LogWarning("MONO: %s", cstr);
        mono_free(cstr);
    }

    //////////////////////////////////////////
    inline void MazeLogError(MonoString* _log)
    {
        Char* cstr = mono_string_to_utf8(_log);
        Debug::LogError("MONO: %s", cstr);
        mono_free(cstr);
    }


    //////////////////////////////////////////
    inline bool GetKeyState(S32 _keyCode)
    {
        return InputManager::GetInstancePtr()->getKeyState(KeyCode(_keyCode));
    }

    //////////////////////////////////////////
    inline bool GetCursorButtonState(S32 _cursorId, S32 _buttonIndex)
    {
        return InputManager::GetInstancePtr()->getCursorButtonState(_cursorId, _buttonIndex);
    }

    //////////////////////////////////////////
    inline ComponentId GetComponentIdByMonoType(MonoReflectionType* _type)
    {
        MonoType* monoType = mono_reflection_type_get_type(_type);
        ComponentId id = MonoEngine::GetComponentIdByMonoType(monoType);
        return id;
    }

    //////////////////////////////////////////
    inline Entity* InstantiateEntity(Component* _component, AssetUnitId _auid)
    {
        if (!EntityPrefabManager::GetInstancePtr())
            return nullptr;

        EcsWorld* ecsWorld = _component->getEntityRaw()->getEcsWorld();
        EcsScene* ecsScene = _component->getEntityRaw()->getEcsScene();
        EntityPtr newEntity = EntityPrefabManager::GetInstancePtr()->instantiatePrefab(_auid, ecsWorld, ecsScene);
        if (!newEntity)
            return nullptr;

        return newEntity.get();
    }

    //////////////////////////////////////////
    inline MonoString* EcsSceneGetAssetFilePath(S32 _sceneId)
    {
        EcsScenePtr const& scene = SceneManager::GetInstancePtr()->getScene(EcsSceneId(_sceneId));
        if (!scene)
            return nullptr;

        if (!scene->getMetaClass()->isInheritedFrom<EcsAssetScene>())
            return nullptr;

        EcsAssetScene* ecsAssetScene = scene->castRaw<EcsAssetScene>();
        AssetFilePtr const& file = ecsAssetScene->getFile();
        if (!file)
            return nullptr;

        return mono_string_new(mono_domain_get(), file->getFullPath().toUTF8().c_str());
    }

    //////////////////////////////////////////
    inline bool AssetFileIdIsValid(U32 _assetFileId)
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_assetFileId);
        return !!assetFile;
    }

    //////////////////////////////////////////
    inline bool AssetFileReadAsDataBlock(
        U32 _assetFileId,
        U32* _size,
        U8 const** _bytes)
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_assetFileId);
        if (!assetFile)
            return false;

        TaskManager* taskManager = TaskManager::GetInstancePtr();
        if (!taskManager)
            return false;

        ByteBufferPtr byteBuffer = assetFile->readAsByteBuffer();
        byteBuffer.incRef();

        *_size = byteBuffer->getSize();
        *_bytes = byteBuffer->getDataRO();

        taskManager->addMainThreadTask(
            [weakPtr = (ByteBufferWPtr)byteBuffer]()
            {
                if (ByteBufferPtr strongPtr = weakPtr.lock())
                    strongPtr.decRef();
            });

        return true;
    }

    //////////////////////////////////////////
    inline Entity* CreateEntity(Component* _component)
    {
        if (!EntityPrefabManager::GetInstancePtr())
            return nullptr;

        EcsWorld* ecsWorld = _component->getEntityRaw()->getEcsWorld();
        EcsScene* ecsScene = _component->getEntityRaw()->getEcsScene();
        EntityPtr newEntity = ecsScene->createEntity();
        if (!newEntity)
            return nullptr;

        return newEntity.get();
    }

    //////////////////////////////////////////
    inline S32 EntityGetEntityId(Entity* _entity) { return (S32)_entity->getId(); }

    //////////////////////////////////////////
    inline Component* EntityGetComponent(Entity* _entity, ComponentId _componentId)
    {
        if (!_entity)
            return nullptr;

        ComponentPtr const& component = _entity->getComponentById(_componentId);
        return component.get();
    }

    //////////////////////////////////////////
    inline MonoObject* EntityGetMonoBehaviourComponentObject(Entity* _entity, ComponentId _componentId)
    {
        if (!_entity)
            return nullptr;

        ComponentPtr const& component = _entity->getComponentById(_componentId);
        if (!component)
            return nullptr;

        MAZE_ERROR_RETURN_VALUE_IF(component->getClassUID() != ClassInfo<MonoBehaviour>::UID(), nullptr, "Component is not MonoBehaviour!");
        return component->castRaw<MonoBehaviour>()->getMonoInstance()->getInstance();
    }

    //////////////////////////////////////////
    inline Component* EntityCreateNativeComponent(Entity* _entity, ComponentId _componentId)
    {
        if (!_entity)
            return nullptr;

        auto const& componentFactory = EntityManager::GetInstancePtr()->getComponentFactory();
        if (!componentFactory)
            return nullptr;

        ComponentPtr component = componentFactory->createComponent(_componentId);
        if (!component)
            return nullptr;

        _entity->addComponent(component);
        
        return component.get();
    }

    //////////////////////////////////////////
    inline MonoObject* EntityCreateMonoBehaviourComponentObject(Entity* _entity, ComponentId _componentId)
    {
        if (!_entity)
            return nullptr;

        ScriptClassPtr const& scriptClass = MonoEngine::GetMonoBehaviourSubClass(_componentId);
        MAZE_ERROR_RETURN_VALUE_IF(!scriptClass, nullptr, "Undefined MonoBehaviour script with componentId=%d", _componentId);

        MonoBehaviourPtr component = MonoBehaviour::Create();
        component->setMonoClass(scriptClass);

        MAZE_ERROR_RETURN_VALUE_IF(!component->getMonoInstance(), nullptr, "Failed to create MonoBehaviour script instance - %s", scriptClass->getFullName().c_str());
        
        _entity->addComponent(component);

        return component->getMonoInstance()->getInstance();
    }

    //////////////////////////////////////////
    inline void EntityRemoveFromEcsWorld(Entity* _entity)
    {
        _entity->removeFromEcsWorld();
    }

    //////////////////////////////////////////
    inline void EntityGetActive(Entity* _entity, bool& _outValue)
    {
        _outValue = _entity->getActiveSelf();
    }

    //////////////////////////////////////////
    inline void EntitySetActive(Entity* _entity, bool _value)
    {
        _entity->setActiveSelf(_value);
    }


    //////////////////////////////////////////
    inline S32 ComponentGetFrameNumber(Component* _component) { return _component->getEntityRaw()->getEcsWorld()->getFrameNumber(); }

    //////////////////////////////////////////
    inline S32 ComponentGetEntityId(Component* _component) { return (S32)_component->getEntityId(); }

    //////////////////////////////////////////
    inline S32 ComponentGetSceneId(Component* _component)
    {
        Entity* entity = _component->getEntityRaw();
        if (!entity)
            return -1;

        EcsScene* ecsScene = entity->getEcsScene();
        if (!ecsScene)
            return -1;

        return ecsScene->getId();
    }

    //////////////////////////////////////////
    inline ComponentId ComponentGetComponentId(MonoString* _name)
    {
        Char* cstr = mono_string_to_utf8(_name);
        ComponentId id = GetComponentIdByName(cstr);
        mono_free(cstr);

        return id;
    }

    //////////////////////////////////////////
    inline Component* ComponentGetComponent(Component* _component, ComponentId _componentId)
    {
        if (!_component->getEntityRaw())
            return nullptr;

        ComponentPtr const& component = _component->getEntityRaw()->getComponentById(_componentId);
        return component.get();
    }

    //////////////////////////////////////////
    inline Component* ComponentGetComponentInheritedFrom(Component* _component, ComponentId _componentId)
    {
        if (!_component->getEntityRaw())
            return nullptr;

        ComponentPtr const& component = _component->getEntityRaw()->getComponentInheritedFrom(_componentId);
        return component.get();
    }

    //////////////////////////////////////////
    inline MonoObject* ComponentGetMonoBehaviourComponentObject(Component* _component, ComponentId _componentId)
    {
        if (!_component->getEntityRaw())
            return nullptr;

        ComponentPtr const& component = _component->getEntityRaw()->getComponentById(_componentId);
        if (!component)
            return nullptr;

        MAZE_ERROR_RETURN_VALUE_IF(component->getClassUID() != ClassInfo<MonoBehaviour>::UID(), nullptr, "Component is not MonoBehaviour!");
        return component->castRaw<MonoBehaviour>()->getMonoInstance()->getInstance();
    }

    //////////////////////////////////////////
    inline Component* ComponentCreateNativeComponent(Component* _component, ComponentId _componentId)
    {
        if (!_component)
            return nullptr;

        auto const& componentFactory = EntityManager::GetInstancePtr()->getComponentFactory();
        if (!componentFactory)
            return nullptr;

        ComponentPtr component = componentFactory->createComponent(_componentId);
        if (!component)
            return nullptr;

        _component->getEntityRaw()->addComponent(component);

        return component.get();
    }

    //////////////////////////////////////////
    inline MonoObject* ComponentCreateMonoBehaviourComponentObject(Component* _component, ComponentId _componentId)
    {
        if (!_component)
            return nullptr;

        ScriptClassPtr const& scriptClass = MonoEngine::GetMonoBehaviourSubClass(_componentId);
        MAZE_ERROR_RETURN_VALUE_IF(!scriptClass, nullptr, "Undefined MonoBehaviour script with componentId=%d", _componentId);

        MonoBehaviourPtr component = MonoBehaviour::Create();
        component->setMonoClass(scriptClass);

        MAZE_ERROR_RETURN_VALUE_IF(!component->getMonoInstance(), nullptr, "Failed to create MonoBehaviour script instance - %s", scriptClass->getFullName().c_str());

        _component->getEntityRaw()->addComponent(component);

        return component->getMonoInstance()->getInstance();
    }

    //////////////////////////////////////////
    inline Entity* ComponentGetEntity(Component* _component)
    {
        return _component->getEntityRaw();
    }

    //////////////////////////////////////////
    inline void ComponentSendMonoEvent(Component* _component, Entity* _entityReceiver, MonoObject* _monoEventPtr)
    {
        MonoObject* monoEvent = (MonoObject*)mono_gchandle_get_target((uint32_t)(uintptr_t)_monoEventPtr);
        _component->getEntityRaw()->getEcsWorld()->sendEvent<Maze::MonoEvent>(_entityReceiver->getId(), monoEvent);
    }

    //////////////////////////////////////////
    inline void ComponentBroadcastMonoEvent(Component* _component, void* _monoEventPtr)
    {
        MonoObject* monoEvent = (MonoObject*)mono_gchandle_get_target((uint32_t)(uintptr_t)_monoEventPtr);
        _component->getEntityRaw()->getEcsWorld()->broadcastEvent<Maze::MonoEvent>(monoEvent);
    }

    //////////////////////////////////////////
    inline bool ComponentIsEditorMode(Component* _component)
    {
        static Size const c_editorHash = MAZE_HCS("editor").hash;
        Set<HashedString> const& tags = _component->getEntityRaw()->getEcsWorld()->getTags();
        for (HashedString const& tag : tags)
            if (tag.getHash() == c_editorHash)
                return true;

        return false;
    }


    //////////////////////////////////////////
    inline void Transform3DTranslate(Component* _component, Vec3F const& _delta)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _component->castRaw<Transform3D>()->translate(_delta);
    }

    //////////////////////////////////////////
    inline void Transform3DRotate(Component* _component, Vec3F const& _axis, F32 _angle)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _component->castRaw<Transform3D>()->rotate(_axis, _angle);
    }

    //////////////////////////////////////////
    inline void Transform3DSetParent(Component* _component, Component* _parent)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        MAZE_ERROR_RETURN_IF(_parent->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _component->castRaw<Transform3D>()->setParent(_parent ? _parent->cast<Transform3D>() : nullptr);
    }

    //////////////////////////////////////////
    inline void Transform3DGetPosition(Component* _component, Vec3F& _outPosition)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _outPosition = _component->castRaw<Transform3D>()->getLocalPosition();
    }

    //////////////////////////////////////////
    inline void Transform3DSetPosition(Component* _component, Vec3F const& _position)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _component->castRaw<Transform3D>()->setLocalPosition(_position);
    }

    //////////////////////////////////////////
    inline void Transform3DGetWorldPosition(Component* _component, Vec3F& _outPosition)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _outPosition = _component->castRaw<Transform3D>()->getWorldPosition();
    }

    //////////////////////////////////////////
    inline void Transform3DGetRotation(Component* _component, Quaternion& _outRotation)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _outRotation = _component->castRaw<Transform3D>()->getLocalRotation();
    }

    //////////////////////////////////////////
    inline void Transform3DSetRotation(Component* _component, Quaternion const& _rotation)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _component->castRaw<Transform3D>()->setLocalRotation(_rotation);
    }

    //////////////////////////////////////////
    inline void Transform3DGetScale(Component* _component, Vec3F& _outScale)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _outScale = _component->castRaw<Transform3D>()->getLocalScale();
    }

    //////////////////////////////////////////
    inline void Transform3DGetLocalTransform(Component* _component, TMat& _outTMat)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _outTMat = _component->castRaw<Transform3D>()->getLocalTransform();
    }

    //////////////////////////////////////////
    inline void Transform3DSetLocalTransform(Component* _component, TMat const& _tm)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _component->castRaw<Transform3D>()->setLocalTransform(_tm);
    }

    //////////////////////////////////////////
    inline void Transform3DGetWorldTransform(Component* _component, TMat& _outTMat)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _outTMat = _component->castRaw<Transform3D>()->getWorldTransform();
    }

    //////////////////////////////////////////
    inline void Transform3DSetWorldTransform(Component* _component, TMat const& _tm)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _component->castRaw<Transform3D>()->setWorldTransform(_tm);
    }

    //////////////////////////////////////////
    inline void Transform3DSetScale(Component* _component, Vec3F const& _scale)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _component->castRaw<Transform3D>()->setLocalScale(_scale);
    }


    //////////////////////////////////////////
    inline MonoString* NameGetName(Component* _component)
    {
        MAZE_ERROR_RETURN_VALUE_IF(_component->getClassUID() != ClassInfo<Name>::UID(), nullptr, "Component is not Name!");
        return mono_string_new(mono_domain_get(), _component->castRaw<Name>()->getName().c_str());
    }

    //////////////////////////////////////////
    inline void NameSetName(Component* _component, MonoString* _name)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Name>::UID(), "Component is not Name!");

        Char* cstr = mono_string_to_utf8(_name);
        _component->castRaw<Name>()->setName(cstr);
        mono_free(cstr);
    }


    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API BindCppFunctionsCore()
    {
        // Log
        MAZE_CORE_MONO_BIND_FUNC(MazeLog);
        MAZE_CORE_MONO_BIND_FUNC(MazeLogWarning);
        MAZE_CORE_MONO_BIND_FUNC(MazeLogError);

        // Input
        MAZE_CORE_MONO_BIND_FUNC(GetKeyState);
        MAZE_CORE_MONO_BIND_FUNC(GetCursorButtonState);

        // Ecs
        MAZE_CORE_MONO_BIND_FUNC(GetComponentIdByMonoType);
        MAZE_CORE_MONO_BIND_FUNC(CreateEntity);
        MAZE_CORE_MONO_BIND_FUNC(InstantiateEntity);

        // EcsScene
        MAZE_CORE_MONO_BIND_FUNC(EcsSceneGetAssetFilePath);

        // Entity
        MAZE_CORE_MONO_BIND_FUNC(EntityGetEntityId);
        MAZE_CORE_MONO_BIND_FUNC(EntityGetComponent);
        MAZE_CORE_MONO_BIND_FUNC(EntityGetMonoBehaviourComponentObject);
        MAZE_CORE_MONO_BIND_FUNC(EntityCreateNativeComponent);
        MAZE_CORE_MONO_BIND_FUNC(EntityCreateMonoBehaviourComponentObject);
        MAZE_CORE_MONO_BIND_FUNC(EntityRemoveFromEcsWorld);
        MAZE_CORE_MONO_BIND_FUNC(EntityGetActive);
        MAZE_CORE_MONO_BIND_FUNC(EntitySetActive);

        // Component
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetFrameNumber);
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetEntityId);
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetSceneId);
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetComponentId);
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetComponent);
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetComponentInheritedFrom);
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetMonoBehaviourComponentObject);
        MAZE_CORE_MONO_BIND_FUNC(ComponentCreateNativeComponent);
        MAZE_CORE_MONO_BIND_FUNC(ComponentCreateMonoBehaviourComponentObject);
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetEntity);
        MAZE_CORE_MONO_BIND_FUNC(ComponentSendMonoEvent);
        MAZE_CORE_MONO_BIND_FUNC(ComponentBroadcastMonoEvent);
        MAZE_CORE_MONO_BIND_FUNC(ComponentIsEditorMode);

        // Transform3D
        MAZE_CORE_MONO_BIND_FUNC(Transform3DTranslate);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DRotate);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DSetParent);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DGetPosition);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DSetPosition);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DGetWorldPosition);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DGetRotation);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DSetRotation);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DGetScale);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DSetScale);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DGetLocalTransform);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DSetLocalTransform);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DGetWorldTransform);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DSetWorldTransform);

        // Name
        MAZE_CORE_MONO_BIND_FUNC(NameGetName);
        MAZE_CORE_MONO_BIND_FUNC(NameSetName);

        // AssetFile
        MAZE_CORE_MONO_BIND_FUNC(AssetFileIdIsValid);
        MAZE_CORE_MONO_BIND_FUNC(AssetFileReadAsDataBlock);
    }

} // namespace Maze
//////////////////////////////////////////
