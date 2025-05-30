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
#if (!defined(_MazeCSharpService_hpp_))
#define _MazeCSharpService_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-plugin-csharp/mono/MazeScriptInstance.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MonoSerializationManager);
    MAZE_USING_SHARED_PTR(ScriptableObjectManager);


    //////////////////////////////////////////
    // Class CSharpService
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API CSharpService
        : public MultiDelegateCallbackReceiver
    {
    public:
        
        //////////////////////////////////////////
        CSharpService();

        //////////////////////////////////////////
        ~CSharpService();
        

        //////////////////////////////////////////
        void initialize();

        //////////////////////////////////////////
        void shutdown();


        //////////////////////////////////////////
        MonoDomain* getMonoDomain();


        //////////////////////////////////////////
        MonoAssembly* loadEditorAssembly(HashedCString _csharpFile);

        //////////////////////////////////////////
        MonoAssembly* loadAppAssembly(HashedCString _csharpFile);


        //////////////////////////////////////////
        static CSharpService& GetInstance();

        //////////////////////////////////////////
        static inline CSharpService* GetInstancePtr() { return &GetInstance(); }


    protected:
        MonoSerializationManagerPtr m_monoSerializationManager;
        ScriptableObjectManagerPtr m_scriptableObjectManager;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCSharpService_hpp_
//////////////////////////////////////////
