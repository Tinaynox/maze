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
#include "maze-editor-tools/editor-actions/MazeEditorActionTransform3DTranslate.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorActionTransform3DTranslate
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorActionTransform3DTranslate, EditorAction);
 
    //////////////////////////////////////////
    EditorActionTransform3DTranslate::EditorActionTransform3DTranslate()
    {

    }
     
    //////////////////////////////////////////
    EditorActionTransform3DTranslate::~EditorActionTransform3DTranslate()
    {

    }

    //////////////////////////////////////////
    EditorActionTransform3DTranslatePtr EditorActionTransform3DTranslate::Create(
        EntityPtr const& _entity,
        Vec3F const& _newPosition)
    {
        EditorActionTransform3DTranslatePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorActionTransform3DTranslate, object, init(_entity, _newPosition));
        return object;
    }

    //////////////////////////////////////////
    bool EditorActionTransform3DTranslate::init(
        EntityPtr const& _entity,
        Vec3F const& _newPosition)
    {
        if (!_entity)
            return false;

        Transform3D* transform = _entity->getComponentRaw<Transform3D>();
        if (!transform)
            return false;

        m_entity = _entity;
        m_prevPosition = transform->getLocalPosition();
        m_newPosition = _newPosition;

        return true;
    }

    //////////////////////////////////////////
    void EditorActionTransform3DTranslate::merge(Vec3F const& _newPosition)
    {
        m_newPosition = _newPosition;

        if (m_applied)
        {
            resetTimestamp();
            applyImpl();
        }
    }

    //////////////////////////////////////////
    void EditorActionTransform3DTranslate::applyImpl()
    {
        Transform3D* transform = m_entity->getComponentRaw<Transform3D>();
        if (!transform)
            return;

        transform->setLocalPosition(m_newPosition);
    }

    //////////////////////////////////////////
    void EditorActionTransform3DTranslate::revertImpl()
    {
        Transform3D* transform = m_entity->getComponentRaw<Transform3D>();
        if (!transform)
            return;

        transform->setLocalPosition(m_prevPosition);
    }


} // namespace Maze
//////////////////////////////////////////
