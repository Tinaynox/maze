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
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/assets/MazeAssetUnit.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class AssetFileAssetFileAssetUnitIdGeneratedEvent
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetFileAssetFileAssetUnitIdGeneratedEvent, Event);


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
        removeAssetUnits();
    }
    

    //////////////////////////////////////////
    bool AssetFile::init()
    {
            
        return true;
    }

    //////////////////////////////////////////
    void AssetFile::setAssetFileId(AssetFileId _id)
    {
        AssetFileId prevAssetFileId = m_assetFileId;
        m_assetFileId = _id;
        eventAssetFileIdChanged(this, prevAssetFileId, m_assetFileId);
    }

    //////////////////////////////////////////
    UnorderedMap<Path, AssetFilePtr> const* AssetFile::getChildrenAssets() const
    {
        return nullptr;
    }

    //////////////////////////////////////////
    void AssetFile::updateDataFromAssetFile(DataBlock& _metaData) const
    {
        _metaData.setU32(MAZE_HCS("afid"), getAssetFileId());

        if (m_tags.empty())
            _metaData.removeDataBlock(MAZE_HCS("tags"));
        else
            _metaData.setDataBlockAsSetString(MAZE_HCS("tags"), m_tags);

        if (!m_assetUnits.empty())
        {
            DataBlock& assetUnitsBlock = _metaData[MAZE_HCS("assetUnits")];

            assetUnitsBlock.clearData();

            for (AssetUnitPtr const& assetUnit : m_assetUnits)
            {
                DataBlock* assetUnitDataBlock = assetUnitsBlock.addNewDataBlock(assetUnit->getDataBlockId());
                if (assetUnitDataBlock)
                {
                    *assetUnitDataBlock = assetUnit->getData();
                    assetUnit->updateDataFromAssetUnit(*assetUnitDataBlock);
                }
            }
        }
        else
        {
            _metaData.removeDataBlock(MAZE_HCS("assetUnits"));
        }
    }

    //////////////////////////////////////////
    void AssetFile::saveInfoToMetaData()
    {
        DataBlock metaData;
        updateDataFromAssetFile(metaData);
        AssetManager::GetInstancePtr()->saveMetaData(cast<AssetFile>(), metaData);
    }

    //////////////////////////////////////////
    void AssetFile::loadInfoFromMetaData(DataBlock const& _metaData)
    {
        setTags(_metaData.getDataBlockAsSetString(MAZE_HCS("tags")));

        AssetFileId afid = _metaData.getU32(MAZE_HCS("afid"), c_invalidAssetFileId);
        if (afid != c_invalidAssetFileId)
            setAssetFileId(afid);           

        updateAssetUnitsFromMetaData(_metaData);
    }

    //////////////////////////////////////////
    bool AssetFile::loadInfoFromMetaData()
    {
        DataBlock metaData;
        if (!AssetManager::GetInstancePtr()->loadMetaData(cast<AssetFile>(), metaData))
            return false;

        loadInfoFromMetaData(metaData);
        return true;
    }

    //////////////////////////////////////////
    void AssetFile::getChildrenAssets(
        Vector<AssetFilePtr>& _outResult,
        ClassUID _classUID,
        bool _recursive) const
    {
        UnorderedMap<Path, AssetFilePtr> const* childrenAssets = getChildrenAssets();
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
    void AssetFile::getChildrenAssets(
        Vector<AssetFilePtr>& _outResult,
        bool _recursive) const
    {
        UnorderedMap<Path, AssetFilePtr> const* childrenAssets = getChildrenAssets();
        if (!childrenAssets)
            return;

        for (auto const& _assetFileData : *childrenAssets)
        {
            _outResult.push_back(_assetFileData.second);

            if (_recursive)
                _assetFileData.second->getChildrenAssets(_outResult, _recursive);
        }
    }

    //////////////////////////////////////////
    String AssetFile::readAsString() const
    {
        String result;
        readToString(result);
        return result;
    }

    //////////////////////////////////////////
    Path AssetFile::getExtension() const
    {
        Path const& fileName = getFileName();
        if (fileName.empty())
            return Path();
        
        Size position = fileName.getPath().find_last_of('.');
        if (position == Path::StringType::npos)
            return String();
        
        return fileName.getPath().substr(position + 1, fileName.size() - position - 1);
    }

    //////////////////////////////////////////
    bool AssetFile::readToByteBuffer(ByteBufferPtr const& _byteBuffer) const
    {
        ByteBuffer& buffer = *_byteBuffer.get();
        return readToByteBuffer(buffer);
    }

    //////////////////////////////////////////
    bool AssetFile::readHeaderToByteBuffer(ByteBufferPtr const& _byteBuffer, Size _size) const
    {
        ByteBuffer& buffer = *_byteBuffer.get();
        return readHeaderToByteBuffer(buffer, _size);
    }
    
    //////////////////////////////////////////
    ByteBufferPtr AssetFile::readAsByteBuffer() const
    {
        ByteBufferPtr byteBuffer = ByteBuffer::Create();
        readToByteBuffer(byteBuffer);

        return byteBuffer;
    }

    //////////////////////////////////////////
    ByteBufferPtr AssetFile::readHeaderAsByteBuffer(Size _size) const
    {
        ByteBufferPtr byteBuffer = ByteBuffer::Create(_size);
        readHeaderToByteBuffer(byteBuffer, _size);

        return byteBuffer;
    }

    //////////////////////////////////////////
    bool AssetFile::readToDataBlock(DataBlock& _dataBlock) const
    {
        ByteBufferPtr byteBuffer = readAsByteBuffer();
        if (!byteBuffer)
            return false;

        return _dataBlock.loadFromByteBuffer(*byteBuffer.get());
    }

    //////////////////////////////////////////
    DataBlock AssetFile::readAsDataBlock() const
    {
        DataBlock dataBlock;
        readToDataBlock(dataBlock);
        return std::move(dataBlock);
    }

    //////////////////////////////////////////
    void AssetFile::updateAssetUnitsFromMetaData(DataBlock const& _metaData)
    {
        removeAssetUnits();

        if (DataBlock const* assetUnits = _metaData.getDataBlock(MAZE_HCS("assetUnits")))
        {
            for (DataBlock const* assetUnitBlock : *assetUnits)
            {
                if (assetUnitBlock->isComment())
                    continue;

                AssetUnitPtr assetUnit = AssetUnitManager::GetInstancePtr()->createAssetUnit(cast<AssetFile>(), *assetUnitBlock);
                if (!assetUnit)
                    continue;

                addAssetUnit(assetUnit);
            }
        }
    }

    //////////////////////////////////////////
    void AssetFile::addAssetUnit(AssetUnitPtr const& _assetUnit)
    {
        if (_assetUnit->getAssetUnitId() == c_invalidAssetUnitId && AssetUnitManager::GetInstancePtr()->getGenerateIdsForNewAssetUnits())
        {
            _assetUnit->setAssetUnitId(AssetUnitManager::GetInstancePtr()->generateAssetUnitId());

            if (EventManager::GetInstancePtr())
                EventManager::GetInstancePtr()->broadcastEventImmediate<AssetFileAssetFileAssetUnitIdGeneratedEvent>(
                    getSharedPtr(),
                    _assetUnit->getAssetUnitId());
        }

        AssetUnitManager::GetInstancePtr()->addAssetUnit(_assetUnit);

        m_assetUnits.push_back(_assetUnit);
    }

    //////////////////////////////////////////
    void AssetFile::updateAssetUnitsFromMetaData()
    {
        DataBlock metaData;
        if (!AssetManager::GetInstancePtr()->loadMetaData(cast<AssetFile>(), metaData))
            return;

        updateAssetUnitsFromMetaData(metaData);
    }

    //////////////////////////////////////////
    void AssetFile::removeAssetUnits()
    {
        while (!m_assetUnits.empty())
        {
            AssetUnitPtr assetUnit = m_assetUnits.back();
            m_assetUnits.pop_back();

            assetUnit->unloadNow();

            if (AssetUnitManager::GetInstancePtr())
                AssetUnitManager::GetInstancePtr()->removeAssetUnit(
                    assetUnit->getAssetUnitId());
        }
    }

} // namespace Maze
//////////////////////////////////////////
