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
#include "maze-editor-tools/helpers/MazeEditorActionHelper.hpp"
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
#include "maze-editor-tools/editor-actions/MazeEditorActionSelectEntities.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionSelectObjects.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionMetaInstancesSetString.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionCustom.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionTransform3DTranslate.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionTransform3DRotate.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionTransform3DScale.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace EditorActionHelper
    {
        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SetValueString(
            String const& _value,
            MetaProperty* _metaProperty,
            Set<MetaInstance> const& _metaInstances)
        {
            if (EditorActionManager::GetInstancePtr())
            {
                EditorActionManager::GetInstancePtr()->applyAction(
                    EditorActionMetaInstancesSetString::Create(
                        _metaProperty,
                        Vector<MetaInstance>(_metaInstances.begin(), _metaInstances.end()),
                        _value));
            }
            else
            {
                MetaClassHelper::SetValueString(_value, _metaProperty, _metaInstances);
            }
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SelectEntities(
            Vector<EntityPtr> const& _entities,
            bool _clearSelectionList)
        {
            if (EditorActionManager::GetInstancePtr())
            {
                EditorActionManager::GetInstancePtr()->applyAction(
                    EditorActionSelectEntities::Create(
                        true,
                        _entities,
                        _clearSelectionList));
            }
            else
            {
                SelectionManager::GetInstancePtr()->selectObjects(
                    _entities, _clearSelectionList);
            }
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SelectEntities(
            EntityPtr const& _entity,
            bool _clearSelectionList)
        {
            SelectEntities(Vector<EntityPtr>{ _entity }, _clearSelectionList);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void UnselectEntities(
            Vector<EntityPtr> const& _entities)
        {
            if (EditorActionManager::GetInstancePtr())
            {
                EditorActionManager::GetInstancePtr()->applyAction(
                    EditorActionSelectEntities::Create(
                        false,
                        _entities));
            }
            else
            {
                SelectionManager::GetInstancePtr()->unselectObjects(_entities);
            }
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void UnselectEntities(
            EntityPtr const& _entity)
        {
            UnselectEntities(Vector<EntityPtr>{ _entity });
        }


        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SelectObjects(
            Vector<ObjectPtr> const& _entities,
            bool _clearSelectionList)
        {
            if (EditorActionManager::GetInstancePtr())
            {
                EditorActionManager::GetInstancePtr()->applyAction(
                    EditorActionSelectObjects::Create(
                        true,
                        _entities,
                        _clearSelectionList));
            }
            else
            {
                SelectionManager::GetInstancePtr()->selectObjects(
                    _entities, _clearSelectionList);
            }
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SelectObjects(
            ObjectPtr const& _entity,
            bool _clearSelectionList)
        {
            SelectObjects(Vector<ObjectPtr>{ _entity }, _clearSelectionList);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void UnselectObjects(
            Vector<ObjectPtr> const& _entities)
        {
            if (EditorActionManager::GetInstancePtr())
            {
                EditorActionManager::GetInstancePtr()->applyAction(
                    EditorActionSelectObjects::Create(
                        false,
                        _entities));
            }
            else
            {
                SelectionManager::GetInstancePtr()->unselectObjects(_entities);
            }
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void UnselectObjects(
            ObjectPtr const& _entity)
        {
            UnselectObjects(Vector<ObjectPtr>{ _entity });
        }


        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API bool AreMaterialsUniformEqual(
            Set<MaterialPtr> const& _materials,
            ShaderUniformVariant const& _value)
        {
            if (_materials.empty())
                return false;

            for (MaterialPtr const& material : _materials)
            {
                ShaderUniformVariantPtr const& variant = material->getUniform(_value.getName());
                if (variant && *variant.get() != _value)
                    return false;
            }

            return true;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SetMaterialsUniform(
            Set<MaterialPtr> const& _materials,
            ShaderUniformVariant const& _value)
        {
            if (EditorActionManager::GetInstancePtr())
            {
                if (AreMaterialsUniformEqual(_materials, _value))
                    return;

                DataBlock dataBlock;
                _value.toDataBlock(dataBlock);

                Vector<MaterialPtr> materials(_materials.begin(), _materials.end());

                bool merged = false;
                if (EditorActionMaterialSetUniformPtr lastAction = EditorActionManager::GetInstancePtr()->castLastEditorAction<EditorActionMaterialSetUniform>())
                {
                    if (EditorAction::GetCurrentTimestamp() - lastAction->getTimestamp() <= 1 &&
                        lastAction->getMaterials() == materials &&
                        lastAction->getShaderUniformName() == _value.getName())
                    {
                        merged = true;
                        lastAction->merge(dataBlock);
                    }
                }

                if (!merged)
                {
                    EditorActionManager::GetInstancePtr()->applyAction(
                        EditorActionMaterialSetUniform::Create(
                            materials,
                            _value.getName(),
                            dataBlock));
                }
            }
            else
            {
                for (MaterialPtr const& material : _materials)
                    material->setUniform(_value);
            }
        }


        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void CustomAction(
            std::function<void()> const& _applyFunction,
            std::function<void()> const& _revertFunction)
        {
            EditorActionManager::GetInstancePtr()->applyAction(
                EditorActionCustom::Create(
                    _applyFunction,
                    _revertFunction));
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void Translate(
            EntityPtr const& _entity,
            Vec3F const& _newPosition)
        {
            if (!_entity)
                return;

            Transform3D* transform = _entity->getComponentRaw<Transform3D>();
            if (!transform)
                return;

            if (transform->getLocalPosition() == _newPosition)
                return;

            if (EditorActionManager::GetInstancePtr())
            {
                bool merged = false;
                if (EditorActionTransform3DTranslatePtr lastAction = EditorActionManager::GetInstancePtr()->castLastEditorAction<EditorActionTransform3DTranslate>())
                {
                    if (EditorAction::GetCurrentTimestamp() - lastAction->getTimestamp() <= 1 &&
                        lastAction->getEntity() == _entity)
                    {
                        merged = true;
                        lastAction->merge(_newPosition);
                    }
                }

                if (!merged)
                {
                    EditorActionManager::GetInstancePtr()->applyAction(
                        EditorActionTransform3DTranslate::Create(_entity, _newPosition));
                }
            }
            else
            {
                transform->setLocalPosition(_newPosition);
            }
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void Rotate(
            EntityPtr const& _entity,
            Quaternion const& _newRotation)
        {
            if (!_entity)
                return;

            Transform3D* transform = _entity->getComponentRaw<Transform3D>();
            if (!transform)
                return;

            if (transform->getLocalRotation() == _newRotation)
                return;

            if (EditorActionManager::GetInstancePtr())
            {
                bool merged = false;
                if (EditorActionTransform3DRotatePtr lastAction = EditorActionManager::GetInstancePtr()->castLastEditorAction<EditorActionTransform3DRotate>())
                {
                    if (EditorAction::GetCurrentTimestamp() - lastAction->getTimestamp() <= 1 &&
                        lastAction->getEntity() == _entity)
                    {
                        merged = true;
                        lastAction->merge(_newRotation);
                    }
                }

                if (!merged)
                {
                    EditorActionManager::GetInstancePtr()->applyAction(
                        EditorActionTransform3DRotate::Create(_entity, _newRotation));
                }
            }
            else
            {
                transform->setLocalRotation(_newRotation);
            }
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void Scale(
            EntityPtr const& _entity,
            Vec3F const& _newScale)
        {
            if (!_entity)
                return;

            Transform3D* transform = _entity->getComponentRaw<Transform3D>();
            if (!transform)
                return;

            if (transform->getLocalScale() == _newScale)
                return;

            if (EditorActionManager::GetInstancePtr())
            {
                bool merged = false;
                if (EditorActionTransform3DScalePtr lastAction = EditorActionManager::GetInstancePtr()->castLastEditorAction<EditorActionTransform3DScale>())
                {
                    if (EditorAction::GetCurrentTimestamp() - lastAction->getTimestamp() <= 1 &&
                        lastAction->getEntity() == _entity)
                    {
                        merged = true;
                        lastAction->merge(_newScale);
                    }
                }

                if (!merged)
                {
                    EditorActionManager::GetInstancePtr()->applyAction(
                        EditorActionTransform3DScale::Create(_entity, _newScale));
                }
            }
            else
            {
                transform->setLocalScale(_newScale);
            }
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void Transform(
            EntityPtr const& _entity,
            TMat const& _newTransform)
        {
            Transform3DPtr component = _entity->getComponent<Transform3D>();

            if (EditorActionManager::GetInstancePtr())
            {
                EditorActionManager::GetInstancePtr()->applyAction(
                    EditorActionCustom::Create(
                        [component, newValue = _newTransform]() { component->setLocalTransform(newValue); },
                        [component, oldValue = component->getLocalTransform()]() { component->setLocalTransform(oldValue); }));
            }
            else
                component->setLocalTransform(_newTransform);
        }

    } // namespace EditorActionHelper
    //////////////////////////////////////////
    

} // namespace Maze
//////////////////////////////////////////
