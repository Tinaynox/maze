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
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Transform2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Transform2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, localPosition, Vec2F::c_zero, getLocalPosition, setLocalPosition),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Rotation2D, localRotation, Rotation2D(), getLocalRotation, setLocalRotation),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, localScale, Vec2F::c_one, getLocalScale, setLocalScale),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, pivot, Vec2F(0.5f, 0.5f), getPivot, setPivot),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, size, Vec2F(100.0f, 100.0f), getSize, setSize),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, anchor, Vec2F(0.5f, 0.5f), getAnchor, setAnchor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, z, 0, getZ, setZ),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, parent, ComponentPtr(), getParentComponent, setParent));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Transform2D);

    //////////////////////////////////////////
    U32 Transform2D::s_globalOrderOfArrival = 1;

    //////////////////////////////////////////
    Transform2D::Transform2D()
        : m_localPosition(Vec2F::c_zero)
        , m_localScale(Vec2F::c_one)
        , m_pivot(Vec2F(0.5f, 0.5f))
        , m_size(Vec2F(100.0f, 100.0f))
        , m_anchor(Vec2F(0.5f, 0.5f))
        , m_flags(0)
        , m_localTransform(Mat4F::c_identity)
        , m_worldTransform(Mat4F::c_identity)
        , m_z(0)
        , m_orderOfArrival(0)
    {
    }

    //////////////////////////////////////////
    Transform2D::~Transform2D()
    {
        while (!m_children.empty())
            m_children.front()->setParent(Transform2DPtr());
    }

    //////////////////////////////////////////
    Transform2DPtr Transform2D::Create()
    {
        Transform2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Transform2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Transform2D::init()
    {
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);

        return true;
    }

    //////////////////////////////////////////
    bool Transform2D::init(
        Component* _component,
        EcsWorld* _world,
        EntityCopyData _copyData)
    {
        if (!Component::init(_component, _world, _copyData))
            return false;

        Transform2D* transform2D = static_cast<Transform2D*>(_component);
        m_flags = transform2D->m_flags;
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);

        for (Size i = 0, in = transform2D->m_children.size(); i < in; ++i)
        {
            Transform2D* childTransform = transform2D->m_children[i];

            EntityPtr child = childTransform->getEntityRaw()->createCopy(_world, _copyData);
            Transform2D* childTransformCopy = child->getComponentRaw<Transform2D>();
            childTransformCopy->setParent(cast<Transform2D>());
        }

        return true;
    }

    //////////////////////////////////////////
    void Transform2D::setLocalPosition(Vec2F const& _localPosition)
    {
        if (m_localPosition == _localPosition)
            return;

        m_localPosition = _localPosition;
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    void Transform2D::translate(Vec2F const& _offset)
    {
        setLocalPosition(m_localPosition + _offset);
    }

    //////////////////////////////////////////
    void Transform2D::calculateLocalCorners(Vec2F _corners[4])
    {
        Vec2F const& size = getSize();
        Mat4F const& localTransform = getLocalTransform();
        Vec2F lb = localTransform.transformAffine(Vec2F::c_zero);
        Vec2F rb = localTransform.transformAffine(Vec2F(size.x, 0.0f));
        Vec2F rt = localTransform.transformAffine(Vec2F(size.x, size.y));
        Vec2F lt = localTransform.transformAffine(Vec2F(0.0f, size.y));

        _corners[0] = lb;
        _corners[1] = rb;
        _corners[2] = rt;
        _corners[3] = lt;
    }

    //////////////////////////////////////////
    AABB2D Transform2D::calculateLocalAABB()
    {
        Vec2F corners[4];
        calculateLocalCorners(corners);

        Vec2F lb = corners[0];
        Vec2F rb = corners[1];
        Vec2F rt = corners[2];
        Vec2F lt = corners[3];

        F32 minX = Math::Min(lb.x, rb.x, rt.x, lt.x);
        F32 maxX = Math::Max(lb.x, rb.x, rt.x, lt.x);
        F32 minY = Math::Min(lb.y, rb.y, rt.y, lt.y);
        F32 maxY = Math::Max(lb.y, rb.y, rt.y, lt.y);

        return AABB2D(minX, minY, maxX, maxY);
    }

    //////////////////////////////////////////
    void Transform2D::calculateWorldCorners(Vec2F _corners[4])
    {
        Vec2F const& size = getSize();
        Mat4F const& worldTransform = getWorldTransform();
        Vec2F lb = worldTransform.transformAffine(Vec2F::c_zero);
        Vec2F rb = worldTransform.transformAffine(Vec2F(size.x, 0.0f));
        Vec2F rt = worldTransform.transformAffine(Vec2F(size.x, size.y));
        Vec2F lt = worldTransform.transformAffine(Vec2F(0.0f, size.y));

        _corners[0] = lb;
        _corners[1] = rb;
        _corners[2] = rt;
        _corners[3] = lt;
    }

    //////////////////////////////////////////
    AABB2D Transform2D::calculateWorldAABB()
    {
        Vec2F corners[4];
        calculateWorldCorners(corners);

        Vec2F lb = corners[0];
        Vec2F rb = corners[1];
        Vec2F rt = corners[2];
        Vec2F lt = corners[3];

        F32 minX = Math::Min(lb.x, rb.x, rt.x, lt.x);
        F32 maxX = Math::Max(lb.x, rb.x, rt.x, lt.x);
        F32 minY = Math::Min(lb.y, rb.y, rt.y, lt.y);
        F32 maxY = Math::Max(lb.y, rb.y, rt.y, lt.y);

        return AABB2D(minX, minY, maxX, maxY);
    }

    //////////////////////////////////////////
    void Transform2D::setPivot(Vec2F const& _pivot)
    {
        if (m_pivot == _pivot)
            return;

        m_pivot = _pivot;
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    void Transform2D::setSize(Vec2F const& _size)
    {
        if (m_size == _size)
            return;

        m_size = _size;
        MAZE_DEBUG_ERROR_IF(!m_size.isFinite(), "Size is not finite!");

        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame | Flags::SizeChangedCurrentFrame);
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    void Transform2D::setAnchor(Vec2F const& _anchor)
    {
        if (m_anchor == _anchor)
            return;

        m_anchor = _anchor;
        m_flags |= Flags::AnchorChangedCurrentFrame;
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    void Transform2D::setLocalRotation(Rotation2D const& _localRotation)
    {
        if (m_localRotation == _localRotation)
            return;

        m_localRotation = _localRotation;
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    void Transform2D::setLocalScale(Vec2F const& _localScale)
    {
        if (m_localScale == _localScale)
            return;

        m_localScale = _localScale;
        m_flags |= (Flags::LocalTransformDirty | Flags::LocalTransformChangedCurrentFrame);
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    void Transform2D::setLocalTransform(Mat4F const& _localTransform)
    {
        if (m_localTransform == _localTransform)
            return;

        m_localTransform = _localTransform;

        m_localPosition = m_localTransform.getAffineTranslation2D();
        m_localRotation = m_localTransform.getAffineRotation2D();
        m_localScale = m_localTransform.getAffineScale2DSignless();

        m_flags &= ~Flags::LocalTransformDirty;
        dirtyWorldTransform();
    }

    //////////////////////////////////////////
    Mat4F const& Transform2D::getLocalTransform()
    {
        if (m_flags & Flags::LocalTransformDirty)
            return calculateLocalTransform();
        else
            return m_localTransform;
    }

    //////////////////////////////////////////
    Mat4F const& Transform2D::calculateLocalTransform()
    {
        m_localTransform = Mat4F::CreateAffineTransformMatrix(
            m_localPosition,
            m_localRotation,
            m_localScale,
            m_pivot,
            m_size);

        m_flags &= ~Flags::LocalTransformDirty;
        m_flags |= Flags::WorldTransformDirty;

        return m_localTransform;
    }

    //////////////////////////////////////////
    Mat4F const& Transform2D::getWorldTransform()
    {
        if (m_flags & Flags::WorldTransformDirty)
            return calculateWorldTransform();
        else
            return m_worldTransform;
    }

    //////////////////////////////////////////
    Mat4F const& Transform2D::calculateWorldTransform()
    {
        if (m_parent)
        {
            Mat4F anchorMatrix = Mat4F::CreateTranslationMatrix(m_parent->getSize() * getAnchor());

            m_parent->getWorldTransform().concatenateAffine(
                anchorMatrix * getLocalTransform(),
                m_worldTransform);
        }
        else
        {
            m_worldTransform = getLocalTransform();
        }

        m_flags &= ~Flags::WorldTransformDirty;

        return m_worldTransform;
    }

    //////////////////////////////////////////
    void Transform2D::setParent(Transform2DPtr const& _parent)
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
            getEntityRaw() && getEntityRaw()->getEcsWorld() && _parent && _parent->getEntityRaw() && _parent->getEntityRaw()->getEcsWorld() &&
            getEntityRaw()->getEcsWorld() != _parent->getEntityRaw()->getEcsWorld(), "It's forbidden to connect entities from different worlds!");

        m_parent = _parent;

        m_flags |= ParentChangedCurrentFrame;

        dirtyWorldTransform(
            Flags::WorldTransformDirty | 
            Flags::WorldTransformChangedCurrentFrame |
            Flags::HierarchyChangedCurrentFrame);

        if (m_parent)
        {
            m_parent->m_children.push_back(this);
            m_flags |= ChildrenOrderDirty;
            m_orderOfArrival = s_globalOrderOfArrival++;

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
    void Transform2D::setZ(S32 _z)
    {
        if (m_z == _z)
            return;

        m_z = _z;

        if (m_parent)
            m_parent->m_flags |= ChildrenOrderDirty;
    }

    //////////////////////////////////////////
    bool Transform2D::updateChildrenOrder()
    {
        // Z sort
        if (m_flags & ChildrenOrderDirty)
        {
            S32 i = 0;
            S32 j = 0;
            S32 length = (S32)m_children.size();
            Transform2D* tempItem;

            // Insertion sort
            for( i = 1; i < length; i++)
            {
                tempItem = m_children[i];
                j = i - 1;

                while (j >= 0 && ( tempItem->getZ() < m_children[j]->getZ() || ( tempItem->getZ() == m_children[j]->getZ() && tempItem->m_orderOfArrival < m_children[j]->m_orderOfArrival)))
                {
                    m_children[j + 1] = m_children[j];
                    j = j - 1;
                }
                m_children[j + 1] = tempItem;
            }

            m_flags &= ~ChildrenOrderDirty;
            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    void Transform2D::removeAllChildren()
    {
        while (!m_children.empty())
        {
            m_children.back()->setParent(Transform2DPtr());
        }
    }

    //////////////////////////////////////////
    void Transform2D::destroyAllChildren()
    {
        while (!m_children.empty())
        {
            Transform2D* child = m_children.back();
            Entity* childEntityRaw = child->getEntityRaw();
            EntityPtr childEntity = childEntityRaw->getSharedPtr();
            child->setParent(Transform2DPtr());

            child->destroyAllChildren();

            EcsWorld* world = childEntity->getEcsWorld();
            if (world)
                world->removeEntity(childEntity);
        }
    }

    //////////////////////////////////////////
    void Transform2D::getAllEntitiesRaw(Vector<Entity*>& _result)
    {
        _result.emplace_back(getEntityRaw());

        for (Transform2D* transform : m_children)
            transform->getAllEntitiesRaw(_result);
    }

    //////////////////////////////////////////
    Vector<Entity*> Transform2D::getAllEntitiesRaw()
    {
        Vector<Entity*> result;
        getAllEntitiesRaw(result);
        return result;
    }

    //////////////////////////////////////////
    Transform2D* Transform2D::findChild(std::function<bool(Transform2D*)> _pred)
    {
        for (Transform2D* child : m_children)
        {
            if (_pred(child))
                return child;
        }

        return nullptr;
    }

    //////////////////////////////////////////
    Size Transform2D::getIndexOfChild(Transform2D const* _child)
    {
        if (!_child)
            return Size(-1);

        for (Size i = 0; i < m_children.size(); ++i)
            if (m_children[i] == _child)
                return i;

        return Size(-1);
    }

    //////////////////////////////////////////
    bool Transform2D::containsChild(Transform2D const* _child)
    {
        return (getIndexOfChild(_child) != -1);
    }

    //////////////////////////////////////////
    bool Transform2D::swapChildren(U32 _index0, U32 _index1)
    {
        MAZE_DEBUG_BP_RETURN_VALUE_IF(_index0 >= m_children.size(), false);
        MAZE_DEBUG_BP_RETURN_VALUE_IF(_index1 >= m_children.size(), false);

        std::swap(m_children[_index0], m_children[_index1]);
        std::swap(m_children[_index0]->m_z, m_children[_index1]->m_z);
        std::swap(m_children[_index0]->m_orderOfArrival, m_children[_index1]->m_orderOfArrival);

        return true;
    }

    //////////////////////////////////////////
    void Transform2D::processComponentRemoved()
    {
        setParent(Transform2DPtr());
        removeAllChildren();
    }

    //////////////////////////////////////////
    void Transform2D::processEntityEnabled()
    {
        m_flags |= EntityActiveChangedCurrentFrame;

        for (Transform2D* transform : m_children)
            transform->getEntityRaw()->setDisabledByHierarchy(false);
    }

    //////////////////////////////////////////
    void Transform2D::processEntityDisabled()
    {
        m_flags |= EntityActiveChangedCurrentFrame;

        for (Transform2D* transform : m_children)
            transform->getEntityRaw()->setDisabledByHierarchy(true);
    }

    //////////////////////////////////////////
    bool Transform2D::isMetaPropertyCopyable(MetaProperty* _metaProperty)
    {
        if (GetParentProperty() == _metaProperty)
            return false;

        return true;
    }

    //////////////////////////////////////////
    MetaProperty* Transform2D::GetParentProperty()
    {
        static MetaProperty* s_parentMetaProperty = Transform2D::GetMetaClass()->getProperty("parent");
        return s_parentMetaProperty;
    }
        

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(Transform2DEntityRemoved,
        {},
        {},
        EntityRemovedEvent const& _event,
        Entity* _entity,
        Transform2D* _transform2D)
    {
        if (_transform2D)
        {
            _transform2D->setParent(Transform2DPtr());
            _transform2D->destroyAllChildren();
        }
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(Transform2DPreUpdate,
        {},
        {},
        PreUpdateEvent const& _event,
        Entity* _entity,
        Transform2D* _transform2D)
    {
        _transform2D->processPreUpdate();
    }
    
} // namespace Maze
//////////////////////////////////////////
