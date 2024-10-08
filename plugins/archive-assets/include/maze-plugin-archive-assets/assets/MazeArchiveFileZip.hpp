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
#if (!defined(_MazeArchiveFileZip_hpp_))
#define _MazeArchiveFileZip_hpp_


//////////////////////////////////////////
#include "maze-plugin-archive-assets/MazeArchiveAssetsHeader.hpp"
#include "maze-core/assets/MazeArchiveFile.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/system/MazeFileStats.hpp"
#include <mz.h>
#include <mz_compat.h>
#include <mz_crypt.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ArchiveFileZip);   


    //////////////////////////////////////////
    // Class ArchiveFileZip
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_ARCHIVE_ASSETS_API ArchiveFileZip
        : public ArchiveFile
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ArchiveFileZip, ArchiveFile);
        
    public:

        //////////////////////////////////////////
        struct ZipFileInfo
        {
            unz_file_pos filePos;
            U32 uncompressedSize;
            Path fullPath;
            Path fileName;
        };

        //////////////////////////////////////////
        using ZipNavigationMap = StringKeyMap<ZipFileInfo>;

    public:

        //////////////////////////////////////////
        virtual ~ArchiveFileZip();

        //////////////////////////////////////////
        static ArchiveFileZipPtr Create(Path const& _fullPath);


        //////////////////////////////////////////
        inline Path const& getFullPath() const { return m_fullPath; }


        //////////////////////////////////////////
        bool updateZipNavigationMap();

        //////////////////////////////////////////
        Vector<Path> getArchivedFilePathes();

        //////////////////////////////////////////
        Size readArchivedFileToBuffer(Path const& _filePath, U8* _bytes, Size _bufferSize);

        //////////////////////////////////////////
        Size readArchivedFileToString(Path const& _filePath, String& _stringBuffer);

        //////////////////////////////////////////
        ByteBufferPtr readArchivedFileAsByteBuffer(Path const& _filePath);

        //////////////////////////////////////////
        Size getArchivedFileLength(Path const& _filePath);

        //////////////////////////////////////////
        FileStats getArchivedFileStats(Path const& _filePath);

        //////////////////////////////////////////
        bool isFileExists();

        //////////////////////////////////////////
        bool isFileExists(Path const& _filePath);

    protected:

        //////////////////////////////////////////
        ArchiveFileZip();

        //////////////////////////////////////////
        virtual bool init(Path const& _fullPath);


        //////////////////////////////////////////
        bool openZip(Path const& _fullPath);

        //////////////////////////////////////////
        void closeZip();
    
        //////////////////////////////////////////
        S32 tryUnzOpenCurrentFile(Path const& _fileName, unzFile _file);

    protected:
        Path m_fullPath;
        unzFile m_zipHandle = nullptr;

        ZipNavigationMap m_zipNavigationMap;
        bool m_zipNavigationMapDirty = false;

        Mutex m_zipMutex;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeArchiveFileZip_hpp_
//////////////////////////////////////////
