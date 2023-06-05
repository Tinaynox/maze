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
#include "ProjectileManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
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
#include "game/Projectile.hpp"
#include "game/UnitBounds.hpp"
#include "game/TrailProjectile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ProjectileManager
    //
    //////////////////////////////////////////
    ProjectileManager* ProjectileManager::s_instance = nullptr;

    //////////////////////////////////////////
    ProjectileManager::ProjectileManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    ProjectileManager::~ProjectileManager()
    {
        s_instance = nullptr;

        if (Game::GetInstancePtr())
        {
            Game::GetInstancePtr()->eventCoreGameResourcesLoaded.unsubscribe(this, &ProjectileManager::notifyCoreGameResourcesLoaded);
        }
    }

    //////////////////////////////////////////
    void ProjectileManager::Initialize(ProjectileManagerPtr& _playerManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(ProjectileManager, _playerManager, init());
    }

    //////////////////////////////////////////
    bool ProjectileManager::init()
    {
        Game::GetInstancePtr()->eventCoreGameResourcesLoaded.subscribe(this, &ProjectileManager::notifyCoreGameResourcesLoaded);

        return true;
    }

    //////////////////////////////////////////
    void ProjectileManager::createProjectilePrefabs()
    {
        ECSWorld* world = EntityManager::GetInstancePtr()->getLibraryWorldRaw();

        {
            EntityPtr entity = world->createEntity();
            entity->ensureComponent<Name>("Projectile");
            
            MeshRendererPtr meshRenderer = entity->createComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getRenderMeshManager()->getDefaultQuadMesh());
            meshRenderer->setMaterial("Projectile00.mzmaterial");
            
            ProjectilePtr projectile = entity->ensureComponent<Projectile>();
            projectile->setSpeed(15.0f);
            
            projectile->getTransform()->setLocalScale(0.7f);
            
            m_projectilePrefabs[ProjectileAvatarType::RedFlare00] = entity;
        }

        {
            EntityPtr entity = world->createEntity();
            entity->ensureComponent<Name>("Projectile");

            MeshRendererPtr meshRenderer = entity->createComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getRenderMeshManager()->getDefaultQuadMesh());
            meshRenderer->setMaterial("Projectile00B.mzmaterial");

            ProjectilePtr projectile = entity->ensureComponent<Projectile>();
            projectile->setSpeed(5.0f);
            projectile->getTransform()->setLocalScale(0.35f);

            projectile->setDestroyEffect(SpecialEffectType::Explosion01);

            m_projectilePrefabs[ProjectileAvatarType::YellowFlare00] = entity;
        }

        {
            EntityPtr entity = world->createEntity();
            entity->ensureComponent<Name>("Projectile");

            TrailRenderer3DPtr trailRenderer = entity->ensureComponent<TrailRenderer3D>();
            trailRenderer->setMaterial("Projectile01.mzmaterial");
            trailRenderer->setMinVertexDistance(0.35f);        
            trailRenderer->setWidth(0.2f);
            trailRenderer->setTime(0.085f);

            TrailProjectilePtr trailProjectile = entity->ensureComponent<TrailProjectile>();

            ColorGradient gradient;
            gradient.addKey(0.0f, { 0.0f, 0.0f, 0.0f, 1.0f });
            gradient.addKey(0.1f, { 1.5f, 1.5f, 2.0f, 1.0f });
            gradient.addKey(1.0f, { 1.3f, 0.6f, 2.75f, 1.0f });

            trailProjectile->setGradient(gradient);

            ProjectilePtr projectile = entity->ensureComponent<Projectile>();
            // projectile->getUnitBounds()->setSize(0.25f, 0.25f);
            projectile->getTransform()->setLocalScale(0.7f);

            projectile->setDestroyEffect(SpecialEffectType::Explosion00);

            m_projectilePrefabs[ProjectileAvatarType::Plasma00] = entity;
        }
    }

    //////////////////////////////////////////
    void ProjectileManager::notifyCoreGameResourcesLoaded()
    {
        createProjectilePrefabs();
    }

    /////////////////////////////////////////
    EntityPtr ProjectileManager::createProjectile(ProjectileAvatarType _type, ECSScene* _scene)
    {
        EntityPtr const& prefab = getProjectilePrefab(_type);

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
