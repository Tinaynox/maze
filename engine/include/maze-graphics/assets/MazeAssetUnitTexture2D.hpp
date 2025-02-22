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
#if (!defined(_MazeAssetUnitTexture2D_hpp_))
#define _MazeAssetUnitTexture2D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/assets/MazeAssetUnit.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(AssetUnitTexture2D);   
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);
    MAZE_USING_MANAGED_SHARED_PTR(Texture2D);


    //////////////////////////////////////////
    // Class AssetUnitTexture2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API AssetUnitTexture2D
        : public AssetUnit
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetUnitTexture2D, AssetUnit);
        
        //////////////////////////////////////////
        MAZE_FORCEINLINE static HashedCString GetDataBlockId() { return MAZE_HCS("texture2d"); }

    public:

        //////////////////////////////////////////
        virtual ~AssetUnitTexture2D();

        //////////////////////////////////////////
        virtual HashedCString getDataBlockId() const MAZE_OVERRIDE { return GetDataBlockId(); }

        //////////////////////////////////////////
        static AssetUnitTexture2DPtr Create(
            AssetFilePtr const& _assetFile,
            DataBlock const& _data = DataBlock::c_empty);


        //////////////////////////////////////////
        inline Texture2DPtr const& getTexture() const { return m_texture; }

        //////////////////////////////////////////
        Texture2DPtr const& loadTexture(bool _syncLoad = false);


        //////////////////////////////////////////
        Texture2DPtr const& initTexture();


        //////////////////////////////////////////
        virtual void updateDataFromAssetUnit(DataBlock& _metaData) const MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        AssetUnitTexture2D();

        //////////////////////////////////////////
        virtual bool init(
            AssetFilePtr const& _assetFile,
            DataBlock const& _data) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool loadNowImpl() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool unloadNowImpl() MAZE_OVERRIDE;
    
    protected:
        Texture2DPtr m_texture;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetUnitTexture2D_hpp_
//////////////////////////////////////////
