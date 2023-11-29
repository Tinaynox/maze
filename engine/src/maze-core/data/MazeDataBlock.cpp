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
//
// Origin idea:
// https://github.com/GaijinEntertainment/DagorEngine/blob/main/prog/dagorInclude/ioSys/dag_dataBlock.h
//
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeCoreHeader.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/encoding/MazeEncodingBase64.hpp"
#include "maze-core/serialization/MazeValueSerialization.hpp"
#include "maze-core/data/MazeDataBlockShared.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
// Namespace
namespace Maze
{
    //////////////////////////////////////////
    MAZE_FORCEINLINE static DataBlock::ParamIndex FindParamIndex(
        DataBlock::SharedStringId _nameId,
        DataBlock::Param const* __restrict _start,
        DataBlock::Param const* __restrict _end)
    {
        for (DataBlock::Param const* __restrict it = _start; it != _end; ++it)
        {
            if (it->nameId == _nameId)
                return it - _start;
        }

        return -1;
    }


    //////////////////////////////////////////
    // DataBlockDataBuffer
    //
    //////////////////////////////////////////
    U8* DataBlockDataBuffer::insertAt(U32 _at, Size _size, U8 const* _data)
    {
        U8* destPtr = m_buffer.insert(_at, _size);
        memcpy(destPtr, _data, _size);
        return destPtr;
    }

    //////////////////////////////////////////
    void DataBlockDataBuffer::erase(U32 _at, Size _size)
    {
        m_buffer.erase(_at, _size);
    }

    //////////////////////////////////////////
    void DataBlockDataBuffer::clear()
    {
        m_buffer.clear();
    }


    //////////////////////////////////////////
    // DataBlock
    //
    //////////////////////////////////////////
    DataBlock* DataBlock::Create()
    {
        return MAZE_NEW(DataBlock);
    }

    //////////////////////////////////////////
    DataBlock::DataBlock()
        : m_shared(MAZE_NEW(DataBlockShared))
    {
        m_nameIdAndFlags |= U32(DataBlockFlags::TopmostBlock);
        createDataBuffer();
    }

    //////////////////////////////////////////
    DataBlock::DataBlock(DataBlock&& _value)
    {
        if (isTopmost())
        {
            std::swap(m_shared, _value.m_shared);
            std::swap(m_dataBuffer, _value.m_dataBuffer);
            std::swap(m_nameIdAndFlags, _value.m_nameIdAndFlags);
            std::swap(m_paramsCount, _value.m_paramsCount);
            std::swap(m_dataBlocksCount, _value.m_dataBlocksCount);
        }
        else
        {
            m_shared = MAZE_NEW(DataBlockShared);
            m_nameIdAndFlags |= U32(DataBlockFlags::TopmostBlock);
            copyFrom(&_value);
        }
    }

    //////////////////////////////////////////
    DataBlock::DataBlock(DataBlockShared* _shared, HashedCString _name)
        : m_shared(_shared)
    {
        m_nameIdAndFlags |= addString(_name);
        createDataBuffer();
    }

    //////////////////////////////////////////
    DataBlock::~DataBlock()
    {
        if (!m_shared && !m_dataBuffer)
            return;

        for (U32 i = 0, e = getDataBlocksCount(); i < e; ++i)
        {
            DataBlock* dataBlock = getDataBlock(i);
            dataBlock->~DataBlock();
            m_shared->freeDataBlock(dataBlock);
        }

        deleteBuffers();
    }

    //////////////////////////////////////////
    HashedCString DataBlock::getName() const
    {
        SharedStringId nameId = getNameId();
        if (nameId == 0)
            return HashedCString();

        return getSharedHashedCString(nameId);
    }

    //////////////////////////////////////////
    Size DataBlock::getComplexParamsUsedSize() const
    {
        return m_dataBuffer->getDataSize() - (getBlocksUsedSize() + getParamsUsedSize());
    }

    //////////////////////////////////////////
    void DataBlock::clearParams()
    {
        m_dataBuffer->erase(0, m_dataBuffer->getDataSize() - getBlocksUsedSize());
        m_paramsCount = 0;
    }

    //////////////////////////////////////////
    void DataBlock::clearData()
    {
        for (U32 i = 0, e = getDataBlocksCount(); i < e; ++i)
        {
            DataBlock* dataBlock = getDataBlock(i);
            dataBlock->~DataBlock();
            m_shared->freeDataBlock(dataBlock);
        }

        m_dataBuffer->clear();

        m_paramsCount = 0;
        m_dataBlocksCount = 0;
    }

    //////////////////////////////////////////
    void DataBlock::appendParamsFrom(DataBlock const* _from)
    {
        if (_from->getParamsCount() == 0u)
            return;

        Param const* paramPtr = _from->getParamsPtr();

        for (ParamIndex i = 0, e = _from->getParamsCount(); i < e; ++i, ++paramPtr)
        {
            Param const& param = *paramPtr;
            SharedStringId paramNameId = addString(_from->getSharedHashedCString(param.nameId));
            
            if (param.type == U32(DataBlockParamType::ParamString))
            {
                insertParamAt<CString>(
                    (ParamIndex)getParamsCount(),
                    paramNameId,
                    _from->getParamValueString<CString>(param.value));
            }
            else
            {
                Size size = c_dataBlockParamTypeSize[param.type];
                U8 const* paramData = (size <= MAZE_DATA_BLOCK_INPLACE_PARAM_SIZE) ? (U8 const*)&param.value
                                                                                   : _from->getMemoryBufferData(_from->getParamsUsedSize() + param.value);
                insertParamAt(
                    (ParamIndex)getParamsCount(),
                    paramNameId,
                    (DataBlockParamType)param.type,
                    size,
                    paramData);
            }
        }
    }

    //////////////////////////////////////////
    void DataBlock::copyParamsFrom(DataBlock const* _from)
    {
        clearParams();
        appendParamsFrom(_from);
    }

    //////////////////////////////////////////
    void DataBlock::copyFrom(DataBlock const* _from)
    {
        if (_from == this)
            return;

        clearData();
        if (_from == nullptr)
            return;

        copyParamsFrom(_from);
        for (DataBlockIndex i = 0, e = _from->getDataBlocksCount(); i < e; ++i)
            addNewDataBlock(_from->getDataBlock(i));
    }

    //////////////////////////////////////////
    DataBlock* DataBlock::duplicate() const
    {
        DataBlock* newBlock = DataBlock::Create();
        newBlock->copyFrom(this);
        return newBlock;
    }

    //////////////////////////////////////////
    DataBlock::Param& DataBlock::getParam(ParamIndex _index)
    {
        MAZE_ASSERT((Size)_index < getParamsCount());
        return getParamsPtr()[_index];
    }

    //////////////////////////////////////////
    DataBlock::Param const& DataBlock::getParam(ParamIndex _index) const
    {
        MAZE_ASSERT((Size)_index < getParamsCount());
        return getParamsPtr()[_index];
    }

    //////////////////////////////////////////
    DataBlock::ParamIndex DataBlock::findParamIndex(SharedStringId _nameId) const
    {
        if (m_paramsCount == 0 || _nameId < 0)
            return -1;

        return FindParamIndex(_nameId, getParamsPtr(), getParamsEndPtr());
    }

    //////////////////////////////////////////
    DataBlock::ParamIndex DataBlock::findParamIndexReverse(SharedStringId _nameId, ParamIndex _startBefore) const
    {
        if (_startBefore <= 0 || m_paramsCount == 0)
            return -1;

        Param const* ptr = getParamsPtr();
        if (_startBefore > (ParamIndex)m_paramsCount)
            _startBefore = (ParamIndex)m_paramsCount;

        Param const* __restrict it = ptr + _startBefore - 1;
        Param const* __restrict end = ptr - 1;

        for (; it != end; --it)
            if (it->nameId == _nameId)
                return it - ptr;
        return -1;
    }

    //////////////////////////////////////////
    void DataBlock::createDataBuffer()
    {
        MAZE_ASSERT(!m_dataBuffer);
        if (m_shared)
            m_dataBuffer = new(m_shared->allocateDataBuffer()) DataBlockDataBuffer();
    }

    //////////////////////////////////////////
    void DataBlock::deleteBuffers()
    {
        if (m_dataBuffer)
        {
            MAZE_ASSERT(m_shared);
            m_dataBuffer->~DataBlockDataBuffer();
            m_shared->freeDataBuffer(m_dataBuffer);
            m_dataBuffer = nullptr;
        }

        if (isTopmost())
        {
            MAZE_SAFE_DELETE(m_shared);
        }
    }

    //////////////////////////////////////////
    U8* DataBlock::insertDataAt(U32 _at, Size _size, U8 const* _data)
    {
        return m_dataBuffer->insertAt(_at, _size, _data);
    }

    //////////////////////////////////////////
    DataBlock::SharedStringId DataBlock::addString(HashedCString _name)
    {
        return m_shared->addString(_name);
    }

    //////////////////////////////////////////
    DataBlock::SharedStringId DataBlock::getStringId(HashedCString _name) const
    {
        return m_shared->getStringId(_name);
    }

    //////////////////////////////////////////
    DataBlock::SharedStringId DataBlock::addString(CString _str, Size _length)
    {
        return m_shared->addString(_str, _length);
    }

    //////////////////////////////////////////
    DataBlock::SharedStringId DataBlock::getStringId(CString _str, Size _length) const
    {
        return m_shared->getStringId(_str, _length);
    }

    //////////////////////////////////////////
    CString DataBlock::getSharedCString(SharedStringId _nameId) const
    {
        return m_shared->getCString(_nameId);
    }

    //////////////////////////////////////////
    HashedCString DataBlock::getSharedHashedCString(SharedStringId _nameId) const
    {
        return m_shared->getHashedCString(_nameId);
    }

    //////////////////////////////////////////
#define MAZE_DECLARE_DATA_BLOCK_GET_SET_API_BASE(__DValueType, __DValueRefType, __typeName)                                     \
    __DValueType DataBlock::get##__typeName(DataBlock::ParamIndex _index) const                                                   \
    {                                                                                                                             \
        return getParamValue<__DValueType>(_index);                                                                                    \
    }                                                                                                                             \
    __DValueType DataBlock::get##__typeName(HashedCString _name, __DValueRefType _defaultValue) const                             \
    {                                                                                                                             \
        return getParamValueByName<__DValueType>(_name, _defaultValue);                                                                \
    }                                                                                                                             \
    __DValueType DataBlock::get##__typeName(HashedCString _name) const                                                            \
    {                                                                                                                             \
        return getParamValueByName<__DValueType>(_name);                                                                               \
    }                                                                                                                             \
    __DValueType DataBlock::get##__typeName##ByNameId(DataBlock::SharedStringId _nameId, __DValueRefType _defaultValue) const        \
    {                                                                                                                             \
        return getParamValueByNameId<__DValueType>(_nameId, _defaultValue);                                                            \
    }                                                                                                                             \
    bool DataBlock::set##__typeName(ParamIndex _index, __DValueRefType _value)                                                    \
    {                                                                                                                             \
        if ((Size)_index >= getParamsCount())                                                                                           \
          return false;                                                                                                           \
        return setParam<__DValueType>(_index, _value);                                                                            \
    }                                                                                                                             \
    DataBlock::ParamIndex DataBlock::set##__typeName##ByNameId(DataBlock::SharedStringId _nameId, __DValueRefType _value)            \
    {                                                                                                                             \
        return setParamByNameId<__DValueType>(_nameId, _value);                                                                   \
    }                                                                                                                             \
    DataBlock::ParamIndex DataBlock::set##__typeName(HashedCString _name, __DValueRefType _value)                                 \
    {                                                                                                                             \
        return setParamByName<__DValueType>(_name, _value);                                                                       \
    }                                                                                                                             \
    DataBlock::ParamIndex DataBlock::add##__typeName(HashedCString _name, __DValueRefType _value)                                 \
    {                                                                                                                             \
        return addParamByName<__DValueType>(_name, _value);                                                                       \
    }                                                                                                                             \
    DataBlock::ParamIndex DataBlock::addNew##__typeName##ByNameId(DataBlock::SharedStringId _nameId, __DValueRefType _value)         \
    {                                                                                                                             \
        return addParamByNameId<__DValueType>(_nameId, _value);                                                                   \
    }

#define MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(__DValueType, __typeName) MAZE_DECLARE_DATA_BLOCK_GET_SET_API_BASE(__DValueType, __DValueType, __typeName)
#define MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(__DValueType, __typeName) MAZE_DECLARE_DATA_BLOCK_GET_SET_API_BASE(__DValueType, __DValueType const&, __typeName)

    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(S32, S32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(F32, F32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(F64, F64);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(bool, Bool);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec2DS, Vec2DS);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec3DS, Vec3DS);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4DS, Vec4DS);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec2DU, Vec2DU);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec3DU, Vec3DU);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4DU, Vec4DU);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec2DF, Vec2DF);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec3DF, Vec3DF);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4DF, Vec4DF);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec2DB, Vec2DB);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec3DB, Vec3DB);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4DB, Vec4DB);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Mat3DF, Mat3DF);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Mat4DF, Mat4DF);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(CString, String);

#undef MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_BASE
#undef MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API
#undef MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF

    //////////////////////////////////////////
    bool DataBlock::removeParam(HashedCString _name)
    {
        if (m_paramsCount == 0)
            return false;

        SharedStringId nameId = getStringId(_name);
        if (nameId < 0)
            return false;

        ParamIndex paramIdx = findParamIndexReverse(nameId, (ParamIndex)m_paramsCount);
        if (paramIdx >= 0)
        {
            do
            {
                removeParam(paramIdx);
            }
            while ((paramIdx = findParamIndexReverse(nameId, paramIdx)) >= 0);

            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    bool DataBlock::removeParam(ParamIndex _index)
    {
        if (_index >= (ParamIndex)m_paramsCount)
            return false;

        m_dataBuffer->erase(_index * sizeof(Param), sizeof(Param));
        --m_paramsCount;
        return true;
    }

    //////////////////////////////////////////
    DataBlock* DataBlock::getDataBlock(HashedCString _name)
    {
        SharedStringId nameId = getStringId(_name);
        return getDataBlockByNameId(nameId);
    }

    //////////////////////////////////////////
    DataBlock const* DataBlock::getDataBlock(HashedCString _name) const
    {
        SharedStringId nameId = getStringId(_name);
        return getDataBlockByNameId(nameId);
    }

    //////////////////////////////////////////
    DataBlock* DataBlock::getDataBlock(DataBlockIndex _index)
    {
        if ((U32)_index < getDataBlocksCount())
            return *(getDataBlocksPtr() + _index);
        return nullptr;
    }

    //////////////////////////////////////////
    DataBlock const* DataBlock::getDataBlock(DataBlockIndex _index) const
    {
        if ((U32)_index < getDataBlocksCount())
            return *(getDataBlocksPtr() + _index);
        return nullptr;
    }

    //////////////////////////////////////////
    DataBlock* DataBlock::addDataBlock(HashedCString _name)
    {
        DataBlock* blk = getDataBlock(_name);
        if (blk)
            return blk;
        return addNewDataBlock(_name);
    }

    //////////////////////////////////////////
    DataBlock* DataBlock::addNewDataBlock(HashedCString _name)
    {
        DataBlock* newBlock = new (m_shared->allocateDataBlock()) DataBlock(m_shared, _name);
        insertDataAt(m_dataBuffer->getDataSize(), sizeof(DataBlock*), (U8 const*)&newBlock);
        m_dataBlocksCount++;
        return newBlock;
    }

    //////////////////////////////////////////
    DataBlock* DataBlock::addNewDataBlock(DataBlock const* _copyFrom, HashedCString _name)
    {
        if (!_copyFrom)
            return nullptr;

        DataBlock* newBlock = addNewDataBlock(!_name.empty() ? _name : _copyFrom->getName());

        // Copy params
        newBlock->copyParamsFrom(_copyFrom);

        // Copy blocks
        for (DataBlockIndex i = 0, e = _copyFrom->getDataBlocksCount(); i < e; ++i)
            newBlock->addNewDataBlock(_copyFrom->getDataBlock(i));

        return newBlock;
    }

    //////////////////////////////////////////
    DataBlock const* DataBlock::getDataBlockByNameId(SharedStringId _nameId, DataBlockIndex _startAfter) const
    {
        return const_cast<DataBlock const*>(const_cast<DataBlock *>(this)->getDataBlockByNameId(_nameId, _startAfter));
    }

    //////////////////////////////////////////
    DataBlock* DataBlock::getDataBlockByNameId(SharedStringId _nameId, DataBlockIndex _startAfter)
    {
        DataBlockIndex blockIndex = findDataBlockIndex(_nameId, _startAfter);
        return getDataBlock(blockIndex);
    }

    //////////////////////////////////////////
    DataBlock::DataBlockIndex DataBlock::findDataBlockIndex(SharedStringId _nameId, DataBlockIndex _startAfter) const
    {
        DataBlock const** blockPtr = getDataBlocksPtr() + _startAfter;
        for (U32 i = _startAfter, e = getDataBlocksCount(); i < e; ++i, ++blockPtr)
            if ((*blockPtr)->getNameId() == _nameId)
                return i;
        return -1;
    }

    //////////////////////////////////////////
    DataBlock::DataBlockIndex DataBlock::findDataBlockIndexReverse(SharedStringId _nameId, DataBlockIndex _startBefore) const
    {
        if (_startBefore <= 0 || m_dataBlocksCount == 0)
            return -1;

        if (_startBefore > m_dataBlocksCount)
            _startBefore = m_dataBlocksCount;

        DataBlock const* const* __restrict it = getDataBlocksPtr() + _startBefore - 1;
        DataBlock const* const* __restrict end = getDataBlocksPtr() - 1;

        for (; it != end; --it)
            if ((*it)->getNameId() == _nameId)
                return it - getDataBlocksPtr();

        return -1;
    }

    //////////////////////////////////////////
    bool DataBlock::isDataBlockExists(HashedCString _name) const
    {
        SharedStringId nameId = getStringId(_name);
        return getDataBlockByNameId(nameId) != nullptr;
    }

    //////////////////////////////////////////
    bool DataBlock::removeDataBlock(HashedCString _name)
    {
        if (m_dataBlocksCount == 0)
            return false;

        SharedStringId nameId = getStringId(_name);
        if (nameId < 0)
            return false;

        DataBlockIndex blockIndex = findDataBlockIndexReverse(nameId, (DataBlockIndex)m_dataBlocksCount);
        if (blockIndex >= 0)
        {
            do
            {
                removeDataBlock(blockIndex);
            }
            while ((blockIndex = findDataBlockIndexReverse(nameId, blockIndex)) >= 0);
            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    bool DataBlock::removeDataBlock(DataBlockIndex _index)
    {
        if (_index >= (DataBlockIndex)m_dataBlocksCount)
            return false;

        DataBlock** dataBlock = getDataBlocksPtr() + _index;
        (*dataBlock)->~DataBlock();
        m_shared->freeDataBlock(*dataBlock);
        m_dataBuffer->erase(U32((ptrdiff_t)getDataBlocksPtr() - (ptrdiff_t)getParamsPtr()), sizeof(DataBlock*));
        --m_dataBlocksCount;
        return true;
    }

} // namespace Maze
//////////////////////////////////////////
