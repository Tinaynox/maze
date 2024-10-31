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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
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
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-editor-tools/ecs/components/MazeInspectorController.hpp"
#include "maze-editor-tools/ecs/components/MazeAssetsController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace EditorToolsHelper
    {

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API Set<ComponentId> CollectIntersectingComponentIds(Set<EntityPtr> const& _entities)
        {
            Set<ComponentId> result;

            if (_entities.empty())
                return result;

            Set<EntityPtr>::iterator it = _entities.begin();
            Set<EntityPtr>::iterator end = _entities.end();

            EntityPtr const& firstEntity = *it++;
            for (auto const& componentData : firstEntity->getComponents())
                result.emplace(componentData.first);

            for (; it != end; ++it)
            {
                EntityPtr const& entity = *it;

                for (auto const& componentData : entity->getComponents())
                    if (result.count(componentData.first) == 0)
                        result.erase(componentData.first);
            }

            return result;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API UnorderedMap<ComponentId, MetaClass*> CollectIntersectingComponentMetaClasses(Set<EntityPtr> const& _entities)
        {
            UnorderedMap<ComponentId, MetaClass*> result;

            if (_entities.empty())
                return result;

            Set<EntityPtr>::iterator it = _entities.begin();
            Set<EntityPtr>::iterator end = _entities.end();

            EntityPtr const& firstEntity = *it++;
            for (auto const& componentData : firstEntity->getComponents())
            {
                result.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(componentData.first),
                    std::forward_as_tuple(componentData.second->getMetaClass()));
            }

            for (; it != end; ++it)
            {
                EntityPtr const& entity = *it;

                for (auto const& componentData : entity->getComponents())
                {
                    if (result.count(componentData.second->getComponentId()) == 0)
                        result.erase(componentData.second->getComponentId());
                }
            }

            return result;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API String BuildPropertyName(CString _propertyName, CString _label)
        {
            static std::function<bool(char)> isCapitalChar =
                [](char _c) -> bool
                {
                    if (    (_c >= '0' && _c <= '9')
                        ||  (_c >= 'A' && _c <= 'Z'))
                    {
                        return true;
                    }

                    return false;
                };

            String name;
            if (_label)
            {
                name = _label;
            }
            else
            {
                name = _propertyName;
                if (!name.empty())
                {
                    for (String::iterator it = name.begin(); it != name.end();)
                    {                    
                        if (it != name.begin())
                        {
                            char prevChar = *(it - 1);
                            char currChar = (*it);

                            if (isCapitalChar(currChar) && !isCapitalChar(prevChar))
                            {
                                it = name.insert(it, ' ') + 2;
                                continue;
                            }
                        }

                        ++it;
                    }

                    name[0] = toupper(name[0]);
                }
            }

            return name;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API String BuildComponentName(CString _componentName, CString _label)
        {
            return BuildPropertyName(_componentName, _label);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API EntityPtr CreateEntity(CString _entityName, EcsScene* _scene)
        {
            EcsScene* mainScene = _scene ? _scene : SceneManager::GetInstancePtr()->getMainScene().get();
            MAZE_ERROR_RETURN_VALUE_IF(!mainScene, nullptr, "MainScene is null!");

            EntityPtr entity = mainScene->createEntity(_entityName);
            return entity;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API EntityPtr CreateEntity2D(CString _entityName, EcsScene* _scene)
        {
            EcsScene* mainScene = _scene ? _scene : SceneManager::GetInstancePtr()->getMainScene().get();
            MAZE_ERROR_RETURN_VALUE_IF(!mainScene, nullptr, "MainScene is null!");

            Transform2DPtr transform = SpriteHelper::CreateTransform2D(
                Vec2F(100.0f, 100.0f),
                Vec2F::c_zero,
                nullptr,
                mainScene);

            if (_entityName)
                EcsHelper::SetName(transform->getEntityRaw(), _entityName);

            return transform->getEntity();
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API EntityPtr CreateEntity3D(CString _entityName, EcsScene* _scene)
        {
            EntityPtr entity = CreateEntity(_entityName, _scene);

            entity->ensureComponent<Transform3D>();

            return entity;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API Path BuildNewAssetFileName(Path const& _fullPath)
        {
            if (!AssetManager::GetInstancePtr()->getAssetFileByFullPath(_fullPath))
                return _fullPath;

            S32 i = 1;

            Path dir = FileHelper::GetParentDirectoryInPath(_fullPath);
            Path fileName = FileHelper::GetFileNameInPath(_fullPath);
            Path fileNameWithoutExtension = FileHelper::GetFileNameWithoutExtension(fileName);
            Path fileExtension = FileHelper::GetFileExtension(fileName);

            Path newPath;
            do
            {
                newPath = dir + "/" + fileNameWithoutExtension + " " + StringHelper::ToString(i);
                if (!fileExtension.empty())
                    newPath += Path(".") + fileExtension;
                ++i;
            }
            while (AssetManager::GetInstancePtr()->getAssetFileByFullPath(newPath));

            return newPath;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API String GetNameWithoutExtension(String const& _name)
        {
            Size position = _name.find_last_of('.');
            if (position == Path::StringType::npos)
                return _name;

            return _name.substr(0, position);
        }

    } // namespace EditorToolsHelper
    //////////////////////////////////////////
    

} // namespace Maze
//////////////////////////////////////////
