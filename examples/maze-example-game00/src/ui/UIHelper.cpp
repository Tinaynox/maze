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
#include "UIHelper.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace UIHelper
    {
        //////////////////////////////////////////
        ClickButton2DPtr CreateDefaultGameClickButton(
            ECSRenderScene* _scene,
            Transform2DPtr const& _parent,
            Vec2F32 _position,
            Vec2F32 _size,
            String const& _label,
            Delegate<void, Button2D*, CursorInputEvent const&> _onClickDelegate)
        {
            EntityPtr buttonEntity = _scene->createEntity();
            Transform2DPtr buttonEntityTransform = buttonEntity->ensureComponent<Transform2D>();
            buttonEntityTransform->setSize(_size);
            buttonEntityTransform->setParent(_parent);
            buttonEntityTransform->setLocalPosition(_position);
            ClickButton2DPtr button = buttonEntity->ensureComponent<ClickButton2D>();
            button->eventClick.subscribe(_onClickDelegate);
            buttonEntity->ensureComponent<Name>("Button");

            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
            SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();


            SpriteRenderer2DPtr buttonSprite = SpriteHelper::CreateSprite(
                "Button00.mztexture",
                buttonEntityTransform->getSize(),
                Vec2F32::c_zero,
                spriteManager->getDefaultSpriteMaterial(),
                buttonEntityTransform,
                _scene);
            buttonSprite->getEntityRaw()->ensureComponent<Name>("ButtonSpriteDefault");
            UITweenTransitionScalePtr pressedTransition = buttonSprite->getEntityRaw()->ensureComponent<UITweenTransitionScale>();
            pressedTransition->setHiddenScale(Vec2F32(1.0f, 1.0f));
            pressedTransition->setShownScale(Vec2F32(0.95f, 0.9f));
            pressedTransition->setShowTime(0.2f);
            pressedTransition->hideInstantly();

            SpriteRenderer2DPtr buttonFocusedSprite = SpriteHelper::CreateSprite(
                "Button00Focused.mztexture",
                buttonEntityTransform->getSize(),
                Vec2F32::c_zero,
                spriteManager->getDefaultSpriteMaterial(),
                buttonSprite->getTransform(),
                _scene);
            buttonFocusedSprite->getEntityRaw()->ensureComponent<Name>("ButtonSpriteFocused");
            UITweenTransitionAlphaPtr focusedTransition = buttonFocusedSprite->getEntityRaw()->ensureComponent<UITweenTransitionAlpha>();
            focusedTransition->setShowTime(0.3f);
            focusedTransition->setAlphaPower(1.0f);
            focusedTransition->setHidden(!button->getFocused());

            button->eventFocusChanged.subscribe(
                [focusedTransition](Button2D* _button, bool _value)
                {
                    focusedTransition->setHidden(!_value);
                });

            button->eventPressedChanged.subscribe(
                [pressedTransition](Button2D* _button, bool _value)
                {
                    pressedTransition->setHidden(!_value);
                });


            EntityPtr labelEntity = _scene->createEntity();
            Transform2DPtr labelEntityTransform = labelEntity->ensureComponent<Transform2D>();
            labelEntityTransform->setSize(buttonSprite->getTransform()->getSize());
            labelEntityTransform->setParent(buttonSprite->getTransform());
            labelEntityTransform->setLocalScale(Vec2F32(0.85f, 1.0f) * 0.75f);
            labelEntity->ensureComponent<Name>("LabelEntity");

            for (S32 i = 0; i < 8; ++i)
            {
                F32 outlineOffset = 1.75f;
                Vec2F32 offset;
                switch (i)
                {
                    case 0: offset = Vec2F32(-outlineOffset, 0.0f); break;
                    case 1: offset = Vec2F32(+outlineOffset, 0.0f); break;
                    case 2: offset = Vec2F32(0.0f, -outlineOffset); break;
                    case 3: offset = Vec2F32(0.0f, +outlineOffset); break;
                    case 4: offset = Vec2F32(-outlineOffset, +outlineOffset); break;
                    case 5: offset = Vec2F32(+outlineOffset, +outlineOffset); break;
                    case 6: offset = Vec2F32(-outlineOffset, -outlineOffset); break;
                    case 7: offset = Vec2F32(+outlineOffset, -outlineOffset); break;
                }

                SystemTextRenderer2DPtr systemText = SystemUIHelper::CreateSystemText(
                    _label.c_str(),
                    16,
                    HorizontalAlignment2D::Center,
                    VerticalAlignment2D::Middle,
                    buttonSprite->getTransform()->getSize(),
                    Vec2F32(0.0f, 2.0f) + offset,
                    labelEntityTransform,
                    _scene);
                systemText->setColor(ColorU32(38, 32, 27, 255));
                systemText->getEntityRaw()->ensureComponent<Name>("Shadow");
            }

            SystemTextRenderer2DPtr systemText = SystemUIHelper::CreateSystemText(
                _label.c_str(),
                16,
                HorizontalAlignment2D::Center,
                VerticalAlignment2D::Middle,
                buttonSprite->getTransform()->getSize(),
                Vec2F32(0.0f, 2.0f),
                labelEntityTransform,
                _scene);
            systemText->setColor(ColorU32(255, 160, 0, 255));
            systemText->getEntityRaw()->ensureComponent<Name>("Text");

            return button;
        }


        //////////////////////////////////////////
        ToggleButton2DPtr CreateDefaultGameToggleButton(
            ECSRenderScene* _scene,
            Transform2DPtr const& _parent,
            Vec2F32 _position,
            Vec2F32 _size,
            String const& _label,
            Delegate<void, Button2D*, CursorInputEvent const&> _onClickDelegate)
        {
            EntityPtr buttonEntity = _scene->createEntity();
            Transform2DPtr buttonEntityTransform = buttonEntity->ensureComponent<Transform2D>();
            buttonEntityTransform->setSize(_size);
            buttonEntityTransform->setParent(_parent);
            buttonEntityTransform->setLocalPosition(_position);
            ToggleButton2DPtr button = buttonEntity->ensureComponent<ToggleButton2D>();
            button->eventClick.subscribe(_onClickDelegate);
            buttonEntity->ensureComponent<Name>("Button");

            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
            SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();


            SpriteRenderer2DPtr buttonSprite = SpriteHelper::CreateSprite(
                "Button00.mztexture",
                buttonEntityTransform->getSize(),
                Vec2F32::c_zero,
                spriteManager->getDefaultSpriteMaterial(),
                buttonEntityTransform,
                _scene);
            buttonSprite->getEntityRaw()->ensureComponent<Name>("ButtonSpriteDefault");
            UITweenTransitionScalePtr pressedTransition = buttonSprite->getEntityRaw()->ensureComponent<UITweenTransitionScale>();
            UITweenTransitionScale* pressedTransitionRaw = pressedTransition.get();
            pressedTransition->setHiddenScale(Vec2F32(1.0f, 1.0f));
            pressedTransition->setShownScale(Vec2F32(0.95f, 0.9f));
            pressedTransition->setShowTime(0.2f);
            pressedTransition->hideInstantly();

            SpriteRenderer2DPtr buttonFocusedSprite = SpriteHelper::CreateSprite(
                "Button00Focused.mztexture",
                buttonEntityTransform->getSize(),
                Vec2F32::c_zero,
                spriteManager->getDefaultSpriteMaterial(),
                buttonSprite->getTransform(),
                _scene);
            buttonFocusedSprite->getEntityRaw()->ensureComponent<Name>("ButtonSpriteFocused");
            UITweenTransitionAlphaPtr focusedTransition = buttonFocusedSprite->getEntityRaw()->ensureComponent<UITweenTransitionAlpha>();
            UITweenTransitionAlpha* focusedTransitionRaw = focusedTransition.get();
            focusedTransition->setShowTime(0.3f);
            focusedTransition->setAlphaPower(1.0f);
            focusedTransition->setHidden(!button->getFocused());
            

            auto updateButtonState =
                [](ToggleButton2D* button, UITweenTransitionAlpha* _focusedTransition, UITweenTransitionScale* _pressedTransition)
                {
                    _focusedTransition->setHidden(!(button->getFocused() || button->getChecked()));
                    _pressedTransition->setHidden(!(button->getPressed() || button->getChecked()));
                };

            button->eventFocusChanged.subscribe(
                [focusedTransitionRaw, pressedTransitionRaw, updateButtonState](Button2D* _button, bool _value)
                {
                    ToggleButton2D* button = _button->castRaw<ToggleButton2D>();
                    updateButtonState(button, focusedTransitionRaw, pressedTransitionRaw);
                });

            button->eventPressedChanged.subscribe(
                [focusedTransitionRaw, pressedTransitionRaw, updateButtonState](Button2D* _button, bool _value)
                {
                    ToggleButton2D* button = _button->castRaw<ToggleButton2D>();
                    updateButtonState(button, focusedTransitionRaw, pressedTransitionRaw);                    
                });

            button->eventCheckedChanged.subscribe(
                [focusedTransitionRaw, pressedTransitionRaw, updateButtonState](ToggleButton2D* _button, bool _value)
                {
                    ToggleButton2D* button = _button->castRaw<ToggleButton2D>();
                    updateButtonState(button, focusedTransitionRaw, pressedTransitionRaw);
                });

            updateButtonState(button.get(), focusedTransitionRaw, pressedTransitionRaw);

            EntityPtr labelEntity = _scene->createEntity();
            Transform2DPtr labelEntityTransform = labelEntity->ensureComponent<Transform2D>();
            labelEntityTransform->setSize(buttonSprite->getTransform()->getSize());
            labelEntityTransform->setParent(buttonSprite->getTransform());
            labelEntityTransform->setLocalScale(Vec2F32(0.85f, 1.0f) * 0.75f);
            labelEntity->ensureComponent<Name>("LabelEntity");

            for (S32 i = 0; i < 8; ++i)
            {
                F32 outlineOffset = 1.75f;
                Vec2F32 offset;
                switch (i)
                {
                    case 0: offset = Vec2F32(-outlineOffset, 0.0f); break;
                    case 1: offset = Vec2F32(+outlineOffset, 0.0f); break;
                    case 2: offset = Vec2F32(0.0f, -outlineOffset); break;
                    case 3: offset = Vec2F32(0.0f, +outlineOffset); break;
                    case 4: offset = Vec2F32(-outlineOffset, +outlineOffset); break;
                    case 5: offset = Vec2F32(+outlineOffset, +outlineOffset); break;
                    case 6: offset = Vec2F32(-outlineOffset, -outlineOffset); break;
                    case 7: offset = Vec2F32(+outlineOffset, -outlineOffset); break;
                }

                SystemTextRenderer2DPtr systemText = SystemUIHelper::CreateSystemText(
                    _label.c_str(),
                    16,
                    HorizontalAlignment2D::Center,
                    VerticalAlignment2D::Middle,
                    buttonSprite->getTransform()->getSize(),
                    Vec2F32(0.0f, 2.0f) + offset,
                    labelEntityTransform,
                    _scene);
                systemText->setColor(ColorU32(38, 32, 27, 255));
                systemText->getEntityRaw()->ensureComponent<Name>("Shadow");
            }

            SystemTextRenderer2DPtr systemText = SystemUIHelper::CreateSystemText(
                _label.c_str(),
                16,
                HorizontalAlignment2D::Center,
                VerticalAlignment2D::Middle,
                buttonSprite->getTransform()->getSize(),
                Vec2F32(0.0f, 2.0f),
                labelEntityTransform,
                _scene);
            systemText->setColor(ColorU32(255, 160, 0, 255));
            systemText->getEntityRaw()->ensureComponent<Name>("Text");

            return button;
        }

    } // namespace UIHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
