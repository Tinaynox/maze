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
#if (!defined(_MazeTransform2D_hpp_))
#define _MazeTransform2D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/math/MazeRotation2D.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Transform2D);


    //////////////////////////////////////////
    // Class Transform2D
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Transform2D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Transform2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Transform2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        static U32 s_globalOrderOfArrival;

        //////////////////////////////////////////
        enum Flags
        {
            LocalTransformDirty               = MAZE_BIT(0),
            WorldTransformDirty               = MAZE_BIT(1),
            ChildrenOrderDirty                = MAZE_BIT(2),

            LocalTransformChangedCurrentFrame = MAZE_BIT(8),  LocalTransformChangedPreviousFrame = MAZE_BIT(9),
            WorldTransformChangedCurrentFrame = MAZE_BIT(10), WorldTransformChangedPreviousFrame = MAZE_BIT(11),
            ParentChangedCurrentFrame         = MAZE_BIT(12), ParentChangedPreviousFrame         = MAZE_BIT(13),
            HierarchyChangedCurrentFrame      = MAZE_BIT(14), HierarchyChangedPreviousFrame      = MAZE_BIT(15),
            AnchorChangedCurrentFrame         = MAZE_BIT(16), AnchorChangedPreviousFrame         = MAZE_BIT(17),
            SizeChangedCurrentFrame           = MAZE_BIT(18), SizeChangedPreviousFrame           = MAZE_BIT(19),
            EntityActiveChangedCurrentFrame   = MAZE_BIT(20), EntityActiveChangedPreviousFrame   = MAZE_BIT(21)
        };

    public:

        //////////////////////////////////////////
        virtual ~Transform2D();

        //////////////////////////////////////////
        static Transform2DPtr Create();


        //////////////////////////////////////////
        inline Vec2DF const& getLocalPosition() const { return m_localPosition; }

        //////////////////////////////////////////
        void setLocalPosition(Vec2DF const& _localPosition);

        //////////////////////////////////////////
        inline void setLocalPosition(F32 _x, F32 _y)
        {
            setLocalPosition(Vec2DF(_x, _y));
        }

        //////////////////////////////////////////
        inline void setLocalX(F32 _x)
        {
            setLocalPosition(Vec2DF(_x, m_localPosition.y));
        }

        //////////////////////////////////////////
        inline void setLocalY(F32 _y)
        {
            setLocalPosition(Vec2DF(m_localPosition.x, _y));
        }

        //////////////////////////////////////////
        void translate(Vec2DF const& _offset);

        //////////////////////////////////////////
        inline Vec2DF getWorldPosition() { return getWorldTransform().transformAffine(Vec2DF::c_zero); }


        //////////////////////////////////////////
        void calculateLocalCorners(Vec2DF _corners[4]);

        //////////////////////////////////////////
        AABB2D calculateLocalAABB();

        //////////////////////////////////////////
        void calculateWorldCorners(Vec2DF _corners[4]);

        //////////////////////////////////////////
        AABB2D calculateWorldAABB();


        //////////////////////////////////////////
        void setPivot(Vec2DF const& _pivot);

        //////////////////////////////////////////
        inline void setPivot(F32 _x, F32 _y) { setPivot(Vec2DF(_x, _y)); }

        //////////////////////////////////////////
        inline Vec2DF const& getPivot() const { return m_pivot; }

        //////////////////////////////////////////
        void setSize(Vec2DF const& _size);

        //////////////////////////////////////////
        inline void setSize(F32 _x, F32 _y) { setSize(Vec2DF(_x, _y)); }

        //////////////////////////////////////////
        inline Vec2DF const& getSize() const { return m_size; }

        //////////////////////////////////////////
        inline F32 getWidth() const { return m_size.x; }

        //////////////////////////////////////////
        inline void setWidth(F32 _w) { setSize(_w, m_size.y); }

        //////////////////////////////////////////
        inline F32 getHeight() const { return m_size.y; }

        //////////////////////////////////////////
        inline void setHeight(F32 _h) { setSize(m_size.x, _h); }

        //////////////////////////////////////////
        void setAnchor(Vec2DF const& _anchor);

        //////////////////////////////////////////
        inline void setAnchor(F32 _x, F32 _y) { setAnchor(Vec2DF(_x, _y)); }

        //////////////////////////////////////////
        inline Vec2DF const& getAnchor() const { return m_anchor; }


        //////////////////////////////////////////
        Rotation2D const& getLocalRotation() const { return m_localRotation; }

        //////////////////////////////////////////
        void setLocalRotation(Rotation2D const& _localRotation);



        //////////////////////////////////////////
        Vec2DF const& getLocalScale() const { return m_localScale; }

        //////////////////////////////////////////
        void setLocalScale(Vec2DF const& _localScale);

        //////////////////////////////////////////
        inline void setLocalScale(F32 _localScale) { setLocalScale(Vec2DF(_localScale)); }

        //////////////////////////////////////////
        inline void setLocalScale(F32 _x, F32 _y) { setLocalScale(Vec2DF(_x, _y)); }


        //////////////////////////////////////////
        Mat4DF const& getLocalTransform();

        //////////////////////////////////////////
        inline bool isLocalTransformDirty() { return m_flags & Flags::LocalTransformDirty; }

        //////////////////////////////////////////
        inline bool isLocalTransformChangedCurrentFrame() { return m_flags & Flags::LocalTransformChangedCurrentFrame; }

        //////////////////////////////////////////
        inline bool isLocalTransformChangedPreviousFrame() { return m_flags & Flags::LocalTransformChangedPreviousFrame; }

        //////////////////////////////////////////
        inline bool isLocalTransformChanged() { return isLocalTransformChangedCurrentFrame() || isLocalTransformChangedPreviousFrame(); }

        //////////////////////////////////////////
        Mat4DF const& calculateLocalTransform();

        //////////////////////////////////////////
        void setLocalTransform(Mat4DF const& _localTransform);


        //////////////////////////////////////////
        Mat4DF const& getWorldTransform();

        //////////////////////////////////////////
        inline bool isWorldTransformDirty() { return m_flags & Flags::WorldTransformDirty; }

        //////////////////////////////////////////
        inline bool isWorldTransformChangedCurrentFrame() { return m_flags & Flags::WorldTransformChangedCurrentFrame; }

        //////////////////////////////////////////
        inline bool isWorldTransformChangedPreviousFrame() { return m_flags & Flags::WorldTransformChangedPreviousFrame; }

        //////////////////////////////////////////
        inline bool isWorldTransformChanged() { return isWorldTransformChangedCurrentFrame() || isWorldTransformChangedPreviousFrame(); }

        //////////////////////////////////////////
        Mat4DF const& calculateWorldTransform();


        //////////////////////////////////////////
        inline bool isParentChangedCurrentFrame() { return m_flags & Flags::ParentChangedCurrentFrame; }

        //////////////////////////////////////////
        inline bool isParentChangedPreviousFrame() { return m_flags & Flags::ParentChangedPreviousFrame; }

        //////////////////////////////////////////
        inline bool isParentChanged() { return isParentChangedCurrentFrame() || isParentChangedPreviousFrame(); }

        //////////////////////////////////////////
        inline bool isHierarchyChangedCurrentFrame() { return m_flags & Flags::HierarchyChangedCurrentFrame; }

        //////////////////////////////////////////
        inline bool isHierarchyChangedPreviousFrame() { return m_flags & Flags::HierarchyChangedPreviousFrame; }

        //////////////////////////////////////////
        inline bool isHierarchyChanged() { return isHierarchyChangedCurrentFrame() || isHierarchyChangedPreviousFrame(); }


        //////////////////////////////////////////
        void setParent(Transform2DPtr const& _parent);

        //////////////////////////////////////////
        Transform2DPtr const& getParent() const { return m_parent; }

        //////////////////////////////////////////
        inline void setParent(ComponentPtr _value) { setParent(_value ? _value->cast<Transform2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getParentComponent() const { return m_parent; }

        //////////////////////////////////////////
        inline void resetParent() { setParent(Transform2DPtr()); }


        //////////////////////////////////////////
        void setZ(S32 _z);

        //////////////////////////////////////////
        inline S32 getZ() const { return m_z; }

        //////////////////////////////////////////
        inline U32 getOrderOfArrival() const { return m_orderOfArrival; }

        //////////////////////////////////////////
        inline void dirtyWorldTransform(S32 _flags = Flags::WorldTransformDirty | Flags::WorldTransformChangedCurrentFrame)
        {
            _dirtyWorldTransformRecursive(_flags);
        }

        //////////////////////////////////////////
        inline void processEndFrame()
        {
            S32 flags = m_flags;

            flags &= ~(
                LocalTransformChangedPreviousFrame |
                WorldTransformChangedPreviousFrame |
                ParentChangedPreviousFrame |
                HierarchyChangedPreviousFrame |
                AnchorChangedPreviousFrame |
                SizeChangedPreviousFrame |
                EntityActiveChangedPreviousFrame);
            
            flags |=
                (m_flags & LocalTransformChangedCurrentFrame) << 1 |
                (m_flags & WorldTransformChangedCurrentFrame) << 1 |
                (m_flags & ParentChangedCurrentFrame) << 1 |
                (m_flags & HierarchyChangedCurrentFrame) << 1 |
                (m_flags & AnchorChangedCurrentFrame) << 1 |
                (m_flags & SizeChangedCurrentFrame) << 1 |
                (m_flags & EntityActiveChangedCurrentFrame) << 1;

            flags &= ~(
                LocalTransformChangedCurrentFrame | 
                WorldTransformChangedCurrentFrame |
                ParentChangedCurrentFrame |
                HierarchyChangedCurrentFrame |
                AnchorChangedCurrentFrame |
                SizeChangedCurrentFrame |
                EntityActiveChangedCurrentFrame);

            m_flags = flags;
        }

        //////////////////////////////////////////
        inline bool isSizeChangedCurrentFrame() { return m_flags & Flags::SizeChangedCurrentFrame; }

        //////////////////////////////////////////
        inline bool isSizeChangedPreviousFrame() { return m_flags & Flags::SizeChangedPreviousFrame; }

        //////////////////////////////////////////
        inline bool isSizeChanged() { return isSizeChangedCurrentFrame() || isSizeChangedPreviousFrame(); }

        //////////////////////////////////////////
        inline bool isEntityActiveChangedCurrentFrame() { return m_flags & Flags::EntityActiveChangedCurrentFrame; }

        //////////////////////////////////////////
        inline bool isEntityActiveChangedPreviousFrame() { return m_flags & Flags::EntityActiveChangedPreviousFrame; }

        //////////////////////////////////////////
        inline bool isEntityActiveChanged() { return isEntityActiveChangedCurrentFrame() || isEntityActiveChangedPreviousFrame(); }

        //////////////////////////////////////////
        Vector<Transform2D*> const& getChildren() const { return m_children; }

        //////////////////////////////////////////
        inline Transform2D* getChild(Size _i) { return m_children[_i]; }

        //////////////////////////////////////////
        Transform2D* findChild(std::function<bool(Transform2D*)> _pred);

        //////////////////////////////////////////
        template <typename TComponent>
        TComponent* findChildComponent(std::function<bool(Transform2D*)> _pred)
        {
            for (Transform2D* child : m_children)
            {
                TComponent* result = child->getEntityRaw()->getComponentRaw<TComponent>();
                if (!result)
                    continue;

                if (_pred(child))
                    return result;
            }

            return nullptr;
        }

        //////////////////////////////////////////
        template <typename TComponent>
        TComponent* findChildComponent(String const& _name)
        {
            for (Transform2D* child : m_children)
            {
                TComponent* result = child->getEntityRaw()->getComponentRaw<TComponent>();
                if (!result)
                    continue;

                Name* name = child->getEntityRaw()->getComponentRaw<Name>();
                if (!name)
                    continue;

                if (name->getName() == _name)
                    return result;
            }

            return nullptr;
        }

        //////////////////////////////////////////
        template <typename TComponent>
        TComponent* findChildComponentRecursive(String const& _name)
        {
            for (Transform2D* child : m_children)
            {
                TComponent* result = child->getEntityRaw()->getComponentRaw<TComponent>();
                if (result)
                {

                    Name* name = child->getEntityRaw()->getComponentRaw<Name>();
                    if (name)
                    {
                        if (name->getName() == _name)
                            return result;
                    }
                }

                result = child->findChildComponentRecursive<TComponent>(_name);
                if (result)
                    return result;
            }

            return nullptr;
        }

        //////////////////////////////////////////
        Size getIndexOfChild(Transform2D const* _child);

        //////////////////////////////////////////
        bool containsChild(Transform2D const* _child);

        //////////////////////////////////////////
        bool swapChildren(U32 _index0, U32 _index1);

        //////////////////////////////////////////
        bool updateChildrenOrder();


        //////////////////////////////////////////
        void removeAllChildren();

        //////////////////////////////////////////
        void destroyAllChildren();


        //////////////////////////////////////////
        template <typename TComponent>
        void getAllComponents(Vector<SharedPtr<TComponent>>& _result)
        {
            SharedPtr<TComponent> component = getEntityRaw()->getComponentInheritedFrom<TComponent>();
            if (component)
                _result.emplace_back(component);

            for (Transform2D* transform : m_children)
                transform->template getAllComponents<TComponent>(_result);
        }

        //////////////////////////////////////////
        template <typename TComponent>
        Vector<SharedPtr<TComponent>> getAllComponents()
        {
            Vector<SharedPtr<TComponent>> result;
            this->template getAllComponents<TComponent>(result);
            return result;
        }

        //////////////////////////////////////////
        template <typename TComponent>
        void getAllComponentsRaw(Vector<TComponent*>& _result, bool _activeOnly = false)
        {
            if (_activeOnly && !getEntityRaw()->getActiveSelf())
                return;

            TComponent* component = getEntityRaw()->getComponentRawInheritedFrom<TComponent>();
            if (component)
                _result.emplace_back(component);

            for (Transform2D* transform : m_children)
                transform->template getAllComponentsRaw<TComponent>(_result, _activeOnly);
        }

        //////////////////////////////////////////
        template <typename TComponent>
        Vector<TComponent*> getAllComponentsRaw(bool _activeOnly = false)
        {
            Vector<TComponent*> result;
            this->template getAllComponentsRaw<TComponent>(result, _activeOnly);
            return result;
        }


        //////////////////////////////////////////
        inline Size getNestingLevel()
        {
            if (m_parent)
                return m_parent->getNestingLevel() + 1;

            return 0;
        }


        //////////////////////////////////////////
        inline Transform2D* getRootTransform()
        {
            if (m_parent)
                return m_parent->getRootTransform();

            return this;
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline TComponent* getFirstTrunkComponent()
        {
            TComponent* component = this->getEntityRaw()->template getComponentRaw<TComponent>();
            if (component)
                return component;

            if (m_parent)
                return m_parent->template getFirstTrunkComponent<TComponent>();

            return nullptr;
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline TComponent* getLastTrunkComponent()
        {
            TComponent* component = nullptr;
            getLastTrunkComponentHelper<TComponent>(component);
            return component;
        }

        //////////////////////////////////////////
        virtual bool isMetaPropertyCopyable(MetaProperty* _metaProperty) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        template <typename TComponent>
        inline void getLastTrunkComponentHelper(TComponent*& _value)
        {
            TComponent* component = this->getEntityRaw()->template getComponentRaw<TComponent>();
            if (component)
                _value = component;

            if (m_parent)
                m_parent->template getLastTrunkComponentHelper<TComponent>(_value);
        }

    protected:

        //////////////////////////////////////////
        Transform2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            ECSWorld* _world,
            EntityCopyData _copyData = EntityCopyData()) MAZE_OVERRIDE;

        ////////////////////////////////////
        inline void _dirtyWorldTransformRecursive(S32 _flags)
        {
            m_flags |= _flags;

            for (Transform2D* transform : m_children)
                transform->_dirtyWorldTransformRecursive(_flags);
        }

        //////////////////////////////////////////
        void processComponentRemoved() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void processEntityRemoved() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void processEntityEnabled() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void processEntityDisabled() MAZE_OVERRIDE;


    public:
        //////////////////////////////////////////
        static MetaProperty* GetParentProperty();

    protected:

        Vec2DF m_localPosition;
        Rotation2D m_localRotation;
        Vec2DF m_localScale;
        Vec2DF m_pivot;
        Vec2DF m_size;
        Vec2DF m_anchor;

        S32 m_flags;
        Mat4DF m_localTransform;
        Mat4DF m_worldTransform;

        Transform2DPtr m_parent;
        Vector<Transform2D*> m_children;

        S32 m_z;
        U32 m_orderOfArrival;
    };


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_COMPONENT_SERIALIZATION(Transform2D);


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTransform2D_hpp_
//////////////////////////////////////////
