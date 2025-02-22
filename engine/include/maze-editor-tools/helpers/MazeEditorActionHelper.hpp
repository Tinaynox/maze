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
#if (!defined(_MazeEditorActionHelper_hpp_))
#define _MazeEditorActionHelper_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/helpers/MazeMetaClassHelper.hpp"
#include "maze-graphics/MazeShaderUniformVariant.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsActionManager.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionMetaInstancesSetDataBlock.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionMaterialSetUniform.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_MANAGED_SHARED_PTR(Object);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    namespace EditorActionHelper
    {
        //////////////////////////////////////////
        template <typename TValue>
        inline void SetValue(
            TValue const& _value,
            MetaProperty* _metaProperty,
            Set<MetaInstance> const& _metaInstances)
        {
            if (EditorToolsActionManager::GetInstancePtr())
            {
                DataBlock dataBlock;
                ValueToDataBlock(_value, dataBlock);

                EditorToolsActionManager::GetInstancePtr()->applyAction(
                    EditorActionMetaInstancesSetDataBlock::Create(
                        _metaProperty,
                        Vector<MetaInstance>(_metaInstances.begin(), _metaInstances.end()),
                        dataBlock));
            }
            else
            {
                MetaClassHelper::SetValue(_value, _metaProperty, _metaInstances);
            }
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SetValueString(
            String const& _value,
            MetaProperty* _metaProperty,
            Set<MetaInstance> const& _metaInstances);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SelectEntities(
            Vector<EntityPtr> const& _entities,
            bool _clearSelectionList = true);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SelectEntities(
            EntityPtr const& _entity,
            bool _clearSelectionList = true);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void UnselectEntities(
            Vector<EntityPtr> const& _entities);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void UnselectEntities(
            EntityPtr const& _entity);



        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SelectObjects(
            Vector<ObjectPtr> const& _entities,
            bool _clearSelectionList = true);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SelectObjects(
            ObjectPtr const& _entity,
            bool _clearSelectionList = true);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void UnselectObjects(
            Vector<ObjectPtr> const& _entities);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void UnselectObjects(
            ObjectPtr const& _entity);


        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SetMaterialsUniform(
            Set<MaterialPtr> const& _materials,
            ShaderUniformVariant const& _value);


        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void CustomAction(
            std::function<void()> const& _applyFunction,
            std::function<void()> const& _revertFunction);


        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void Translate(
            EntityPtr const& _entity,
            Vec3F const& _newPosition);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void Rotate(
            EntityPtr const& _entity,
            Quaternion const& _newRotation);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void Scale(
            EntityPtr const& _entity,
            Vec3F const& _newScale);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void Transform(
            EntityPtr const& _entity,
            TMat const& _newTransform);

    } // namespace EditorActionHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorActionHelper_hpp_
//////////////////////////////////////////
