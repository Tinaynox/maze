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
#include "maze-editor-tools/editor-actions/MazeEditorActionEntityRemove.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorActionEntityRemove
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorActionEntityRemove, EditorAction);
 
    //////////////////////////////////////////
    EditorActionEntityRemove::EditorActionEntityRemove()
    {

    }
     
    //////////////////////////////////////////
    EditorActionEntityRemove::~EditorActionEntityRemove()
    {

    }

    //////////////////////////////////////////
    EditorActionEntityRemovePtr EditorActionEntityRemove::Create(
        EntityPtr const& _entity)
    {
        EditorActionEntityRemovePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorActionEntityRemove, object, init(_entity));
        return object;
    }

    //////////////////////////////////////////
    bool EditorActionEntityRemove::init(
        EntityPtr const& _entity)
    {
        if (!_entity || !_entity->getEcsWorld())
            return false;

        m_entity = _entity;
        m_world = _entity->getEcsWorld()->getSharedPtr();
        m_sceneId = m_entity->getEcsScene() ? m_entity->getEcsScene()->getId() : EcsSceneId();

        /*
        if (Transform3DPtr transform3D = m_entity->getComponent<Transform3D>())
            m_parentTransform3D = transform3D->getParent();
        else
        if (Transform2DPtr transform2D = m_entity->getComponent<Transform2D>())
            m_parentTransform2D = transform2D->getParent();
        */

        if (Transform3DPtr transform3D = m_entity->getComponent<Transform3D>())
        {
            Vector<Transform3DPtr> children = transform3D->getAllComponents<Transform3D>();
            for (Vector<Transform3DPtr>::reverse_iterator it = children.rbegin(),
                                                          end = children.rend();
                                                          it != end;
                                                          ++it)
            {
                EntityData entityData;
                entityData.entity = (*it)->getEntity();
                entityData.parentTransform3D = (*it)->getParent();
                m_entityData.push_back(entityData);
            }
        }
        else
        if (Transform2DPtr transform2D = m_entity->getComponent<Transform2D>())
        {
            Vector<Transform2DPtr> children = transform2D->getAllComponents<Transform2D>();
            for (Vector<Transform2DPtr>::reverse_iterator it = children.rbegin(),
                                                          end = children.rend();
                                                          it != end;
                                                          ++it)
            {
                EntityData entityData;
                entityData.entity = (*it)->getEntity();
                entityData.parentTransform2D = (*it)->getParent();
                m_entityData.push_back(entityData);
            }
        }


        return true;
    }

    //////////////////////////////////////////
    void EditorActionEntityRemove::applyImpl()
    {
        for (EntityData const& entityData : m_entityData)
        {
            m_world->removeEntity(entityData.entity);
        }
    }

    //////////////////////////////////////////
    void EditorActionEntityRemove::revertImpl()
    {
        EcsScenePtr const& scene = SceneManager::GetInstancePtr()->getScene(m_sceneId);

        for (EntityData const& entityData : m_entityData)
        {
            m_world->addEntity(entityData.entity);
            entityData.entity->setEcsScene(scene.get());

            if (entityData.parentTransform2D)
                entityData.entity->getComponent<Transform2D>()->setParent(entityData.parentTransform2D);
            else
            if (entityData.parentTransform3D)
                entityData.entity->getComponent<Transform3D>()->setParent(entityData.parentTransform3D);
        }
    }


} // namespace Maze
//////////////////////////////////////////
