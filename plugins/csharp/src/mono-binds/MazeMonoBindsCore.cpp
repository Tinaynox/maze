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
    inline S32 GetFrameNumber(Component* _component) { return _component->getEntityRaw()->getEcsWorld()->getFrameNumber(); }

    //////////////////////////////////////////
    inline S32 GetEntityId(Component* _component) { return (S32)_component->getEntityId(); }

    //////////////////////////////////////////
    inline ComponentId GetComponentIdHelper(MonoString* _name)
    {
        Char* cstr = mono_string_to_utf8(_name);
        ComponentId id = GetComponentIdByName(cstr);
        mono_free(cstr);

        return id;
    }

    //////////////////////////////////////////
    inline ComponentId GetComponentIdByMonoType(MonoReflectionType* _type)
    {
        MonoType* monoType = mono_reflection_type_get_type(_type);
        ComponentId id = MonoEngine::GetComponentIdByMonoType(monoType);
        return id;
    }


    //////////////////////////////////////////
    inline Component* GetComponent(Component* _component, ComponentId _componentId)
    {
        if (!_component->getEntityRaw())
            return nullptr;

        ComponentPtr const& component = _component->getEntityRaw()->getComponentById(_componentId);
        return component.get();
    }

    //////////////////////////////////////////
    inline MonoObject* GetMonoBehaviourComponentObject(Component* _component, ComponentId _componentId)
    {
        if (!_component->getEntityRaw())
            return nullptr;

        ComponentPtr const& component = _component->getEntityRaw()->getComponentById(_componentId);
        if (!component)
            return nullptr;

        MAZE_ERROR_RETURN_VALUE_IF(component->getClassUID() != ClassInfo<MonoBehaviour>::UID(), nullptr, "Component is not MonoBehaviour!");
        return component->castRaw<MonoBehaviour>()->getMonoInstance().getInstance();
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
    void MAZE_PLUGIN_CSHARP_API BindCppFunctionsCore()
    {
        MAZE_CORE_MONO_BIND_FUNC(MazeLog);
        MAZE_CORE_MONO_BIND_FUNC(MazeLogWarning);
        MAZE_CORE_MONO_BIND_FUNC(MazeLogError);
        MAZE_CORE_MONO_BIND_FUNC(GetKeyState);
        MAZE_CORE_MONO_BIND_FUNC(GetFrameNumber);
        MAZE_CORE_MONO_BIND_FUNC(GetEntityId);
        MAZE_CORE_MONO_BIND_FUNC_WITH_NAME(GetComponentIdHelper, GetComponentId);
        MAZE_CORE_MONO_BIND_FUNC(GetComponentIdByMonoType);
        MAZE_CORE_MONO_BIND_FUNC(GetComponent);
        MAZE_CORE_MONO_BIND_FUNC(GetMonoBehaviourComponentObject);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DTranslate);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DRotate);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DGetPosition);
        MAZE_CORE_MONO_BIND_FUNC(Transform3DSetPosition);
    }

} // namespace Maze
//////////////////////////////////////////
