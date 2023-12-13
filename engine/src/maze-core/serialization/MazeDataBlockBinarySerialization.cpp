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
#include "maze-core/serialization/MazeDataBlockBinarySerialization.hpp"
#include "maze-core/serialization/MazeDataBlockSerializationUtils.hpp"
#include "maze-core/data/MazeDataBlockShared.hpp"
#include "maze-core/data/MazeByteBufferWriteStream.hpp"
#include "maze-core/data/MazeByteBufferReadStream.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include "maze-core/helpers/MazeByteBufferHelper.hpp"
#include "maze-core/hash/MazeHashFNV1.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    #pragma warning(push)
    #pragma warning(disable:4307)
    U32 const c_mzDataBlockBinaryHeaderMagic = Hash::CalculateFNV1("MZDATA");
    #pragma warning(pop)


    //////////////////////////////////////////
    namespace DataBlockBinarySerialization
    {
        //////////////////////////////////////////
        MAZE_CORE_API void WriteDataBlockBinary(ByteBufferWriteStream& _stream, DataBlock const& _dataBlock)
        {
            U32 paramUsedSize = _dataBlock.getParamsUsedSize();
            U32 complexParamsUsedSize = _dataBlock.getComplexParamsUsedSize();

            // Params
            _stream << _dataBlock.getParamsCount();
            _stream << complexParamsUsedSize;
            if (paramUsedSize + complexParamsUsedSize > 0)
                _stream.write(_dataBlock.getDataBuffer()->getDataUnsafe(), paramUsedSize + complexParamsUsedSize);

            // Sub blocks
            _stream << _dataBlock.getDataBlocksCount();
            for (DataBlock::DataBlockIndex i = 0; i < (DataBlock::DataBlockIndex)_dataBlock.getDataBlocksCount(); ++i)
            {
                DataBlock const* subBlock = _dataBlock.getDataBlock(i);

                _stream << subBlock->getNameId();
                WriteDataBlockBinary(_stream, *subBlock);
            }
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool ReadDataBlockBinary(ByteBufferReadStream& _stream, DataBlock& _dataBlock)
        {
            // Params
            U16 paramsCount = 0;
            U32 complexParamsUsedSize = 0;

            _stream >> paramsCount;
            _stream >> complexParamsUsedSize;

            U32 paramUsedSize = (U32)paramsCount * sizeof(DataBlock::Param);
            U32 paramsAndComplexParamsSize = paramUsedSize + complexParamsUsedSize;

            if (!_stream.canRead(paramsAndComplexParamsSize))
                return false;

            _dataBlock.copyParamsFrom(paramsCount, _stream.getData() + _stream.getOffset(), paramsAndComplexParamsSize);
            _stream.setOffset(_stream.getOffset() + paramsAndComplexParamsSize);

            // Sub blocks
            U16 dataBlocksCount = 0;
            _stream >> dataBlocksCount;

            for (DataBlock::DataBlockIndex i = 0; i < (DataBlock::DataBlockIndex)dataBlocksCount; ++i)
            {
                DataBlock::SharedStringId nameId;
                _stream >> nameId;

                HashedCString blockName = _dataBlock.getShared()->getHashedCString(nameId);
                DataBlock* subBlock = _dataBlock.addNewDataBlock(blockName);
                if (!ReadDataBlockBinary(_stream, *subBlock))
                    return false;
            }

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool SaveBinary(DataBlock const& _dataBlock, ByteBuffer& _buffer, U32 _flags)
        {
            _buffer.clear();

            StringKeyMap<DataBlock::SharedStringId> const& stringsMap = _dataBlock.getShared()->getStrings();

            ByteBufferWriteStream writeStream(_buffer);
            writeStream.reserve(stringsMap.size() * 32 + _dataBlock.getDataBuffer()->getDataSize());

            writeStream << c_mzDataBlockBinaryHeaderMagic;

            writeStream << _flags;

            writeStream << (U32)stringsMap.size() << _dataBlock.getShared()->getStringsIndexCounter();
            
            for (StringKeyMap<DataBlock::SharedStringId>::const_iterator it = stringsMap.begin(),
                                                                         end = stringsMap.end();
                                                                         it != end;
                                                                         ++it)
            {
                U16 stringSize = (U16)it->first.size();
                writeStream << stringSize;
                writeStream.write((U8 const*)it->first.c_str(), stringSize);
                writeStream << it->second;
            }

            WriteDataBlockBinary(writeStream, _dataBlock);

            if (_flags & U32(DataBlockBinaryFlags::CheckSumProtection))
            {
                U32 checkSumHash = Hash::CalculateCRC32(_buffer.getData(), _buffer.getSize());
                writeStream << checkSumHash;
            }

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool LoadBinary(DataBlock& _dataBlock, ByteBuffer const& _buffer)
        {
            _dataBlock.clear();

            ByteBufferReadStream readStream(_buffer);

            U32 headerMagic = 0u;
            readStream >> headerMagic;
            MAZE_ERROR_RETURN_VALUE_IF(headerMagic != c_mzDataBlockBinaryHeaderMagic, false, "Invalid data block format!");

            U32 flags = 0;
            readStream >> flags;

            U32 stringsCount = 0u;
            U32 stringsIndexCounter = 0u;
            String string;

            readStream >> stringsCount >> stringsIndexCounter;
            _dataBlock.getShared()->setStringsIndexCounter(stringsIndexCounter);
            
            for (U32 i = 0; i < stringsCount; ++i)
            {
                U16 stringSize = 0u;
                readStream >> stringSize;

                string.resize((Size)stringSize);
                Size bytesReaded = readStream.read((U8*)&string[0], (Size)stringSize);
                if (bytesReaded != (Size)stringSize)
                {
                    _dataBlock.clear();
                    MAZE_ERROR_RETURN_VALUE(false, "Binary data is corrupted.");
                }

                DataBlock::SharedStringId stringId;
                readStream >> stringId;

                _dataBlock.getShared()->setString(HashedString((CString)&string[0], (Size)stringSize), stringId);
            }

            if (!ReadDataBlockBinary(readStream, _dataBlock))
            {
                _dataBlock.clear();
                MAZE_ERROR_RETURN_VALUE(false, "Binary data is corrupted.");
            }

            if (flags & U32(DataBlockBinaryFlags::CheckSumProtection))
            {
                U32 loadedCheckSumHash = 0u;
                readStream >> loadedCheckSumHash;

                U32 calculatedCheckSumHash = Hash::CalculateCRC32(_buffer.getData(), _buffer.getSize() - sizeof(U32));
                
                if (loadedCheckSumHash != calculatedCheckSumHash)
                {
                    _dataBlock.clear();
                    MAZE_WARNING_RETURN_VALUE(false, "Checksum doesn't match.");
                }
            }

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool SaveBinaryFile(DataBlock const& _dataBlock, Path const& _path, U32 _flags)
        {
            ByteBuffer byteBuffer;
            if (!SaveBinary(_dataBlock, byteBuffer, _flags))
                return false;

            return ByteBufferHelper::SaveBinaryFile(byteBuffer, _path);
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool LoadBinaryFile(DataBlock& _dataBlock, Path const& _path)
        {
            ByteBuffer byteBuffer;
            if (!ByteBufferHelper::LoadBinaryFile(byteBuffer, _path))
                return false;

            return LoadBinary(_dataBlock, byteBuffer);
        }


    } // namespace DataBlockBinarySerialization
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
