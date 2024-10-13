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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/MazeEcsAssetScene.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EcsAssetScene
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EcsAssetScene, EcsRenderScene);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(EcsAssetScene);

    //////////////////////////////////////////
    EcsAssetScene::EcsAssetScene()
    {

    }

    //////////////////////////////////////////
    EcsAssetScene::~EcsAssetScene()
    {
    }

    //////////////////////////////////////////
    EcsAssetScenePtr EcsAssetScene::Create(
        RenderTargetPtr const& _renderTarget,
        EcsWorld* _world)
    {
        EcsAssetScenePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EcsAssetScene, object, init(_renderTarget, _world));
        return object;
    }

    //////////////////////////////////////////
    bool EcsAssetScene::init(
        RenderTargetPtr const& _renderTarget,
        EcsWorld* _world)
    {
        m_world = _world;

        if (!EcsRenderScene::init(_renderTarget))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void EcsAssetScene::serializeSceneCommonInfo(DataBlock& _info)
    {
        EcsRenderScene::serializeSceneCommonInfo(_info);

        _info.setString(MAZE_HCS("name"), m_name);
    }

    //////////////////////////////////////////
    void EcsAssetScene::deserializeSceneCommonInfo(DataBlock const& _info)
    {
        EcsRenderScene::deserializeSceneCommonInfo(_info);

        HashedCString name = _info.getHashedCString(MAZE_HCS("name"));
        if (!name.empty())
            setName(HashedString(name));
    }

    
} // namespace Maze
//////////////////////////////////////////
