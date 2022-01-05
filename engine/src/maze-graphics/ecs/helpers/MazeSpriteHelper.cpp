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
#include "MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeEntityAspect.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SpriteHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API Transform2DPtr CreateTransform2D(
            Vec2DF const& _size, 
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor,
            Vec2DF const& _pivot)
        {
            Maze::EntityPtr spriteRendererEntity = _ecsScene->createEntity();
            spriteRendererEntity->ensureComponent<Name>("Node2D");
        
            Maze::Transform2DPtr const& transform = spriteRendererEntity->ensureComponent<Transform2D>();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            return transform;
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API SpriteRenderer2DPtr CreateSprite(
            SpritePtr const& _sprite,
            Vec2DF const& _size, 
            Vec2DF const& _position,
            MaterialPtr const& _material,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor,
            Vec2DF const& _pivot)
        {
            Maze::EntityPtr spriteRendererEntity = _ecsScene->createEntity();
            spriteRendererEntity->ensureComponent<Name>("Sprite");
        
            Maze::SpriteRenderer2DPtr spriteRenderer = spriteRendererEntity->createComponent<Maze::SpriteRenderer2D>();
            spriteRenderer->setSprite(_sprite);
            spriteRenderer->setMaterial(_material);

            if (_sprite && _sprite->getSliceBorder().isBorderExists())
                spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            Maze::Transform2DPtr const& transform = spriteRenderer->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            return spriteRenderer;
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API SpriteRenderer2DPtr CreateSprite(
            CString _spriteName,
            Vec2DF const& _size, 
            Vec2DF const& _position,
            MaterialPtr const& _material,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor,
            Vec2DF const& _pivot)
        {
            Maze::EntityPtr spriteRendererEntity = _ecsScene->createEntity();
            spriteRendererEntity->ensureComponent<Name>("Sprite");
        
            Maze::SpriteRenderer2DPtr spriteRenderer = spriteRendererEntity->createComponent<Maze::SpriteRenderer2D>();
            spriteRenderer->setSprite(_spriteName);
            spriteRenderer->setMaterial(_material ? _material : SpriteManager::GetCurrentInstance()->getDefaultSpriteMaterial());

            if (spriteRenderer->getSprite()->getSliceBorder().isBorderExists())
                spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);

            Maze::Transform2DPtr const& transform = spriteRenderer->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            return spriteRenderer;
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API SpriteRenderer2DPtr CreateSprite(
            CString _spriteName,
            Vec2DF const& _size, 
            Vec2DF const& _position,
            CString _materialName,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor,
            Vec2DF const& _pivot)
        {
            return CreateSprite(
                _spriteName,
                _size,
                _position,
                _materialName   ? MaterialManager::GetCurrentInstance()->getMaterial(_materialName)
                                : SpriteManager::GetCurrentInstance()->getDefaultSpriteMaterial(),
                _parent,
                _ecsScene,
                _anchor,
                _pivot);
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API SpriteRenderer2DPtr CreateSprite(
            ColorU32 _color,
            Vec2DF const& _size, 
            Vec2DF const& _position,
            MaterialPtr const& _material,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor,
            Vec2DF const& _pivot)
        {
            Maze::EntityPtr spriteRendererEntity = _ecsScene->createEntity();
            spriteRendererEntity->ensureComponent<Name>("Sprite");
        
            Maze::SpriteRenderer2DPtr spriteRenderer = spriteRendererEntity->createComponent<Maze::SpriteRenderer2D>();
            spriteRenderer->setColor(_color);
            spriteRenderer->setMaterial(_material ? _material : MaterialManager::GetCurrentInstance()->getColorMaterial());

            Maze::Transform2DPtr const& transform = spriteRenderer->getTransform();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            return spriteRenderer;
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API SystemTextRenderer2DPtr CreateSystemText(
            CString _text,
            U32 _fontSize,
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2DF const& _size, 
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor,
            Vec2DF const& _pivot)
        {
            Maze::EntityPtr textRendererEntity = _ecsScene->createEntity();
            textRendererEntity->ensureComponent<Name>("Text");
        
            Maze::SystemTextRenderer2DPtr textRenderer = textRendererEntity->createComponent<Maze::SystemTextRenderer2D>();
            textRenderer->setText(_text);
            textRenderer->setFontSize(_fontSize);
            textRenderer->setHorizontalAlignment(_horizontalAlignment);
            textRenderer->setVerticalAlignment(_verticalAlignment);
            textRenderer->setSystemFont(SystemFontManager::GetCurrentInstancePtr()->getSystemFont());

            Transform2DPtr transform = textRendererEntity->ensureComponent<Transform2D>();
            transform->setParent(_parent);
            transform->setSize(_size);
            transform->setLocalPosition(_position);
            transform->setAnchor(_anchor);
            transform->setPivot(_pivot);

            return textRenderer;
        }

    } // namespace SpriteHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
