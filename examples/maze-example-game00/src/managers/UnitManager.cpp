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
#include "UnitManager.hpp"
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
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
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
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeCircleCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "Game.hpp"
#include "scenes/SceneFadePreloader.hpp"
#include "scenes/SceneMainMenu.hpp"
#include "player/Player.hpp"
#include "game/SpaceObject.hpp"
#include "game/UnitBounds.hpp"
#include "game/UnitMeshPartRenderer.hpp"
#include "game/Health.hpp"
#include "game/AI/SpaceObjectAI_SinMovement.hpp"
#include "game/SpaceObjectAvatar.hpp"
#include "game/ViewTriggerParticleSystems.hpp"
#include "game/ViewTriggers.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class UnitManager
    //
    //////////////////////////////////////////
    UnitManager* UnitManager::s_instance = nullptr;

    //////////////////////////////////////////
    UnitManager::UnitManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    UnitManager::~UnitManager()
    {
        s_instance = nullptr;

        if (Game::GetInstancePtr())
        {
            Game::GetInstancePtr()->eventCoreGameResourcesLoaded.unsubscribe(this, &UnitManager::notifyCoreGameResourcesLoaded);
        }
    }

    //////////////////////////////////////////
    void UnitManager::Initialize(UnitManagerPtr& _playerManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(UnitManager, _playerManager, init());
    }

    //////////////////////////////////////////
    bool UnitManager::init()
    {
        Game::GetInstancePtr()->eventCoreGameResourcesLoaded.subscribe(this, &UnitManager::notifyCoreGameResourcesLoaded);

        return true;
    }

    //////////////////////////////////////////
    void UnitManager::createSpaceObjectPrefabs()
    {
        ECSWorld* world = EntityManager::GetInstancePtr()->getLibraryWorldRaw();

        RenderSystem* renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        Vector<MaterialPtr> defaultExtraMaterials =
        {
            renderSystem->getMaterialManager()->getMaterial("DamageHit000.mzmaterial")
        };

        Vector<MaterialPtr> myPlayerExtraMaterials =
        {
            renderSystem->getMaterialManager()->getMaterial("DamageHit001.mzmaterial")
        };

        // PlayerShip000
        {
            EntityPtr entity = world->createEntity();
            entity->ensureComponent<Name>("Unit");

            Transform3DPtr transform = entity->createComponent<Transform3D>();


            SpaceObjectPtr spaceObject = entity->ensureComponent<SpaceObject>();
            spaceObject->setFireCooldown(0.135f);
            spaceObject->setDamage(60.0f);
            spaceObject->setWeaponPoint({ 0.95f, -0.275f });
            spaceObject->setProjectileAvatar(ProjectileAvatarType::Plasma00);
            spaceObject->setEngineForce(Vec2DF(80.0f, 60.0f) * 2.5f);


            // Avatar
            {
                EntityPtr avatarEntity = world->createEntity();
                avatarEntity->ensureComponent<Name>("Avatar");
                SpaceObjectAvatarPtr avatar = avatarEntity->ensureComponent<SpaceObjectAvatar>();
                Transform3DPtr const& avatarTransform = avatar->getTransform();

                avatarTransform->setParent(transform);
                spaceObject->setAvatar(avatar);

                // Mesh
                {
                    EntityPtr meshEntity = world->createEntity();
                    meshEntity->ensureComponent<Name>("Mesh");
                    Transform3DPtr meshTransform = meshEntity->createComponent<Transform3D>();

                    RenderMeshPtr spaceshipRenderMesh = renderSystem->getRenderMeshManager()->getRenderMesh("PlayerShip00.obj");
                    MaterialPtr spaceshipMaterial = renderSystem->getMaterialManager()->getMaterial("PlayerShip00.mzmaterial");
                    MaterialPtr spaceshipDeathMaterial = renderSystem->getMaterialManager()->getMaterial("PlayerShip00_Dissolve.mzmaterial");

                    MeshRendererPtr meshRenderer = meshEntity->createComponent<MeshRenderer>();
                    meshRenderer->setRenderMesh(spaceshipRenderMesh);
                    meshRenderer->setMaterial(spaceshipMaterial);

                    meshTransform->setParent(avatarTransform);
                    meshTransform->setLocalScale(0.0015f);
                    meshTransform->setLocalRotation(Quaternion(0.0f, Math::c_pi, 0.0f));

                    UnitMeshPartRendererPtr unitMeshPartRenderer = meshEntity->ensureComponent<UnitMeshPartRenderer>();
                    unitMeshPartRenderer->setExtraMaterials(myPlayerExtraMaterials);
                    U32 vaoCount = (U32)meshRenderer->getRenderMesh()->getVertexArrayObjects().size();
                    unitMeshPartRenderer->setDefaultMaterialsCount(vaoCount);
                    unitMeshPartRenderer->setMaterials(
                        spaceshipMaterial,
                        spaceshipDeathMaterial);
                }

                // FX
                {
                    EntityPtr exhaust00 = EntitySerializationManager::GetInstancePtr()->loadPrefab(
                        "Exhaust01.mzprefab",
                        world);
                    Transform3DPtr exhaustTransform = exhaust00->ensureComponent<Transform3D>();
                    exhaustTransform->setParent(avatarTransform);
                    exhaustTransform->setLocalPosition(0.5f, -0.175f, -0.95f);

                    EntityPtr exhaust01 = exhaust00->createCopy();
                    exhaustTransform = exhaust01->ensureComponent<Transform3D>();
                    exhaustTransform->setParent(avatarTransform);
                    exhaustTransform->setLocalPosition(-0.5f, -0.175f, -0.95f);
                }

                // Muzzle Flash
                {
                    EntityPtr muzzleFlash00 = EntitySerializationManager::GetInstancePtr()->loadPrefab(
                        "MuzzleFlash00.mzprefab",
                        world);
                    Transform3DPtr muzzleFlashTransform = muzzleFlash00->ensureComponent<Transform3D>();
                    muzzleFlashTransform->setParent(avatarTransform);
                    muzzleFlashTransform->setLocalPosition(0.0f, -0.275f, 1.185f);

                    ViewTriggerParticleSystemsPtr trigger = muzzleFlash00->ensureComponent<ViewTriggerParticleSystems>();
                    trigger->setTrigger(ViewTriggers::g_fire);
                }
            }


            BoxCollider2DPtr const& boxCollider = entity->createComponent<BoxCollider2D>();
            
            boxCollider->setSize(1.8f, 0.55f);

            spaceObject->getRigidbody2D()->setLinearDamping(16.0f);

            m_spaceshipPrefabs[SpaceObjectAvatarType::PlayerShip000] = entity;
        }
        
        // Drone000
        {
            EntityPtr entity = world->createEntity();
            entity->ensureComponent<Name>("Unit");

            Transform3DPtr transform = entity->createComponent<Transform3D>();


            SpaceObjectPtr spaceObject = entity->ensureComponent<SpaceObject>();
            spaceObject->getHealth()->setHealthMax(150.0f);
            spaceObject->getHealth()->setHealth(spaceObject->getHealth()->getHealthMax());
            spaceObject->setFireCooldown(0.135f);
            spaceObject->setDamage(20.0f);
            spaceObject->setWeaponPoint({ 0.375f, -0.11f });
            spaceObject->setProjectileAvatar(ProjectileAvatarType::YellowFlare00);
            spaceObject->setFireCooldown(2.5f);
            spaceObject->setEngineForce(Vec2DF(80.0f, 60.0f) * 0.15f);


            // Avatar
            {
                EntityPtr avatarEntity = world->createEntity();
                avatarEntity->ensureComponent<Name>("Avatar");
                SpaceObjectAvatarPtr avatar = avatarEntity->ensureComponent<SpaceObjectAvatar>();
                Transform3DPtr const& avatarTransform = avatar->getTransform();

                avatarTransform->setParent(transform);
                spaceObject->setAvatar(avatar);

                // Mesh
                {
                    EntityPtr meshEntity = world->createEntity();
                    meshEntity->ensureComponent<Name>("Mesh");
                    Transform3DPtr meshTransform = meshEntity->createComponent<Transform3D>();

                    RenderMeshPtr spaceshipRenderMesh = renderSystem->getRenderMeshManager()->getRenderMesh("Drone000.obj");
                    MaterialPtr spaceshipMaterial = renderSystem->getMaterialManager()->getMaterial("Drone000.mzmaterial");
                    MaterialPtr spaceshipDeathMaterial = renderSystem->getMaterialManager()->getMaterial("Drone000_Dissolve.mzmaterial");

                    MeshRendererPtr meshRenderer = meshEntity->createComponent<MeshRenderer>();
                    meshRenderer->setRenderMesh(spaceshipRenderMesh);
                    meshRenderer->setMaterial(spaceshipMaterial);

                    meshTransform->setParent(avatarTransform);
                    meshTransform->setLocalScale(0.6f);
                    meshTransform->setLocalRotation(Quaternion(0.0f, Math::c_pi, 0.0f));

                    UnitMeshPartRendererPtr unitMeshPartRenderer = meshEntity->ensureComponent<UnitMeshPartRenderer>();
                    unitMeshPartRenderer->setExtraMaterials(defaultExtraMaterials);
                    U32 vaoCount = (U32)meshRenderer->getRenderMesh()->getVertexArrayObjects().size();
                    unitMeshPartRenderer->setDefaultMaterialsCount(vaoCount);
                    unitMeshPartRenderer->setMaterials(
                        spaceshipMaterial,
                        spaceshipDeathMaterial);
                }

                // Muzzle Flash
                {
                    EntityPtr muzzleFlash = EntitySerializationManager::GetInstancePtr()->loadPrefab(
                        "MuzzleFlash01.mzprefab",
                        world);
                    Transform3DPtr muzzleFlashTransform = muzzleFlash->ensureComponent<Transform3D>();
                    muzzleFlashTransform->setParent(avatarTransform);
                    muzzleFlashTransform->setLocalPosition(0.3f, -0.1f, 0.63f);

                    ViewTriggerParticleSystemsPtr trigger = muzzleFlash->ensureComponent<ViewTriggerParticleSystems>();
                    trigger->setTrigger(ViewTriggers::g_fire);
                }
                {
                    EntityPtr muzzleFlash = EntitySerializationManager::GetInstancePtr()->loadPrefab(
                        "MuzzleFlash01.mzprefab",
                        world);
                    Transform3DPtr muzzleFlashTransform = muzzleFlash->ensureComponent<Transform3D>();
                    muzzleFlashTransform->setParent(avatarTransform);
                    muzzleFlashTransform->setLocalPosition(-0.3f, -0.1f, 0.63f);

                    ViewTriggerParticleSystemsPtr trigger = muzzleFlash->ensureComponent<ViewTriggerParticleSystems>();
                    trigger->setTrigger(ViewTriggers::g_fire);
                }
            }


            BoxCollider2DPtr const& boxCollider = entity->createComponent<BoxCollider2D>();
            boxCollider->setSize(0.75f, 0.75f);

            spaceObject->getRigidbody2D()->setLinearDamping(6.0f);

            entity->ensureComponent<SpaceObjectAI_SinMovement>();

            m_spaceshipPrefabs[SpaceObjectAvatarType::Drone000] = entity;
        }

        // Drone001
        {
            EntityPtr entity = world->createEntity();
            entity->ensureComponent<Name>("Unit");

            Transform3DPtr transform = entity->createComponent<Transform3D>();


            SpaceObjectPtr spaceObject = entity->ensureComponent<SpaceObject>();
            spaceObject->getHealth()->setHealthMax(110.0f);
            spaceObject->getHealth()->setHealth(spaceObject->getHealth()->getHealthMax());
            spaceObject->setFireCooldown(0.135f);
            spaceObject->setDamage(20.0f);
            spaceObject->setWeaponPoint({ 0.175f, -0.0f });
            spaceObject->setProjectileAvatar(ProjectileAvatarType::None);
            spaceObject->setEngineForce(Vec2DF(80.0f, 60.0f) * 0.15f);


            // Avatar
            {
                EntityPtr avatarEntity = world->createEntity();
                avatarEntity->ensureComponent<Name>("Avatar");
                SpaceObjectAvatarPtr avatar = avatarEntity->ensureComponent<SpaceObjectAvatar>();
                Transform3DPtr const& avatarTransform = avatar->getTransform();

                avatarTransform->setParent(transform);
                spaceObject->setAvatar(avatar);


                // Mesh
                {
                    EntityPtr meshEntity = world->createEntity();
                    meshEntity->ensureComponent<Name>("Mesh");
                    Transform3DPtr meshTransform = meshEntity->createComponent<Transform3D>();

                    RenderMeshPtr spaceshipRenderMesh = renderSystem->getRenderMeshManager()->getRenderMesh("Drone001.obj");
                    MaterialPtr spaceshipMaterial = renderSystem->getMaterialManager()->getMaterial("Drone001.mzmaterial");
                    MaterialPtr spaceshipDeathMaterial = renderSystem->getMaterialManager()->getMaterial("Drone001_Dissolve.mzmaterial");

                    MeshRendererPtr meshRenderer = meshEntity->createComponent<MeshRenderer>();
                    meshRenderer->setRenderMesh(spaceshipRenderMesh);
                    meshRenderer->setMaterial(spaceshipMaterial);

                    meshTransform->setParent(avatarTransform);
                    meshTransform->setLocalScale(0.175f);
                    meshTransform->setLocalRotation(Quaternion(0.0f, Math::c_pi, 0.0f));

                    UnitMeshPartRendererPtr unitMeshPartRenderer = meshEntity->ensureComponent<UnitMeshPartRenderer>();
                    unitMeshPartRenderer->setExtraMaterials(defaultExtraMaterials);
                    U32 vaoCount = (U32)meshRenderer->getRenderMesh()->getVertexArrayObjects().size();
                    unitMeshPartRenderer->setDefaultMaterialsCount(vaoCount);
                    unitMeshPartRenderer->setMaterials(
                        spaceshipMaterial,
                        spaceshipDeathMaterial);

                    meshEntity->ensureComponent<Rotor3D>(Vec3DF::c_unitZ, Math::c_pi * 0.75f);
                }
            }


            BoxCollider2DPtr const& boxCollider = entity->createComponent<BoxCollider2D>();
            boxCollider->setSize(0.75f, 0.75f);

            spaceObject->getRigidbody2D()->setLinearDamping(6.0f);

            m_spaceshipPrefabs[SpaceObjectAvatarType::Drone001] = entity;
        }

        // Mine000
        {
            EntityPtr entity = world->createEntity();
            entity->ensureComponent<Name>("Unit");

            Transform3DPtr transform = entity->createComponent<Transform3D>();


            SpaceObjectPtr spaceObject = entity->ensureComponent<SpaceObject>();
            spaceObject->getHealth()->setHealthMax(75.0f);
            spaceObject->getHealth()->setHealth(spaceObject->getHealth()->getHealthMax());
            spaceObject->setFireCooldown(0.135f);
            spaceObject->setDamage(20.0f);
            spaceObject->setWeaponPoint({ 0.175f, -0.0f });
            spaceObject->setProjectileAvatar(ProjectileAvatarType::None);
            spaceObject->setEngineForce(Vec2DF(80.0f, 60.0f) * 0.15f);


            // Avatar
            {
                EntityPtr avatarEntity = world->createEntity();
                avatarEntity->ensureComponent<Name>("Avatar");
                SpaceObjectAvatarPtr avatar = avatarEntity->ensureComponent<SpaceObjectAvatar>();
                Transform3DPtr const& avatarTransform = avatar->getTransform();

                avatarTransform->setParent(transform);
                spaceObject->setAvatar(avatar);


                // Mesh
                {
                    EntityPtr meshEntity = world->createEntity();
                    meshEntity->ensureComponent<Name>("Mesh");
                    Transform3DPtr meshTransform = meshEntity->createComponent<Transform3D>();

                    RenderMeshPtr spaceshipRenderMesh = renderSystem->getRenderMeshManager()->getRenderMesh("Mine000.obj");
                    MaterialPtr spaceshipMaterial = renderSystem->getMaterialManager()->getMaterial("Mine000.mzmaterial");
                    MaterialPtr spaceshipDeathMaterial = renderSystem->getMaterialManager()->getMaterial("Mine000_Dissolve.mzmaterial");

                    MeshRendererPtr meshRenderer = meshEntity->createComponent<MeshRenderer>();
                    meshRenderer->setRenderMesh(spaceshipRenderMesh);
                    meshRenderer->setMaterial(spaceshipMaterial);

                    meshTransform->setParent(avatarTransform);
                    meshTransform->setLocalScale(0.006f);
                    meshTransform->setLocalRotation(Quaternion(0.0f, Math::c_pi, 0.0f));

                    UnitMeshPartRendererPtr unitMeshPartRenderer = meshEntity->ensureComponent<UnitMeshPartRenderer>();
                    unitMeshPartRenderer->setExtraMaterials(defaultExtraMaterials);
                    U32 vaoCount = (U32)meshRenderer->getRenderMesh()->getVertexArrayObjects().size();
                    unitMeshPartRenderer->setDefaultMaterialsCount(vaoCount);
                    unitMeshPartRenderer->setMaterials(
                        spaceshipMaterial,
                        spaceshipDeathMaterial);

                    meshEntity->ensureComponent<Rotor3D>(Vec3DF(0.0f, 1.0f, 1.0f).normalizedCopy(), Math::c_pi * 0.75f);
                }
            }


            BoxCollider2DPtr const& boxCollider = entity->createComponent<BoxCollider2D>();
            boxCollider->setSize(0.75f, 0.75f);

            spaceObject->getRigidbody2D()->setLinearDamping(6.0f);

            m_spaceshipPrefabs[SpaceObjectAvatarType::Mine000] = entity;
        }

        // Mine001
        {
            EntityPtr entity = world->createEntity();
            entity->ensureComponent<Name>("Unit");

            Transform3DPtr transform = entity->createComponent<Transform3D>();


            SpaceObjectPtr spaceObject = entity->ensureComponent<SpaceObject>();
            spaceObject->getHealth()->setHealthMax(75.0f);
            spaceObject->getHealth()->setHealth(spaceObject->getHealth()->getHealthMax());
            spaceObject->setFireCooldown(0.135f);
            spaceObject->setDamage(20.0f);
            spaceObject->setWeaponPoint({ 0.175f, -0.0f });
            spaceObject->setProjectileAvatar(ProjectileAvatarType::None);
            spaceObject->setEngineForce(Vec2DF(80.0f, 60.0f) * 0.15f);


            // Avatar
            {
                EntityPtr avatarEntity = world->createEntity();
                avatarEntity->ensureComponent<Name>("Avatar");
                SpaceObjectAvatarPtr avatar = avatarEntity->ensureComponent<SpaceObjectAvatar>();
                Transform3DPtr const& avatarTransform = avatar->getTransform();

                avatarTransform->setParent(transform);
                spaceObject->setAvatar(avatar);


                // Mesh
                {
                    EntityPtr meshEntity = world->createEntity();
                    meshEntity->ensureComponent<Name>("Mesh");
                    Transform3DPtr meshTransform = meshEntity->createComponent<Transform3D>();

                    RenderMeshPtr spaceshipRenderMesh = renderSystem->getRenderMeshManager()->getRenderMesh("Mine001.obj");
                    MaterialPtr spaceshipMaterial = renderSystem->getMaterialManager()->getMaterial("Mine001.mzmaterial");
                    MaterialPtr spaceshipDeathMaterial = renderSystem->getMaterialManager()->getMaterial("Mine001_Dissolve.mzmaterial");

                    MeshRendererPtr meshRenderer = meshEntity->createComponent<MeshRenderer>();
                    meshRenderer->setRenderMesh(spaceshipRenderMesh);
                    meshRenderer->setMaterial(spaceshipMaterial);

                    meshTransform->setParent(transform);
                    meshTransform->setLocalScale(0.01f);
                    meshTransform->setLocalRotation(Quaternion(0.0f, Math::c_pi, 0.0f));

                    UnitMeshPartRendererPtr unitMeshPartRenderer = meshEntity->ensureComponent<UnitMeshPartRenderer>();
                    unitMeshPartRenderer->setExtraMaterials(defaultExtraMaterials);
                    U32 vaoCount = (U32)meshRenderer->getRenderMesh()->getVertexArrayObjects().size();
                    unitMeshPartRenderer->setDefaultMaterialsCount(vaoCount);
                    unitMeshPartRenderer->setMaterials(
                        spaceshipMaterial,
                        spaceshipDeathMaterial);

                    meshEntity->ensureComponent<Rotor3D>(Vec3DF(0.0f, 1.0f, 1.0f).normalizedCopy(), Math::c_pi * 0.75f);
                }
            }


            BoxCollider2DPtr const& boxCollider = entity->createComponent<BoxCollider2D>();
            boxCollider->setSize(0.75f, 0.75f);

            spaceObject->getRigidbody2D()->setLinearDamping(6.0f);

            m_spaceshipPrefabs[SpaceObjectAvatarType::Mine001] = entity;
        }
        
    }

    //////////////////////////////////////////
    void UnitManager::notifyCoreGameResourcesLoaded()
    {
        createSpaceObjectPrefabs();
    }

    /////////////////////////////////////////
    EntityPtr UnitManager::createSpaceObject(SpaceObjectAvatarType _type, ECSScene* _scene)
    {
        MAZE_ERROR_RETURN_VALUE_IF(_type == SpaceObjectAvatarType::None, EntityPtr(), "Null avatar type");
        EntityPtr const& prefab = getSpaceObjectPrefab(_type);

        EntityPtr spaceObject = prefab->createCopy(
            EntityManager::GetInstancePtr()->getDefaultWorldRaw());
        SpaceObject* spaceship = spaceObject->getComponentRaw<SpaceObject>();

        Vector<Entity*> entities = spaceship->getTransform()->getAllEntitiesRaw();
        for (Entity* entity : entities)
        {
            entity->setECSScene(_scene);
        }
  

        return spaceObject;
    }


} // namespace Maze
//////////////////////////////////////////