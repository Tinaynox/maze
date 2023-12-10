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
#if (!defined(_MazeLinearMovement3D_hpp_))
#define _MazeLinearMovement3D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRotation2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(LinearMovement3D);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    // Class LinearMovement3D
    //
    //////////////////////////////////////////
    class MAZE_CORE_API LinearMovement3D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(LinearMovement3D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(LinearMovement3D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~LinearMovement3D();

        //////////////////////////////////////////
        static LinearMovement3DPtr Create(Vec3F const& _axis = Vec3F::c_unitY, F32 _speed = 5.0f);


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        inline bool getActive() const { return m_active; }

        //////////////////////////////////////////
        inline void setActive(bool _active) { m_active = _active; }


        //////////////////////////////////////////
        inline Vec3F const& getAxis() const { return m_axis; }

        //////////////////////////////////////////
        inline void setAxis(Vec3F const& _axis) { m_axis = _axis; }


        //////////////////////////////////////////
        inline F32 getSpeed() const { return m_speed; }

        //////////////////////////////////////////
        inline void setSpeed(F32 _speed) { m_speed = _speed; }

    protected:

        //////////////////////////////////////////
        LinearMovement3D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(Vec3F const& _axis = Vec3F::c_unitY, F32 _speed = 5.0f);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        bool m_active = true;
        Vec3F m_axis = Vec3F::c_unitY;
        F32 m_speed = 5.0f;

        Transform3DPtr m_transform;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLinearMovement3D_hpp_
//////////////////////////////////////////
