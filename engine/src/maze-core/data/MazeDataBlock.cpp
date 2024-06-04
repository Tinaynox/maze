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
#include "maze-core/data/MazeDataBlockShared.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/serialization/MazeDataBlockBinarySerialization.hpp"
#include "maze-core/serialization/MazeDataBlockTextSerialization.hpp"
#include "maze-core/serialization/MazeDataBlockSerializationUtils.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"


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
                return (DataBlock::ParamIndex)(it - _start);
        }

        return -1;
    }

    //////////////////////////////////////////
    static inline StringKeyMap<DataBlockParamType> ConstructDataBlockParamTypeByName()
    {
        StringKeyMap<DataBlockParamType> result;

        for (Size i = 0; i < (Size)DataBlockParamType::MAX; ++i)
        {
            DataBlockParamTypeInfo const& info = c_dataBlockParamTypeInfo[i];
            result.insert(info.name, DataBlockParamType(i));
        }

        // Aliases
        result.insert(MAZE_HASHED_CSTRING("Vec2S32"), DataBlockParamType(DataBlockParamType::ParamVec2S32));
        result.insert(MAZE_HASHED_CSTRING("Vec3S32"), DataBlockParamType(DataBlockParamType::ParamVec3S32));
        result.insert(MAZE_HASHED_CSTRING("Vec4S32"), DataBlockParamType(DataBlockParamType::ParamVec4S32));

        result.insert(MAZE_HASHED_CSTRING("Vec2U32"), DataBlockParamType(DataBlockParamType::ParamVec2U32));
        result.insert(MAZE_HASHED_CSTRING("Vec3U32"), DataBlockParamType(DataBlockParamType::ParamVec3U32));
        result.insert(MAZE_HASHED_CSTRING("Vec4U32"), DataBlockParamType(DataBlockParamType::ParamVec4U32));

        result.insert(MAZE_HASHED_CSTRING("Vec2F32"), DataBlockParamType(DataBlockParamType::ParamVec2F32));
        result.insert(MAZE_HASHED_CSTRING("Vec3F32"), DataBlockParamType(DataBlockParamType::ParamVec3F32));
        result.insert(MAZE_HASHED_CSTRING("Vec4F32"), DataBlockParamType(DataBlockParamType::ParamVec4F32));

        result.insert(MAZE_HASHED_CSTRING("Mat3F32"), DataBlockParamType(DataBlockParamType::ParamMat3F32));
        result.insert(MAZE_HASHED_CSTRING("Mat4F32"), DataBlockParamType(DataBlockParamType::ParamMat4F32));

        return result;
    }

    //////////////////////////////////////////
    StringKeyMap<DataBlockParamType> const MAZE_CORE_API c_dataBlockParamTypeByName = ConstructDataBlockParamTypeByName();

    //////////////////////////////////////////
    MAZE_CORE_API DataBlockParamType GetDataBlockParamType(HashedCString _name)
    {
        auto it = c_dataBlockParamTypeByName.find(_name);
        if (it != c_dataBlockParamTypeByName.end())
            return it->second;
        
        return DataBlockParamType::None;
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
    DataBlock const DataBlock::c_empty;


    //////////////////////////////////////////
    DataBlock* DataBlock::Create()
    {
        return MAZE_NEW(DataBlock);
    }

    //////////////////////////////////////////
    DataBlock* DataBlock::LoadBinaryFile(Path const& _path)
    {
        DataBlock* block = Create();
        if (!block)
            return nullptr;

        if (!block->loadBinaryFile(_path))
        {
            MAZE_DELETE(block);
            return nullptr;
        }

        return block;
    }

    //////////////////////////////////////////
    DataBlock* DataBlock::LoadTextFile(Path const& _path)
    {
        DataBlock* block = Create();
        if (!block)
            return nullptr;

        if (!block->loadTextFile(_path))
        {
            MAZE_DELETE(block);
            return nullptr;
        }

        return block;
    }

    //////////////////////////////////////////
    DataBlock::DataBlock()
        : m_shared(MAZE_NEW(DataBlockShared))
    {
        m_nameIdAndFlags |= U32(DataBlockFlags::TopmostBlock);
    }

    //////////////////////////////////////////
    DataBlock::DataBlock(DataBlock const& _value)
        : DataBlock()
    {
        if (_value.getNameId() != 0)
            setName(_value.getName());

        copyFrom(&_value);
    }

    //////////////////////////////////////////
    DataBlock::DataBlock(DataBlock&& _value) noexcept
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
        m_nameIdAndFlags |= addSharedString(_name);
    }

    //////////////////////////////////////////
    DataBlock::~DataBlock()
    {
        if (!m_shared && !m_dataBuffer)
            return;

        for (DataBlockIndex i = 0, e = (DataBlockIndex)getDataBlocksCount(); i < e; ++i)
        {
            DataBlock* dataBlock = getDataBlock(i);
            dataBlock->~DataBlock();
            m_shared->freeDataBlock(dataBlock);
        }

        deleteBuffers();
    }

    //////////////////////////////////////////
    bool DataBlock::isComment() const
    {
        HashedCString name = getName();
        if (name.empty())
            return false;

        return name[0u] == MAZE_DATA_BLOCK_COMMENT_PREFIX[0] && name[1u] == MAZE_DATA_BLOCK_COMMENT_PREFIX[1];
    }

    //////////////////////////////////////////
    HashedCString DataBlock::getName() const
    {
        SharedStringId nameId = getNameId();
        if (nameId == 0)
            return HashedCString();

        return getNameHashedCString(nameId);
    }

    //////////////////////////////////////////
    void DataBlock::setName(HashedCString _name)
    {
        MAZE_DEBUG_ASSERT(!_name.empty());
        if (!_name.empty())
            m_nameIdAndFlags = addSharedString(_name) | (m_nameIdAndFlags & U32(DataBlockFlags::TopmostBlock));
    }

    //////////////////////////////////////////
    U32 DataBlock::getComplexParamsUsedSize() const
    {
        if (!m_dataBuffer)
            return 0;

        return m_dataBuffer->getDataSize() - (getBlocksUsedSize() + getParamsUsedSize());
    }

    //////////////////////////////////////////
    void DataBlock::clearParams()
    {
        if (m_dataBuffer)
            m_dataBuffer->erase(0, m_dataBuffer->getDataSize() - getBlocksUsedSize());

        m_paramsCount = 0;
    }

    //////////////////////////////////////////
    void DataBlock::clearData()
    {
        for (DataBlockIndex i = 0, e = (DataBlockIndex)getDataBlocksCount(); i < e; ++i)
        {
            DataBlock* dataBlock = getDataBlock(i);
            dataBlock->~DataBlock();
            m_shared->freeDataBlock(dataBlock);
        }


        if (m_dataBuffer)
            m_dataBuffer->clear();

        m_paramsCount = 0;
        m_dataBlocksCount = 0;
    }

    //////////////////////////////////////////
    void DataBlock::clear()
    {
        MAZE_ERROR_RETURN_IF(!isTopmost(), "Clear is cannot be called for the not topmost block");

        m_nameIdAndFlags &= ~(0xFFFFFF);

        clearData();
        m_shared->clear();
    }

    //////////////////////////////////////////
    void DataBlock::appendParamsFrom(DataBlock const* _from)
    {
        if (_from->getParamsCount() == 0u)
            return;

        Param const* paramPtr = _from->getParamsPtr();

        for (ParamIndex i = 0, e = (ParamIndex)_from->getParamsCount(); i < e; ++i, ++paramPtr)
        {
            Param const& param = *paramPtr;
            SharedStringId paramNameId = addSharedString(_from->getNameHashedCString(param.nameId));
            
            if (param.type == U32(DataBlockParamType::ParamString))
            {
                insertParamAt<CString>(
                    (ParamIndex)getParamsCount(),
                    paramNameId,
                    _from->getParamValueCString(param.value));
            }
            else
            {
                Size size = c_dataBlockParamTypeInfo[param.type].size;
                U8 const* paramData = (size <= MAZE_DATA_BLOCK_INPLACE_PARAM_SIZE) ? (U8 const*)&param.value
                                                                                   : _from->getDataBufferDataRO(_from->getParamsUsedSize() + param.value);
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
    void DataBlock::copyParamsFrom(U16 _paramsCount, U8 const* _paramsData, U32 _paramsDataSize)
    {
        clearParams();

        m_paramsCount = _paramsCount;
        ensureDataBuffer()->insertAt(0, _paramsDataSize, _paramsData);
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
        for (DataBlockIndex i = 0, e = (DataBlockIndex)_from->getDataBlocksCount(); i < e; ++i)
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
    bool DataBlock::isEqual(DataBlock const& _value) const
    {
        if (getParamsCount() != _value.getParamsCount() || getDataBlocksCount() != _value.getDataBlocksCount())
            return false;

        Param const* param0Ptr = getParamsPtr();
        Param const* param1Ptr = _value.getParamsPtr();
        for (ParamIndex i = 0, e = (ParamIndex)getParamsCount(); i < e; ++i, ++param0Ptr, ++param1Ptr)
        {
            Param const& param0 = *param0Ptr;
            Param const& param1 = *param1Ptr;

            if (param0.nameId != param1.nameId || param0.type != param1.type)
                return false;

            if (param0.type == U32(DataBlockParamType::ParamString))
            {
                if (getParamValueString(param0.value) != _value.getParamValueString(param1.value))
                    return false;
            }
            else
            {
                Size size = c_dataBlockParamTypeInfo[param0.type].size;
                U8 const* param0Data = (size <= MAZE_DATA_BLOCK_INPLACE_PARAM_SIZE) ? (U8 const*)&param0.value
                    : getDataBufferDataRO(getParamsUsedSize() + param0.value);
                U8 const* param1Data = (size <= MAZE_DATA_BLOCK_INPLACE_PARAM_SIZE) ? (U8 const*)&param1.value
                    : _value.getDataBufferDataRO(_value.getParamsUsedSize() + param1.value);
                if (memcmp(param0Data, param1Data, size) != 0)
                    return false;
            }
        }

        for (DataBlockIndex i = 0; i < (DataBlockIndex)getDataBlocksCount(); ++i)
        {
            DataBlock const* dataBlock0 = getDataBlock(i);
            DataBlock const* dataBlock1 = _value.getDataBlock(i);

            if (!dataBlock0->isEqual(*dataBlock1))
                return false;
        }

        return true;
    }

    //////////////////////////////////////////
    bool DataBlock::saveBinary(ByteBuffer& _byteBuffer, U32 _flags) const
    {
        return DataBlockBinarySerialization::SaveBinary(*this, _byteBuffer, _flags);
    }

    //////////////////////////////////////////
    ByteBufferPtr DataBlock::saveBinary(U32 _flags) const
    {
        ByteBufferPtr byteBuffer = ByteBuffer::Create();
        if (!saveBinary(*byteBuffer.get(), _flags))
            return nullptr;
        return byteBuffer;
    }

    //////////////////////////////////////////
    bool DataBlock::loadBinary(ByteBuffer const& _byteBuffer)
    {
        return DataBlockBinarySerialization::LoadBinary(*this, _byteBuffer);
    }

    //////////////////////////////////////////
    bool DataBlock::saveBinaryFile(Path const& _path, U32 _flags) const
    {
        return DataBlockBinarySerialization::SaveBinaryFile(*this, _path, _flags);
    }

    //////////////////////////////////////////
    bool DataBlock::loadBinaryFile(Path const& _path)
    {
        return DataBlockBinarySerialization::LoadBinaryFile(*this, _path);
    }


    //////////////////////////////////////////
    bool DataBlock::saveText(ByteBuffer& _byteBuffer, U32 _flags) const
    {
        return DataBlockTextSerialization::SaveText(*this, _byteBuffer, _flags);
    }

    //////////////////////////////////////////
    ByteBufferPtr DataBlock::saveText(U32 _flags) const
    {
        ByteBufferPtr byteBuffer = ByteBuffer::Create();
        if (!saveText(*byteBuffer.get(), _flags))
            return nullptr;
        return byteBuffer;
    }

    //////////////////////////////////////////
    bool DataBlock::loadText(ByteBuffer const& _byteBuffer)
    {
        return DataBlockTextSerialization::LoadText(*this, _byteBuffer);
    }

    //////////////////////////////////////////
    bool DataBlock::saveTextFile(Path const& _path, U32 _flags) const
    {
        return DataBlockTextSerialization::SaveTextFile(*this, _path, _flags);
    }

    //////////////////////////////////////////
    bool DataBlock::loadTextFile(Path const& _path)
    {
        return DataBlockTextSerialization::LoadTextFile(*this, _path);
    }

    //////////////////////////////////////////
    bool DataBlock::loadFile(Path const& _path)
    {
        FILE* file = StdHelper::OpenFile(_path, "rb");
        if (!file)
            return false;

        U8 buff[sizeof(c_mzDataBlockBinaryHeaderMagic)];
        fread(buff, sizeof(c_mzDataBlockBinaryHeaderMagic), 1, file);
        fclose(file);

        if (memcmp(buff, &c_mzDataBlockBinaryHeaderMagic, sizeof(c_mzDataBlockBinaryHeaderMagic)) == 0)
            return loadBinaryFile(_path);
        else
            return loadTextFile(_path);
    }

    //////////////////////////////////////////
    bool DataBlock::loadFromByteBuffer(ByteBuffer const& _byteBuffer)
    {
        if (_byteBuffer.getSize() >= sizeof(c_mzDataBlockBinaryHeaderMagic) &&
            memcmp((void*)_byteBuffer.getDataRO(), &c_mzDataBlockBinaryHeaderMagic, sizeof(c_mzDataBlockBinaryHeaderMagic)) == 0)
        {
            return loadBinary(_byteBuffer);
        }
        else
        {
            return loadText(_byteBuffer);
        }
    }


    //////////////////////////////////////////
    DataBlock::Param& DataBlock::getParam(ParamIndex _index)
    {
        MAZE_DEBUG_ASSERT(_index >= 0 && _index < (ParamIndex)getParamsCount());
        return getParamsPtr()[_index];
    }

    //////////////////////////////////////////
    DataBlock::Param const& DataBlock::getParam(ParamIndex _index) const
    {
        MAZE_DEBUG_ASSERT(_index >= 0 && _index < (ParamIndex)getParamsCount());
        return getParamsPtr()[_index];
    }

    //////////////////////////////////////////
    U8 const* DataBlock::getParamData(ParamIndex _index) const
    {
        DataBlock::Param const& param = getParam(_index);
        ParamValue const& value = param.value;
        if (param.type == U32(DataBlockParamType::ParamString))
            return (U8 const*)getSharedCString(value);
        
        if (c_dataBlockParamTypeInfo[param.type].size <= MAZE_DATA_BLOCK_INPLACE_PARAM_SIZE)
            return (U8 const*)(&value);
        else
            return getDataBufferDataRO(getParamsUsedSize() + value);
    }

    //////////////////////////////////////////
    DataBlockParamType DataBlock::getParamType(ParamIndex _index) const
    {
        return (_index >= 0 && _index < (ParamIndex)getParamsCount()) ? (DataBlockParamType)getParam(_index).type : DataBlockParamType::None;
    }

    //////////////////////////////////////////
    DataBlock::ParamIndex DataBlock::findParamIndex(SharedStringId _nameId) const
    {
        if (m_paramsCount == 0 || _nameId < 0)
            return -1;

        return FindParamIndex(_nameId, getParamsPtr(), getParamsEndPtr());
    }

    //////////////////////////////////////////
    DataBlock::ParamIndex DataBlock::findParamIndex(HashedCString _name) const
    {
        return findParamIndex(getSharedStringId(_name));
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
                return (DataBlock::ParamIndex)(it - ptr);
        return -1;
    }

    //////////////////////////////////////////
    void DataBlock::createDataBuffer()
    {
        MAZE_DEBUG_ASSERT(!m_dataBuffer);
        if (m_shared)
            m_dataBuffer = new(m_shared->allocateDataBuffer()) DataBlockDataBuffer();
    }

    //////////////////////////////////////////
    void DataBlock::deleteBuffers()
    {
        if (m_dataBuffer)
        {
            MAZE_DEBUG_ASSERT(m_shared);
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
        return ensureDataBuffer()->insertAt(_at, _size, _data);
    }

    //////////////////////////////////////////
    DataBlock::SharedStringId DataBlock::addSharedString(HashedCString _name)
    {
        return m_shared->addString(_name);
    }

    //////////////////////////////////////////
    DataBlock::SharedStringId DataBlock::getSharedStringId(HashedCString _name) const
    {
        return m_shared->getStringId(_name);
    }

    //////////////////////////////////////////
    DataBlock::SharedStringId DataBlock::addSharedString(CString _str, Size _length)
    {
        return m_shared->addString(_str, _length);
    }

    //////////////////////////////////////////
    DataBlock::SharedStringId DataBlock::getSharedStringId(CString _str, Size _length) const
    {
        return m_shared->getStringId(_str, _length);
    }

    //////////////////////////////////////////
    CString DataBlock::getSharedCString(SharedStringId _nameId) const
    {
        return m_shared->getCString(_nameId);
    }

    //////////////////////////////////////////
    String const& DataBlock::getSharedString(SharedStringId _nameId) const
    {
        return m_shared->getString(_nameId);
    }

    //////////////////////////////////////////
    HashedCString DataBlock::getNameHashedCString(SharedStringId _nameId) const
    {
        return m_shared->getNameHashedCString(_nameId);
    }

    //////////////////////////////////////////
#define MAZE_DECLARE_DATA_BLOCK_GET_SET_API_BASE(__DValueType, __DValueRefType, __typeName)                                       \
    __DValueType DataBlock::get##__typeName(DataBlock::ParamIndex _index) const                                                   \
    {                                                                                                                             \
        return getParamValue<__DValueType>(_index);                                                                               \
    }                                                                                                                             \
    __DValueType DataBlock::get##__typeName(HashedCString _name, __DValueRefType _defaultValue) const                             \
    {                                                                                                                             \
        return getParamValueByName<__DValueType>(_name, _defaultValue);                                                           \
    }                                                                                                                             \
    __DValueType DataBlock::get##__typeName(HashedCString _name) const                                                            \
    {                                                                                                                             \
        return getParamValueByName<__DValueType>(_name);                                                                          \
    }                                                                                                                             \
    __DValueType DataBlock::get##__typeName##ByNameId(DataBlock::SharedStringId _nameId, __DValueRefType _defaultValue) const     \
    {                                                                                                                             \
        return getParamValueByNameId<__DValueType>(_nameId, _defaultValue);                                                       \
    }                                                                                                                             \
    bool DataBlock::set##__typeName(ParamIndex _index, __DValueRefType _value)                                                    \
    {                                                                                                                             \
        if (_index >= (ParamIndex)getParamsCount())                                                                               \
          return false;                                                                                                           \
        return setParam<__DValueType>(_index, _value);                                                                            \
    }                                                                                                                             \
    DataBlock::ParamIndex DataBlock::set##__typeName##ByNameId(DataBlock::SharedStringId _nameId, __DValueRefType _value)         \
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
    DataBlock::ParamIndex DataBlock::addNew##__typeName##ByNameId(DataBlock::SharedStringId _nameId, __DValueRefType _value)      \
    {                                                                                                                             \
        return addParamByNameId<__DValueType>(_nameId, _value);                                                                   \
    }

#define MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(__DValueType, __typeName) MAZE_DECLARE_DATA_BLOCK_GET_SET_API_BASE(__DValueType, __DValueType, __typeName)
#define MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(__DValueType, __typeName) MAZE_DECLARE_DATA_BLOCK_GET_SET_API_BASE(__DValueType, __DValueType const&, __typeName)

    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(S32, S32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(S64, S64);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(U32, U32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(U64, U64);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(F32, F32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(F64, F64);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_VAL(Bool, Bool);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4S8, Vec4S8);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4U8, Vec4U8);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec2S32, Vec2S32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec2S, Vec2S);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec3S32, Vec3S32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec3S, Vec3S);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4S32, Vec4S32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4S, Vec4S);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec2U32, Vec2U32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec2U, Vec2U);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec3U32, Vec3U32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec3U, Vec3U);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4U32, Vec4U32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4U, Vec4U);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec2F32, Vec2F32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec2F, Vec2F);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec3F32, Vec3F32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec3F, Vec3F);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4F32, Vec4F32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4F, Vec4F);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec2B, Vec2B);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec3B, Vec3B);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Vec4B, Vec4B);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Mat3F32, Mat3F32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Mat3F, Mat3F);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Mat4F32, Mat4F32);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(Mat4F, Mat4F);
    MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF(CString, CString);

#undef MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_BASE
#undef MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API
#undef MAZE_IMPLEMENT_DATA_BLOCK_GET_SET_API_REF

    //////////////////////////////////////////
    String const& DataBlock::getString(ParamIndex _index) const
    {
        String const nullValue;
        return getString(_index, nullValue);
    }

    //////////////////////////////////////////
    String const& DataBlock::getString(HashedCString _name, String const& _defaultValue) const
    {
        SharedStringId nameId = getSharedStringId(_name);
        return nameId < 0 ? _defaultValue : getStringByNameId(nameId, _defaultValue);
    }

    //////////////////////////////////////////
    String const& DataBlock::getString(HashedCString _name) const
    {
        String const nullValue;
        return getString(_name, nullValue);
    }

    //////////////////////////////////////////
    String const& DataBlock::getStringByNameId(SharedStringId _nameId, String const& _defaultValue) const
    {
        ParamIndex paramIdx = findParamIndex(_nameId);
        return paramIdx < 0 ? _defaultValue : getString(paramIdx);
    }

    //////////////////////////////////////////
    bool DataBlock::setString(ParamIndex _index, String const& _value)
    {
        if (_index >= (ParamIndex)getParamsCount())
            return false;
        return setParam<String const&>(_index, _value);
    }

    //////////////////////////////////////////
    DataBlock::ParamIndex DataBlock::setStringByNameId(SharedStringId _nameId, String const& _value)
    {
        ParamIndex paramIdx = findParamIndex(_nameId);
        if (paramIdx < 0)
            return addParamByNameId(_nameId, _value);
        setParam(paramIdx, _value);
        return paramIdx;
    }

    //////////////////////////////////////////
    DataBlock::ParamIndex DataBlock::setString(HashedCString _name, String const& _value)
    {
        SharedStringId nameId = addSharedString(_name);
        return setStringByNameId(nameId, _value);
    }

    //////////////////////////////////////////
    DataBlock::ParamIndex DataBlock::addString(HashedCString _name, String const& _value)
    {
        SharedStringId nameId = addSharedString(_name);
        return addParamByNameId(nameId, _value);
    }

    //////////////////////////////////////////
    DataBlock::ParamIndex DataBlock::addNewStringByNameId(SharedStringId _nameId, String const& _value)
    {
        return addParamByNameId(_nameId, _value);
    }

    //////////////////////////////////////////
    bool DataBlock::removeParam(HashedCString _name)
    {
        if (m_paramsCount == 0)
            return false;

        SharedStringId nameId = getSharedStringId(_name);
        if (nameId == 0)
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

        ensureDataBuffer()->erase(_index * sizeof(Param), sizeof(Param));
        --m_paramsCount;
        return true;
    }

    //////////////////////////////////////////
    DataBlock* DataBlock::getDataBlock(HashedCString _name)
    {
        SharedStringId nameId = getSharedStringId(_name);
        if (nameId == 0)
            return nullptr;
        return getDataBlockByNameId(nameId);
    }

    //////////////////////////////////////////
    DataBlock const* DataBlock::getDataBlock(HashedCString _name) const
    {
        SharedStringId nameId = getSharedStringId(_name);
        if (nameId == 0)
            return nullptr;
        return getDataBlockByNameId(nameId);
    }

    //////////////////////////////////////////
    DataBlock* DataBlock::getDataBlock(DataBlockIndex _index)
    {
        if (_index >= 0 && _index < getDataBlocksCount())
            return *(getDataBlocksPtr() + _index);
        return nullptr;
    }

    //////////////////////////////////////////
    DataBlock const* DataBlock::getDataBlock(DataBlockIndex _index) const
    {
        if (_index >= 0 && _index < getDataBlocksCount())
            return *(getDataBlocksPtr() + _index);
        return nullptr;
    }

    //////////////////////////////////////////
    DataBlock* DataBlock::getDataBlock(HashedCString _name, DataBlock* _defValue)
    {
        DataBlock* dataBlock = getDataBlock(_name);
        return dataBlock ? dataBlock : _defValue;
    }

    //////////////////////////////////////////
    DataBlock const* DataBlock::getDataBlock(HashedCString _name, DataBlock const* _defValue) const
    {
        DataBlock const* dataBlock = getDataBlock(_name);
        return dataBlock ? dataBlock : _defValue;
    }

    //////////////////////////////////////////
    DataBlock& DataBlock::getDataBlock(HashedCString _name, DataBlock& _defValue)
    {
        DataBlock* dataBlock = getDataBlock(_name);
        return dataBlock ? *dataBlock : _defValue;
    }

    //////////////////////////////////////////
    DataBlock const& DataBlock::getDataBlock(HashedCString _name, DataBlock const& _defValue) const
    {
        DataBlock const* dataBlock = getDataBlock(_name);
        return dataBlock ? *dataBlock : _defValue;
    }

    //////////////////////////////////////////
    DataBlock& DataBlock::operator[](HashedCString _name)
    {
        DataBlock* result = getDataBlock(_name);
        if (result != nullptr)
            return *result;
        return *addNewDataBlock(_name);
    }

    //////////////////////////////////////////
    DataBlock& DataBlock::operator=(DataBlock const& _value)
    {
        copyFrom(&_value);
        return *this;
    }

    //////////////////////////////////////////
    DataBlock& DataBlock::operator=(DataBlock&& _value) noexcept
    {
        if (isTopmost() && _value.isTopmost())
        {
            std::swap(m_shared, _value.m_shared);
            std::swap(m_dataBuffer, _value.m_dataBuffer);
            std::swap(m_nameIdAndFlags, _value.m_nameIdAndFlags);
            std::swap(m_paramsCount, _value.m_paramsCount);
            std::swap(m_dataBlocksCount, _value.m_dataBlocksCount);
        }
        else
        {
            if (isTopmost())
            {
                clear();

                if (_value.getNameId() != 0)
                    setName(_value.getName());
            }

            copyFrom(&_value);
        }

        return *this;
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
        insertDataAt(ensureDataBuffer()->getDataSize(), sizeof(DataBlock*), (U8 const*)&newBlock);
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
        for (DataBlockIndex i = 0, e = (DataBlockIndex)_copyFrom->getDataBlocksCount(); i < e; ++i)
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
        if (m_dataBlocksCount == 0)
            return -1;

        DataBlock const** blockPtr = getDataBlocksPtr() + _startAfter;
        for (DataBlockIndex i = _startAfter, e = (DataBlockIndex)getDataBlocksCount(); i < e; ++i, ++blockPtr)
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
                return (DataBlock::DataBlockIndex)(it - getDataBlocksPtr());

        return -1;
    }

    //////////////////////////////////////////
    bool DataBlock::isDataBlockExists(HashedCString _name) const
    {
        SharedStringId nameId = getSharedStringId(_name);
        if (nameId == 0)
            return false;

        return getDataBlockByNameId(nameId) != nullptr;
    }

    //////////////////////////////////////////
    bool DataBlock::removeDataBlock(HashedCString _name)
    {
        if (m_dataBlocksCount == 0)
            return false;

        SharedStringId nameId = getSharedStringId(_name);
        if (nameId == 0)
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
        ensureDataBuffer()->erase(U32((ptrdiff_t)getDataBlocksPtr() - (ptrdiff_t)getParamsPtr()), sizeof(DataBlock*));
        --m_dataBlocksCount;
        return true;
    }

} // namespace Maze
//////////////////////////////////////////
