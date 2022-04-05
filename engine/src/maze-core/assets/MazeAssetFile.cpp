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
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetFile
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetFile, Object);
    
    //////////////////////////////////////////
    AssetFile::AssetFile()
    {

    }

    //////////////////////////////////////////
    AssetFile::~AssetFile()
    {
    }
    

    //////////////////////////////////////////
    bool AssetFile::init()
    {
            
        return true;
    }

    //////////////////////////////////////////
    StringKeyMap<AssetFilePtr> const* AssetFile::getChildrenAssets() const
    {
        return nullptr;
    }

    //////////////////////////////////////////
    void AssetFile::getChildrenAssets(
        Vector<AssetFilePtr>& _outResult,
        ClassUID _classUID,
        bool _recursive) const
    {
        StringKeyMap<AssetFilePtr> const* childrenAssets = getChildrenAssets();
        if (!childrenAssets)
            return;

        for (auto const& _assetFileData : *childrenAssets)
        {
            if (_classUID == _assetFileData.second->getClassUID())
                _outResult.push_back(_assetFileData.second);

            if (_recursive)
                _assetFileData.second->getChildrenAssets(_outResult, _classUID, _recursive);
        }
    }

    //////////////////////////////////////////
    String AssetFile::readAsString()
    {
        String result;
        readToString(result);
        return result;
    }

    //////////////////////////////////////////
    String AssetFile::getExtension() const
    {
        String const& fileName = getFileName();
        if (fileName.empty())
            return String();
        
        Size position = fileName.find_last_of('.');
        if (position == String::npos)
            return String();
        
        return fileName.substr(position + 1, fileName.size() - position - 1);
    }

    //////////////////////////////////////////
    bool AssetFile::readToByteBuffer(ByteBufferPtr const& _byteBuffer)
    {
        ByteBuffer& buffer = *_byteBuffer.get();
        return readToByteBuffer(buffer);
    }

    //////////////////////////////////////////
    bool AssetFile::readHeaderToByteBuffer(ByteBufferPtr const& _byteBuffer, Size _size)
    {
        ByteBuffer& buffer = *_byteBuffer.get();
        return readHeaderToByteBuffer(buffer, _size);
    }
    
    //////////////////////////////////////////
    ByteBufferPtr AssetFile::readAsByteBuffer()
    {
        ByteBufferPtr byteBuffer = ByteBuffer::Create();
        readToByteBuffer(byteBuffer);

        return byteBuffer;
    }

    //////////////////////////////////////////
    ByteBufferPtr AssetFile::readHeaderAsByteBuffer(Size _size)
    {
        ByteBufferPtr byteBuffer = ByteBuffer::Create(_size);
        readHeaderToByteBuffer(byteBuffer, _size);

        return byteBuffer;
    }

} // namespace Maze
//////////////////////////////////////////
