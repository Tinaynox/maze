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
#if (!defined(_MazeSinMovement3D_hpp_))
#define _MazeSinMovement3D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRotation2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SinMovement3D);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    // Class SinMovement3D
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SinMovement3D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SinMovement3D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SinMovement3D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~SinMovement3D();

        //////////////////////////////////////////
        static SinMovement3DPtr Create(
            Vec3F const& _axis = Vec3F::c_unitY,
            F32 _frequency = 2.0f,
            F32 _amplitude = 1.0f);


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        inline Vec3F const& getAxis() const { return m_axis; }

        //////////////////////////////////////////
        inline void setAxis(Vec3F const& _axis) { m_axis = _axis; }


        //////////////////////////////////////////
        inline F32 getFrequency() const { return m_frequency; }

        //////////////////////////////////////////
        inline void setFrequency(F32 _frequency) { m_frequency = _frequency; }


        //////////////////////////////////////////
        inline F32 getAmplitude() const { return m_amplitude; }

        //////////////////////////////////////////
        inline void setAmplitude(F32 _amplitude) { m_amplitude = _amplitude; }

    protected:

        //////////////////////////////////////////
        SinMovement3D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(
            Vec3F const& _axis = Vec3F::c_unitY,
            F32 _frequency = 2.0f,
            F32 _amplitude = 1.0f);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        Vec3F m_axis;
        F32 m_frequency;
        F32 m_amplitude;
        Vec3F m_startPosition;

        F32 m_timer;

        Transform3DPtr m_transform;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSinMovement3D_hpp_
//////////////////////////////////////////
