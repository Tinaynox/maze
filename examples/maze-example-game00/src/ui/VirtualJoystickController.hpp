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
#if (!defined(_VirtualJoystickController_hpp_))
#define _VirtualJoystickController_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(VirtualJoystickController);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(ClickButton2D);


    //////////////////////////////////////////
    // Class VirtualJoystickController
    //
    //////////////////////////////////////////
    class VirtualJoystickController
        : public Component
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(VirtualJoystickController, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(VirtualJoystickController);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~VirtualJoystickController();

        //////////////////////////////////////////
        static VirtualJoystickControllerPtr Create();

        //////////////////////////////////////////
        virtual void update(F32 dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

    protected:

        //////////////////////////////////////////
        VirtualJoystickController();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processComponentAdded() MAZE_OVERRIDE;

    protected:
        Transform2DPtr m_transform;

        ClickButton2DPtr m_leftButton;
        SpriteRenderer2DPtr m_leftStickSprite;
        SpriteRenderer2DPtr m_leftStickPadSprite;

        ClickButton2DPtr m_rightButton;

        Vec2DF m_cursorPositionOS;

        F32 const c_radius;

        GamepadPtr m_gamepad;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _VirtualJoystickController_hpp_
//////////////////////////////////////////
