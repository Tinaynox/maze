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
#include "MazeCoreHeader.hpp"
#include "maze-core/assets/MazeAssetRegularFile.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"



//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetRegularFile
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetRegularFile, AssetFile);
    
    //////////////////////////////////////////
    AssetRegularFile::AssetRegularFile()
    {

    }

    //////////////////////////////////////////
    AssetRegularFile::~AssetRegularFile()
    {
    }

    ////////////////////////////////////
    AssetRegularFilePtr AssetRegularFile::Create(String const& _fullPath)
    {
        AssetRegularFilePtr result;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetRegularFile, result, init(_fullPath));
        return result;
    }

    //////////////////////////////////////////
    bool AssetRegularFile::init(String const& _fullPath, bool _normalizePath)
    {
        if (!AssetFile::init())
            return false;
        
        setFullPath(_fullPath, _normalizePath);

        return true;
    }
    
    //////////////////////////////////////////
    void AssetRegularFile::setFullPath(String const& _fullPath, bool _normalizePath)
    {
        m_fullPath = _fullPath;
        
        if (_normalizePath)
        {
            m_fullPath = FileHelper::NormalizedFilePath(m_fullPath);
        }
        
        m_fileName = FileHelper::GetFileNameInPath(m_fullPath);
    }

    //////////////////////////////////////////
    Size AssetRegularFile::getFileSize()
    {
        FILE* fileHandler = StdHelper::OpenFile(m_fullPath.c_str(), "rb");
        if (!fileHandler)
            return 0;

        Size size = 0;

        U32 prevPos = ftell(fileHandler);
        fseek(fileHandler, 0, SEEK_END);
        size = ftell(fileHandler);
        fseek(fileHandler, prevPos, SEEK_SET);        

        fclose(fileHandler);

        return size;
    }
    
    //////////////////////////////////////////
    Size AssetRegularFile::readToString(String& _string)
    {
        std::ifstream ifs(m_fullPath.c_str());
        _string.assign((std::istreambuf_iterator<S8>(ifs)),
                       (std::istreambuf_iterator<S8>()));
        
        return _string.size();
    }

    //////////////////////////////////////////
    bool AssetRegularFile::readToXMLDocument(tinyxml2::XMLDocument& _doc)
    {
        tinyxml2::XMLError loadError = _doc.LoadFile(getFullPath().c_str());
        if (tinyxml2::XML_SUCCESS != loadError)
        {
            MAZE_ERROR("File '%s' loading error - XMLError: %d!", getFileName().c_str(), (S32)loadError);
            return false;
        }

        return true;
    }

    //////////////////////////////////////////
    bool AssetRegularFile::readToByteBuffer(ByteBuffer& _byteBuffer)
    {
        FILE* fileHandler = StdHelper::OpenFile(m_fullPath.c_str(), "rb");
        if (!fileHandler)
            return false;

        Size size = 0;

        fseek(fileHandler, 0, SEEK_END);
        size = ftell(fileHandler);
        fseek(fileHandler, 0, SEEK_SET);

        _byteBuffer.resize(size);

        U8* dataPointer = _byteBuffer.getDataPointer();
        Size totalBytesRead = 0;
        while (!feof(fileHandler))
        {
            Size bytesRead = fread(dataPointer, sizeof(U8), 1024, fileHandler);
            totalBytesRead += bytesRead;
            dataPointer += bytesRead;
        }

        fclose(fileHandler);

        return true;
    }

    //////////////////////////////////////////
    bool AssetRegularFile::readHeaderToByteBuffer(ByteBuffer& _byteBuffer, Size _size)
    {
        FILE* fileHandler = StdHelper::OpenFile(m_fullPath.c_str(), "rb");
        if (!fileHandler)
            return false;

        _byteBuffer.resize(_size);
        fread(_byteBuffer.getDataPointer(), sizeof(U8), _size, fileHandler);

        fclose(fileHandler);

        return true;
    }


} // namespace Maze
//////////////////////////////////////////
