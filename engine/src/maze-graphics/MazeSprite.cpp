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
#include "maze-graphics/assets/MazeAssetUnitSprite.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"


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
    MAZE_IMPLEMENT_INDEXED_RESOURCE(Sprite);


    //////////////////////////////////////////
    // Class Sprite
    //
    //////////////////////////////////////////
    // Sprite* Sprite::s_instancesList = nullptr;

    //////////////////////////////////////////
    Sprite::Sprite()
        : m_colorOffset(Vec2F::c_zero)
        , m_colorPosition(Vec2F::c_zero)
        , m_colorSize(Vec2F::c_zero)
        , m_nativeSize(Vec2F::c_zero)
        , m_textureCoordLB(Vec2F::c_zero)
        , m_textureCoordRT(Vec2F::c_one)
    {
        //if (s_instancesList)
        //    s_instancesList->m_instancesListNext = this;
        //m_instancesListPrev = s_instancesList;
        //s_instancesList = this;
    }

    //////////////////////////////////////////
    Sprite::~Sprite()
    {
        //if (m_instancesListPrev)
        //    m_instancesListPrev->m_instancesListNext = m_instancesListNext;
        //if (m_instancesListNext)
        //    m_instancesListNext->m_instancesListPrev = m_instancesListPrev;
        //else
        //if (s_instancesList == this)
        //    s_instancesList = m_instancesListPrev;
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
        Vec2F const& _colorPosition,
        Vec2F const& _colorSize,
        Vec2F const& _colorOffset,
        Vec2F const& _nativeSize)
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
        Vec2F const& _nativePosition,
        Vec2F const& _nativeSize)
    {
        return Create(
            _texture,
            _nativePosition,
            _nativeSize,
            Vec2F::c_zero,
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
        Vec2F const& _colorPosition,
        Vec2F const& _colorSize,
        Vec2F const& _colorOffset,
        Vec2F const& _nativeSize)
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
                Vec2F::c_zero,
                _texture->getSize(),
                Vec2F::c_zero,
                _texture->getSize());
        }
    }

    //////////////////////////////////////////
    void Sprite::set(
        Texture2DPtr const& _texture,
        Vec2F const& _colorPosition,
        Vec2F const& _colorSize,
        Vec2F const& _colorOffset,
        Vec2F const& _nativeSize)
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

            if (m_colorSize == Vec2F::c_zero)
                m_colorSize = m_nativeSize = m_texture->getSize();
        }

        updateTextureCoords();

        eventDataChanged(this);
    }

    //////////////////////////////////////////
    void Sprite::setColorSize(Vec2F const& _colorSize)
    {
        m_colorSize = _colorSize;
        updateTextureCoords();

        eventDataChanged(this);
    }

    //////////////////////////////////////////
    void Sprite::setColorPosition(Vec2F const& _colorPosition)
    {
        m_colorPosition = _colorPosition;
        updateTextureCoords();

        eventDataChanged(this);
    }

    //////////////////////////////////////////
    void Sprite::setNativeSize(Vec2F const& _nativeSize)
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

        m_textureCoordLB = Vec2F(
            m_colorPosition.x / tw,
            m_colorPosition.y / th);

        m_textureCoordRT = Vec2F(
            (m_colorPosition.x + m_colorSize.x) / tw,
            (m_colorPosition.y + m_colorSize.y) / th);
    }

    //////////////////////////////////////////
    String Sprite::toString() const
    {
        String result;
        Sprite::ToString(this, result);
        return result;
    }

    //////////////////////////////////////////
    void Sprite::setString(CString _data, Size _count)
    {
        MAZE_TODO;
    }

    //////////////////////////////////////////
    void Sprite::FromString(SpritePtr& _value, CString _data, Size _count)
    {
        MAZE_PROFILE_EVENT("Sprite::FromString");

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
            _value = spriteManager->getOrLoadSprite(MAZE_HASHED_CSTRING(_data));
        }
    }

    //////////////////////////////////////////
    void Sprite::ToString(Sprite const* _value, String& _data)
    {
        MAZE_PROFILE_EVENT("Sprite::ToString");

        if (!_value)
        {
            _data.clear();
            return;
        }

        String const& spriteName = _value->getName();
        if (!spriteName.empty())
        {
            _data = spriteName;
        }
        else
        {
            // #TODO: Replace with ResourceId
            StringHelper::FormatString(_data, "ptr:%p", _value);
        }
    }

    //////////////////////////////////////////
    //void Sprite::IterateSprites(std::function<bool(Sprite*)> _cb)
    //{
    //    Sprite* instance = s_instancesList;
    //    while (instance)
    //    {
    //        if (!_cb(instance))
    //            break;

    //        instance = instance->m_instancesListPrev;
    //    }
    //}


    //////////////////////////////////////////
    // Class SpriteAssetRef
    //
    //////////////////////////////////////////
    String SpriteAssetRef::toString() const
    {
        if (!m_sprite)
            return String();

        HashedCString name = RenderSystem::GetCurrentInstancePtr()->getSpriteManager()->getSpriteName(m_sprite.get());
        return !name.empty() ? String(name.str) : m_sprite->getName().getString();
    }

    //////////////////////////////////////////
    void SpriteAssetRef::setString(CString _data, Size _count)
    {
        SpritePtr const& sprite = RenderSystem::GetCurrentInstancePtr()->getSpriteManager()->getOrLoadSprite(_data);
        setSprite(sprite);
    }

    //////////////////////////////////////////
    bool SpriteAssetRef::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        DataBlock::ParamIndex paramIndex = _dataBlock.findParamIndex(MAZE_HCS("value"));
        if (paramIndex >= 0)
        {
            DataBlockParamType paramType = _dataBlock.getParamType(paramIndex);
            switch (paramType)
            {
                // by AUID
                case DataBlockParamType::ParamU32:
                {
                    AssetUnitId auid = _dataBlock.getU32(paramIndex);

                    AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(auid);
                    if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitSprite>::UID())
                    {
                        setSprite(assetUnit->castRaw<AssetUnitSprite>()->loadSprite(true));
                        return true;
                    }

                    break;
                }
                // by ResourceId
                case DataBlockParamType::ParamS32:
                {
                    ResourceId resourceId(_dataBlock.getS32(paramIndex));
                    setSprite(Sprite::GetResource(resourceId));
                    return true;
                }
                // by name
                case DataBlockParamType::ParamString:
                {
                    String const& name = _dataBlock.getString(paramIndex);
                    SpritePtr const& sprite = SpriteManager::GetCurrentInstance()->getOrLoadSprite(name);
                    setSprite(sprite);
                    return true;
                }
                default:
                {
                    MAZE_ERROR("No supported asset ref type: %s!", c_dataBlockParamTypeInfo[(U8)paramType].name.str);
                    break;
                }
            }
        }

        setSprite(SpritePtr());
        return true;
    }

    //////////////////////////////////////////
    void SpriteAssetRef::toDataBlock(DataBlock& _dataBlock) const
    {
        if (!m_sprite)
        {
            _dataBlock.clearData();
            return;
        }

        // Static asset (AUID)
        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(m_sprite->getName());
            if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitSprite>::UID())
            {
                SpritePtr const& assetUnitMaterial = assetUnit->castRaw<AssetUnitSprite>()->getSprite();
                if (assetUnitMaterial == m_sprite)
                {
                    ValueToDataBlock(assetUnit->getAssetUnitId(), _dataBlock);
                    return;
                }
            }
        }

        // Built-in asset (name)
        if (SpriteManager::GetCurrentInstance())
        {
            for (S32 i = 1; i < BuiltinSpriteType::MAX; ++i)
            {
                if (SpriteManager::GetCurrentInstance()->getBuiltinSprite((BuiltinSpriteType)i) == m_sprite)
                {
                    ValueToDataBlock(m_sprite->getName().c_str(), _dataBlock);
                    return;
                }
            }
        }

        // Runtime resource (ResourceId)
        ValueToDataBlock((S32)m_sprite->getResourceId().getId(), _dataBlock);
    }


} // namespace Maze
//////////////////////////////////////////
