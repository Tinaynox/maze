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
#include "SpecialEffectManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3D.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderPNG.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "Game.hpp"
#include "scenes/SceneFadePreloader.hpp"
#include "scenes/SceneMainMenu.hpp"
#include "player/Player.hpp"
#include "game/SpecialEffect.hpp"
#include "game/UnitBounds.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SpecialEffectManager
    //
    //////////////////////////////////////////
    SpecialEffectManager* SpecialEffectManager::s_instance = nullptr;

    //////////////////////////////////////////
    SpecialEffectManager::SpecialEffectManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    SpecialEffectManager::~SpecialEffectManager()
    {
        s_instance = nullptr;

        if (Game::GetInstancePtr())
        {
            Game::GetInstancePtr()->eventCoreGameResourcesLoaded.unsubscribe(this, &SpecialEffectManager::notifyCoreGameResourcesLoaded);
        }
    }

    //////////////////////////////////////////
    void SpecialEffectManager::Initialize(SpecialEffectManagerPtr& _playerManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(SpecialEffectManager, _playerManager, init());
    }

    //////////////////////////////////////////
    bool SpecialEffectManager::init()
    {
        Game::GetInstancePtr()->eventCoreGameResourcesLoaded.subscribe(this, &SpecialEffectManager::notifyCoreGameResourcesLoaded);

        return true;
    }

    //////////////////////////////////////////
    void SpecialEffectManager::createSpecialEffectPrefabs()
    {
        

        auto loadPrefabFunc = 
            [this](String const& _prefabName, SpecialEffectType _type)
            {
                ECSWorld* world = EntityManager::GetInstancePtr()->getLibraryWorldRaw();

                EntityPtr entity = EntitySerializationManager::GetInstancePtr()->loadPrefab(
                    _prefabName,
                    world);

                SpecialEffectPtr effect = entity->ensureComponent<SpecialEffect>();

                m_projectilePrefabs[_type] = entity;
            };

        loadPrefabFunc("Explosion00.mzprefab", SpecialEffectType::Explosion00);
        loadPrefabFunc("Explosion01.mzprefab", SpecialEffectType::Explosion01);
        loadPrefabFunc("UnitExplosion00.mzprefab", SpecialEffectType::UnitExplosion00);
        loadPrefabFunc("UnitExplosion01.mzprefab", SpecialEffectType::UnitExplosion01);
    }

    //////////////////////////////////////////
    void SpecialEffectManager::notifyCoreGameResourcesLoaded()
    {
        createSpecialEffectPrefabs();
    }

    /////////////////////////////////////////
    EntityPtr SpecialEffectManager::createSpecialEffect(SpecialEffectType _type, ECSScene* _scene)
    {
        EntityPtr const& prefab = getSpecialEffectPrefab(_type);

        EntityPtr projectileObject = prefab->createCopy(
            EntityManager::GetInstancePtr()->getDefaultWorldRaw());

        Vector<Entity*> entities = projectileObject->getComponent<Transform3D>()->getAllEntitiesRaw();
        for (Entity* entity : entities)
        {
            entity->setECSScene(_scene);
        }

        return projectileObject;
    }


} // namespace Maze
//////////////////////////////////////////
