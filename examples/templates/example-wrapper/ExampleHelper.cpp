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
#include "ExampleHelper.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazePlatformHelper.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Plugin.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/managers/MazeDynLibManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-core/ecs/systems/MazeBounds2DSystem.hpp"
#include "maze-core/ecs/systems/MazeSizePolicy2DSystem.hpp"
#include "maze-core/ecs/systems/MazeTransformUtilsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderPreparationSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-ui/ecs/systems/MazeInputSystem2D.hpp"
#include "maze-ui/ecs/systems/MazeUITweenTransitionSystem.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
#include "maze-physics2d/ecs/systems/MazePhysicsControlSystem2D.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace ExampleHelper
    {
        //////////////////////////////////////////
        EntityPtr CreateBox(
            ECSRenderScene* _scene,
            Vec3DF const& _position,
            Vec3DF const& _scale,
            MaterialPtr const& _material,
            Vec2DF textureScale)
        {
            EntityPtr objectEntity = _scene->createEntity();
            Transform3DPtr transform = objectEntity->createComponent<Transform3D>();
            MeshRendererPtr meshRenderer = objectEntity->createComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(RenderMeshManager::GetCurrentInstancePtr()->getDefaultCubeMesh());

            if (_material)
            {
                MaterialPtr material = _material->createCopy();
                material->setUniform("u_baseMapST", Vec4DF(textureScale, 0.0f, 0.0f));
                meshRenderer->setMaterial(material);
            }
            else
            {
                meshRenderer->setMaterial(MaterialManager::GetCurrentInstance()->getBuiltinMaterial(BuiltinMaterialType::SpecularDS));
            }

            transform->setLocalPosition(_position);
            transform->setLocalScale(_scale);

            return objectEntity;
        }

        //////////////////////////////////////////
        void BuildSimpleLevel(
            ECSRenderScene* _scene,
            Vec2DF const& _levelSize,
            SimpleLevelConfig config)
        {
            F32 floorThickness = 0.1f;

            CreateBox(
                _scene,
                Vec3DF(0.0f, -floorThickness * 0.5f, 0.0f),
                Vec3DF(_levelSize.x, floorThickness, _levelSize.y),
                config.floorMaterial,
                _levelSize / config.floorTextureScale);

            F32 wallsHeight = 0.6f;
            F32 wallsWidth = 0.2f;

            // +X Wall
            Vec3DF const wallPositiveXSize = Vec3DF(wallsWidth, wallsHeight + floorThickness, _levelSize.y);
            CreateBox(
                _scene,
                Vec3DF(_levelSize.x * 0.5f + wallsWidth * 0.5f, (wallsHeight - floorThickness) * 0.5f, 0.0f),
                wallPositiveXSize,
                config.wallMaterial,
                Vec2DF(wallPositiveXSize.z, wallPositiveXSize.y) / config.wallTextureScale);
            // -X Wall
            Vec3DF const wallNegativeXSize = Vec3DF(wallsWidth, wallsHeight + floorThickness, _levelSize.y);
            CreateBox(
                _scene,
                Vec3DF(-_levelSize.x * 0.5f - wallsWidth * 0.5f, (wallsHeight - floorThickness) * 0.5f, 0.0f),
                wallNegativeXSize,
                config.wallMaterial,
                Vec2DF(wallPositiveXSize.z, wallPositiveXSize.y) / config.wallTextureScale);

            // +Z Wall
            Vec3DF const wallPositiveZSize = Vec3DF(_levelSize.x + wallsWidth * 2.0f, wallsHeight + floorThickness, wallsWidth);
            CreateBox(
                _scene,
                Vec3DF(0.0f, (wallsHeight - floorThickness) * 0.5f, _levelSize.y * 0.5f + wallsWidth * 0.5f),
                wallPositiveZSize,
                config.wallMaterial,
                Vec2DF(wallPositiveZSize.x, wallPositiveXSize.y) / config.wallTextureScale);
            // -Z Wall
            Vec3DF const wallNegativeZSize = Vec3DF(_levelSize.x + wallsWidth * 2.0f, wallsHeight + floorThickness, wallsWidth);
            CreateBox(
                _scene,
                Vec3DF(0.0f, (wallsHeight - floorThickness) * 0.5f, -_levelSize.y * 0.5f - wallsWidth * 0.5f),
                wallNegativeZSize,
                config.wallMaterial,
                Vec2DF(wallNegativeZSize.x, wallPositiveXSize.y) / config.wallTextureScale);
        }


    } // namespace ExampleHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
