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
#include "MazeUIHeader.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-ui/ecs/components/MazeEditBox2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/managers/MazeFontManager.hpp"
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Namespace UIHelper
    namespace UIHelper
    {
        //////////////////////////////////////////
        MAZE_UI_API EditBox2DPtr CreateDefaultEditBox(
            CString _text,
            U32 _fontSize,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot,
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr editBoxEntity = _ecsScene->createEntity();
            editBoxEntity->ensureComponent<Name>("EdidBox");

            EditBox2DPtr editBox = editBoxEntity->createComponent<EditBox2D>();

            ScissorMask2DPtr scissorMask = editBoxEntity->createComponent<ScissorMask2D>();

            Transform2DPtr const& transform = editBox->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr spriteRenderer = editBoxEntity->createComponent<SpriteRenderer2D>();
            SpriteRenderer2D* spriteRendererRaw = spriteRenderer.get();
            spriteRenderer->setSprite(UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default));
            spriteRenderer->setMaterialCopy(
                renderSystem->getMaterialManager()->getColorTextureMaterial());
            spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            auto updateEditBoxState =
                [](EditBox2D* _editBox, SpriteRenderer2D* _spriteRenderer)
            {
                if (_editBox->getSelected())
                {
                    _spriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Selected));
                }
                else
                {
                    if (_editBox->getFocused())
                    {
                        _spriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused));
                    }
                    else
                    {
                        _spriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default));
                    }
                }
            };

            editBox->eventFocusChanged.subscribe(
                [=](EditBox2D* _editBox, bool _value)
            {
                updateEditBoxState(_editBox, spriteRendererRaw);
            });

            editBox->eventSelectedChanged.subscribe(
                [=](EditBox2D* _editBox, bool _value)
            {
                updateEditBoxState(_editBox, spriteRendererRaw);
            });

            TextRenderer2DPtr textRenderer = UIHelper::CreateText(
                "",
                _fontSize,
                _horizontalAlignment,
                _verticalAlignment,
                _size + Vec2F(-10.0f, 0.0f),
                Vec2F::c_zero,
                transform,
                _ecsScene);
            textRenderer->setColor(ColorU32::c_black);

            SizePolicy2DPtr textSizePolicy = textRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();
            textSizePolicy->setSizeDelta(-10.0f, 0.0f);

            editBox->setTextRenderer(textRenderer->cast<AbstractTextRenderer2D>());
            editBox->setText(_text);

            TrueTypeFontPtr const& ttf = textRenderer->getFontMaterial()->getFont()->getDefaultFont();

            F32 ascent = ttf->getAscender(_fontSize);
            F32 descent = ttf->getDescender(_fontSize);            

            F32 cursorHeight = ascent - descent;
            SpriteRenderer2DPtr cursorRenderer = SpriteHelper::CreateSprite(
                textRenderer->getColor(),
                Vec2F(1.0f, cursorHeight),
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorMaterial(),
                textRenderer->getTransform(),
                _ecsScene,
                Vec2F::c_zero,
                Vec2F(0.0f, -descent / cursorHeight));

            editBox->setCursorRenderer(cursorRenderer);

            return editBox;
        }

        //////////////////////////////////////////
        MAZE_UI_API Dropdown2DPtr CreateDefaultDropdown(
            U32 _fontSize,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr dropdownEntity = _ecsScene->createEntity();
            dropdownEntity->ensureComponent<Name>("Dropdown");

            Dropdown2DPtr dropdown = dropdownEntity->createComponent<Dropdown2D>();

            Transform2DPtr const& transform = dropdown->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr spriteRenderer = dropdownEntity->createComponent<SpriteRenderer2D>();
            SpriteRenderer2D* spriteRendererRaw = spriteRenderer.get();
            spriteRenderer->setSprite(UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default));
            spriteRenderer->setMaterialCopy(
                renderSystem->getMaterialManager()->getColorTextureMaterial());
            spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);
            spriteRenderer->setColor(ColorU32(250, 250, 250));

            auto updateDropdownState =
                [](
                    Dropdown2D* _dropdown,
                    SpriteRenderer2D* _spriteRenderer,
                    Entity* _listEntity)
            {
                _listEntity->setActiveSelf(_dropdown->getSelected());

                Transform2DPtr transform = _listEntity->getComponent<Transform2D>();
                Canvas* rootCanvas = transform->getLastTrunkComponent<Canvas>();
                if (rootCanvas)
                {
                    transform->setAnchor(0.0f, 0.0f);
                    transform->setPivot(0.0f, 1.0f);
                    transform->setLocalPosition(0.0f, 0.0f);

                    Transform2D* root = rootCanvas->getTransform().get();
                    Vec2F const& rootSize = root->getSize();

                    Vec2F positionOS(
                        transform->getWidth(),
                        transform->getHeight());

                    Vec2F positionWS = transform->getWorldTransform().transformAffine(positionOS);
                    positionWS.y = Math::Clamp(positionWS.y, transform->getHeight(), rootSize.y);

                    Vec2F menuListPositionOS = transform->getParent()->getWorldTransform().inversedAffineCopy().transformAffine(positionWS);

                    transform->setPivot(1.0f, 1.0f);
                    transform->setAnchor(0.0f, 0.0f);
                    transform->setLocalPosition(menuListPositionOS);
                }

                if (_dropdown->getSelected())
                {
                    _spriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Selected));
                }
                else
                {
                    if (_dropdown->getFocused())
                    {
                        _spriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused));
                    }
                    else
                    {
                        _spriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default));
                    }
                }
            };


            TextRenderer2DPtr textRenderer = UIHelper::CreateText(
                "Dropdown",
                _fontSize,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                _size,
                Vec2F::c_zero,
                transform,
                _ecsScene);
            textRenderer->setColor(ColorU32::c_black);
            ScissorMask2DPtr scissorMask = textRenderer->getEntityRaw()->createComponent<ScissorMask2D>();

            SizePolicy2DPtr textSizePolicy = textRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();
            textSizePolicy->setSizeDelta(-10.0f, 0.0f);

            dropdown->setTextRenderer(textRenderer->cast<AbstractTextRenderer2D>());


            SpriteRenderer2DPtr expandButtonSprite = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded),
                Vec2F(8.0f, 8.0f) * 1.75f,
                Vec2F(-5.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                spriteRenderer->getEntityRaw()->getECSScene(),
                Vec2F(1.0f, 0.5f),
                Vec2F(1.0f, 0.5f));
            expandButtonSprite->setColor(ColorU32::c_black);


            // List entity
            Entity* listEntity = nullptr;
            {

                SpriteRenderer2DPtr listTemplateSpriteRenderer = SpriteHelper::CreateSprite(
                    UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                    Vec2F(_size.x, 140.0f),
                    Vec2F::c_zero,
                    renderSystem->getMaterialManager()->getColorTextureMaterial(),
                    transform,
                    _ecsScene,
                    Vec2F(0.0f, 0.0f),
                    Vec2F(0.0f, 1.0f));
                listTemplateSpriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

                listEntity = listTemplateSpriteRenderer->getEntityRaw();

                CanvasPtr canvas = listEntity->ensureComponent<Canvas>();

                canvas->setClearColor(ColorU32::c_red);
                canvas->setClearColorFlag(false);
                canvas->setSortOrder(300000);
                canvas->getCanvasScaler()->setScaleMode(CanvasScaler::ScaleMode::None);
                canvas->setViewportTransformPolicy(ViewportTransformPolicy::TransformToViewport);


                UIElement2DPtr uiElement = listEntity->ensureComponent<UIElement2D>();
                uiElement->setCaptureCursorHits(true);

                dropdown->setListCanvas(canvas);

                // Item Prefab
                {
                    Transform2DPtr itemPrefabTransform = SpriteHelper::CreateTransform2D(
                        Vec2F(_size.x - 2.0f, 20.0f),
                        Vec2F(1.0f, 0.0f),
                        transform,
                        _ecsScene,
                        Vec2F(0.0f, 1.0f),
                        Vec2F(0.0f, 1.0f));

                    itemPrefabTransform->getEntityRaw()->setActiveSelf(false);

                    SizePolicy2DPtr itemPrefabSizePolicy = itemPrefabTransform->getEntityRaw()->ensureComponent<SizePolicy2D>();
                    itemPrefabSizePolicy->setFlag(SizePolicy2D::Height, false);
                    itemPrefabSizePolicy->setSizeDelta(-4.0f, 0.0f);

                    SpriteRenderer2DPtr backgroundSpriteRenderer = SpriteHelper::CreateSprite(
                        ColorU32::c_white,
                        Vec2F(_size.x, 20.0f - 4.0f),
                        Vec2F(0.0f, 2.0f),
                        renderSystem->getMaterialManager()->getColorTextureMaterial(),
                        itemPrefabTransform,
                        _ecsScene);
                    backgroundSpriteRenderer->getEntityRaw()->ensureComponent<Name>()->setName("Background");
                    backgroundSpriteRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();

                    SpriteRenderer2DPtr checkMarkSprite = SpriteHelper::CreateSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::CheckMark),
                        Vec2F(8.0f, 8.0f) * 1.75f,
                        Vec2F(10.0f, 10.0f),
                        renderSystem->getMaterialManager()->getColorTextureMaterial(),
                        itemPrefabTransform,
                        spriteRenderer->getEntityRaw()->getECSScene(),
                        Vec2F(0.0f, 0.0f),
                        Vec2F(0.5f, 0.5f));
                    checkMarkSprite->getEntityRaw()->ensureComponent<Name>()->setName("CheckMark");
                    checkMarkSprite->setColor(ColorU32::c_black);

                    TextRenderer2DPtr itemTextRenderer = UIHelper::CreateText(
                        "Option 1",
                        _fontSize,
                        HorizontalAlignment2D::Left,
                        VerticalAlignment2D::Middle,
                        Vec2F(_size.x, 20.0f),
                        Vec2F(20.0f, 0.0f),
                        itemPrefabTransform,
                        _ecsScene,
                        Vec2F::c_zero,
                        Vec2F::c_zero);
                    itemTextRenderer->getEntityRaw()->ensureComponent<Name>()->setName("Label");
                    itemTextRenderer->setColor(ColorU32::c_black);
                    ScissorMask2DPtr scissorMask = itemTextRenderer->getEntityRaw()->createComponent<ScissorMask2D>();
                    SizePolicy2DPtr itemTextRendererSizePolicy = itemTextRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();
                    itemTextRendererSizePolicy->setSizeDelta(-20.0f, -2.0f);

                    ClickButton2DPtr button = itemPrefabTransform->getEntityRaw()->createComponent<ClickButton2D>();
                    button->getUIElement()->setCaptureCursorHits(false);



                    dropdown->setItemPrefabTransform(itemPrefabTransform);

                }
            }

            dropdown->eventFocusChanged.subscribe(
                [=](Dropdown2D* _dropdown, bool _value)
            {
                updateDropdownState(_dropdown, spriteRendererRaw, listEntity);
            });

            dropdown->eventSelectedChanged.subscribe(
                [=](Dropdown2D* _dropdown, bool _value)
            {
                updateDropdownState(_dropdown, spriteRendererRaw, listEntity);
            });

            updateDropdownState(dropdown.get(), spriteRendererRaw, listEntity);

            return dropdown;
        }

        //////////////////////////////////////////
        MAZE_UI_API HorizontalLayout2DPtr CreateHorizontalLayout(
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            EntityPtr entity = _ecsScene->createEntity();
            entity->ensureComponent<Name>("HLayout");

            HorizontalLayout2DPtr layout = entity->createComponent<HorizontalLayout2D>();
            layout->setHorizontalAlignment(_horizontalAlignment);
            layout->setVerticalAlignment(_verticalAlignment);

            Transform2DPtr const& transform = layout->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            return layout;
        }

        //////////////////////////////////////////
        MAZE_UI_API VerticalLayout2DPtr CreateVerticalLayout(
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            EntityPtr entity = _ecsScene->createEntity();
            entity->ensureComponent<Name>("VLayout");

            VerticalLayout2DPtr layout = entity->createComponent<VerticalLayout2D>();
            layout->setHorizontalAlignment(_horizontalAlignment);
            layout->setVerticalAlignment(_verticalAlignment);

            Transform2DPtr const& transform = layout->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            return layout;
        }

        //////////////////////////////////////////
        MAZE_UI_API ClickButton2DPtr CreateClickButton(
            SpritePtr const& _default,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr entity = _ecsScene->createEntity();
            entity->ensureComponent<Name>("ClickButton");

            ClickButton2DPtr clickButton = entity->createComponent<ClickButton2D>();
            clickButton->setNormalColor({ 245, 245, 245 });
            clickButton->setFocusedColor({ 213, 231, 248 });
            clickButton->setSelectedColor({ 200, 220, 240 });
            clickButton->setPressedColor({ 213, 231, 248 });

            Transform2DPtr const& transform = clickButton->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr spriteRenderer = SpriteHelper::CreateSprite(
                _default,
                transform->getSize(),
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            clickButton->setTransitionSprite(spriteRenderer);

            return clickButton;
        }

        //////////////////////////////////////////
        MAZE_UI_API ToggleButton2DPtr CreateToggleButton(
            SpritePtr const& _default,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot,
            ColorU32 _normalColor,
            ColorU32 _focusedColor,
            ColorU32 _checkedColor,
            ColorU32 _pressedColor,
            ColorU32 _focusedPressedColor)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr entity = _ecsScene->createEntity();
            entity->ensureComponent<Name>("ToggleButton");

            ToggleButton2DPtr toggleButton = entity->createComponent<ToggleButton2D>();

            Transform2DPtr const& transform = toggleButton->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr spriteRenderer = SpriteHelper::CreateSprite(
                _default,
                transform->getSize(),
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);
            SpriteRenderer2D* spriteRendererRaw = spriteRenderer.get();
            toggleButton->setTransitionSprite(spriteRenderer);

            auto updateToggleButtonState =
                [_normalColor, _focusedColor, _checkedColor, _pressedColor, _focusedPressedColor](
                    Button2D* _button,
                    SpriteRenderer2D* _spriteRenderer)
            {
                ToggleButton2D* toggleButton = _button->castRaw<ToggleButton2D>();

                if (toggleButton->getPressed())
                {
                    _spriteRenderer->setColor(_pressedColor);
                }
                else
                {
                    if (toggleButton->getFocused())
                    {
                        if (toggleButton->getChecked())
                            _spriteRenderer->setColor(_focusedPressedColor);
                        else
                            _spriteRenderer->setColor(_focusedColor);
                    }
                    else
                    {
                        if (toggleButton->getChecked())
                            _spriteRenderer->setColor(_checkedColor);
                        else
                            _spriteRenderer->setColor(_normalColor);
                    }
                }                
            };

            toggleButton->eventFocusChanged.subscribe(
                [=](Button2D* _button, bool _value)
                {
                    updateToggleButtonState(
                        _button,
                        spriteRendererRaw);
                });

            toggleButton->eventPressedChanged.subscribe(
                [=](Button2D* _button, bool _value)
                {
                    updateToggleButtonState(
                        _button,
                        spriteRendererRaw);
                });

            toggleButton->eventCheckedChanged.subscribe(
                [=](ToggleButton2D* _button, bool _value)
                {
                    updateToggleButtonState(
                        _button,
                        spriteRendererRaw);
                });

            updateToggleButtonState(
                toggleButton.get(),
                spriteRendererRaw);

            return toggleButton;
        }

        //////////////////////////////////////////
        MAZE_UI_API ClickButton2DPtr CreateDefaultClickButton(
            CString _text,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr entity = _ecsScene->createEntity();
            entity->ensureComponent<Name>("ClickButton");

            ClickButton2DPtr clickButton = entity->createComponent<ClickButton2D>();
            clickButton->setNormalColor({ 245, 245, 245 });
            clickButton->setFocusedColor({ 213, 231, 248 });
            clickButton->setSelectedColor({ 200, 220, 240 });
            clickButton->setPressedColor({ 213, 231, 248 });

            Transform2DPtr const& transform = clickButton->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr spriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                transform->getSize(),
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            if (_text && strcmp(_text, "") != 0)
            {
                SystemTextRenderer2DPtr textRenderer = SystemUIHelper::CreateSystemText(
                    _text,
                    8,
                    HorizontalAlignment2D::Center,
                    VerticalAlignment2D::Middle,
                    _size,
                    Vec2F::c_zero,
                    spriteRenderer->getTransform(),
                    _ecsScene);
                textRenderer->setColor(ColorU32::c_black);
            }

            clickButton->setTransitionSprite(spriteRenderer);

            return clickButton;
        }

        //////////////////////////////////////////
        MAZE_UI_API ClickButton2DPtr CreateClickButton(
            SpritePtr const& _default,
            SpritePtr const& _focused,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr entity = _ecsScene->createEntity();
            entity->ensureComponent<Name>("ClickButton");

            ClickButton2DPtr clickButton = entity->createComponent<ClickButton2D>();

            Transform2DPtr const& transform = clickButton->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr spriteRenderer = entity->createComponent<SpriteRenderer2D>();
            SpriteRenderer2D* spriteRendererRaw = spriteRenderer.get();
            spriteRenderer->setSprite(_default);
            spriteRenderer->setMaterialCopy(
                renderSystem->getMaterialManager()->getColorTextureMaterial());
            spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            SpriteWPtr defaultWeak = _default;
            SpriteWPtr focusedWeak = _focused;

            auto updateButtonState =
                [defaultWeak, focusedWeak](Button2D* _button, SpriteRenderer2D* _spriteRenderer)
                {
                    if (_button->getFocused())
                    {
                        _spriteRenderer->setSprite(
                            focusedWeak.lock());
                    }
                    else
                    {
                        _spriteRenderer->setSprite(
                            defaultWeak.lock());
                    }
                };

            clickButton->eventFocusChanged.subscribe(
                [=](Button2D* _button, bool _value)
                {
                    updateButtonState(_button, spriteRendererRaw);
                });

            return clickButton;
        }

        //////////////////////////////////////////
        MAZE_UI_API ToggleButton2DPtr CreateDefaultToggleButton(
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr entity = _ecsScene->createEntity();
            entity->ensureComponent<Name>("ToggleButton");

            ToggleButton2DPtr toggleButton = entity->createComponent<ToggleButton2D>();

            Transform2DPtr const& transform = toggleButton->getTransform();
            transform->setParent(_parent);
            transform->setSize(Vec2F(14.0f, 14.0f));
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr spriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                transform->getSize(),
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);
            SpriteRenderer2D* spriteRendererRaw = spriteRenderer.get();


            SpriteRenderer2DPtr checkMarkSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::CheckMark),
                Vec2F(16.0f, 16.0f),
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                spriteRenderer->getTransform(),
                _ecsScene);
            checkMarkSpriteRenderer->setColor(ColorU32(12, 12, 12));
            SpriteRenderer2D* checkMarkSpriteRendererRaw = checkMarkSpriteRenderer.get();

            auto updateToggleButtonState =
                [](
                    Button2D* _button,
                    SpriteRenderer2D* _spriteRenderer,
                    SpriteRenderer2D* _checkMarkSpriteRenderer)
            {
                ToggleButton2D* toggleButton = _button->castRaw<ToggleButton2D>();

                if (toggleButton->getPressed())
                {
                    _spriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Selected));
                }
                else
                {
                    if (toggleButton->getFocused())
                    {
                        _spriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused));
                    }
                    else
                    {
                        _spriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default));
                    }
                }

                _checkMarkSpriteRenderer->getMeshRenderer()->setEnabled(toggleButton->getChecked());
            };

            toggleButton->eventFocusChanged.subscribe(
                [=](Button2D* _button, bool _value)
                {
                    updateToggleButtonState(
                        _button,
                        spriteRendererRaw,
                        checkMarkSpriteRendererRaw);
                });

            toggleButton->eventPressedChanged.subscribe(
                [=](Button2D* _button, bool _value)
                {
                    updateToggleButtonState(
                        _button,
                        spriteRendererRaw,
                        checkMarkSpriteRendererRaw);
                });

            toggleButton->eventCheckedChanged.subscribe(
                [=](ToggleButton2D* _button, bool _value)
                {
                    updateToggleButtonState(
                        _button,
                        spriteRendererRaw,
                        checkMarkSpriteRendererRaw);
                });

            updateToggleButtonState(
                toggleButton.get(),
                spriteRendererRaw,
                checkMarkSpriteRendererRaw);

            return toggleButton;
        }

        //////////////////////////////////////////
        MAZE_UI_API ColorEdit2DPtr CreateDefaultColorEdit(
            ColorU32 _color,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr colorEditEntity = _ecsScene->createEntity();
            colorEditEntity->ensureComponent<Name>("ColorEdit");

            ColorEdit2DPtr colorEdit = colorEditEntity->createComponent<ColorEdit2D>();
            colorEdit->setColor(_color);

            ScissorMask2DPtr scissorMask = colorEditEntity->createComponent<ScissorMask2D>();

            Transform2DPtr const& transform = colorEdit->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr colorSpriteRenderer = SpriteHelper::CreateSprite(
                _color,
                _size - 2.0f,
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            colorSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>()->setSizeDelta(-2.0f, -2.0f);
            colorEdit->setColorRenderer(colorSpriteRenderer);
            // SpriteRenderer2D* colorSpriteRendererRaw = colorSpriteRenderer.get();

            SpriteRenderer2DPtr alphaBackSpriteRenderer = SpriteHelper::CreateSprite(
                ColorU32::c_black,
                { _size.x, 3.0f},
                Vec2F(0.0f, 1.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene,
                Vec2F(0.5f, 0.0f),
                Vec2F(0.5f, 0.0f));
            SizePolicy2DPtr alphaBackSizePolicy = alphaBackSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>();
            alphaBackSizePolicy->setFlag(SizePolicy2D::Height, false);
            alphaBackSizePolicy->setSizeDelta(-2.0f, 0.0f);

            SpriteRenderer2DPtr alphaSpriteRenderer = SpriteHelper::CreateSprite(
                ColorU32::c_white,
                { (_size.x - 2.0f) * 0.5f, 3.0f },
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                alphaBackSpriteRenderer->getTransform(),
                _ecsScene,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.0f, 0.0f));
            SizePolicy2DPtr alphaSizePolicy = alphaSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>();
            // alphaSizePolicy->setFlag(SizePolicy2D::Width, false);
            colorEdit->setAlphaRenderer(alphaSpriteRenderer);


            SpriteRenderer2DPtr borderSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default),
                _size,
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            SpriteRenderer2D* borderSpriteRendererRaw = borderSpriteRenderer.get();
            borderSpriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            

            auto updateColorEditState =
                [](ColorEdit2D* _colorEdit, SpriteRenderer2D* _borderSpriteRenderer)
            {
                if (_colorEdit->getSelected())
                {
                    _borderSpriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Selected));
                }
                else
                {
                    if (_colorEdit->getFocused())
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Focused));
                    }
                    else
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default));
                    }
                }
            };

            colorEdit->eventFocusChanged.subscribe(
                [=](ColorEdit2D* _colorEdit, bool _value)
                {
                    updateColorEditState(_colorEdit, borderSpriteRendererRaw);
                });

            colorEdit->eventSelectedChanged.subscribe(
                [=](ColorEdit2D* _colorEdit, bool _value)
                {
                    updateColorEditState(_colorEdit, borderSpriteRendererRaw);
                });

            return colorEdit;
        }

        //////////////////////////////////////////
        MAZE_UI_API ColorHDREdit2DPtr CreateDefaultColorHDREdit(
            ColorF128 _color,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot,
            bool _hdrLabel,
            bool _alphaBand)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr colorEditEntity = _ecsScene->createEntity();
            colorEditEntity->ensureComponent<Name>("ColorHDREdit");

            ColorHDREdit2DPtr colorEdit = colorEditEntity->createComponent<ColorHDREdit2D>();
            colorEdit->setColor(_color);

            ScissorMask2DPtr scissorMask = colorEditEntity->createComponent<ScissorMask2D>();

            Transform2DPtr const& transform = colorEdit->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr colorSpriteRenderer = SpriteHelper::CreateSprite(
                ColorU32::FromVec4F32(_color),
                _size - 2.0f,
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::ColorHDR),
                transform,
                _ecsScene);
            colorSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>()->setSizeDelta(-2.0f, -2.0f);
            colorEdit->setColorRenderer(colorSpriteRenderer);

            SystemTextRenderer2D* itemTextRendererRaw = nullptr;
            if (_hdrLabel)
            {
                SystemTextRenderer2DPtr itemTextRenderer = SystemUIHelper::CreateSystemText(
                    "HDR",
                    8,
                    HorizontalAlignment2D::Center,
                    VerticalAlignment2D::Middle,
                    colorSpriteRenderer->getTransform()->getSize(),
                    Vec2F(0.0f, 0.0f),
                    colorSpriteRenderer->getTransform(),
                    _ecsScene,
                    Vec2F(0.5f, 0.5f),
                    Vec2F(0.5f, 0.5f));
                itemTextRendererRaw = itemTextRenderer.get();
            }

            if (_alphaBand)
            {
                SpriteRenderer2DPtr alphaBackSpriteRenderer = SpriteHelper::CreateSprite(
                    ColorU32::c_black,
                    { _size.x, 3.0f },
                    Vec2F(0.0f, 1.0f),
                    renderSystem->getMaterialManager()->getColorTextureMaterial(),
                    transform,
                    _ecsScene,
                    Vec2F(0.5f, 0.0f),
                    Vec2F(0.5f, 0.0f));
                SizePolicy2DPtr alphaBackSizePolicy = alphaBackSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>();
                alphaBackSizePolicy->setFlag(SizePolicy2D::Height, false);
                alphaBackSizePolicy->setSizeDelta(-2.0f, 0.0f);

                SpriteRenderer2DPtr alphaSpriteRenderer = SpriteHelper::CreateSprite(
                    ColorU32::c_white,
                    { (_size.x - 2.0f) * 0.5f, 3.0f },
                    Vec2F(0.0f, 0.0f),
                    renderSystem->getMaterialManager()->getColorTextureMaterial(),
                    alphaBackSpriteRenderer->getTransform(),
                    _ecsScene,
                    Vec2F(0.0f, 0.0f),
                    Vec2F(0.0f, 0.0f));
                SizePolicy2DPtr alphaSizePolicy = alphaSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>();
                colorEdit->setAlphaRenderer(alphaSpriteRenderer);
            }


            SpriteRenderer2DPtr borderSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default),
                _size,
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            SpriteRenderer2D* borderSpriteRendererRaw = borderSpriteRenderer.get();
            borderSpriteRenderer->setRenderMode(SpriteRenderMode::Sliced);



            auto updateColorEditState =
                [](ColorHDREdit2D* _colorEdit, SpriteRenderer2D* _borderSpriteRenderer)
            {
                if (_colorEdit->getSelected())
                {
                    _borderSpriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Selected));
                }
                else
                {
                    if (_colorEdit->getFocused())
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Focused));
                    }
                    else
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default));
                    }
                }
            };

            auto updateHDRColor =
                [](ColorHDREdit2D* _colorEdit, SystemTextRenderer2D* _hdrTextRenderer)
                {
                    Vec3F const luminosityCoeff(0.2125f, 0.7154f, 0.0721f);
                    F32 l = Math::Clamp(luminosityCoeff.dotProduct(_colorEdit->getColor()), 0.0f, 1.0f);
                    F32 invL = 1.0f - l;
                    _hdrTextRenderer->setColor(
                        ColorU32::FromVec3F32({ invL, invL, invL }));
                };


            colorEdit->eventFocusChanged.subscribe(
                [=](ColorHDREdit2D* _colorEdit, bool _value)
                {
                    updateColorEditState(_colorEdit, borderSpriteRendererRaw);
                });

            colorEdit->eventSelectedChanged.subscribe(
                [=](ColorHDREdit2D* _colorEdit, bool _value)
                {
                    updateColorEditState(_colorEdit, borderSpriteRendererRaw);
                });

            if (_hdrLabel)
            {
                colorEdit->eventColorChanged.subscribe(
                    [=](ColorHDREdit2D* _colorEdit, ColorF128 const& _color)
                    {
                        updateHDRColor(_colorEdit, itemTextRendererRaw);
                    });

                updateHDRColor(colorEdit.get(), itemTextRendererRaw);
            }


            return colorEdit;
        }


        //////////////////////////////////////////
        MAZE_UI_API ColorGradientEdit2DPtr CreateDefaultColorGradientEdit(
            ColorGradient const& _gradient,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr colorEditEntity = _ecsScene->createEntity();
            colorEditEntity->ensureComponent<Name>("ColorGradientEdit");

            ColorGradientEdit2DPtr colorEdit = colorEditEntity->createComponent<ColorGradientEdit2D>();
            colorEdit->setGradient(_gradient);

            ScissorMask2DPtr scissorMask = colorEditEntity->createComponent<ScissorMask2D>();

            Transform2DPtr const& transform = colorEdit->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr transparentBackSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TransparentChess),
                _size - 2.0f,
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            transparentBackSpriteRenderer->getMaterial()->setUniform(
                "u_baseMapST",
                Vec4F(
                    transparentBackSpriteRenderer->getTransform()->getWidth() / 8.0f,
                    transparentBackSpriteRenderer->getTransform()->getHeight() / 8.0f,
                    0.0f,
                    0.0f));
            transparentBackSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>()->setSizeDelta(-2.0f, -2.0f);

            SpriteRenderer2DPtr gradientSpriteRenderer = SpriteHelper::CreateSprite(
                ColorU32::c_white,
                _size - 2.0f,
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getColorMaterial(),
                transform,
                _ecsScene);
            gradientSpriteRenderer->setRenderMode(SpriteRenderMode::Custom);
            gradientSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>()->setSizeDelta(-2.0f, -2.0f);
            colorEdit->setGradientRenderer(gradientSpriteRenderer);


            SpriteRenderer2DPtr borderSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default),
                _size,
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            SpriteRenderer2D* borderSpriteRendererRaw = borderSpriteRenderer.get();
            borderSpriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            auto updateColorEditState =
                [](ColorGradientEdit2D* _colorEdit, SpriteRenderer2D* _borderSpriteRenderer)
            {
                if (_colorEdit->getSelected())
                {
                    _borderSpriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Selected));
                }
                else
                {
                    if (_colorEdit->getFocused())
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Focused));
                    }
                    else
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default));
                    }
                }
            };

            colorEdit->eventFocusChanged.subscribe(
                [=](ColorGradientEdit2D* _colorEdit, bool _value)
                {
                    updateColorEditState(_colorEdit, borderSpriteRendererRaw);
                });

            colorEdit->eventSelectedChanged.subscribe(
                [=](ColorGradientEdit2D* _colorEdit, bool _value)
                {
                    updateColorEditState(_colorEdit, borderSpriteRendererRaw);
                });

            return colorEdit;
        }

        //////////////////////////////////////////
        MAZE_UI_API AnimationCurveEdit2DPtr CreateDefaultAnimationCurveEdit(
            AnimationCurve const& _curve,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr curveEditEntity = _ecsScene->createEntity();
            curveEditEntity->ensureComponent<Name>("CurveEdit");

            AnimationCurveEdit2DPtr curveEdit = curveEditEntity->createComponent<AnimationCurveEdit2D>();
            curveEdit->setCurve(_curve);

            ScissorMask2DPtr scissorMask = curveEditEntity->createComponent<ScissorMask2D>();

            Transform2DPtr const& transform = curveEdit->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr curveSpriteRenderer = SpriteHelper::CreateSprite(
                ColorU32::c_white,
                _size - 2.0f,
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            curveSpriteRenderer->getEntityRaw()->createComponent<SizePolicy2D>()->setSizeDelta(-2.0f, -2.0f);
            curveEdit->setCurveRenderer(curveSpriteRenderer);


            SpriteRenderer2DPtr borderSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default),
                _size,
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            SpriteRenderer2D* borderSpriteRendererRaw = borderSpriteRenderer.get();
            borderSpriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            auto updateCurveEditState =
                [](AnimationCurveEdit2D* _curveEdit, SpriteRenderer2D* _borderSpriteRenderer)
            {
                if (_curveEdit->getSelected())
                {
                    _borderSpriteRenderer->setSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Selected));
                }
                else
                {
                    if (_curveEdit->getFocused())
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Focused));
                    }
                    else
                    {
                        _borderSpriteRenderer->setSprite(
                            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel01Default));
                    }
                }
            };

            curveEdit->eventFocusChanged.subscribe(
                [=](AnimationCurveEdit2D* _colorEdit, bool _value)
                {
                    updateCurveEditState(_colorEdit, borderSpriteRendererRaw);
                });

            curveEdit->eventSelectedChanged.subscribe(
                [=](AnimationCurveEdit2D* _colorEdit, bool _value)
                {
                    updateCurveEditState(_colorEdit, borderSpriteRendererRaw);
                });

            return curveEdit;
        }

        //////////////////////////////////////////
        Slider2DPtr CreateDefaultSlider(
            F32 _value,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr sliderEntity = _ecsScene->createEntity();
            sliderEntity->ensureComponent<Name>("Slider");

            Slider2DPtr slider = sliderEntity->createComponent<Slider2D>();

            Transform2DPtr const& transform = slider->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            {
                SpriteRenderer2DPtr background = SpriteHelper::CreateSprite(
                    UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                    _size,
                    Vec2F::c_zero,
                    renderSystem->getMaterialManager()->getColorTextureMaterial(),
                    transform,
                    _ecsScene,
                    Vec2F(0.0f, 0.5f),
                    Vec2F(0.0f, 0.5f));
                background->setRenderMode(SpriteRenderMode::Sliced);
                background->setColor(ColorU32(144, 144, 144));
                background->getEntityRaw()->ensureComponent<Name>()->setName("Background");
                background->getEntityRaw()->ensureComponent<SizePolicy2D>()->setSizeDelta(0.0f, -0.75f * _size.y);
            }

            {
                Transform2DPtr fillArea = SpriteHelper::CreateTransform2D(
                    Vec2F(_size.x, 0.25f * _size.y),
                    Vec2F::c_zero,
                    transform,
                    _ecsScene,
                    Vec2F(0.0f, 0.5f),
                    Vec2F(0.0f, 0.5f));
                fillArea->getEntityRaw()->ensureComponent<Name>()->setName("Fill Area");
                fillArea->getEntityRaw()->ensureComponent<SizePolicy2D>()->setSizeDelta(0.0f, -0.75f * _size.y);

                SpriteRenderer2DPtr fill = SpriteHelper::CreateSprite(
                    UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
                    Vec2F(0.25f * fillArea->getSize().x, fillArea->getSize().y),
                    Vec2F::c_zero,
                    renderSystem->getMaterialManager()->getColorTextureMaterial(),
                    fillArea,
                    _ecsScene,
                    Vec2F(0.0f, 0.5f),
                    Vec2F(0.0f, 0.5f));
                fill->setRenderMode(SpriteRenderMode::Sliced);
                fill->getEntityRaw()->ensureComponent<Name>()->setName("Fill");

                slider->setFillRectTransform(fill->getTransform());
            }

            SpriteRenderer2DPtr handleRenderer;
            {
                Transform2DPtr handleSlideArea = SpriteHelper::CreateTransform2D(
                    Vec2F(_size.x, 0.25f * _size.y),
                    Vec2F::c_zero,
                    transform,
                    _ecsScene,
                    Vec2F(0.0f, 0.5f),
                    Vec2F(0.0f, 0.5f));
                handleSlideArea->getEntityRaw()->ensureComponent<Name>()->setName("Handle Slide Area");
                handleSlideArea->getEntityRaw()->ensureComponent<SizePolicy2D>()->setSizeDelta(0.0f, -0.75f * _size.y);

                handleRenderer = SpriteHelper::CreateSprite(
                    UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::SliderHandle),
                    Vec2F(_size.y, _size.y) * 0.8f,
                    Vec2F::c_zero,
                    renderSystem->getMaterialManager()->getColorTextureMaterial(),
                    handleSlideArea,
                    _ecsScene,
                    Vec2F(0.0f, 0.5f),
                    Vec2F(0.5f, 0.5f));
                handleRenderer->getEntityRaw()->ensureComponent<Name>()->setName("Handle");

                slider->setHandleTransform(handleRenderer->getTransform());
            }
            SpriteRenderer2D* handleRendererRaw = handleRenderer.get();

            slider->setValue(_value);

            auto updateSliderState =
                [](Slider2D* _slider, SpriteRenderer2D* _spriteRenderer)
                {
                    if (_slider->getPressed())
                    {
                        _spriteRenderer->setColor(ColorU32(220, 220, 220));
                    }
                    else
                    {
                        _spriteRenderer->setColor(ColorU32::c_white);
                    }
                };

            slider->eventPressedChanged.subscribe(
                [=](Slider2D* _slider, bool _pressed)
                {
                    updateSliderState(_slider, handleRendererRaw);
                });

            return slider;
        }

        //////////////////////////////////////////
        MAZE_UI_API MenuListItem2DPtr CreateDefaultMenuListItem(
            String const& _name,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr menuListItemEntity = _ecsScene->createEntity();
            menuListItemEntity->ensureComponent<Name>("Menu List Item");

            MenuListItem2DPtr menuListItem = menuListItemEntity->createComponent<MenuListItem2D>();

            Transform2DPtr const& transform = menuListItem->getTransform();
            transform->setSize(_size);
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr backgroundSpriteRenderer = SpriteHelper::CreateSprite(
                ColorU32::c_white,
                _size,
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene);
            backgroundSpriteRenderer->getEntityRaw()->ensureComponent<Name>()->setName("Background");
            backgroundSpriteRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();
            menuListItem->getButton()->setTransitionSprite(backgroundSpriteRenderer);
            menuListItem->getButton()->setNormalColor(ColorU32(240, 240, 240));
            menuListItem->getButton()->setFocusedColor(ColorU32(209, 226, 242));
            menuListItem->getButton()->setPressedColor(ColorU32(209, 226, 242));

            SpriteRenderer2DPtr checkMarkSprite = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::CheckMark),
                Vec2F(8.0f, 8.0f) * 1.75f,
                Vec2F(10.0f, 10.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene,
                Vec2F(0.0f, 0.0f),
                Vec2F(0.5f, 0.5f));
            checkMarkSprite->getEntityRaw()->ensureComponent<Name>()->setName("CheckMark");
            checkMarkSprite->setColor(ColorU32::c_black);
            menuListItem->setCheckedNode(checkMarkSprite->getEntity());


            SpriteRenderer2DPtr subMenuMarkSprite = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::SubMenuMark),
                Vec2F(4.0f, 7.0f),
                Vec2F(-8.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene,
                Vec2F(1.0f, 0.5f),
                Vec2F(1.0f, 0.5f));
            subMenuMarkSprite->getEntityRaw()->ensureComponent<Name>()->setName("SubMenuMark");
            subMenuMarkSprite->setColor(ColorU32::c_black);
            menuListItem->setSubMenuNode(subMenuMarkSprite->getEntity());

            SystemTextRenderer2DPtr itemTextRenderer = SystemUIHelper::CreateSystemText(
                _name.c_str(),
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                _size,
                Vec2F(20.0f, 0.0f),
                transform,
                _ecsScene,
                Vec2F::c_zero,
                Vec2F::c_zero);
            itemTextRenderer->getEntityRaw()->ensureComponent<Name>()->setName("Label");
            itemTextRenderer->setColor(ColorU32::c_black);
            ScissorMask2DPtr scissorMask = itemTextRenderer->getEntityRaw()->createComponent<ScissorMask2D>();
            SizePolicy2DPtr itemTextRendererSizePolicy = itemTextRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();
            itemTextRendererSizePolicy->setSizeDelta(-20.0f, -2.0f);

            return menuListItem;
        }

        //////////////////////////////////////////
        MAZE_UI_API MenuList2DPtr CreateDefaultMenuList(
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr menuListEntity = _ecsScene->createEntity();
            menuListEntity->ensureComponent<Name>("Menu List");

            MenuList2DPtr menuList = menuListEntity->createComponent<MenuList2D>();

            Transform2DPtr const& transform = menuList->getTransform();
            transform->setSize(_size);
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            SpriteRenderer2DPtr itemsListSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel03),
                _size,
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            itemsListSpriteRenderer->setColor(240, 240, 240);
            itemsListSpriteRenderer->getEntityRaw()->ensureComponent<Name>()->setName("List Layout");
            VerticalLayout2DPtr itemsListLayout = itemsListSpriteRenderer->getEntityRaw()->ensureComponent<VerticalLayout2D>();
            itemsListLayout->setHorizontalAlignment(HorizontalAlignment2D::Center);
            itemsListLayout->setVerticalAlignment(VerticalAlignment2D::Middle);
            itemsListLayout->setPaddingBottom(2.0f);
            itemsListLayout->setPaddingTop(2.0f);
            itemsListLayout->setAutoHeight(true);
            

            {
                UIElement2DPtr uiElement = itemsListSpriteRenderer->getEntityRaw()->ensureComponent<UIElement2D>();
                uiElement->setCaptureCursorHits(true);
            }

            menuList->setItemsListTransform(itemsListSpriteRenderer->getTransform());


            CanvasPtr canvas = menuListEntity->ensureComponent<Canvas>();

            canvas->setClearColor(ColorU32::c_red);
            canvas->setClearColorFlag(false);
            canvas->setSortOrder(3000000);
            canvas->getCanvasScaler()->setScaleMode(CanvasScaler::ScaleMode::None);
            canvas->setViewportTransformPolicy(ViewportTransformPolicy::TransformToViewport);
            canvas->setRenderTarget(_ecsScene->castRaw<ECSRenderScene>()->getRenderTarget());

            UIElement2DPtr uiElement = menuListEntity->ensureComponent<UIElement2D>();
            uiElement->setCaptureCursorHits(true);

            MenuListItem2DPtr listItemPrefab = CreateDefaultMenuListItem(
                "",
                Vec2F(_size.x - 4.0f, 16.0f),
                Vec2F(2.0f, 0.0f),
                transform,
                _ecsScene,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            listItemPrefab->getEntityRaw()->setActiveSelf(false);
            menuList->setItemPrefab(listItemPrefab);

            SizePolicy2DPtr itemPrefabSizePolicy = listItemPrefab->getEntityRaw()->ensureComponent<SizePolicy2D>();
            itemPrefabSizePolicy->setFlag(SizePolicy2D::Height, false);
            itemPrefabSizePolicy->setSizeDelta(-4.0f, 0.0f);


            return menuList;
        }

        //////////////////////////////////////////
        MAZE_UI_API MenuListTree2DPtr CreateDefaultMenuListTree(
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            EntityPtr menuListTreeEntity = _ecsScene->createEntity();
            menuListTreeEntity->ensureComponent<Name>("Menu List Tree");

            MenuListTree2DPtr menuListTree = menuListTreeEntity->createComponent<MenuListTree2D>();

            Transform2DPtr const& transform = menuListTree->getTransform();
            transform->setParent(_parent);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);
            transform->setSize(Vec2F(150.0f, 150.0f));

            Transform2DPtr itemsListTransform = SpriteHelper::CreateTransform2D(
                transform->getSize(),
                Vec2F::c_zero,
                transform,
                _ecsScene,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            itemsListTransform->getEntityRaw()->ensureComponent<Name>()->setName("List");

            menuListTree->setItemsListTransform(itemsListTransform);

            MenuList2DPtr menuListPrefab = CreateDefaultMenuList(
                Vec2F(200.0f, 150.0f),
                Vec2F::c_zero,
                transform,
                _ecsScene,
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            menuListPrefab->getEntityRaw()->setActiveSelf(false);
            menuListTree->setMenuListPrefab(menuListPrefab);

            return menuListTree;
        }

        //////////////////////////////////////////
        MAZE_UI_API MenuBar2DPtr CreateDefaultMenuBarList(
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {

            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr menuListTreeEntity = _ecsScene->createEntity();
            menuListTreeEntity->ensureComponent<Name>("Menu Bar");

            MenuBar2DPtr menuBar = menuListTreeEntity->createComponent<MenuBar2D>();

            Transform2DPtr const& transform = menuBar->getTransform();
            transform->setParent(_parent);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);
            transform->setSize(_size);

            SpriteRenderer2DPtr backgroundSpriteRenderer = SpriteHelper::CreateSprite(
                ColorU32(245, 245, 245),
                _size,
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                transform,
                _ecsScene,
                Vec2F(0.5f, 0.5f),
                Vec2F(0.5f, 0.5f));
            SizePolicy2DPtr itemTextRendererSizePolicy = backgroundSpriteRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();

            
            HorizontalLayout2DPtr buttonsLayout = CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                _size,
                Vec2F::c_zero,
                transform,
                _ecsScene,
                Vec2F(0.0f, 0.5f),
                Vec2F(0.0f, 0.5f));
            buttonsLayout->setAutoWidth(false);
            menuBar->setLayoutTransform(buttonsLayout->getTransform());

            {
                EntityPtr buttonEntity = _ecsScene->createEntity();
                buttonEntity->setActiveSelf(false);
                buttonEntity->ensureComponent<Name>("ToggleButton");

                ToggleButton2DPtr toggleButton = buttonEntity->createComponent<ToggleButton2D>();
                toggleButton->setCheckByClick(false);
                toggleButton->setNormalColor({ 245, 245, 245 });
                toggleButton->setFocusedColor({ 213, 231, 248 });
                toggleButton->setSelectedColor({ 200, 220, 240 } );
                toggleButton->setPressedColor({ 213, 231, 248 });

                Transform2DPtr const& toggleTransform = toggleButton->getTransform();
                toggleTransform->setParent(transform);
                toggleTransform->setSize(Vec2F(100.0f, 14.0f));
                toggleTransform->setLocalPosition(_position);
                toggleTransform->setAnchor(_anchor);
                toggleTransform->setPivot(_pivot);

                SpriteRenderer2DPtr toggleButtonSpriteRenderer = SpriteHelper::CreateSprite(
                    ColorU32::c_red,
                    toggleTransform->getSize(),
                    Vec2F(0.0f, 0.0f),
                    renderSystem->getMaterialManager()->getColorMaterial(),
                    toggleTransform,
                    _ecsScene);
                toggleButtonSpriteRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();
                toggleButton->setTransitionSprite(toggleButtonSpriteRenderer);

                SystemTextRenderer2DPtr text = SystemUIHelper::CreateSystemText(
                    "File",
                    8,
                    HorizontalAlignment2D::Center,
                    VerticalAlignment2D::Middle,
                    toggleTransform->getSize(),
                    Vec2F::c_zero,
                    toggleButtonSpriteRenderer->getTransform(),
                    _ecsScene);
                text->getEntityRaw()->ensureComponent<SizePolicy2D>();
                text->getEntityRaw()->ensureComponent<Name>()->setName("Label");
                text->setColor(ColorU32::c_black);

                menuBar->setMenuButtonPrefab(toggleButton);
            }

            {
                MenuListTree2DPtr menuListTree = CreateDefaultMenuListTree(
                    Vec2F::c_zero,
                    transform,
                    _ecsScene,
                    Vec2F(0.0f, 1.0f),
                    Vec2F(0.0f, 1.0f));
                menuListTree->getEntityRaw()->setActiveSelf(false);
                
            }

            return menuBar;
        }

        //////////////////////////////////////////
        MAZE_UI_API Scrollbar2DPtr CreateDefaultScrollbarHorizontal(
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr scrollbarEntity = _ecsScene->createEntity();
            scrollbarEntity->ensureComponent<Name>("Horizontal Scrollbar");

            Scrollbar2DPtr scrollbar = scrollbarEntity->createComponent<Scrollbar2D>();
            scrollbar->setDirection(Scrollbar2DDirection::LeftToRight);

            Transform2DPtr const& transform = scrollbar->getTransform();
            transform->setParent(_parent);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);
            transform->setSize(_size);

            SpriteRenderer2DPtr spriteRenderer = scrollbarEntity->createComponent<SpriteRenderer2D>();
            spriteRenderer->setSprite(UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default));
            spriteRenderer->setMaterialCopy(
                renderSystem->getMaterialManager()->getColorTextureMaterial());
            spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);
            spriteRenderer->setColor(227, 227, 227);

            Transform2DPtr slidingAreaTransform = SpriteHelper::CreateTransform2D(
                _size,
                Vec2F::c_zero,
                transform,
                _ecsScene,
                Vec2F::c_zero,
                Vec2F::c_zero);
            slidingAreaTransform->getEntityRaw()->ensureComponent<SizePolicy2D>();

            SpriteRenderer2DPtr handleSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused),
                Vec2F(transform->getWidth() * 0.5f, transform->getHeight()),
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                slidingAreaTransform,
                _ecsScene,
                Vec2F(0.0f, 0.5f),
                Vec2F(0.0f, 0.5f));
            scrollbar->setHandleTransform(handleSpriteRenderer->getTransform());
            scrollbar->setHandleSpriteRenderer(handleSpriteRenderer);
            

            return scrollbar;
        }

        //////////////////////////////////////////
        MAZE_UI_API Scrollbar2DPtr CreateDefaultScrollbarVertical(
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr scrollbarEntity = _ecsScene->createEntity();
            scrollbarEntity->ensureComponent<Name>("Vertical Scrollbar");

            Scrollbar2DPtr scrollbar = scrollbarEntity->createComponent<Scrollbar2D>();
            scrollbar->setDirection(Scrollbar2DDirection::TopToBottom);

            Transform2DPtr const& transform = scrollbar->getTransform();
            transform->setParent(_parent);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);
            transform->setSize(_size);

            SpriteRenderer2DPtr spriteRenderer = scrollbarEntity->createComponent<SpriteRenderer2D>();
            spriteRenderer->setSprite(UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default));
            spriteRenderer->setMaterialCopy(
                renderSystem->getMaterialManager()->getColorTextureMaterial());
            spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);
            spriteRenderer->setColor(227, 227, 227);

            Transform2DPtr slidingAreaTransform = SpriteHelper::CreateTransform2D(
                _size,
                Vec2F::c_zero,
                transform,
                _ecsScene,
                Vec2F::c_zero,
                Vec2F::c_zero);
            slidingAreaTransform->getEntityRaw()->ensureComponent<SizePolicy2D>();

            SpriteRenderer2DPtr handleSpriteRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused),
                Vec2F(transform->getWidth() * 0.5f, transform->getHeight()),
                Vec2F(0.0f, 0.0f),
                renderSystem->getMaterialManager()->getColorTextureMaterial(),
                slidingAreaTransform,
                _ecsScene,
                Vec2F(0.0f, 0.5f),
                Vec2F(0.0f, 0.5f));
            scrollbar->setHandleTransform(handleSpriteRenderer->getTransform());
            scrollbar->setHandleSpriteRenderer(handleSpriteRenderer);


            return scrollbar;
        }

        //////////////////////////////////////////
        MAZE_UI_API ScrollRect2DPtr CreateDefaultScrollRect(
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot,
            bool _horizontalScrollbar,
            bool _verticalScrollbar)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr scrollbarEntity = _ecsScene->createEntity("ScrollRect");

            ScrollRect2DPtr scrollRect = scrollbarEntity->createComponent<ScrollRect2D>();
            ScrollRect2DWPtr scrollRectWeak = scrollRect;

            Transform2DPtr const& transform = scrollRect->getTransform();
            transform->setParent(_parent);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);
            transform->setSize(_size);

            scrollRect->getUIElement()->eventCursorWheel.subscribe(
                [scrollRectWeak](CursorWheelInputEvent const& _event)
                {
                    ScrollRect2DPtr scrollRect = scrollRectWeak.lock();
                    if (scrollRect)
                    {
                        F32 value = scrollRect->getNormalizedPosition(1);
                        scrollRect->setNormalizedPosition(
                            Math::Clamp(value - 0.001f * _event.deltaWheel, 0.0f, 1.0f),
                            1);
                    }
                });

            SpriteRenderer2DPtr spriteRenderer = scrollbarEntity->createComponent<SpriteRenderer2D>();
            spriteRenderer->setSprite(UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default));
            spriteRenderer->setMaterialCopy(
                renderSystem->getMaterialManager()->getColorTextureMaterial());
            spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);
            spriteRenderer->setColor(255, 255, 255);

            {
                Transform2DPtr viewportTransform = SpriteHelper::CreateTransform2D(
                    Vec2F(
                        _size.x + (_horizontalScrollbar ? -18.0f : 0.0f),
                        _size.y + (_verticalScrollbar ? -18.0f : 0.0f)),
                    Vec2F(0.0f, 0.0f),
                    transform,
                    _ecsScene,
                    Vec2F(0.0f, 1.0f),
                    Vec2F(0.0f, 1.0f));
                viewportTransform->getEntityRaw()->ensureComponent<Name>()->setName("Viewport");
                ScissorMask2DPtr scissorMark = viewportTransform->getEntityRaw()->ensureComponent<ScissorMask2D>();
                scissorMark->setPadding(1.0f, 1.0f, 1.0f, 1.0f);
                scrollRect->setViewportTransform(viewportTransform);

                {

                    Transform2DPtr contentTransform = SpriteHelper::CreateTransform2D(
                        Vec2F(_size.x, _size.y),
                        Vec2F(0.0f, 0.0f),
                        viewportTransform,
                        _ecsScene,
                        Vec2F(0.0f, 1.0f),
                        Vec2F(0.0f, 1.0f));
                    contentTransform->getEntityRaw()->ensureComponent<Name>()->setName("Content");
                    scrollRect->setContentTransform(contentTransform);
                }
            }

            if (_horizontalScrollbar)
            {
                F32 deltaWidth = _verticalScrollbar ? -18.0f : 0.0f;
                Scrollbar2DPtr scrollbarHorizontal = CreateDefaultScrollbarHorizontal(
                    Vec2F(_size.x + deltaWidth, 18),
                    Vec2F(0.0f, 0.0f),
                    transform,
                    _ecsScene,
                    Vec2F(0.0f, 0.0f),
                    Vec2F(0.0f, 0.0f));
                SizePolicy2DPtr scrollHorizontalSizeDelta = scrollbarHorizontal->getEntityRaw()->ensureComponent<SizePolicy2D>();
                scrollHorizontalSizeDelta->setFlag(SizePolicy2D::Height, false);
                scrollHorizontalSizeDelta->setSizeDelta(deltaWidth, 0.0f);
                scrollRect->setHorizontalScrollbar(scrollbarHorizontal);
            }

            if (_verticalScrollbar)
            {
                F32 deltaHeight = _horizontalScrollbar ? -18.0f : 0.0f;
                Scrollbar2DPtr scrollbarVertical = CreateDefaultScrollbarVertical(
                    Vec2F(18, _size.y + deltaHeight),
                    Vec2F(0.0f, 0.0f),
                    transform,
                    _ecsScene,
                    Vec2F(1.0f, 1.0f),
                    Vec2F(1.0f, 1.0f));
                SizePolicy2DPtr scrollVerticalSizeDelta = scrollbarVertical->getEntityRaw()->ensureComponent<SizePolicy2D>();
                scrollVerticalSizeDelta->setFlag(SizePolicy2D::Width, false);
                scrollVerticalSizeDelta->setSizeDelta(0.0f, deltaHeight);
                scrollRect->setVerticalScrollbar(scrollbarVertical);
            }


            return scrollRect;
        }

        //////////////////////////////////////////
        MAZE_UI_API TextRenderer2DPtr CreateText(
            CString _text,
            U32 _fontSize,
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            EntityPtr textRendererEntity = _ecsScene->createEntity();
            textRendererEntity->ensureComponent<Name>("Text");

            TextRenderer2DPtr textRenderer = textRendererEntity->createComponent<TextRenderer2D>();
            textRenderer->setText(_text);
            textRenderer->setFontSize(_fontSize);
            textRenderer->setHorizontalAlignment(_horizontalAlignment);
            textRenderer->setVerticalAlignment(_verticalAlignment);
            textRenderer->setFontMaterial(FontMaterialManager::GetInstancePtr()->getDefaultFontMaterial());

            Transform2DPtr transform = textRendererEntity->ensureComponent<Transform2D>();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            return textRenderer;
        }
    
    } // namespace UIHelper
    //////////////////////////////////////////
    
} // namespace Maze
//////////////////////////////////////////
