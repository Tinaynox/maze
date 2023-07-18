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
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/MazeTexture2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct SpriteBorder
    //
    //////////////////////////////////////////
    SpriteSliceBorder::SpriteSliceBorder()
        : left(0.0f)
        , bottom(0.0f)
        , right(0.0f)
        , top(0.0f)
    {
    }

    //////////////////////////////////////////
    SpriteSliceBorder::SpriteSliceBorder(F32 _left, F32 _bottom, F32 _right, F32 _top)
        : left(_left)
        , bottom(_bottom)
        , right(_right)
        , top(_top)
    {
    }

    //////////////////////////////////////////
    bool SpriteSliceBorder::isBorderExists() const
    {
        return (left != 0.0f) || (bottom != 0.0f) || (right != 0.0f) || (top != 0.0f);
    }


    //////////////////////////////////////////
    // Class Sprite
    //
    //////////////////////////////////////////
    Sprite::Sprite()
        : m_colorOffset(Vec2DF::c_zero)
        , m_colorPosition(Vec2DF::c_zero)
        , m_colorSize(Vec2DF::c_zero)
        , m_nativeSize(Vec2DF::c_zero)
        , m_textureCoordLB(Vec2DF::c_zero)
        , m_textureCoordRT(Vec2DF::c_one)
    {
    }

    //////////////////////////////////////////
    Sprite::~Sprite()
    {
    }

    //////////////////////////////////////////
    SpritePtr Sprite::Create()
    {
        SpritePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(
            Sprite,
            object,
            init());
        return object;
    }

    //////////////////////////////////////////
    SpritePtr Sprite::Create(
        Texture2DPtr const& _texture,
        Vec2DF const& _colorPosition,
        Vec2DF const& _colorSize,
        Vec2DF const& _colorOffset,
        Vec2DF const& _nativeSize)
    {
        SpritePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(
            Sprite,
            object,
            init(
                _texture,
                _colorPosition,
                _colorSize,
                _colorOffset,
                _nativeSize));
        return object;
    }

    //////////////////////////////////////////
    SpritePtr Sprite::Create(
        Texture2DPtr const& _texture,
        Vec2DF const& _nativePosition,
        Vec2DF const& _nativeSize)
    {
        return Create(
            _texture,
            _nativePosition,
            _nativeSize,
            Vec2DF::c_zero,
            _nativeSize);
    }

    //////////////////////////////////////////
    bool Sprite::init()
    {
        return true;
    }

    //////////////////////////////////////////
    bool Sprite::init(
        Texture2DPtr const& _texture,
        Vec2DF const& _colorPosition,
        Vec2DF const& _colorSize,
        Vec2DF const& _colorOffset,
        Vec2DF const& _nativeSize)
    {
        set(_texture, _colorPosition, _colorSize, _colorOffset, _nativeSize);

        return true;
    }

    //////////////////////////////////////////
    void Sprite::set(Texture2DPtr const& _texture)
    {
        if (_texture)
        {
            set(
                _texture,
                Vec2DF::c_zero,
                _texture->getSize(),
                Vec2DF::c_zero,
                _texture->getSize());
        }
    }

    //////////////////////////////////////////
    void Sprite::set(
        Texture2DPtr const& _texture,
        Vec2DF const& _colorPosition,
        Vec2DF const& _colorSize,
        Vec2DF const& _colorOffset,
        Vec2DF const& _nativeSize)
    {
        m_texture = _texture;
        m_colorPosition = _colorPosition;
        m_colorSize = _colorSize;
        m_colorOffset = _colorOffset;
        m_nativeSize = _nativeSize;

        if (m_texture)
        {
            if (m_name.empty())
                m_name = m_texture->getName();

            if (m_colorSize == Vec2DF::c_zero)
                m_colorSize = m_nativeSize = m_texture->getSize();
        }

        updateTextureCoords();

        eventDataChanged(this);
    }

    //////////////////////////////////////////
    void Sprite::setColorSize(Vec2DF const& _colorSize)
    {
        m_colorSize = _colorSize;
        updateTextureCoords();

        eventDataChanged(this);
    }

    //////////////////////////////////////////
    void Sprite::setColorPosition(Vec2DF const& _colorPosition)
    {
        m_colorPosition = _colorPosition;
        updateTextureCoords();

        eventDataChanged(this);
    }

    //////////////////////////////////////////
    void Sprite::setNativeSize(Vec2DF const& _nativeSize)
    {
        m_nativeSize = _nativeSize;
        setColorSize(m_nativeSize);

        eventDataChanged(this);
    }

    //////////////////////////////////////////
    void Sprite::updateTextureCoords()
    {
        if (!m_texture)
            return;

        F32 tw = (F32)m_texture->getWidth();
        F32 th = (F32)m_texture->getHeight();

        m_textureCoordLB = Vec2DF(
            m_colorPosition.x / tw,
            m_colorPosition.y / th);

        m_textureCoordRT = Vec2DF(
            (m_colorPosition.x + m_colorSize.x) / tw,
            (m_colorPosition.y + m_colorSize.y) / th);
    }

    //////////////////////////////////////////
    void Sprite::FromString(SpritePtr& _value, CString _data, Size _count)
    {
        if (!_data || strcmp(_data, "") == 0)
        {
            _value.reset();
            return;
        }

        if (_count == 0)
            _count = strlen(_data);

        RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();

        if (StringHelper::IsStartsWith(_data, "ptr:"))
        {
            String data = String(_data + 4, _data + _count);
            StringHelper::StringToObjectPtr(_value, data);
        }
        else
        {
            _value = spriteManager->getSprite(MAZE_HASHED_CSTRING(_data));
        }
    }

    //////////////////////////////////////////
    void Sprite::ToString(Sprite const* _value, String& _data)
    {
        if (!_value)
        {
            _data.clear();
            return;
        }

        // RenderSystemPtr const& renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        // SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        // String const& spriteName = spriteManager->getSpriteName(_value);

        String const& spriteName = _value->getName();
        if (!spriteName.empty())
        {
            _data = spriteName;
        }
        else
        {
            StringHelper::FormatString(_data, "ptr:%p", _value);
        }
    }


} // namespace Maze
//////////////////////////////////////////
