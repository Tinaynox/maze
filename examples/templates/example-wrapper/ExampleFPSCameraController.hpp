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
#if (!defined(_ExampleFPSCameraController_hpp_))
#define _ExampleFPSCameraController_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include "maze-core/math/MazeRay.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(ExampleFPSCameraController);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    // Class ExampleFPSCameraController
    //
    //////////////////////////////////////////
    class ExampleFPSCameraController
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ExampleFPSCameraController, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ExampleFPSCameraController);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~ExampleFPSCameraController();

        //////////////////////////////////////////
        static ExampleFPSCameraControllerPtr Create();


        //////////////////////////////////////////
        inline Transform3DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        inline Camera3DPtr const& getCamera3D() const { return m_camera3D; }


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        inline bool getForward() const { return m_forward; }

        //////////////////////////////////////////
        inline void setForward(bool _value) { m_forward = _value; }

        //////////////////////////////////////////
        inline bool getBackward() const { return m_backward; }

        //////////////////////////////////////////
        inline void setBackward(bool _value) { m_backward = _value; }

        //////////////////////////////////////////
        inline bool getLeft() const { return m_left; }

        //////////////////////////////////////////
        inline void setLeft(bool _value) { m_left = _value; }

        //////////////////////////////////////////
        inline bool getRight() const { return m_right; }

        //////////////////////////////////////////
        inline void setRight(bool _value) { m_right = _value; }

        //////////////////////////////////////////
        inline bool getJump() const { return m_jump; }

        //////////////////////////////////////////
        inline void setJump(bool _value) { m_jump = _value; }


        //////////////////////////////////////////
        inline F32 getYawAngle() const { return m_yawAngle; }

        //////////////////////////////////////////
        inline void setYawAngle(F32 _value) { m_yawAngle = _value; }

        //////////////////////////////////////////
        inline F32 getPitchAngle() const { return m_pitchAngle; }

        //////////////////////////////////////////
        inline void setPitchAngle(F32 _value) { m_pitchAngle = _value; }


        //////////////////////////////////////////
        inline void setLevelSize(Vec2F32 const& _value) { m_levelSize = _value; }

        //////////////////////////////////////////
        Vec2F32 const& getLevelSize() const { return m_levelSize; }

    protected:

        //////////////////////////////////////////
        ExampleFPSCameraController();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void createElements();

    protected:
        Transform3DPtr m_transform;
        Camera3DPtr m_camera3D;

        AnimationCurve m_jumpCurve;
        F32 m_jumpProgress = 1.0f;

        Vec3F32 m_targetPosition = Vec3F32::c_zero;
        F32 m_yawAngle = 0.0f;
        F32 m_pitchAngle = 0.0f;

        F32 m_speed = 4.0f;

        bool m_forward = false;
        bool m_backward = false;
        bool m_left = false;
        bool m_right = false;
        bool m_jump = false;

        F32 m_radius = 0.5f;
        Vec2F32 m_levelSize = Vec2F32(5.0f, 10.0f);
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _ExampleFPSCameraController_hpp_
//////////////////////////////////////////
