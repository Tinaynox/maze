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
#if (!defined(_MazeAssetArchivedFile_hpp_))
#define _MazeAssetArchivedFile_hpp_


//////////////////////////////////////////
#include "maze-plugin-archive-assets/MazeArchiveAssetsHeader.hpp"
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
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-plugin-archive-assets/assets/MazeArchiveFileZip.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(AssetRegularArchive);
    MAZE_USING_MANAGED_SHARED_PTR(AssetArchivedFile);   


    //////////////////////////////////////////
    // Class AssetArchivedFile
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_ARCHIVE_ASSETS_API AssetArchivedFile
        : public AssetFile
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetArchivedFile, AssetFile);
        
    public:

        //////////////////////////////////////////
        friend class AssetManager;        
        friend class AssetRegularArchive;

    public:

        //////////////////////////////////////////
        virtual ~AssetArchivedFile();

        //////////////////////////////////////////
        ArchiveFileZipPtr const& getAssetArchive() const { return m_archive; }

        //////////////////////////////////////////
        Path const& getZipArchiveFilePath() const { return m_zipArchiveFilePath; }

        //////////////////////////////////////////
        Path const& getFileName() const MAZE_OVERRIDE { return m_fileName; }

        //////////////////////////////////////////
        Path const& getFullPath() const MAZE_OVERRIDE { return m_fullPath; }

        //////////////////////////////////////////
        virtual Size getFileSize() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual FileStats getFileStats() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool isFileExists() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool move(Path const& _newFullPath, Vector<Pair<Path, AssetFilePtr>>& _renamedFiles) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual UnorderedMap<Path, AssetFilePtr> const* getChildrenAssets() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool updateChildrenAssets(
            bool _recursive,
            Vector<AssetFilePtr>* _addedFiles,
            Vector<AssetFilePtr>* _removedFiles) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual Size readToString(String& _string) const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool readToXMLDocument(tinyxml2::XMLDocument& _doc) const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool readToByteBuffer(ByteBuffer& _byteBuffer) const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool readHeaderToByteBuffer(ByteBuffer& _byteBuffer, Size _size) const MAZE_OVERRIDE;

    protected:

        ////////////////////////////////////
        static AssetArchivedFilePtr Create(
            ArchiveFileZipPtr const& _archive,
            Path const& _zipArchiveFilePath);

        //////////////////////////////////////////
        AssetArchivedFile();

        //////////////////////////////////////////
        using AssetFile::init;

        //////////////////////////////////////////
        virtual bool init(
            ArchiveFileZipPtr const& _archive,
            Path const& _zipArchiveFilePath);

    
    protected:
        ArchiveFileZipPtr m_archive;
        Path m_zipArchiveFilePath;
        Path m_fileName;
        Path m_fullPath;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetArchivedFile_hpp_
//////////////////////////////////////////
