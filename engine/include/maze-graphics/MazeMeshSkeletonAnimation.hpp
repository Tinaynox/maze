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
#if (!defined(_MazeMeshSkeletonAnimation_hpp_))
#define _MazeMeshSkeletonAnimation_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeRenderDrawTopology.hpp"
#include "maze-graphics/MazeMeshSkeleton.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/containers/MazeFastVector.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MeshSkeletonAnimation);
    

    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MeshSkeletonAnimationCurve
    {
    public:

        //////////////////////////////////////////
        inline MeshSkeletonAnimationCurve(
            FastVector<F32> const& _times = FastVector<F32>(),
            FastVector<F32> const& _values = FastVector<F32>())
            : m_times(_times)
            , m_values(_values)
        {
            MAZE_DEBUG_ASSERT(m_times.size() == m_values.size());
        }

        //////////////////////////////////////////
        inline MeshSkeletonAnimationCurve(
            FastVector<F32>&& _times,
            FastVector<F32>&& _values)
            : m_times(std::move(_times))
            , m_values(std::move(_values))
        {
            MAZE_DEBUG_ASSERT(m_times.size() == m_values.size());
        }

        //////////////////////////////////////////
        inline MeshSkeletonAnimationCurve(MeshSkeletonAnimationCurve const& _value) = default;

        //////////////////////////////////////////
        inline MeshSkeletonAnimationCurve(MeshSkeletonAnimationCurve&& _value)
            : m_times(std::move(_value.m_times))
            , m_values(std::move(_value.m_values))
        {}

        //////////////////////////////////////////
        inline MeshSkeletonAnimationCurve& operator=(MeshSkeletonAnimationCurve const& _value)
        {
            m_times = _value.m_times;
            m_values = _value.m_values;

            return *this;
        }

        //////////////////////////////////////////
        inline MeshSkeletonAnimationCurve& operator=(MeshSkeletonAnimationCurve&& _value)
        {
            m_times = std::move(_value.m_times);
            m_values = std::move(_value.m_values);

            return *this;
        }


        //////////////////////////////////////////
        inline void setValues(
            FastVector<F32> const& _times,
            FastVector<F32> const& _values)
        {
            m_times = _times;
            m_values = _values;
        }

        //////////////////////////////////////////
        inline void setValues(
            FastVector<F32>&& _times,
            FastVector<F32>&& _values)
        {
            m_times = std::move(_times);
            m_values = std::move(_values);
        }

        //////////////////////////////////////////
        inline void modifyValues(std::function<void(F32&)> const& _cb)
        {
            for (F32& value : m_values)
                _cb(value);
        }

        //////////////////////////////////////////
        inline F32 evaluate(F32 _time) const
        {
            S32 count = (S32)m_values.size();
            if (count == 0)
                return 0.0f;

            if (_time <= m_times[0])
                return m_values[0];

            if (_time >= m_times[count - 1])
                return m_values[count - 1];

            for (S32 i = 1; i < count; ++i)
            {
                if (m_times[i] >= _time)
                {
                    F32 t = (_time - m_times[i - 1]) / (m_times[i] - m_times[i - 1]);
                    return m_values[i - 1] * (1 - t) + m_values[i] * t;
                }
            }

            MAZE_ERROR("Unexpected skeleton curve state!");
            return 0.0f;
        }

        //////////////////////////////////////////
        FastVector<F32> const& getTimes() const { return m_times; }

        //////////////////////////////////////////
        FastVector<F32> const& getValues() const { return m_values; }

    private:
        FastVector<F32> m_times;
        FastVector<F32> m_values;
    };


    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MeshSkeletonRotationCurve
    {
    public:

        //////////////////////////////////////////
        inline MeshSkeletonRotationCurve(
            FastVector<F32> const& _times = FastVector<F32>(),
            FastVector<Quaternion> const& _values = FastVector<Quaternion>())
            : m_times(_times)
            , m_values(_values)
        {
            MAZE_DEBUG_ASSERT(m_times.size() == m_values.size());
        }

        //////////////////////////////////////////
        inline MeshSkeletonRotationCurve(
            FastVector<F32>&& _times,
            FastVector<Quaternion>&& _values)
            : m_times(std::move(_times))
            , m_values(std::move(_values))
        {
            MAZE_DEBUG_ASSERT(m_times.size() == m_values.size());
        }

        //////////////////////////////////////////
        inline MeshSkeletonRotationCurve(MeshSkeletonRotationCurve const& _value) = default;

        //////////////////////////////////////////
        inline MeshSkeletonRotationCurve(MeshSkeletonRotationCurve&& _value)
            : m_times(std::move(_value.m_times))
            , m_values(std::move(_value.m_values))
        {}

        //////////////////////////////////////////
        inline MeshSkeletonRotationCurve& operator=(MeshSkeletonRotationCurve const& _value)
        {
            m_times = _value.m_times;
            m_values = _value.m_values;

            return *this;
        }

        //////////////////////////////////////////
        inline MeshSkeletonRotationCurve& operator=(MeshSkeletonRotationCurve&& _value)
        {
            m_times = std::move(_value.m_times);
            m_values = std::move(_value.m_values);

            return *this;
        }


        //////////////////////////////////////////
        inline void setValues(
            FastVector<F32> const& _times,
            FastVector<Quaternion> const& _values)
        {
            m_times = _times;
            m_values = _values;
        }

        //////////////////////////////////////////
        inline void setValues(
            FastVector<F32>&& _times,
            FastVector<Quaternion>&& _values)
        {
            m_times = std::move(_times);
            m_values = std::move(_values);
        }

        //////////////////////////////////////////
        inline void modifyValues(std::function<void(Quaternion&)> const& _cb)
        {
            for (Quaternion& value : m_values)
                _cb(value);
        }

        //////////////////////////////////////////
        inline Quaternion evaluate(F32 _time) const
        {
            S32 count = (S32)m_values.size();
            if (count == 0)
                return Quaternion::c_identity;

            if (_time <= m_times[0])
                return m_values[0];

            if (_time >= m_times[count - 1])
                return m_values[count - 1];

            for (S32 i = 1; i < count; ++i)
            {
                if (m_times[i] >= _time)
                {
                    F32 t = (_time - m_times[i - 1]) / (m_times[i] - m_times[i - 1]);
                    return Quaternion::Slerp(t, m_values[i - 1], m_values[i]);
                }
            }

            MAZE_ERROR("Unexpected skeleton curve state!");
            return Quaternion::c_identity;
        }

        //////////////////////////////////////////
        FastVector<F32> const& getTimes() const { return m_times; }

        //////////////////////////////////////////
        FastVector<Quaternion> const& getValues() const { return m_values; }

    private:
        FastVector<F32> m_times;
        FastVector<Quaternion> m_values;
    };


    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API MeshSkeletonAnimationBone
    {
        //////////////////////////////////////////
        inline MeshSkeletonAnimationBone() = default;

        //////////////////////////////////////////
        inline MeshSkeletonAnimationBone(MeshSkeletonAnimationBone const& _value) = default;

        //////////////////////////////////////////
        inline MeshSkeletonAnimationBone(MeshSkeletonAnimationBone&& _value)
        {
            this->operator=(std::move(_value));
        }

        //////////////////////////////////////////
        inline MeshSkeletonAnimationBone& operator=(MeshSkeletonAnimationBone const& _value)
        {
            for (S32 i = 0; i < 3; ++i)
            {
                translation[i] = _value.translation[i];
                scale[i] = _value.scale[i];
            }
            rotation = _value.rotation;

            return *this;
        }

        //////////////////////////////////////////
        inline MeshSkeletonAnimationBone& operator=(MeshSkeletonAnimationBone&& _value)
        {
            for (S32 i = 0; i < 3; ++i)
            {
                translation[i] = std::move(_value.translation[i]);
                scale[i] = std::move(_value.scale[i]);
            }
            rotation = std::move(_value.rotation);

            return *this;
        }

        //////////////////////////////////////////
        void evaluateBoneTransform(
            F32 _time,
            Vec3F& _outTranslation,
            Quaternion& _outRotation,
            Vec3F& _outScale) const;

        MeshSkeletonAnimationCurve translation[3];
        MeshSkeletonRotationCurve rotation;
        MeshSkeletonAnimationCurve scale[3];
    };


    //////////////////////////////////////////
    // Class MeshSkeletonAnimation
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MeshSkeletonAnimation
    {
    public:

        //////////////////////////////////////////
        static MeshSkeletonAnimationPtr Create();

        //////////////////////////////////////////
        ~MeshSkeletonAnimation();


        //////////////////////////////////////////
        void clear();


        //////////////////////////////////////////
        inline void setName(String const& _name) { m_name = _name; }

        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }



        //////////////////////////////////////////
        inline Vector<MeshSkeletonAnimationBone> const& getBoneAnimations() const { return m_boneAnimations; }

        //////////////////////////////////////////
        inline void setBoneAnimations(Vector<MeshSkeletonAnimationBone> const& _boneAnimations) { m_boneAnimations = _boneAnimations; }

        //////////////////////////////////////////
        inline void setBoneAnimations(Vector<MeshSkeletonAnimationBone>&& _boneAnimations) { m_boneAnimations = std::move(_boneAnimations); }


        //////////////////////////////////////////
        inline F32 getAnimationTime() const { return m_animationTime; }

        //////////////////////////////////////////
        inline void setAnimationTime(F32 _value) { m_animationTime = _value; }


        //////////////////////////////////////////
        void evaluateBoneTransform(
            MeshSkeleton::BoneIndex _i,
            F32 _time,
            Vec3F& _outTranslation,
            Quaternion& _outRotation,
            Vec3F& _outScale) const;

    protected:

        //////////////////////////////////////////
        MeshSkeletonAnimation();

        //////////////////////////////////////////
        bool init();    
    

    protected:
        String m_name;
        Vector<MeshSkeletonAnimationBone> m_boneAnimations;
        F32 m_animationTime = 0.0f;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshSkeletonAnimation_hpp_
//////////////////////////////////////////
