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
#if (!defined(_MazeParticles3D_hpp_))
#define _MazeParticles3D_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/math/MazeAABB3D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Particles3D);


    //////////////////////////////////////////
    // Class Particles3D
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API Particles3D
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(Particles3D);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Particles3D);

    public:

        //////////////////////////////////////////
        struct ParticleLife
        {
            F32 initial;
            F32 current;
            F32 scalar;
        };

        //////////////////////////////////////////
        struct ParticleSize
        {
            F32 initial;
            F32 current;
        };

        //////////////////////////////////////////
        struct ParticleRotation
        {
            F32 initial;
            F32 current;
        };

        //////////////////////////////////////////
        struct ParticleMovement
        {
            Vec3F velocity;
            Vec3F acceleration;
        };

        //////////////////////////////////////////
        struct ParticleAnimationFrame
        {
            F32 initial;
            F32 current;
        };


    public:

        //////////////////////////////////////////
        Particles3D();

        //////////////////////////////////////////
        void setCapacity(S32 _count);

        //////////////////////////////////////////
        inline S32 getCapacity() const { return m_capacity; }


        //////////////////////////////////////////
        inline S32 getAliveCount() const { return m_aliveCount; }

        //////////////////////////////////////////
        inline void setAliveCount(S32 _aliveCount) { m_aliveCount = _aliveCount; }

        //////////////////////////////////////////
        inline void addAliveCount(S32 _count)
        {
            S32 newAliveCount = m_aliveCount + _count;
            if (newAliveCount > m_capacity)
                setCapacity((S32)Math::GetNextPowerOfTwo(newAliveCount));
            
            m_aliveCount = newAliveCount;
        }


        //////////////////////////////////////////
        void update();


        //////////////////////////////////////////
        inline void resetData(S32 _index0, S32 _index1)
        {
            m_seeds[_index0] = m_seeds[_index1];
            m_positions[_index0] = m_positions[_index1];
            m_directions[_index0] = m_directions[_index1];
            m_rotations[_index0] = m_rotations[_index1];
            m_lifes[_index0] = m_lifes[_index1];
            m_sizes[_index0] = m_sizes[_index1];
            m_colorsInitial[_index0] = m_colorsInitial[_index1];
            m_colorsCurrent[_index0] = m_colorsCurrent[_index1];
            m_movements[_index0] = m_movements[_index1];
            m_animationFrames[_index0] = m_animationFrames[_index1];
            
        }

        //////////////////////////////////////////
        inline void swapData(S32 _index0, S32 _index1)
        {
            std::swap(m_seeds[_index0], m_seeds[_index1]);
            std::swap(m_positions[_index0], m_positions[_index1]);
            std::swap(m_directions[_index0], m_directions[_index1]);
            std::swap(m_rotations[_index0], m_rotations[_index1]);
            std::swap(m_lifes[_index0], m_lifes[_index1]);
            std::swap(m_sizes[_index0], m_sizes[_index1]);
            std::swap(m_colorsInitial[_index0], m_colorsInitial[_index1]);
            std::swap(m_colorsCurrent[_index0], m_colorsCurrent[_index1]);
            std::swap(m_movements[_index0], m_movements[_index1]);
            std::swap(m_animationFrames[_index0], m_animationFrames[_index1]);
        }

        //////////////////////////////////////////
        inline void clearData(S32 _indexFirst, S32 _indexLast)
        {
            S32 count = _indexLast - _indexFirst;

            memset(m_seeds.begin() + _indexFirst, 0, sizeof(m_seeds[0]) * count);
            memset(m_positions.begin() + _indexFirst, 0, sizeof(m_positions[0]) * count);
            memset(m_directions.begin() + _indexFirst, 0, sizeof(m_directions[0]) * count);
            memset(m_rotations.begin() + _indexFirst, 0, sizeof(m_rotations[0]) * count);
            memset(m_lifes.begin() + _indexFirst, 0, sizeof(m_lifes[0]) * count);
            memset(m_sizes.begin() + _indexFirst, 0, sizeof(m_sizes[0]) * count);
            memset(m_colorsInitial.begin() + _indexFirst, 0, sizeof(m_colorsInitial[0]) * count);
            memset(m_colorsCurrent.begin() + _indexFirst, 0, sizeof(m_colorsCurrent[0]) * count);
            memset(m_movements.begin() + _indexFirst, 0, sizeof(m_movements[0]) * count);
            memset(m_animationFrames.begin() + _indexFirst, 0, sizeof(m_animationFrames[0]) * count);            
        }


        //////////////////////////////////////////
        inline S32& accessSeed(S32 _index) { return m_seeds[_index]; }

        //////////////////////////////////////////
        inline Vec3F& accessPosition(S32 _index) { return m_positions[_index]; }

        //////////////////////////////////////////
        inline Vec3F& accessDirection(S32 _index) { return m_directions[_index]; }

        //////////////////////////////////////////
        inline ParticleLife& accessLife(S32 _index) { return m_lifes[_index]; }

        //////////////////////////////////////////
        inline ParticleSize& accessSize(S32 _index) { return m_sizes[_index]; }

        //////////////////////////////////////////
        inline ParticleRotation& accessRotation(S32 _index) { return m_rotations[_index]; }

        //////////////////////////////////////////
        inline Vec4F& accessColorInitial(S32 _index) { return m_colorsInitial[_index]; }

        //////////////////////////////////////////
        inline Vec4F& accessColorCurrent(S32 _index) { return m_colorsCurrent[_index]; }

        //////////////////////////////////////////
        inline ParticleMovement& accessMovement(S32 _index) { return m_movements[_index]; }

        //////////////////////////////////////////
        inline ParticleAnimationFrame& accessAnimationFrame(S32 _index) { return m_animationFrames[_index]; }


        //////////////////////////////////////////
        inline TMat& accessRenderTransform(S32 _index) { return m_renderTransforms[_index]; }

        //////////////////////////////////////////
        inline Vec4F& accessRenderColor(S32 _index) { return m_renderColors[_index]; }

        //////////////////////////////////////////
        inline Vec4F& accessRenderUV(S32 _index) { return m_renderUVs[_index]; }


        //////////////////////////////////////////
        inline F32& accessSqrDistanceToCamera(S32 _index) { return m_sqrDistanceToCamera[_index]; }


        //////////////////////////////////////////
        inline TMat const* getRenderTransforms() const { return m_renderTransforms.begin(); }

        //////////////////////////////////////////
        inline Vec4F const* getRenderColors() const { return m_renderColors.begin(); }

        //////////////////////////////////////////
        inline Vec4F const* getRenderUVs() const { return m_renderUVs.begin(); }

    protected:
        S32 m_capacity;
        S32 m_aliveCount;
        AABB3D m_bounds;

        FastVector<S32> m_seeds;
        FastVector<Vec3F> m_positions;
        FastVector<Vec3F> m_directions;
        FastVector<ParticleLife> m_lifes;
        FastVector<ParticleSize> m_sizes;
        FastVector<ParticleRotation> m_rotations;
        FastVector<Vec4F> m_colorsInitial;
        FastVector<Vec4F> m_colorsCurrent;
        FastVector<ParticleMovement> m_movements;
        FastVector<ParticleAnimationFrame> m_animationFrames;

        FastVector<F32> m_sqrDistanceToCamera;

        FastVector<TMat> m_renderTransforms;
        FastVector<Vec4F> m_renderColors;
        FastVector<Vec4F> m_renderUVs;

    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeParticles3D_hpp_
//////////////////////////////////////////
