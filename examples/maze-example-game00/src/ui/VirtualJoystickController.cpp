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
#include "VirtualJoystickController.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "Game.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class VirtualJoystickController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(VirtualJoystickController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(VirtualJoystickController);

    //////////////////////////////////////////
    VirtualJoystickController::VirtualJoystickController()
        : m_cursorPositionOS(Vec2DF::c_zero)
        , c_radius(160 * 0.5f)
    {
    }

    //////////////////////////////////////////
    VirtualJoystickController::~VirtualJoystickController()
    {

    }

    //////////////////////////////////////////
    VirtualJoystickControllerPtr VirtualJoystickController::Create()
    {
        VirtualJoystickControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(VirtualJoystickController, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool VirtualJoystickController::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        m_gamepad = GamepadManager::GetInstancePtr()->getVirtualGamepad();

        return true;
    }

    //////////////////////////////////////////
    void VirtualJoystickController::update(F32 _dt)
    {
        if (m_leftButton->getUIElement()->getPressed())
        {
            Vec2DF pos = m_cursorPositionOS - m_leftStickSprite->getTransform()->getSize() * m_leftStickPadSprite->getTransform()->getPivot();
            Vec2DF shift = pos / c_radius;
            if (shift.squaredLength() >= 1.0f)
                shift.normalize();

            m_gamepad->setAxisState(0, shift.x);
            m_gamepad->setAxisState(1, shift.y);

            m_leftStickPadSprite->getTransform()->setLocalPosition(
                shift * c_radius);
                        
        }
        else
        {
            m_gamepad->setAxisState(0, 0.0f);
            m_gamepad->setAxisState(1, 0.0f);

            m_leftStickPadSprite->getTransform()->setLocalPosition(
                Math::Lerp(
                    m_leftStickPadSprite->getTransform()->getLocalPosition(),
                    Vec2DF::c_zero,
                    8.0f * _dt));
        }
    }

    //////////////////////////////////////////
    void VirtualJoystickController::processComponentAdded()
    {
        m_transform = getEntityRaw()->ensureComponent<Maze::Transform2D>();
        getEntityRaw()->ensureComponent<Maze::SizePolicy2D>();

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        MaterialPtr const& spriteMaterial = spriteManager->getDefaultSpriteMaterial();

        {
            EntityPtr leftButtonEntity = getEntityRaw()->getECSScene()->createEntity();
            Transform2DPtr buttonEntityTransform = leftButtonEntity->ensureComponent<Transform2D>();
            buttonEntityTransform->setSize(Vec2DF(200, 200));
            buttonEntityTransform->setParent(getTransform());
            buttonEntityTransform->setLocalPosition(Vec2DF(150, 150));
            buttonEntityTransform->setAnchor(0.0f, 0.0f);
            m_leftButton = leftButtonEntity->ensureComponent<ClickButton2D>();
            m_leftButton->getUIElement()->setUnpressOnUnfocus(false);
            leftButtonEntity->ensureComponent<Name>("Left Button");

            m_leftStickSprite = SpriteHelper::CreateSprite(
                "KnobBackground.mztexture",
                buttonEntityTransform->getSize(),
                Vec2DF::c_zero,
                spriteMaterial,
                buttonEntityTransform,
                getEntityRaw()->getECSScene());
            m_leftStickSprite->getEntityRaw()->ensureComponent<Name>("Left Stick Sprite");
            
            m_leftStickPadSprite = SpriteHelper::CreateSprite(
                "KnobCenter.mztexture",
                Vec2DF(120, 120),
                Vec2DF(0, 0),
                spriteMaterial,
                m_leftStickSprite->getTransform(),
                getEntityRaw()->getECSScene());
            m_leftStickPadSprite->getEntityRaw()->ensureComponent<Name>("Left Stick Pad Sprite");

            {
                

                UITweenTransitionScalePtr const& pressedTransition = m_leftStickSprite->getEntityRaw()->ensureComponent<UITweenTransitionScale>();
                pressedTransition->setHiddenScale(Vec2DF(1.0f, 1.0f));
                pressedTransition->setShownScale(Vec2DF(0.95f, 0.95f));
                pressedTransition->setShowTime(0.2f);
                pressedTransition->hideInstantly();

                m_leftButton->eventPressedChanged.subscribe(
                    [pressedTransition](Button2D* _button, bool _value)
                    {
                        pressedTransition->setHidden(!_value);
                    });


                m_leftButton->getUIElement()->eventCursorPressIn.subscribe(
                    [&](Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
                    {
                        m_cursorPositionOS = _positionOS;
                    });

                m_leftButton->getUIElement()->eventCursorMoveIn.subscribe(
                    [&](Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
                    {
                        m_cursorPositionOS = _positionOS;
                    });

                m_leftButton->getUIElement()->eventCursorMoveOut.subscribe(
                    [&](CursorInputEvent const& _inputEvent)
                    {
                        if (m_leftButton->getUIElement()->getPressed())
                        {
                            m_cursorPositionOS = m_leftButton->getTransform()->getWorldTransform().inversedAffineCopy().transformAffine(_inputEvent.position);
                        }
                    });
            }
        }

        {
            EntityPtr rightButtonEntity = getEntityRaw()->getECSScene()->createEntity();
            Transform2DPtr buttonEntityTransform = rightButtonEntity->ensureComponent<Transform2D>();
            buttonEntityTransform->setSize(Vec2DF(120, 120));
            buttonEntityTransform->setParent(getTransform());
            buttonEntityTransform->setLocalPosition(Vec2DF(-120, 120));
            buttonEntityTransform->setAnchor(1.0f, 0.0f);
            m_rightButton = rightButtonEntity->ensureComponent<ClickButton2D>();
            m_rightButton->getUIElement()->setUnpressOnUnfocus(false);
            rightButtonEntity->ensureComponent<Name>("Right Button");
            rightButtonEntity->setActiveSelf(false);

            SpriteRenderer2DPtr rightStickSprite = SpriteHelper::CreateSprite(
                "Knob02.mztexture",
                buttonEntityTransform->getSize(),
                Vec2DF::c_zero,
                spriteMaterial,
                buttonEntityTransform,
                getEntityRaw()->getECSScene());
            rightStickSprite->getEntityRaw()->ensureComponent<Name>("Right Stick Sprite");

            {
                UITweenTransitionScalePtr const& pressedTransition = rightStickSprite->getEntityRaw()->ensureComponent<UITweenTransitionScale>();
                pressedTransition->setHiddenScale(Vec2DF(1.0f, 1.0f));
                pressedTransition->setShownScale(Vec2DF(0.9f, 0.9f));
                pressedTransition->setShowTime(0.2f);
                pressedTransition->hideInstantly();

                m_rightButton->eventPressedChanged.subscribe(
                    [pressedTransition, this](Button2D* _button, bool _value)
                    {
                        pressedTransition->setHidden(!_value);

                        m_gamepad->setButtonState(0, _value);
                    });
            }
        }

    }


} // namespace Maze
//////////////////////////////////////////
