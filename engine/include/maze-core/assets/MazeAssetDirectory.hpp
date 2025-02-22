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
#if (!defined(_MazeAssetDirectory_hpp_))
#define _MazeAssetDirectory_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/assets/MazeAssetRegularFile.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(AssetDirectory);


    //////////////////////////////////////////
    // Class AssetDirectory
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AssetDirectory
        : public AssetRegularFile
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetDirectory, AssetFile);
        
    public:

        //////////////////////////////////////////
        friend class AssetManager;


    public:

        //////////////////////////////////////////
        virtual ~AssetDirectory();

        //////////////////////////////////////////
        virtual UnorderedMap<Path, AssetFilePtr> const* getChildrenAssets() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool move(Path const& _newFullPath, Vector<Pair<Path, AssetFilePtr>>& _movedFiles) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool updateChildrenAssets(
            bool _recursive,
            Vector<AssetFilePtr>* _addedFiles,
            Vector<AssetFilePtr>* _removedFiles) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        static AssetDirectoryPtr Create(Path const& _fullPath);

        //////////////////////////////////////////
        AssetDirectory();
        
        //////////////////////////////////////////
        virtual bool initAssetDirectory(Path const& _fullPath);

    protected:

        UnorderedMap<Path, AssetFilePtr> m_childrenAssets;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetDirectory_hpp_
//////////////////////////////////////////
