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
#if (!defined(_MazeMonoHelper_hpp_))
#define _MazeMonoHelper_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ScriptClass);
    MAZE_USING_SHARED_PTR(ScriptProperty);
    MAZE_USING_SHARED_PTR(ScriptField);


    //////////////////////////////////////////
    namespace MonoHelper
    {
        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void PrintAssemblyTypes(MonoAssembly* _assembly);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoObject* InvokeMethod(MonoObject* _instance, MonoMethod* _method, void** _params = nullptr);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoObject* InvokeStaticMethod(MonoMethod* _method, void** _params = nullptr);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void ParseMonoEntitySystemAttributes(
            MonoMethod* _method,
            Set<HashedString>& _outTags,
            ComponentSystemOrder& _outOrder);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API bool IsMethodPublic(MonoMethod* _method);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void IterateSerializableProperties(
            ScriptClassPtr const& _scriptClass,
            std::function<void(ScriptPropertyPtr const&)> const& _cb);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void IterateSerializableFields(
            ScriptClassPtr const& _scriptClass,
            std::function<void(ScriptFieldPtr const&)> const& _cb);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoObject* DeserializeComponentFromDataBlock(
            EcsWorld* _world,
            DataBlock const& _dataBlock,
            CString _name,
            MonoType* _monoType);

        //////////////////////////////////////////
        MAZE_CORE_API void SerializeComponentToDataBlock(
            DataBlock& _dataBlock,
            CString _name,
            MonoObject* _componentInstance);

    } // namespace MonoHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMonoHelper_hpp_
//////////////////////////////////////////
