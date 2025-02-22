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
#if (!defined(_MazeAssetRegularFile_hpp_))
#define _MazeAssetRegularFile_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/system/MazePath.hpp"
#include <tinyxml2.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(AssetRegularFile);


    //////////////////////////////////////////
    // Class AssetRegularFile
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AssetRegularFile
        : public AssetFile
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetRegularFile, AssetFile);
        
    public:

        //////////////////////////////////////////
        friend class AssetManager;

    public:

        //////////////////////////////////////////
        static AssetRegularFilePtr Create(Path const& _fullPath);

        //////////////////////////////////////////
        virtual ~AssetRegularFile();

        
        //////////////////////////////////////////
        virtual Path const& getFullPath() const MAZE_OVERRIDE { return m_fullPath; }
        
        //////////////////////////////////////////
        virtual Path const& getFileName() const MAZE_OVERRIDE { return m_fileName; }


        //////////////////////////////////////////
        virtual Size getFileSize() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual FileStats getFileStats() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool isFileExists() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool move(Path const& _newFullPath, Vector<Pair<Path, AssetFilePtr>>& _movedFiles) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual Size readToString(String& string) const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool readToXMLDocument(tinyxml2::XMLDocument& _doc) const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool readToByteBuffer(ByteBuffer& _byteBuffer) const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool readHeaderToByteBuffer(ByteBuffer& _byteBuffer, Size _size) const MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        AssetRegularFile();

        //////////////////////////////////////////
        using AssetFile::init;
        
        //////////////////////////////////////////
        virtual bool init(Path const& _fullPath, bool _normalizePath = true);

        
        //////////////////////////////////////////
        void setFullPath(Path const& _fullPath, bool _normalizePath = true);
    
        //////////////////////////////////////////
        void updateFileName();
        
        //////////////////////////////////////////
        Size calculateFileSize() const;
    
    protected:
        Path m_fullPath;
        
    private:
        Path m_fileName;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetRegularFile_hpp_
//////////////////////////////////////////
