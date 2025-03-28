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
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Transform3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Transform3D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3F, localPosition, Vec3F::c_zero, getLocalPosition, setLocalPosition),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Quaternion, localRotation, Quaternion(), getLocalRotation, setLocalRotation),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3F, localScale, Vec3F::c_one, getLocalScale, setLocalScale),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, parent, ComponentPtr(), getParentComponent, setParent));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Transform3D);

    //////////////////////////////////////////
    Transform3D::Transform3D()
    {
    }

    //////////////////////////////////////////
    Transform3D::~Transform3D()
    {
        while (!m_children.empty())
            m_children.front()->setParent(Transform3DPtr());
    }

    //////////////////////////////////////////
    Transform3DPtr Transform3D::Create()
    {
        Transform3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Transform3D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Transform3D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    bool Transform3D::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        if (!Component::init(_component, _copyData))
            return false;

        Transform3D* transform3D = static_cast<Transform3D*>(_component);
        m_flags = transform3D->m_flags;
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);

        for (Transform3D* childTransform : transform3D->m_children)
        {
            EntityPtr child = childTransform->getEntityRaw()->createCopy(_copyData);
            Transform3D* childTransformCopy = child->getComponentRaw<Transform3D>();
            childTransformCopy->setParent(cast<Transform3D>());
        }

        return true;
    }

    //////////////////////////////////////////
    void Transform3D::setLocalPosition(Vec3F const& _localPosition)
    {
        if (m_localPosition == _localPosition)
            return;

        m_localPosition = _localPosition;
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    void Transform3D::translate(Vec3F const& _offset)
    {
        setLocalPosition(m_localPosition + _offset);
    }

    //////////////////////////////////////////
    void Transform3D::translate(Vec2F const& _offset)
    {
        setLocalPosition(m_localPosition + _offset);
    }

    //////////////////////////////////////////
    void Transform3D::setLocalRotation(Quaternion const& _localRotation)
    {
        if (m_localRotation == _localRotation)
            return;

        m_localRotation = _localRotation;
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    void Transform3D::setLocalDirection(Vec3F const& _localDirection)
    {
        setLocalRotation(Quaternion(Vec3F::c_unitZ, _localDirection));
    }

    //////////////////////////////////////////
    void Transform3D::setLocalScale(Vec3F const& _localScale)
    {
        if (m_localScale == _localScale)
            return;

        m_localScale = _localScale;
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    void Transform3D::rotate(Vec3F const& _axis, F32 _angle)
    {
        setLocalRotation(getLocalRotation() * Quaternion(_angle, _axis));
    }

    //////////////////////////////////////////
    void Transform3D::setLocalTransform(TMat const& _localTransform)
    {
        if (m_localTransform == _localTransform)
            return;

        m_localTransform = _localTransform;

        m_localPosition = m_localTransform.getTranslation();
        Vec3F eulerAngles = Quaternion::GetEuler(m_localTransform);
        m_localRotation.setEulerAngles(eulerAngles);
        m_localScale = m_localTransform.getScaleSignless();

        m_flags &= ~Flags::LocalTransformDirty;
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    TMat const& Transform3D::getLocalTransform()
    {
        if (m_flags & Flags::LocalTransformDirty)
            return calculateLocalTransform();
        else
            return m_localTransform;
    }

    //////////////////////////////////////////
    TMat const& Transform3D::calculateLocalTransform()
    {
        TMat rotationMatrix;
        m_localRotation.toRotationMatrix(rotationMatrix);

        m_localTransform = 
            TMat::CreateTranslation(m_localPosition).transform(
                rotationMatrix).transform(
                    TMat::CreateScale(m_localScale));

        m_flags &= ~Flags::LocalTransformDirty;
        m_flags |= Flags::WorldTransformDirty;

        return m_localTransform;
    }

    //////////////////////////////////////////
    TMat const& Transform3D::getWorldTransform()
    {
        if (m_flags & Flags::WorldTransformDirty)
            return calculateWorldTransform();
        else
            return m_worldTransform;
    }

    //////////////////////////////////////////
    void Transform3D::setWorldTransform(TMat const& _transform)
    {
        if (m_parent)
        {
            TMat localTransform = _transform * m_parent->getWorldTransform().inversed();
            setLocalTransform(localTransform);
        }
        else
        {
            setLocalTransform(_transform);
        }
    }

    //////////////////////////////////////////
    TMat const& Transform3D::calculateWorldTransform()
    {
        if (m_parent)
            m_parent->getWorldTransform().transform(getLocalTransform(), m_worldTransform);
        else
            m_worldTransform = getLocalTransform();

        m_flags &= ~Flags::WorldTransformDirty;

        return m_worldTransform;
    }

    //////////////////////////////////////////
    Quaternion Transform3D::getWorldRotation() const
    {
        if (m_parent)
            return m_localRotation * m_parent->getWorldRotation();
        else
            return m_localRotation;
    }

    //////////////////////////////////////////
    Vec3F Transform3D::getWorldScale() const
    {
        if (m_parent)
            return m_localScale * m_parent->getWorldScale();
        else
            return m_localScale;
    }

    //////////////////////////////////////////
    void Transform3D::setParent(Transform3DPtr const& _parent)
    {
        if (m_parent == _parent)
            return;

        EntityId prevParentEntityId = m_parent ? m_parent->getEntityId() : c_invalidEntityId;

        if (m_parent)
            m_parent->m_children.erase(
                std::find(
                    m_parent->m_children.begin(),
                    m_parent->m_children.end(),
                    this));

        MAZE_DEBUG_ERROR_IF(
            getEntityRaw() && getEntityRaw()->getEcsWorld() && _parent && _parent->getEntityRaw() && _parent->getEntityRaw()->getEcsWorld() &&
            getEntityRaw()->getEcsWorld() != _parent->getEntityRaw()->getEcsWorld(), "It's forbidden to connect entities from different worlds!");
        
        m_parent = _parent;

        m_flags |= ParentChangedCurrentFrame;

        dirtyWorldTransform(
            Flags::WorldTransformDirty | 
            Flags::WorldTransformChangedCurrentFrame |
            Flags::HierarchyChangedCurrentFrame);

        if (getEntityRaw() && getEntityRaw()->getEcsWorld())
        {
            getEntityRaw()->getEcsWorld()->broadcastEvent<EntityParentChangedEvent>(
                getEntityId(),
                prevParentEntityId,
                m_parent ? m_parent->getEntityId() : c_invalidEntityId);
            
        }

        if (m_parent)
        {
            m_parent->m_children.push_back(this);

            if (m_parent->getEntityRaw())
            {
                if (getEntityRaw())
                {
                    if (getEntityRaw()->getDisabledByHierarchy())
                    {
                        if (m_parent->getEntityRaw()->getActiveInHierarchy())
                            getEntityRaw()->setDisabledByHierarchy(false);
                    }
                    else
                    if (getEntityRaw()->getActiveSelf())
                    {
                        if (!m_parent->getEntityRaw()->getActiveInHierarchy())
                        {
                            getEntityRaw()->setDisabledByHierarchy(true);
                        }
                    }
                }
            }
        }
        else
        {
            if (getEntityRaw())
            {
                if (getEntityRaw()->getDisabledByHierarchy())
                {
                    if (!getEntityRaw()->getRemoving())
                        getEntityRaw()->setDisabledByHierarchy(false);
                }
            }
        }
    }

    //////////////////////////////////////////
    void Transform3D::removeAllChildren()
    {
        while (!m_children.empty())
        {
            m_children.back()->setParent(Transform3DPtr());
        }
    }

    //////////////////////////////////////////
    void Transform3D::destroyAllChildren()
    {
        while (!m_children.empty())
        {
            Transform3D* child = m_children.back();
            Entity* childEntityRaw = child->getEntityRaw();
            EntityPtr childEntity = childEntityRaw->getSharedPtr();
            child->setParent(Transform3DPtr());

            child->destroyAllChildren();

            EcsWorld* world = childEntity->getEcsWorld();
            if (world)
                world->removeEntity(childEntity);
        }
    }

    //////////////////////////////////////////
    void Transform3D::getAllEntitiesRaw(Vector<Entity*>& _result)
    {
        _result.emplace_back(getEntityRaw());

        for (Transform3D* transform : m_children)
            transform->getAllEntitiesRaw(_result);
    }

    //////////////////////////////////////////
    Vector<Entity*> Transform3D::getAllEntitiesRaw()
    {
        Vector<Entity*> result;
        getAllEntitiesRaw(result);
        return result;
    }

    //////////////////////////////////////////
    void Transform3D::processComponentRemoved()
    {
        setParent(Transform3DPtr());
        removeAllChildren();
    }


    //////////////////////////////////////////
    void Transform3D::processEntityEnabled()
    {
        for (Transform3D* transform : m_children)
            transform->getEntityRaw()->setDisabledByHierarchy(false);
    }

    ////////////////////////////////////////////
    void Transform3D::processEntityDisabled()
    {
        for (Transform3D* transform : m_children)
            transform->getEntityRaw()->setDisabledByHierarchy(true);
    }

    //////////////////////////////////////////
    bool Transform3D::isMetaPropertyCopyable(MetaProperty* _metaProperty)
    {
        if (GetParentProperty() == _metaProperty)
            return false;

        return true;
    }

    //////////////////////////////////////////
    MetaProperty* Transform3D::GetParentProperty()
    {
        static MetaProperty* s_parentMetaProperty = Transform3D::GetMetaClass()->getProperty("parent");
        return s_parentMetaProperty;
    }

    //////////////////////////////////////////
    void Transform3D::resetTransform()
    {
        setLocalTransform(TMat::c_identity);
    }

    //////////////////////////////////////////
    void Transform3D::processActiveChanged()
    {
        if (getEntityRaw()->getActiveInHierarchy())
        {
            for (Transform3D* transform : m_children)
                transform->getEntityRaw()->setDisabledByHierarchy(false);
        }
        else
        {
            for (Transform3D* transform : m_children)
                transform->getEntityRaw()->setDisabledByHierarchy(true);
        }
    }
    

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(Transform3DEntityRemoved,
        {},
        {},
        EntityRemovedEvent const& _event,
        Entity* _entity,
        Transform3D* _transform3D)
    {
        if (_transform3D)
        {
            _transform3D->setParent(Transform3DPtr());
            _transform3D->destroyAllChildren();
        }
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(Transform3DAppear,
        {},
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        Transform3D* _transform3D)
    {
        if (_transform3D->getParent() &&
            _transform3D->getParent()->getEntityRaw() &&
            !_transform3D->getParent()->getEntityRaw()->getActiveInHierarchy())
            _transform3D->getEntityRaw()->setDisabledByHierarchy(true);
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(Transform3DActiveChangedEvent,
        {},
        {},
        EntityActiveChangedEvent const& _event,
        Entity* _entity,
        Transform3D* _transform3D)
    {
        //if (_transform3D)
        //    _transform3D->processActiveChanged();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(Transform3DPreUpdate,
        {},
        {},
        PreUpdateEvent const& _event,
        Entity* _entity,
        Transform3D* _transform3D)
    {
        _transform3D->processPreUpdate();
    }
    
} // namespace Maze
//////////////////////////////////////////
