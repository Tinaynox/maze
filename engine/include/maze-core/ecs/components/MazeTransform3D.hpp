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
#if (!defined(_MazeTransform3D_hpp_))
#define _MazeTransform3D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    // Class Transform3D
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Transform3D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Transform3D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Transform3D);

        //////////////////////////////////////////
        friend class Entity;

    protected:

        //////////////////////////////////////////
        enum Flags
        {
            LocalTransformDirty               = MAZE_BIT(0),
            WorldTransformDirty               = MAZE_BIT(1),

            LocalTransformChangedCurrentFrame = MAZE_BIT(2), LocalTransformChangedPreviousFrame = MAZE_BIT(3),
            WorldTransformChangedCurrentFrame = MAZE_BIT(4), WorldTransformChangedPreviousFrame = MAZE_BIT(5),
            ParentChangedCurrentFrame         = MAZE_BIT(6), ParentChangedPreviousFrame         = MAZE_BIT(7),
            HierarchyChangedCurrentFrame      = MAZE_BIT(8), HierarchyChangedPreviousFrame      = MAZE_BIT(9)
        };

    public:

        //////////////////////////////////////////
        virtual ~Transform3D();

        //////////////////////////////////////////
        static Transform3DPtr Create();


        //////////////////////////////////////////
        inline Vec3DF const& getLocalPosition() const { return m_localPosition; }

        //////////////////////////////////////////
        inline Vec2DF getLocalPositionXY() const { return { m_localPosition.x, m_localPosition.y }; }

        //////////////////////////////////////////
        void setLocalPosition(Vec3DF const& _localPosition);

        //////////////////////////////////////////
        inline void setLocalPosition(F32 _x, F32 _y, F32 _z = 0.0f) { setLocalPosition(Vec3DF(_x, _y, _z)); }

        //////////////////////////////////////////
        inline void setLocalX(F32 _x) { setLocalPosition(Vec3DF(_x, m_localPosition.y, m_localPosition.z)); }

        //////////////////////////////////////////
        inline void setLocalY(F32 _y) { setLocalPosition(Vec3DF(m_localPosition.x, _y, m_localPosition.z)); }

        //////////////////////////////////////////
        inline void setLocalZ(F32 _z) { setLocalPosition(Vec3DF(m_localPosition.x, m_localPosition.y, _z)); }

        //////////////////////////////////////////
        void translate(Vec3DF const& _offset);

        //////////////////////////////////////////
        void translate(Vec2DF const& _offset);

        //////////////////////////////////////////
        inline Vec3DF getWorldPosition() { return getWorldTransform().getAffineTranslation(); }


        //////////////////////////////////////////
        Quaternion const& getLocalRotation() const { return m_localRotation; }

        //////////////////////////////////////////
        void setLocalRotation(Quaternion const& _localRotation);

        //////////////////////////////////////////
        void setLocalDirection(Vec3DF const& _localDirection);

        //////////////////////////////////////////
        inline void setLocalDirection(F32 _x, F32 _y, F32 _z) { setLocalDirection({ _x, _y, _z }); }


        //////////////////////////////////////////
        Vec3DF const& getLocalScale() const { return m_localScale; }

        //////////////////////////////////////////
        void setLocalScale(Vec3DF const& _localScale);

        //////////////////////////////////////////
        inline void setLocalScale(F32 _scale) { setLocalScale(Vec3DF(_scale, _scale, _scale)); }

        //////////////////////////////////////////
        inline void setLocalScale(F32 _scaleX, F32 _scaleY) { setLocalScale(Vec3DF(_scaleX, _scaleY, 1.0f)); }

        //////////////////////////////////////////
        inline void setLocalScale(F32 _scaleX, F32 _scaleY, F32 _scaleZ) { setLocalScale(Vec3DF(_scaleX, _scaleY, _scaleZ)); }


        //////////////////////////////////////////
        void rotate(Vec3DF const& _axis, F32 _angle);


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
        Mat4DF const& calculateWorldTransform();


        //////////////////////////////////////////
        inline Vec3DF getWorldForwardDirection()
        {
            Mat4DF mat = getWorldTransform();
            mat[0][3] = 0.0f;
            mat[1][3] = 0.0f;
            mat[2][3] = 0.0f;
            return (mat.transformAffine(Vec3DF::c_unitZ)).normalizedCopy();
        } 

        //////////////////////////////////////////
        inline Vec3DF getWorldUpDirection()
        {
            Mat4DF mat = getWorldTransform();
            mat[0][3] = 0.0f;
            mat[1][3] = 0.0f;
            mat[2][3] = 0.0f;
            return (mat.transformAffine(Vec3DF::c_unitY)).normalizedCopy();
        }


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
        void setParent(Transform3DPtr const& _parent);

        //////////////////////////////////////////
        Transform3DPtr const& getParent() const { return m_parent; }

        //////////////////////////////////////////
        inline void setParent(ComponentPtr _value) { setParent(_value ? _value->cast<Transform3D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getParentComponent() const { return m_parent; }

        //////////////////////////////////////////
        inline void resetParent() { setParent(Transform3DPtr()); }



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
                HierarchyChangedPreviousFrame);

            flags |=
                (m_flags & LocalTransformChangedCurrentFrame) << 1 |
                (m_flags & WorldTransformChangedCurrentFrame) << 1 |
                (m_flags & ParentChangedCurrentFrame) << 1 |
                (m_flags & HierarchyChangedCurrentFrame) << 1;

            flags &= ~(
                LocalTransformChangedCurrentFrame | 
                WorldTransformChangedCurrentFrame |
                ParentChangedCurrentFrame |
                HierarchyChangedCurrentFrame);

            m_flags = flags;
        }

        //////////////////////////////////////////
        Vector<Transform3D*> const& getChildren() const { return m_children; }


        //////////////////////////////////////////
        void removeAllChildren();

        //////////////////////////////////////////
        void destroyAllChildren();


        //////////////////////////////////////////
        void getAllEntitiesRaw(Vector<Entity*>& _result);

        //////////////////////////////////////////
        Vector<Entity*> getAllEntitiesRaw();


        //////////////////////////////////////////
        template <typename TComponent>
        void getAllComponents(Vector<SharedPtr<TComponent>>& _result)
        {
            SharedPtr<TComponent> component = getEntityRaw()->getComponentInheritedFrom<TComponent>();
            if (component)
                _result.emplace_back(component);

            for (Transform3D* transform : m_children)
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
        void getAllComponentsRaw(Vector<TComponent*>& _result)
        {
            TComponent* component = getEntityRaw()->getComponentRawInheritedFrom<TComponent>();
            if (component)
                _result.emplace_back(component);

            for (Transform3D* transform : m_children)
                transform->template getAllComponentsRaw<TComponent>(_result);
        }

        //////////////////////////////////////////
        template <typename TComponent>
        Vector<TComponent*> getAllComponentsRaw()
        {
            Vector<TComponent*> result;
            this->template getAllComponentsRaw<TComponent>(result);
            return result;
        }

        //////////////////////////////////////////
        virtual bool isMetaPropertyCopyable(MetaProperty* _metaProperty) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        Transform3D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            ECSWorld* _world,
            EntityCopyData _copyData) MAZE_OVERRIDE;

        ////////////////////////////////////
        inline void _dirtyWorldTransformRecursive(S32 _flags)
        {
            m_flags |= _flags;

            for (Transform3D* transform : m_children)
                transform->_dirtyWorldTransformRecursive(_flags);
        }

        //////////////////////////////////////////
        void processComponentAdded() MAZE_OVERRIDE;

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
        Vec3DF m_localPosition;
        Quaternion m_localRotation;
        Vec3DF m_localScale;

        S32 m_flags;
        Mat4DF m_localTransform;
        Mat4DF m_worldTransform;

        Transform3DPtr m_parent;
        Vector<Transform3D*> m_children;
    };


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_COMPONENT_SERIALIZATION(Transform3D);


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTransform3D_hpp_
//////////////////////////////////////////
