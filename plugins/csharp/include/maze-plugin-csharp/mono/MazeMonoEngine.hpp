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
#if (!defined(_MazeMonoEngine_hpp_))
#define _MazeMonoEngine_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-plugin-csharp/mono/MazeScriptClass.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MonoEngine
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API MonoEngine
    {
    public:
        
        //////////////////////////////////////////
        static bool Initialize();

        //////////////////////////////////////////
        static bool Shutdown();

        

        //////////////////////////////////////////
        static MonoDomain* GetMonoDomain();

        //////////////////////////////////////////
        static MonoAssembly* GetCoreAssembly();

        //////////////////////////////////////////
        static MonoImage* GetCoreAssemblyImage();

        //////////////////////////////////////////
        static ScriptClassPtr const& GetMonoBehaviourClass();

        //////////////////////////////////////////
        static ScriptClassPtr const& GetComponentClass();

        //////////////////////////////////////////
        static ScriptPropertyPtr const& GetNativeComponentPtrProperty();

        //////////////////////////////////////////
        static ScriptClassPtr const& GetNativeComponentClass();


        //////////////////////////////////////////
        static ScriptClassPtr const& GetMonoBehaviourSubClass(HashedCString _name);

        //////////////////////////////////////////
        static ScriptClassPtr const& GetMonoBehaviourSubClass(ComponentId _id);

        //////////////////////////////////////////
        static StringKeyMap<ScriptClassPtr> const& GetMonoBehaviourSubClasses();


        //////////////////////////////////////////
        static ComponentId GetComponentIdByMonoType(MonoType* _monoType);


        //////////////////////////////////////////
        static MonoObject* InstantiateClass(MonoClass* _monoClass);


        //////////////////////////////////////////
        static MonoAssembly* LoadAppAssembly(HashedCString _csharpFile);


    protected:

        //////////////////////////////////////////
        static MonoAssembly* LoadMonoAssembly(ByteBuffer const& _csharpFile);

        //////////////////////////////////////////
        static MonoAssembly* LoadMonoAssembly(AssetFilePtr const& _csharpFile);

        //////////////////////////////////////////
        static MonoAssembly* LoadMonoAssembly(HashedCString _csharpFile);


        //////////////////////////////////////////
        static MonoAssembly* LoadCoreAssembly(HashedCString _csharpFile);

    private:

        //////////////////////////////////////////
        static bool InitializeMono();

        //////////////////////////////////////////
        static void ShutdownMono();
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMonoEngine_hpp_
//////////////////////////////////////////
