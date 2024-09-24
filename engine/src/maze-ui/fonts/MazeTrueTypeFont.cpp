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
#include "maze-ui/fonts/MazeTrueTypeFont.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-ui/managers/MazeTrueTypeFontManager.hpp"
#include "maze-ui/assets/MazeAssetUnitTrueTypeFont.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TrueTypeFont
    //
    //////////////////////////////////////////
    TrueTypeFont::TrueTypeFont()
    {
        
    }

    //////////////////////////////////////////
    TrueTypeFont::~TrueTypeFont()
    {
        
    }

    //////////////////////////////////////////
    TrueTypeFontPtr TrueTypeFont::Create(
        AssetFilePtr const& _assetFile)
    {
        TrueTypeFontPtr font = TrueTypeFontManager::GetInstancePtr()->createTrueTypeFont();
        if (!font)
            return nullptr;

        if (_assetFile)
            font->loadFromAssetFile(_assetFile);

        return font;
    }

    //////////////////////////////////////////
    bool TrueTypeFont::init()
    {
        return true;
    }


    //////////////////////////////////////////
    // Class TrueTypeFontAssetRef
    //
    //////////////////////////////////////////
    bool TrueTypeFontAssetRef::loadFromDataBlock(DataBlock const& _dataBlock)
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
                if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitTrueTypeFont>::UID())
                {
                    setTrueTypeFont(assetUnit->castRaw<AssetUnitTrueTypeFont>()->loadTrueTypeFont(true));
                    return true;
                }

                break;
            }
            // by name
            case DataBlockParamType::ParamString:
            {
                String const& name = _dataBlock.getString(paramIndex);
                TrueTypeFontPtr const& material = TrueTypeFontManager::GetInstancePtr()->getOrLoadTrueTypeFont(name);
                setTrueTypeFont(material);
                return true;
            }
            default:
            {
                MAZE_ERROR("No supported asset ref type: %s!", c_dataBlockParamTypeInfo[(U8)paramType].name.str);
                break;
            }
            }
        }

        setTrueTypeFont(TrueTypeFontPtr());
        return true;
    }

    //////////////////////////////////////////
    void TrueTypeFontAssetRef::toDataBlock(DataBlock& _dataBlock) const
    {
        if (!m_font)
        {
            _dataBlock.clearData();
            return;
        }

        // Save as AUID
        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(m_font->getName());
            if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitTrueTypeFont>::UID())
            {
                TrueTypeFontPtr const& assetUnitTrueTypeFont = assetUnit->castRaw<AssetUnitTrueTypeFont>()->getTrueTypeFont();
                if (assetUnitTrueTypeFont == m_font)
                {
                    ValueToDataBlock(assetUnit->getAssetUnitId(), _dataBlock);
                    return;
                }
            }
        }

        // Save as string
        ValueToDataBlock(m_font->getName().c_str(), _dataBlock);
    }

} // namespace Maze
//////////////////////////////////////////
