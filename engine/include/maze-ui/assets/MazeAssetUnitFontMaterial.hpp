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
#if (!defined(_MazeAssetUnitFontMaterial_hpp_))
#define _MazeAssetUnitFontMaterial_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/assets/MazeAssetUnit.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AssetUnitFontMaterial);   
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(FontMaterial);


    //////////////////////////////////////////
    // Class AssetUnitFontMaterial
    //
    //////////////////////////////////////////
    class MAZE_UI_API AssetUnitFontMaterial
        : public AssetUnit
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetUnitFontMaterial, AssetUnit);
        
        //////////////////////////////////////////
        MAZE_FORCEINLINE static HashedCString GetDataBlockId() { return MAZE_HCS("fontMaterial"); }

    public:

        //////////////////////////////////////////
        virtual ~AssetUnitFontMaterial();

        //////////////////////////////////////////
        virtual HashedCString getDataBlockId() const MAZE_OVERRIDE { return GetDataBlockId(); }

        //////////////////////////////////////////
        static AssetUnitFontMaterialPtr Create(
            AssetFilePtr const& _assetFile,
            DataBlock const& _data = DataBlock::c_empty);


        //////////////////////////////////////////
        inline FontMaterialPtr const& getFontMaterial() const { return m_fontMaterial; }

        //////////////////////////////////////////
        FontMaterialPtr const& loadFontMaterial(bool _syncLoad = false);


        //////////////////////////////////////////
        FontMaterialPtr const& initFontMaterial();

    protected:

        //////////////////////////////////////////
        AssetUnitFontMaterial();

        //////////////////////////////////////////
        virtual bool init(
            AssetFilePtr const& _assetFile,
            DataBlock const& _data) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool loadNowImpl() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool unloadNowImpl() MAZE_OVERRIDE;
    
    protected:
        FontMaterialPtr m_fontMaterial;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetUnitFontMaterial_hpp_
//////////////////////////////////////////
