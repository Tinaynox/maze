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
#include "maze-core/ecs/MazeECSWorld.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Transform3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Transform3D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, localPosition, Vec3DF::c_zero, getLocalPosition, setLocalPosition),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Quaternion, localRotation, Quaternion(), getLocalRotation, setLocalRotation),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DF, localScale, Vec3DF::c_one, getLocalScale, setLocalScale),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, parent, ComponentPtr(), getParentComponent, setParent));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Transform3D);

    //////////////////////////////////////////
    Transform3D::Transform3D()
        : m_localPosition(Vec3DF::c_zero)
        , m_localRotation(Quaternion::c_identity)
        , m_localScale(Vec3DF::c_one)
        , m_flags(0)
        , m_localTransform(Mat4DF::c_identity)
        , m_worldTransform(Mat4DF::c_identity)
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
        ECSWorld* _world,
        EntityCopyData _copyData)
    {
        if (!Component::init(_component, _world, _copyData))
            return false;

        Transform3D* transform3D = static_cast<Transform3D*>(_component);
        m_flags = transform3D->m_flags;
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);

        for (Transform3D* childTransform : transform3D->m_children)
        {
            EntityPtr child = childTransform->getEntityRaw()->createCopy(_world, _copyData);
            Transform3D* childTransformCopy = child->getComponentRaw<Transform3D>();
            childTransformCopy->setParent(cast<Transform3D>());
        }

        return true;
    }

    //////////////////////////////////////////
    void Transform3D::setLocalPosition(Vec3DF const& _localPosition)
    {
        if (m_localPosition == _localPosition)
            return;

        m_localPosition = _localPosition;
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    void Transform3D::translate(Vec3DF const& _offset)
    {
        setLocalPosition(m_localPosition + _offset);
    }

    //////////////////////////////////////////
    void Transform3D::translate(Vec2DF const& _offset)
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
    void Transform3D::setLocalDirection(Vec3DF const& _localDirection)
    {
        setLocalRotation(Quaternion(Vec3DF::c_unitZ, _localDirection));
    }

    //////////////////////////////////////////
    void Transform3D::setLocalScale(Vec3DF const& _localScale)
    {
        if (m_localScale == _localScale)
            return;

        m_localScale = _localScale;
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    void Transform3D::rotate(Vec3DF const& _axis, F32 _angle)
    {
        setLocalRotation(getLocalRotation() * Quaternion(_angle, _axis));
    }

    //////////////////////////////////////////
    void Transform3D::setLocalTransform(Mat4DF const& _localTransform)
    {
        if (m_localTransform == _localTransform)
            return;

        m_localTransform = _localTransform;

        m_localPosition = m_localTransform.getAffineTranslation();
        //Vec3DF eulerAngles = m_localTransform.getAffineRotationEulerAngles();
        Vec3DF eulerAngles = Quaternion::GetEuler(m_localTransform);
        m_localRotation.setEulerAngles(eulerAngles);
        m_localScale = m_localTransform.getAffineScaleSignless();  // #TODO: Signed scale

        m_flags &= ~Flags::LocalTransformDirty;
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    Mat4DF const& Transform3D::getLocalTransform()
    {
        if (m_flags & Flags::LocalTransformDirty)
            return calculateLocalTransform();
        else
            return m_localTransform;
    }

    //////////////////////////////////////////
    Mat4DF const& Transform3D::calculateLocalTransform()
    {
        Mat4DF rotationMatrix;
        m_localRotation.toRotationMatrix(rotationMatrix);

        m_localTransform = 
            Mat4DF::CreateTranslationMatrix(m_localPosition) *
            rotationMatrix *
            Mat4DF::CreateScaleMatrix(m_localScale);

        m_flags &= ~Flags::LocalTransformDirty;
        m_flags |= Flags::WorldTransformDirty;

        return m_localTransform;
    }

    //////////////////////////////////////////
    Mat4DF const& Transform3D::getWorldTransform()
    {
        if (m_flags & Flags::WorldTransformDirty)
            return calculateWorldTransform();
        else
            return m_worldTransform;
    }

    //////////////////////////////////////////
    void Transform3D::setWorldTransform(Mat4DF const& _transform)
    {
        if (m_parent)
        {
            Mat4DF localTransform = m_parent->getWorldTransform().inversedAffineCopy() * _transform;
            setLocalTransform(localTransform);
        }
        else
        {
            setLocalTransform(_transform);
        }
    }

    //////////////////////////////////////////
    Mat4DF const& Transform3D::calculateWorldTransform()
    {
        if (m_parent)
            m_parent->getWorldTransform().concatenateAffine(getLocalTransform(), m_worldTransform);
        else
            m_worldTransform = getLocalTransform();

        m_flags &= ~Flags::WorldTransformDirty;

        return m_worldTransform;
    }

    //////////////////////////////////////////
    void Transform3D::setParent(Transform3DPtr const& _parent)
    {
        if (m_parent == _parent)
            return;

        if (m_parent)
            m_parent->m_children.erase(
                std::find(
                    m_parent->m_children.begin(),
                    m_parent->m_children.end(),
                    this));

        MAZE_DEBUG_ERROR_IF(
            getEntityRaw() && getEntityRaw()->getECSWorld() && _parent && _parent->getEntityRaw() && _parent->getEntityRaw()->getECSWorld() &&
            getEntityRaw()->getECSWorld() != _parent->getEntityRaw()->getECSWorld(), "It's forbidden to connect entities from different worlds!");
        
        m_parent = _parent;

        m_flags |= ParentChangedCurrentFrame;

        dirtyWorldTransform(
            Flags::WorldTransformDirty | 
            Flags::WorldTransformChangedCurrentFrame |
            Flags::HierarchyChangedCurrentFrame);

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

            childEntity->getECSWorld()->removeEntity(childEntity);
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
    void Transform3D::processComponentAdded()
    {
    }

    //////////////////////////////////////////
    void Transform3D::processComponentRemoved()
    {
        setParent(Transform3DPtr());
        removeAllChildren();
    }

    //////////////////////////////////////////
    void Transform3D::processEntityRemoved()
    {
        setParent(Transform3DPtr());
        destroyAllChildren();
    }

    //////////////////////////////////////////
    void Transform3D::processEntityEnabled()
    {
        for (Transform3D* transform : m_children)
            transform->getEntityRaw()->setDisabledByHierarchy(false);
    }

    //////////////////////////////////////////
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
        setLocalTransform(Mat4DF::c_identity);
    }
    
    
} // namespace Maze
//////////////////////////////////////////
