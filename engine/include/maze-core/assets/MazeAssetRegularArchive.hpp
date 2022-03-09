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
#if (!defined(_MazeAssetRegularArchive_hpp_))
#define _MazeAssetRegularArchive_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/MazeObject.hpp"
#include "maze-core/assets/MazeAssetRegularFile.hpp"
#include "maze-core/assets/MazeArchiveFileZip.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AssetRegularArchive);   
    MAZE_USING_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Class AssetRegularArchive
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AssetRegularArchive
        : public AssetRegularFile
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetRegularArchive, AssetRegularFile);
        
    public:

        //////////////////////////////////////////
        friend class AssetManager;
        friend class AssetDirectory;

    public:

        //////////////////////////////////////////
        virtual ~AssetRegularArchive();

        //////////////////////////////////////////
        inline ArchiveFileZipPtr const& getArchive() const { return m_archive; }


        //////////////////////////////////////////
        virtual StringKeyMap<AssetFilePtr> const* getChildrenAssets() const MAZE_OVERRIDE { return &m_childrenAssets; }


        //////////////////////////////////////////
        void updateChildrenAssets(
            Vector<AssetFilePtr>* _addedFiles,
            Vector<AssetFilePtr>* _removedFiles);

    protected:

        ////////////////////////////////////
        static AssetRegularArchivePtr Create(String const& _fullPath);

        //////////////////////////////////////////
        AssetRegularArchive();

        //////////////////////////////////////////
        using AssetFile::init;

        //////////////////////////////////////////
        virtual bool init(String const& _fullPath, bool _normalizePath = true) MAZE_OVERRIDE;

    
    protected:
        ArchiveFileZipPtr m_archive;
        StringKeyMap<AssetFilePtr> m_childrenAssets;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetRegularArchive_hpp_
//////////////////////////////////////////
