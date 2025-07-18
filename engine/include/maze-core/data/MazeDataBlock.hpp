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
#pragma once
#if (!defined(_MazeDataBlock_hpp_))
#define _MazeDataBlock_hpp_


//////////////////////////////////////////
// Include
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include "maze-core/serialization/MazeBinarySerializable.hpp"
#include "maze-core/serialization/MazeStringSerializable.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/memory/MazeBlockMemoryAllocator.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include "maze-core/math/MazeVec2B.hpp"
#include "maze-core/math/MazeVec3B.hpp"
#include "maze-core/math/MazeVec4B.hpp"
#include "maze-core/math/MazeMat3.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeTMat.hpp"
#include "maze-core/data/MazeHashedCString.hpp"
#include "maze-core/data/MazeHashedString.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(DataBlock);
    class DataBlockShared;


    //////////////////////////////////////////
    enum class MAZE_CORE_API DataBlockBinaryFlags : U32
    {
        CheckSumProtection = MAZE_BIT(0)
    };


    //////////////////////////////////////////
    enum class MAZE_CORE_API DataBlockTextFlags : U32
    {
        Compact = MAZE_BIT(0)
    };


    //////////////////////////////////////////
    enum class MAZE_CORE_API DataBlockParamType : U8
    {
        None,
        ParamS8,
        ParamS16,
        ParamS32,
        ParamS64,
        ParamU8,
        ParamU16,
        ParamU32,
        ParamU64,
        ParamF32,
        ParamF64,
        ParamBool,
        ParamVec4S8,
        ParamVec4U8,
        ParamVec2S32,
        ParamVec3S32,
        ParamVec4S32,
        ParamVec2U32,
        ParamVec3U32,
        ParamVec4U32,
        ParamVec2F32,
        ParamVec3F32,
        ParamVec4F32,
        ParamVec2B,
        ParamVec3B,
        ParamVec4B,
        ParamMat3F32,
        ParamMat4F32,
        ParamTMat,
        ParamString,
        
        MAX
    };


    //////////////////////////////////////////
    struct MAZE_CORE_API DataBlockParamTypeInfo
    {
        //////////////////////////////////////////
        DataBlockParamTypeInfo(
            HashedCString _name,
            Size _size)
            : name(_name)
            , size(_size)
        {}

        HashedCString name;
        Size size;
    };


    //////////////////////////////////////////
    DataBlockParamTypeInfo const c_dataBlockParamTypeInfo[(Size)DataBlockParamType::MAX] =
    {
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING(""), 0u),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("S8"), sizeof(S8)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("S16"), sizeof(S16)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("S32"), sizeof(S32)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("S64"), sizeof(S64)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("U8"), sizeof(U8)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("U16"), sizeof(U16)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("U32"), sizeof(U32)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("U64"), sizeof(U64)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("F32"), sizeof(F32)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("F64"), sizeof(F64)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Bool"), sizeof(Bool)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec4S8"), sizeof(Vec4S8)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec4U8"), sizeof(Vec4U8)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec2S"), sizeof(Vec2S)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec3S"), sizeof(Vec3S)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec4S"), sizeof(Vec4S)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec2U"), sizeof(Vec2U)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec3U"), sizeof(Vec3U)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec4U"), sizeof(Vec4U)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec2F"), sizeof(Vec2F)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec3F"), sizeof(Vec3F)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec4F"), sizeof(Vec4F)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec2B"), sizeof(Vec2B)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec3B"), sizeof(Vec3B)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec4B"), sizeof(Vec4B)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Mat3F"), sizeof(Mat3F)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Mat4F"), sizeof(Mat4F)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("TMat"), sizeof(TMat)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("String"), 0u)
    };


    //////////////////////////////////////////
    MAZE_CORE_API DataBlockParamType GetDataBlockParamType(HashedCString _name);


    //////////////////////////////////////////
    // Enum DataBlockFlags
    //
    //////////////////////////////////////////
    enum class MAZE_CORE_API DataBlockFlags : U32
    {
        TopmostBlock = MAZE_BIT_U32(31)
    };


    //////////////////////////////////////////
    // Class DataBlockDataBuffer
    //
    //////////////////////////////////////////
    class MAZE_CORE_API DataBlockDataBuffer MAZE_FINAL
    {
    public:

        //////////////////////////////////////////
        DataBlockDataBuffer() = default;

        //////////////////////////////////////////
        ~DataBlockDataBuffer() = default;

        //////////////////////////////////////////
        inline U32 getDataSize() const { return m_buffer.getSize(); }

        //////////////////////////////////////////
        inline U8* getDataRW(Size _offs = 0u) { return m_buffer.getDataRW() + _offs; }

        //////////////////////////////////////////
        inline U8 const* getDataRO(Size _offs = 0u) const { return m_buffer.getDataRO() + _offs; }

        //////////////////////////////////////////
        inline U8* insertAt(U32 _at, Size _size, U8 const* _data) { return m_buffer.insertAt(_at, _size, _data); }

        //////////////////////////////////////////
        inline void erase(U32 _at, Size _size) { m_buffer.erase(_at, _size); }

        //////////////////////////////////////////
        inline void clear() { m_buffer.clear(); }

    private:
        ByteBuffer m_buffer;
    };


    //////////////////////////////////////////
    // Class DataBlock
    //
    //                       Memory buffer structure:
    //
    //         Params             Complex Params          Data Blocks
    // [Param|Param|Param|...][         ...       ][DataBlock*|DataBlock*|...]
    //////////////////////////////////////////
    class MAZE_CORE_API DataBlock MAZE_FINAL
    {
    public:

        //////////////////////////////////////////
        template <class T>
        struct TypeOf { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::None; };
        template <class T>
        struct TypeOf<T&> { static MAZE_CONSTEXPR DataBlockParamType type = TypeOf<T>::type; };
        template <class T>
        struct TypeOf<T&&> { static MAZE_CONSTEXPR DataBlockParamType type = TypeOf<T>::type; };


        //////////////////////////////////////////
        using SharedStringId = U32;
        using ParamIndex = S32;
        using ParamValue = U32;
        using DataBlockIndex = S32;


        //////////////////////////////////////////
        struct Param
        {
            U32 nameId : 24;
            U32 type : 8;
            ParamValue value;
        };
        static_assert(sizeof(Param) == 8, "sizeof(Param) is not 64 bit");

        //////////////////////////////////////////
        static MAZE_CONSTEXPR Size MAZE_DATA_BLOCK_INPLACE_PARAM_SIZE = sizeof(decltype(Param::value));

        //////////////////////////////////////////
        static DataBlock const c_empty;

    public:

        //////////////////////////////////////////
        template <typename TValue = DataBlock>
        class iterator_base
        {
        public:

            //////////////////////////////////////////
            inline iterator_base(TValue* _dataBlock, DataBlock::DataBlockIndex _index)
                : m_dataBlock(_dataBlock)
                , m_index(_index)
            {}

            //////////////////////////////////////////
            inline TValue* operator*() { return m_dataBlock->getDataBlock(m_index); }

            //////////////////////////////////////////
            inline iterator_base& operator++() { ++m_index; return *this; }

            //////////////////////////////////////////
            inline bool operator!=(iterator_base const& _other) const { return m_index != _other.m_index; }

        private:
            TValue* m_dataBlock = nullptr;
            DataBlock::DataBlockIndex m_index = 0;
        };

        //////////////////////////////////////////
        using iterator = iterator_base<DataBlock>;
        using const_iterator = iterator_base<DataBlock const>;


        //////////////////////////////////////////
        inline iterator begin() { return iterator(this, 0); }

        //////////////////////////////////////////
        inline iterator end() { return iterator(this, static_cast<DataBlockIndex>(m_dataBlocksCount)); }

        //////////////////////////////////////////
        inline const_iterator begin() const { return const_iterator(this, 0); }

        //////////////////////////////////////////
        inline const_iterator end() const { return const_iterator(this, static_cast<DataBlockIndex>(m_dataBlocksCount)); }

    public:

        //////////////////////////////////////////
        static DataBlock* Create();

        //////////////////////////////////////////
        static DataBlock* LoadBinaryFile(Path const& _path);

        //////////////////////////////////////////
        static DataBlock* LoadTextFile(Path const& _path);

        //////////////////////////////////////////
        DataBlock();

        //////////////////////////////////////////
        DataBlock(DataBlockShared* _shared, HashedCString _name);

        //////////////////////////////////////////
        DataBlock(DataBlock const& _value);

        //////////////////////////////////////////
        DataBlock(DataBlock&& _value) noexcept;

        //////////////////////////////////////////
        ~DataBlock();


        //////////////////////////////////////////
        inline bool isTopmost() const { return m_nameIdAndFlags & U32(DataBlockFlags::TopmostBlock); }

        //////////////////////////////////////////
        inline bool isEmpty() const { return getParamsCount() == 0 && getDataBlocksCount() == 0; }

        //////////////////////////////////////////
        bool isComment() const;

        //////////////////////////////////////////
        inline SharedStringId getNameId() const { return m_nameIdAndFlags & 0xFFFFFF; }

        //////////////////////////////////////////
        inline SharedStringId getNameIdAndFlags() const { return m_nameIdAndFlags; }

        //////////////////////////////////////////
        HashedCString getName() const;

        //////////////////////////////////////////
        void setName(HashedCString _name);

        //////////////////////////////////////////
        inline DataBlockShared const* getShared() const { return m_shared; }

        //////////////////////////////////////////
        inline DataBlockShared* getShared() { return m_shared; }

        //////////////////////////////////////////
        inline DataBlockDataBuffer const* getDataBuffer() const { return m_dataBuffer; }



        //////////////////////////////////////////
        inline U32 getParamsUsedSize() const { return (U32)getParamsCount() * sizeof(Param); }

        //////////////////////////////////////////
        inline U32 getBlocksUsedSize() const { return (U32)getDataBlocksCount() * sizeof(DataBlock*); }

        //////////////////////////////////////////
        U32 getComplexParamsUsedSize() const;

        //////////////////////////////////////////
        void clearParams();

        //////////////////////////////////////////
        void clearData();

        //////////////////////////////////////////
        void clear();



        //////////////////////////////////////////
        // Serialization API

        //////////////////////////////////////////
        void appendParamsFrom(DataBlock const* _from);

        //////////////////////////////////////////
        void copyParamsFrom(DataBlock const* _from);

        //////////////////////////////////////////
        void copyParamsFrom(U16 _paramsCount, U8 const* _paramsData, U32 _paramsDataSize);

        //////////////////////////////////////////
        void copyFrom(DataBlock const* _from);

        //////////////////////////////////////////
        DataBlock* duplicate() const;

        //////////////////////////////////////////
        bool isEqual(DataBlock const& _value) const;


        //////////////////////////////////////////
        bool saveBinary(ByteBuffer& _byteBuffer, U32 _flags = 0u) const;

        //////////////////////////////////////////
        ByteBufferPtr saveBinary(U32 _flags = 0u) const;

        //////////////////////////////////////////
        bool loadBinary(ByteBuffer const& _byteBuffer);

        //////////////////////////////////////////
        bool saveBinaryFile(Path const& _path, U32 _flags = 0u) const;

        //////////////////////////////////////////
        bool loadBinaryFile(Path const& _path);


        //////////////////////////////////////////
        bool saveText(ByteBuffer& _byteBuffer, U32 _flags = 0u) const;

        //////////////////////////////////////////
        ByteBufferPtr saveText(U32 _flags = 0u) const;

        //////////////////////////////////////////
        bool loadText(ByteBuffer const& _byteBuffer);

        //////////////////////////////////////////
        bool saveTextFile(Path const& _path, U32 _flags = 0u) const;

        //////////////////////////////////////////
        bool loadTextFile(Path const& _path);

        //////////////////////////////////////////
        bool loadRegularFile(Path const& _path);

        //////////////////////////////////////////
        bool loadFromByteBuffer(ByteBuffer const& _byteBuffer);



        //////////////////////////////////////////
        // Params API

        //////////////////////////////////////////
        inline U16 getParamsCount() const { return m_paramsCount; }

        //////////////////////////////////////////
        Param const& getParam(ParamIndex _index) const;

        //////////////////////////////////////////
        U8 const* getParamData(ParamIndex _index) const;

        //////////////////////////////////////////
        DataBlockParamType getParamType(ParamIndex _index) const;

#define MAZE_DECLARE_DATA_BLOCK_GET_SET_API_BASE(__DValueType, __DValueRefType, __typeName)             \
  __DValueType get##__typeName(ParamIndex _index) const;                                                \
  __DValueType get##__typeName(HashedCString _name, __DValueRefType _defaultValue) const;               \
  __DValueType get##__typeName(HashedCString _name) const;                                              \
  __DValueType get##__typeName##ByNameId(SharedStringId _nameId, __DValueRefType _defaultValue) const;  \
  bool set##__typeName(ParamIndex _index, __DValueRefType _value);                                      \
  ParamIndex set##__typeName##ByNameId(SharedStringId _nameId, __DValueRefType _value);                 \
  ParamIndex set##__typeName(HashedCString _name, __DValueRefType _value);                              \
  ParamIndex add##__typeName(HashedCString _name, __DValueRefType _value);                              \
  ParamIndex addNew##__typeName##ByNameId(SharedStringId _nameId, __DValueRefType _value);

#define MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(__DValueType, __typeName) MAZE_DECLARE_DATA_BLOCK_GET_SET_API_BASE(__DValueType, __DValueType, __typeName)
#define MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(__DValueType, __typeName) MAZE_DECLARE_DATA_BLOCK_GET_SET_API_BASE(__DValueType, __DValueType const&, __typeName)

        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(S8, S8);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(S16, S16);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(S32, S32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(S64, S64);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(U8, U8);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(U16, U16);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(U32, U32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(U64, U64);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(F32, F32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(F64, F64);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(Bool, Bool);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4S8, Vec4S8);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4U8, Vec4U8);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec2S32, Vec2S32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec2S, Vec2S);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec3S32, Vec3S32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec3S, Vec3S);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4S32, Vec4S32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4S, Vec4S);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec2U32, Vec2U32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec2U, Vec2U);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec3U32, Vec3U32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec3U, Vec3U);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4U32, Vec4U32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4U, Vec4U);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec2F32, Vec2F32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec2F, Vec2F);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec3F32, Vec3F32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec3F, Vec3F);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4F32, Vec4F32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4F, Vec4F);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec2B, Vec2B);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec3B, Vec3B);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4B, Vec4B);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Mat3F32, Mat3F32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Mat3F, Mat3F);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Mat4F32, Mat4F32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Mat4F, Mat4F);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(TMat, TMat);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(CString, CString);

#undef MAZE_DECLARE_DATA_BLOCK_GET_SET_API_BASE
#undef MAZE_DECLARE_DATA_BLOCK_GET_SET_API
#undef MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF

        //////////////////////////////////////////
        String const& getString(ParamIndex _index) const;

        //////////////////////////////////////////
        String const& getString(ParamIndex _index, String const& _defaultValue) const;

        //////////////////////////////////////////
        String const& getString(HashedCString _name, String const& _defaultValue) const;

        //////////////////////////////////////////
        String const& getString(HashedCString _name) const;

        //////////////////////////////////////////
        String const& getStringByNameId(SharedStringId _nameId, String const& _defaultValue) const;

        //////////////////////////////////////////
        bool setString(ParamIndex _index, String const& _value);

        //////////////////////////////////////////
        ParamIndex setStringByNameId(SharedStringId _nameId, String const& _value);

        //////////////////////////////////////////
        ParamIndex setString(HashedCString _name, String const& _value);

        //////////////////////////////////////////
        ParamIndex addString(HashedCString _name, String const& _value);

        //////////////////////////////////////////
        ParamIndex addNewStringByNameId(SharedStringId _nameId, String const& _value);



        //////////////////////////////////////////
        HashedCString getHashedCString(ParamIndex _index, HashedCString const& _defaultValue = HashedCString::c_empty) const;

        //////////////////////////////////////////
        HashedCString getHashedCString(HashedCString _name, HashedCString const& _defaultValue = HashedCString::c_empty) const;

        //////////////////////////////////////////
        HashedCString getHashedCStringByNameId(SharedStringId _nameId, HashedCString const& _defaultValue) const;


        //////////////////////////////////////////
        HashedString getHashedString(ParamIndex _index, HashedString const& _defaultValue = HashedString::c_empty) const;

        //////////////////////////////////////////
        HashedString getHashedString(HashedCString _name, HashedString const& _defaultValue = HashedString::c_empty) const;

        //////////////////////////////////////////
        HashedString getHashedStringByNameId(SharedStringId _nameId, HashedString const& _defaultValue) const;
        

    

        //////////////////////////////////////////
        ParamIndex findParamIndex(SharedStringId _nameId) const;

        //////////////////////////////////////////
        ParamIndex findParamIndex(HashedCString _name) const;

        //////////////////////////////////////////
        ParamIndex findParamIndexReverse(SharedStringId _nameId, ParamIndex _startBefore) const;

        //////////////////////////////////////////
        inline bool isParamExists(HashedCString _name) const { return findParamIndex(getSharedStringId(_name)) >= 0; }

        //////////////////////////////////////////
        bool removeParam(HashedCString _name);

        //////////////////////////////////////////
        bool removeParam(ParamIndex _index);

        //////////////////////////////////////////
        template <class TValue>
        inline TValue getParamValue(ParamIndex _index) const;

        //////////////////////////////////////////
        template <class TValue>
        inline TValue getParamValue(ParamIndex _index, TValue const& _defaultValue) const;


        //////////////////////////////////////////
        // DataBlocks API

        //////////////////////////////////////////
        inline U16 getDataBlocksCount() const { return m_dataBlocksCount; }

        //////////////////////////////////////////
        DataBlock* ensureDataBlock(HashedCString _name);

        //////////////////////////////////////////
        DataBlock* addNewDataBlock(HashedCString _name);

        //////////////////////////////////////////
        DataBlock* addNewDataBlock(DataBlock const* _copyFrom, HashedCString _name = HashedCString());

        //////////////////////////////////////////
        DataBlock* getDataBlock(HashedCString _name);

        //////////////////////////////////////////
        DataBlock const* getDataBlock(HashedCString _name) const;

        //////////////////////////////////////////
        DataBlock* getDataBlock(DataBlockIndex _index);

        //////////////////////////////////////////
        DataBlock const* getDataBlock(DataBlockIndex _index) const;

        //////////////////////////////////////////
        DataBlock* getDataBlock(HashedCString _name, DataBlock* _defValue);

        //////////////////////////////////////////
        DataBlock const* getDataBlock(HashedCString _name, DataBlock const* _defValue) const;

        //////////////////////////////////////////
        DataBlock& getDataBlock(HashedCString _name, DataBlock& _defValue);

        //////////////////////////////////////////
        DataBlock const& getDataBlock(HashedCString _name, DataBlock const& _defValue) const;

        //////////////////////////////////////////
        Vector<CString> getDataBlockAsVectorCString(HashedCString _name) const;

        //////////////////////////////////////////
        void setDataBlockAsVectorCString(HashedCString _name, Vector<CString> const& _value);

        //////////////////////////////////////////
        Vector<String> getDataBlockAsVectorString(HashedCString _name) const;

        //////////////////////////////////////////
        void setDataBlockAsVectorString(HashedCString _name, Vector<String> const& _value);

        //////////////////////////////////////////
        Set<String> getDataBlockAsSetString(HashedCString _name) const;

        //////////////////////////////////////////
        void setDataBlockAsSetString(HashedCString _name, Set<String> const& _value);


        //////////////////////////////////////////
        DataBlock& operator[](HashedCString _name);

        //////////////////////////////////////////
        DataBlock& operator=(DataBlock const& _value);

        //////////////////////////////////////////
        DataBlock& operator=(DataBlock&& _value) noexcept;

        //////////////////////////////////////////
        inline bool operator==(DataBlock const& _value) const { return isEqual(_value); }


        //////////////////////////////////////////
        DataBlock const* getDataBlockByNameId(SharedStringId _nameId, DataBlockIndex _startAfter = 0) const;

        //////////////////////////////////////////
        DataBlock* getDataBlockByNameId(SharedStringId _nameId, DataBlockIndex _startAfter = 0);

        //////////////////////////////////////////
        DataBlockIndex findDataBlockIndex(SharedStringId _nameId, DataBlockIndex _startAfter = 0) const;

        //////////////////////////////////////////
        DataBlockIndex findDataBlockIndex(HashedCString _name) const;

        //////////////////////////////////////////
        DataBlockIndex findDataBlockIndexReverse(SharedStringId _nameId, DataBlockIndex _startAfter) const;

        //////////////////////////////////////////
        bool isDataBlockExists(HashedCString _name) const;

        //////////////////////////////////////////
        bool removeDataBlock(HashedCString _name);

        //////////////////////////////////////////
        bool removeDataBlock(DataBlockIndex _index);



        //////////////////////////////////////////
        // Shared API

        //////////////////////////////////////////
        SharedStringId getSharedStringId(HashedCString _name) const;

        //////////////////////////////////////////
        SharedStringId getSharedStringId(CString _str, Size _length) const;

        //////////////////////////////////////////
        CString getSharedCString(SharedStringId _nameId) const;

        //////////////////////////////////////////
        String const& getSharedString(SharedStringId _nameId) const;

        //////////////////////////////////////////
        HashedCString getSharedHashedCString(SharedStringId _nameId) const;

    private:

        //////////////////////////////////////////
        // Params private

        //////////////////////////////////////////        
        inline String const& getParamValue(ParamIndex _index, String const& _defaultValue) const;

        //////////////////////////////////////////
        template <class TValue>
        inline TValue getParamValueByNameId(SharedStringId _nameId, TValue const &_defaultValue) const;

        //////////////////////////////////////////
        template <class TValue>
        inline TValue getParamValueByName(HashedCString _name, TValue const& _defaultValue = TValue()) const;



        //////////////////////////////////////////
        template <class TValue>
        inline bool setParam(ParamIndex _index, TValue const& _value);

        //////////////////////////////////////////
        template <class TValue>
        inline ParamIndex setParamByNameId(SharedStringId _nameId, TValue const& _value);

        //////////////////////////////////////////
        template <class TValue>
        int setParamByName(HashedCString _name, TValue const& _value);

        //////////////////////////////////////////
        template <class TValue>
        int addParamByName(HashedCString _name, TValue const& _value);

        //////////////////////////////////////////
        inline void insertParamAt(
            ParamIndex _at,
            SharedStringId _nameId,
            DataBlockParamType _type,
            Size _size,
            U8 const* _data);

        //////////////////////////////////////////
        template <class TValue>
        inline ParamIndex insertParamAt(ParamIndex _at, SharedStringId _nameId, TValue const& _value);

        //////////////////////////////////////////
        inline ParamIndex insertParamAt(ParamIndex _at, SharedStringId _nameId, String const& _value);

        //////////////////////////////////////////
        template <class TValue>
        inline ParamIndex addParamByNameId(SharedStringId _nameId, TValue const& _value);

        //////////////////////////////////////////
        inline ParamIndex addParamByNameId(SharedStringId _nameId, String const& _value);



        //////////////////////////////////////////
        inline Param* getParamsPtr() { return (Param*)getDataBufferDataRW(); }

        //////////////////////////////////////////
        inline Param* getParamsEndPtr() { return getParamsPtr() + (Size)getParamsCount(); }

        //////////////////////////////////////////
        inline Param const* getParamsPtr() const { return (Param const*)getDataBufferDataRO(); }

        //////////////////////////////////////////
        inline Param const* getParamsEndPtr() const { return getParamsPtr() + (Size)getParamsCount(); }

        //////////////////////////////////////////
        Param& getParam(ParamIndex _index);



        //////////////////////////////////////////
        template <class TValue, class TFrom>
        MAZE_FORCEINLINE static TValue castParamValue(TFrom const& _value);

        //////////////////////////////////////////
        template <class TValue>
        MAZE_FORCEINLINE static TValue castParamValue(U8 const* _value);

    private:

        //////////////////////////////////////////
        // DataBlocks private

        //////////////////////////////////////////
        inline Size getDataBlocksOffset() const
        {
            MAZE_DEBUG_ASSERT(m_dataBuffer);
            return m_dataBuffer->getDataSize() - getDataBlocksCount() * sizeof(DataBlock*);
        }

        //////////////////////////////////////////
        inline DataBlock** getDataBlocksPtr() { return (DataBlock**)(getDataBufferDataRW() + getDataBlocksOffset()); }

        //////////////////////////////////////////
        inline DataBlock const** getDataBlocksPtr() const { return (DataBlock const**)(getDataBufferDataRO() + getDataBlocksOffset()); }


    protected:

        //////////////////////////////////////////
        inline U8* getDataBufferDataRW(Size _offs = 0u)
        {
            return ensureDataBuffer()->getDataRW(_offs);
        }

        //////////////////////////////////////////
        inline U8 const* getDataBufferDataRO(Size _offs = 0u) const
        {
            if (!m_dataBuffer)
                return nullptr;

            return m_dataBuffer->getDataRO(_offs);
        }

        //////////////////////////////////////////
        void createDataBuffer();

        //////////////////////////////////////////
        void deleteBuffers();

        //////////////////////////////////////////
        inline DataBlockDataBuffer* ensureDataBuffer();

        //////////////////////////////////////////
        U8* insertDataAt(U32 _at, Size _size, U8 const* _data);

        //////////////////////////////////////////
        SharedStringId addSharedString(HashedCString _name);

        //////////////////////////////////////////
        SharedStringId addSharedString(CString _str, Size _length);


    protected:
        DataBlockShared* m_shared = nullptr;
        DataBlockDataBuffer* m_dataBuffer = nullptr;
        SharedStringId m_nameIdAndFlags = 0;
        U16 m_paramsCount = 0u;
        U16 m_dataBlocksCount = 0u;
    };


    //////////////////////////////////////////
    template <typename>
    struct IsDataBlock : std::false_type {};
    template <>
    struct IsDataBlock<DataBlock> : std::true_type {};


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeDataBlock.inl"


#endif // _MazeDataBlock_h_
//////////////////////////////////////////
