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
#include "maze-editor-tools/editor-actions/MazeEditorActionTransform3DChangeParent.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorActionTransform3DChangeParent
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorActionTransform3DChangeParent, EditorAction);
 
    //////////////////////////////////////////
    EditorActionTransform3DChangeParent::EditorActionTransform3DChangeParent()
    {

    }
     
    //////////////////////////////////////////
    EditorActionTransform3DChangeParent::~EditorActionTransform3DChangeParent()
    {

    }

    //////////////////////////////////////////
    EditorActionTransform3DChangeParentPtr EditorActionTransform3DChangeParent::Create(
        EntityPtr const& _entity,
        EntityPtr const& _newParentEntity)
    {
        EditorActionTransform3DChangeParentPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorActionTransform3DChangeParent, object, init(_entity, _newParentEntity));
        return object;
    }

    //////////////////////////////////////////
    bool EditorActionTransform3DChangeParent::init(
        EntityPtr const& _entity,
        EntityPtr const& _newParentEntity)
    {
        if (!_entity)
            return false;

        Transform3D* transform = _entity->getComponentRaw<Transform3D>();
        if (!transform)
            return false;

        m_entity = _entity;
        m_newParentEntity = _newParentEntity;

        m_prevParentEntity = transform->getParent() ? transform->getParent()->getEntity() : EntityPtr();
        m_prevTransform = transform->getWorldTransform();

        return true;
    }

    //////////////////////////////////////////
    void EditorActionTransform3DChangeParent::applyImpl()
    {
        Transform3D* transform = m_entity->getComponentRaw<Transform3D>();
        if (!transform)
            return;

        if (m_newParentEntity)
            transform->setParent(m_newParentEntity->getComponent<Transform3D>());
        else
            transform->setParent(Transform3DPtr());

        transform->setWorldTransform(m_prevTransform);
    }

    //////////////////////////////////////////
    void EditorActionTransform3DChangeParent::revertImpl()
    {
        Transform3D* transform = m_entity->getComponentRaw<Transform3D>();
        if (!transform)
            return;

        if (m_prevParentEntity)
            transform->setParent(m_prevParentEntity->getComponent<Transform3D>());
        else
            transform->setParent(Transform3DPtr());

        transform->setWorldTransform(m_prevTransform);
    }


} // namespace Maze
//////////////////////////////////////////
