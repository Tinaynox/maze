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
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntityPrefabManager.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"



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
    inline void EntityRemoveFromEcsWorld(Entity* _entity)
    {
        _entity->removeFromEcsWorld();
    }


    //////////////////////////////////////////
    inline S32 ComponentGetFrameNumber(Component* _component) { return _component->getEntityRaw()->getEcsWorld()->getFrameNumber(); }

    //////////////////////////////////////////
    inline S32 ComponentGetEntityId(Component* _component) { return (S32)_component->getEntityId(); }

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
    inline Entity* ComponentGetEntity(Component* _component)
    {
        return _component->getEntityRaw();
    }


    //////////////////////////////////////////
    inline void Transform3DTranslate(Component* _component, Vec3F _delta)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _component->castRaw<Transform3D>()->translate(_delta);
    }

    //////////////////////////////////////////
    inline void Transform3DRotate(Component* _component, Vec3F _axis, F32 _angle)
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
    inline void Transform3DSetPosition(Component* _component, Vec3F _position)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _component->castRaw<Transform3D>()->setLocalPosition(_position);
    }

    //////////////////////////////////////////
    inline void Transform3DGetRotation(Component* _component, Quaternion& _outRotation)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _outRotation = _component->castRaw<Transform3D>()->getLocalRotation();
    }

    //////////////////////////////////////////
    inline void Transform3DSetRotation(Component* _component, Quaternion _rotation)
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
    inline void Transform3DSetScale(Component* _component, Vec3F _scale)
    {
        MAZE_ERROR_RETURN_IF(_component->getClassUID() != ClassInfo<Transform3D>::UID(), "Component is not Transform3D!");
        _component->castRaw<Transform3D>()->setLocalScale(_scale);
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

        // Ecs
        MAZE_CORE_MONO_BIND_FUNC(GetComponentIdByMonoType);
        MAZE_CORE_MONO_BIND_FUNC(InstantiateEntity);

        // Entity
        MAZE_CORE_MONO_BIND_FUNC(EntityGetEntityId);
        MAZE_CORE_MONO_BIND_FUNC(EntityGetComponent);
        MAZE_CORE_MONO_BIND_FUNC(EntityGetMonoBehaviourComponentObject);
        MAZE_CORE_MONO_BIND_FUNC(EntityRemoveFromEcsWorld);

        // Component
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetFrameNumber);
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetEntityId);
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetComponentId);
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetComponent);
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetMonoBehaviourComponentObject);
        MAZE_CORE_MONO_BIND_FUNC(ComponentGetEntity);

        // Transform3D
        MAZE_CORE_MONO_BIND_FUNC(Transform3DTranslate);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DRotate);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DSetParent);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DGetPosition);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DSetPosition);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DGetRotation);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DSetRotation);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DGetScale);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DSetScale);
    }

} // namespace Maze
//////////////////////////////////////////
