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
#include "maze-ui/ecs/components/MazeSystemTextEditBox2D.hpp"
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
        MAZE_UI_API SystemTextEditBox2DPtr CreateDefaultEditBox(
            CString _text,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor,
            Vec2DF const& _pivot)
        {
            RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

            EntityPtr editBoxEntity = _ecsScene->createEntity();
            editBoxEntity->ensureComponent<Name>("EdidBox");

            SystemTextEditBox2DPtr editBox = editBoxEntity->createComponent<SystemTextEditBox2D>();

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
            spriteRenderer->setMaterial(
                renderSystem->getMaterialManager()->getColorTextureMaterial());
            spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            auto updateEditBoxState =
                [](SystemTextEditBox2D* _editBox, SpriteRenderer2D* _spriteRenderer)
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
                [=](SystemTextEditBox2D* _editBox, bool _value)
            {
                updateEditBoxState(_editBox, spriteRendererRaw);
            });

            editBox->eventSelectedChanged.subscribe(
                [=](SystemTextEditBox2D* _editBox, bool _value)
            {
                updateEditBoxState(_editBox, spriteRendererRaw);
            });

            SystemTextRenderer2DPtr textRenderer = SystemUIHelper::CreateSystemText(
                "",
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                _size + Vec2DF(-10.0f, 0.0f),
                Vec2DF::c_zero,
                transform,
                _ecsScene);
            textRenderer->setColor(ColorU32::c_black);

            SizePolicy2DPtr textSizePolicy = textRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();
            textSizePolicy->setSizeDelta(-10.0f, 0.0f);

            editBox->setTextRenderer(textRenderer->cast<AbstractTextRenderer2D>());
            editBox->setText(_text);

            F32 cursorHeight = 8.0f + 4.0f;
            SpriteRenderer2DPtr cursorRenderer = SpriteHelper::CreateSprite(
                textRenderer->getColor(),
                Vec2DF(1.0f, cursorHeight),
                Vec2DF::c_zero,
                renderSystem->getMaterialManager()->getColorMaterial(),
                textRenderer->getTransform(),
                _ecsScene,
                Vec2DF::c_zero,
                Vec2DF(0.0f, (0.5f * (cursorHeight - (F32)textRenderer->getFontSize()) - 1) / cursorHeight));

            editBox->setCursorRenderer(cursorRenderer);

            return editBox;
        }
    } // namespace SystemUIHelper
    //////////////////////////////////////////
    
} // namespace Maze
//////////////////////////////////////////
