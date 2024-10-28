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
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
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
    inline void Translate3D(S32 _eid, Vec3F const& _delta)
    {
        EntityPtr const& entity = EntityManager::GetInstancePtr()->getDefaultWorldRaw()->getEntity(EntityId(_eid));
        if (entity)
        {
            Transform3D* transform = entity->getComponentRaw<Transform3D>();
            if (transform)
            {
                transform->translate(_delta);
            }
        }
    }

    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API BindCppFunctionsCore()
    {
        MAZE_MONO_BIND_FUNC(MazeLog);
        MAZE_MONO_BIND_FUNC(MazeLogWarning);
        MAZE_MONO_BIND_FUNC(MazeLogError);
        MAZE_MONO_BIND_FUNC(GetKeyState);
        MAZE_MONO_BIND_FUNC(Translate3D);
    }

} // namespace Maze
//////////////////////////////////////////
