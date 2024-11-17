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
#include "MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Component
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Component);

    //////////////////////////////////////////
    Component::Component()
    {
    }

    //////////////////////////////////////////
    Component::~Component()
    {

    }

    //////////////////////////////////////////
    ComponentPtr Component::Create(
        Component* _component,
        EntityCopyData _copyData)
    {
        MetaClass* metaClass = _component->getMetaClass();
        ComponentPtr newComponent(static_cast<Component*>(metaClass->createInstance()));
        MAZE_ERROR_RETURN_VALUE_IF(!newComponent, ComponentPtr(), "Can't instantiate Component %s!", _component->getClassName().str);
        if (!newComponent->init(_component, _copyData))
            return ComponentPtr();
        
        return newComponent;
    }

    //////////////////////////////////////////
    bool Component::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        _copyData.getComponents()[_component] = this;

        MAZE_DEBUG_BP_IF(getMetaClass() != _component->getMetaClass())
        MetaInstance metaInstance = getMetaInstance();
        MetaInstance objMetaInstance = _component->getMetaInstance();
        
        Vector<MetaClass*> const& componentSuperMetaClasses = _component->getMetaClass()->getAllSuperMetaClasses();
        for (MetaClass* metaClass : componentSuperMetaClasses)
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                MetaProperty* metaProperty = metaClass->getProperty(i);

                if (!isMetaPropertyCopyable(metaProperty))
                    continue;

                if (ClassInfo<Vector<ComponentPtr>>::UID() == metaProperty->getValueClassUID())
                {
                    _copyData.getComponentProperties().emplace_back(
                        EntityCopyData::ComponentPropertyData
                        {
                            metaProperty,
                            metaInstance,
                            objMetaInstance
                        });
                }
                else
                if (ClassInfo<Vector<EntityPtr>>::UID() == metaProperty->getValueClassUID())
                {
                    _copyData.getEntityProperties().emplace_back(
                        EntityCopyData::EntityPropertyData
                        {
                            metaProperty,
                            metaInstance,
                            objMetaInstance
                        });
                }
                else
                if (ClassInfo<ComponentPtr>::UID() == metaProperty->getValueClassUID())
                {
                    _copyData.getComponentProperties().emplace_back(
                        EntityCopyData::ComponentPropertyData
                        {
                            metaProperty,
                            metaInstance,
                            objMetaInstance
                        });
                }
                else
                if (ClassInfo<EntityPtr>::UID() == metaProperty->getValueClassUID())
                {
                    _copyData.getEntityProperties().emplace_back(
                        EntityCopyData::EntityPropertyData
                        {
                            metaProperty,
                            metaInstance,
                            objMetaInstance
                        });
                }
                else
                {
                    metaProperty->copy(metaInstance, objMetaInstance);
                }
            }
        }

        return true;
    }

    //////////////////////////////////////////
    EntityPtr Component::getEntity() const
    {
        return m_entityRaw ? m_entityRaw->getSharedPtr() : nullptr; 
    }

    //////////////////////////////////////////
    EntityId Component::getEntityId() const 
    { 
        return m_entityRaw ? m_entityRaw->getId() : c_invalidEntityId;
    }

    //////////////////////////////////////////
    bool Component::removeFromEntity()
    {
        if (!m_entityRaw)
            return false;

        return m_entityRaw->removeComponent(getClassUID());
    }

    //////////////////////////////////////////
    void Component::setEntity(Entity* _entity)
    {
        if (m_entityRaw == _entity)
            return;

        m_entityRaw = _entity;

        if (_entity)
            processComponentAdded();
        else
            processComponentRemoved();
    }

    //////////////////////////////////////////
    S32 Component::getPriority() const
    {
        return EntityManager::GetInstancePtr()->getComponentPriority(getClassUID());
    }


} // namespace Maze
//////////////////////////////////////////
