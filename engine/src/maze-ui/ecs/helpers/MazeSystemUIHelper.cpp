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
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
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
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
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
    // Namespace SystemUIHelper
    namespace SystemUIHelper
    {
        //////////////////////////////////////////
        MAZE_UI_API EditBox2DPtr CreateDefaultEditBox(
            CString _text,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
            Vec2F const& _anchor,
            Vec2F const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr editBoxEntity = _ecsScene->createEntity();
            editBoxEntity->ensureComponent<Name>("EditBox");

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
                renderSystem->getMaterialManager()->getSpriteMaterial());
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

            SystemTextRenderer2DPtr textRenderer = SystemUIHelper::CreateSystemText(
                "",
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                _size + Vec2F(-10.0f, 0.0f),
                Vec2F::c_zero,
                transform,
                _ecsScene);
            textRenderer->setColor(ColorU32::c_black);
            textRenderer->getEntityRaw()->ensureComponent<Name>()->setName("EditBoxText");

            SizePolicy2DPtr textSizePolicy = textRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();
            textSizePolicy->setSizeDelta(-10.0f, 0.0f);

            editBox->setTextRenderer(textRenderer->cast<AbstractTextRenderer2D>());
            editBox->setText(_text);

            F32 cursorHeight = 8.0f + 4.0f;
            F32 descent = (0.5f * (cursorHeight - (F32)textRenderer->getFontSize()) - 1);
            SpriteRenderer2DPtr cursorRenderer = SpriteHelper::CreateSprite(
                textRenderer->getColor(),
                Vec2F(1.0f, cursorHeight),
                Vec2F::c_zero,
                renderSystem->getMaterialManager()->getSpriteMaterial(),
                textRenderer->getTransform(),
                _ecsScene,
                Vec2F::c_zero,
                Vec2F(0.0f, descent / cursorHeight));

            editBox->setCursorRenderer(cursorRenderer);

            return editBox;
        }

        //////////////////////////////////////////
        MAZE_UI_API Dropdown2DPtr CreateDefaultDropdown(
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            EcsScene* _ecsScene,
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
                renderSystem->getMaterialManager()->getSpriteMaterial());
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

                    Vec2F positionWS = transform->getWorldTransform().transform(positionOS);
                    positionWS.y = Math::Clamp(positionWS.y, transform->getHeight(), rootSize.y);

                    Vec2F menuListPositionOS = transform->getParent()->getWorldTransform().inversed().transform(positionWS);

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


            SystemTextRenderer2DPtr textRenderer = SystemUIHelper::CreateSystemText(
                "Dropdown",
                8,
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
                renderSystem->getMaterialManager()->getSpriteMaterial(),
                transform,
                spriteRenderer->getEntityRaw()->getEcsScene(),
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
                    renderSystem->getMaterialManager()->getSpriteMaterial(),
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
                canvas->getCanvasScaler()->setScaleMode(CanvasScalerScaleMode::None);
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
                        renderSystem->getMaterialManager()->getSpriteMaterial(),
                        itemPrefabTransform,
                        _ecsScene);
                    backgroundSpriteRenderer->getEntityRaw()->ensureComponent<Name>()->setName("Background");
                    backgroundSpriteRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();

                    SpriteRenderer2DPtr checkMarkSprite = SpriteHelper::CreateSprite(
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::CheckMark),
                        Vec2F(8.0f, 8.0f) * 1.75f,
                        Vec2F(10.0f, 10.0f),
                        renderSystem->getMaterialManager()->getSpriteMaterial(),
                        itemPrefabTransform,
                        spriteRenderer->getEntityRaw()->getEcsScene(),
                        Vec2F(0.0f, 0.0f),
                        Vec2F(0.5f, 0.5f));
                    checkMarkSprite->getEntityRaw()->ensureComponent<Name>()->setName("CheckMark");
                    checkMarkSprite->setColor(ColorU32::c_black);

                    SystemTextRenderer2DPtr itemTextRenderer = SystemUIHelper::CreateSystemText(
                        "Option 1",
                        8,
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

    } // namespace SystemUIHelper
    //////////////////////////////////////////
    
} // namespace Maze
//////////////////////////////////////////
