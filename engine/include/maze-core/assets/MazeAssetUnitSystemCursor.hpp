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
#if (!defined(_MazeAssetUnitSystemCursor_hpp_))
#define _MazeAssetUnitSystemCursor_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/assets/MazeAssetUnit.hpp"
#include "maze-core/system/MazeSystemCursor.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(AssetUnitSystemCursor);   
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Class AssetUnitSystemCursor
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AssetUnitSystemCursor
        : public AssetUnit
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetUnitSystemCursor, AssetUnit);
        
        //////////////////////////////////////////
        MAZE_FORCEINLINE static HashedCString GetDataBlockId() { return MAZE_HCS("systemCursor"); }

    public:

        //////////////////////////////////////////
        virtual ~AssetUnitSystemCursor();

        //////////////////////////////////////////
        virtual HashedCString getDataBlockId() const MAZE_OVERRIDE { return GetDataBlockId(); }

        //////////////////////////////////////////
        static AssetUnitSystemCursorPtr Create(
            AssetFilePtr const& _assetFile,
            DataBlock const& _data = DataBlock::c_empty);


        //////////////////////////////////////////
        inline SystemCursorPtr const& getCursor() const { return m_cursor; }

        //////////////////////////////////////////
        SystemCursorPtr const& loadCursor();


        //////////////////////////////////////////
        SystemCursorPtr const& initCursor();

    protected:

        //////////////////////////////////////////
        AssetUnitSystemCursor();

        //////////////////////////////////////////
        virtual bool init(
            AssetFilePtr const& _assetFile,
            DataBlock const& _data) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool loadNowImpl() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool unloadNowImpl() MAZE_OVERRIDE;
    

        //////////////////////////////////////////
        virtual bool isForceSetAssetFileName() const { return true; }

    protected:
        SystemCursorPtr m_cursor;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetUnitSystemCursor_hpp_
//////////////////////////////////////////
