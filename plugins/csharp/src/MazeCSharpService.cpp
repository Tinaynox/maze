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
#include "maze-plugin-csharp/MazeCSharpService.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class CSharpService
    //
    //////////////////////////////////////////
    CSharpService& CSharpService::GetInstance()
    {
        static CSharpService s_service;
        return s_service;
    }
     
    //////////////////////////////////////////
    CSharpService::CSharpService()
    {
        
    }

    //////////////////////////////////////////
    CSharpService::~CSharpService()
    {
        
    }

    //////////////////////////////////////////
    void CSharpService::initialize()
    {
        MonoEngine::Initialize();

        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<MonoBehaviour>("Mono");
    }

    //////////////////////////////////////////
    void CSharpService::shutdown()
    {
        MonoEngine::Shutdown();
    }

    //////////////////////////////////////////
    MonoDomain* CSharpService::getMonoDomain()
    {
        return MonoEngine::GetMonoDomain();
    }

    //////////////////////////////////////////
    MonoAssembly* CSharpService::loadMonoAssembly(HashedCString _csharpFile)
    {
        return MonoEngine::LoadAppAssembly(_csharpFile);
    }

} // namespace Maze
//////////////////////////////////////////