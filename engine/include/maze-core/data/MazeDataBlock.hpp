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
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include "maze-core/math/MazeVec2DB.hpp"
#include "maze-core/math/MazeVec3DB.hpp"
#include "maze-core/math/MazeVec4DB.hpp"
#include "maze-core/math/MazeMat3D.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/data/MazeHashedCString.hpp"
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
        ParamS32,
        ParamS64,
        ParamU32,
        ParamU64,
        ParamF32,
        ParamF64,
        ParamBool,
        ParamVec2DS,
        ParamVec3DS,
        ParamVec4DS,
        ParamVec2DU,
        ParamVec3DU,
        ParamVec4DU,
        ParamVec2DF,
        ParamVec3DF,
        ParamVec4DF,
        ParamVec2DB,
        ParamVec3DB,
        ParamVec4DB,
        ParamMat3DF,
        ParamMat4DF,
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
    DataBlockParamTypeInfo const MAZE_CORE_API c_dataBlockParamTypeInfo[(Size)DataBlockParamType::MAX] =
    {
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING(""), 0u),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("S32"), sizeof(S32)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("S64"), sizeof(S64)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("U32"), sizeof(U32)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("U64"), sizeof(U64)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("F32"), sizeof(F32)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("F64"), sizeof(F64)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Bool"), sizeof(Bool)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec2DS"), sizeof(Vec2DS)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec3DS"), sizeof(Vec3DS)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec4DS"), sizeof(Vec4DS)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec2DU"), sizeof(Vec2DU)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec3DU"), sizeof(Vec3DU)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec4DU"), sizeof(Vec4DU)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec2DF"), sizeof(Vec2DF)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec3DF"), sizeof(Vec3DF)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec4DF"), sizeof(Vec4DF)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec2DB"), sizeof(Vec2DB)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec3DB"), sizeof(Vec3DB)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Vec4DB"), sizeof(Vec4DB)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Mat3DF"), sizeof(Mat3DF)),
        DataBlockParamTypeInfo(MAZE_HASHED_CSTRING("Mat4DF"), sizeof(Mat4DF)),
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
        inline Size getDataSize() const { return m_buffer.getSize(); }

        //////////////////////////////////////////
        inline U8* getDataUnsafe(Size _offs = 0u) { return m_buffer.getDataPointer() + _offs; }

        //////////////////////////////////////////
        inline U8 const* getDataUnsafe(Size _offs = 0u) const { return m_buffer.getData() + _offs; }

        //////////////////////////////////////////
        U8* insertAt(U32 _at, Size _size, U8 const* _data);

        //////////////////////////////////////////
        void erase(U32 _at, Size _size);

        //////////////////////////////////////////
        void clear();

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
        DataBlock(DataBlock&& _value);

        //////////////////////////////////////////
        ~DataBlock();


        //////////////////////////////////////////
        inline bool isTopmost() const { return m_nameIdAndFlags & U32(DataBlockFlags::TopmostBlock); }

        //////////////////////////////////////////
        inline bool isEmpty() const { return getParamsCount() == 0 && getDataBlocksCount() == 0; }

        //////////////////////////////////////////
        inline SharedStringId getNameId() const { return m_nameIdAndFlags & 0xFFFFFF; }

        //////////////////////////////////////////
        inline SharedStringId getNameIdAndFlags() const { return m_nameIdAndFlags; }

        //////////////////////////////////////////
        HashedCString getName() const;

        //////////////////////////////////////////
        inline DataBlockShared const* getShared() const { return m_shared; }

        //////////////////////////////////////////
        inline DataBlockShared* getShared() { return m_shared; }

        //////////////////////////////////////////
        inline DataBlockDataBuffer const* getDataBuffer() const { return m_dataBuffer; }



        //////////////////////////////////////////
        inline Size getParamsUsedSize() const { return (Size)getParamsCount() * sizeof(Param); }

        //////////////////////////////////////////
        inline Size getBlocksUsedSize() const { return (Size)getDataBlocksCount() * sizeof(DataBlock*); }

        //////////////////////////////////////////
        Size getComplexParamsUsedSize() const;

        //////////////////////////////////////////
        void clearParams();

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

        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(S32, S32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(S64, S64);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(U32, U32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(U64, U64);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(F32, F32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(F64, F64);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(Bool, Bool);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec2DS, Vec2DS);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec3DS, Vec3DS);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4DS, Vec4DS);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec2DU, Vec2DU);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec3DU, Vec3DU);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4DU, Vec4DU);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec2DF, Vec2DF);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec3DF, Vec3DF);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4DF, Vec4DF);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec2DB, Vec2DB);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec3DB, Vec3DB);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Vec4DB, Vec4DB);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Mat3DF, Mat3DF);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(Mat4DF, Mat4DF);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(CString, CString);

#undef MAZE_DECLARE_DATA_BLOCK_GET_SET_API_BASE
#undef MAZE_DECLARE_DATA_BLOCK_GET_SET_API
#undef MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF

        //////////////////////////////////////////
        String const& getString(ParamIndex _index) const;

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
        ParamIndex findParamIndex(SharedStringId _nameId) const;

        //////////////////////////////////////////
        ParamIndex findParamIndexReverse(SharedStringId _nameId, ParamIndex _startBefore) const;

        //////////////////////////////////////////
        inline bool isParamExists(HashedCString _name) const { return findParamIndex(getSharedStringId(_name)) >= 0; }

        //////////////////////////////////////////
        bool removeParam(HashedCString _name);

        //////////////////////////////////////////
        bool removeParam(ParamIndex _index);


        //////////////////////////////////////////
        // DataBlocks API

        //////////////////////////////////////////
        inline U16 getDataBlocksCount() const { return m_dataBlocksCount; }

        //////////////////////////////////////////
        DataBlock* addDataBlock(HashedCString _name);

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
        DataBlock const* getDataBlockByNameId(SharedStringId _nameId, DataBlockIndex _startAfter = 0) const;

        //////////////////////////////////////////
        DataBlock* getDataBlockByNameId(SharedStringId _nameId, DataBlockIndex _startAfter = 0);

        //////////////////////////////////////////
        DataBlockIndex findDataBlockIndex(SharedStringId _nameId, DataBlockIndex _startAfter = 0) const;

        //////////////////////////////////////////
        DataBlockIndex findDataBlockIndexReverse(SharedStringId _nameId, DataBlockIndex _startAfter) const;

        //////////////////////////////////////////
        bool isDataBlockExists(HashedCString _name) const;

        //////////////////////////////////////////
        bool removeDataBlock(HashedCString _name);

        //////////////////////////////////////////
        bool removeDataBlock(DataBlockIndex _index);


    private:

        //////////////////////////////////////////
        // Params private

        //////////////////////////////////////////
        template <class TValue>
        inline TValue getParamValue(ParamIndex _index) const;

        //////////////////////////////////////////
        template <class TValue>
        inline TValue getParamValue(ParamIndex _index, TValue const& _defaultValue) const;

        //////////////////////////////////////////        
        inline String const& getParamValue(ParamIndex _index, String const& _defaultValue) const;

        //////////////////////////////////////////
        template <class TValue>
        inline TValue getParamValueByNameId(SharedStringId _nameId, TValue const &_defaultValue) const;

        //////////////////////////////////////////
        template <class TValue>
        inline TValue getParamValueByName(HashedCString _name, TValue const& _defaultValue = TValue()) const;

        //////////////////////////////////////////
        inline CString getParamValueCString(ParamValue _value) const;

        //////////////////////////////////////////
        inline String const& getParamValueString(ParamValue _value) const;



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
        inline Param* getParamsPtr() { return (Param*)getDataBufferData(); }

        //////////////////////////////////////////
        inline Param* getParamsEndPtr() { return getParamsPtr() + (Size)getParamsCount(); }

        //////////////////////////////////////////
        inline Param const* getParamsPtr() const { return (Param const*)getDataBufferData(); }

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
            MAZE_ASSERT(m_dataBuffer);
            return m_dataBuffer->getDataSize() - getDataBlocksCount() * sizeof(DataBlock*);
        }

        //////////////////////////////////////////
        inline DataBlock** getDataBlocksPtr() { return (DataBlock**)(getDataBufferData() + getDataBlocksOffset()); }

        //////////////////////////////////////////
        inline DataBlock const** getDataBlocksPtr() const { return (DataBlock const**)(getDataBufferData() + getDataBlocksOffset()); }


    protected:

        //////////////////////////////////////////
        inline U8* getDataBufferData(Size _offs = 0u)
        {
            return ensureDataBuffer()->getDataUnsafe(_offs);
        }

        //////////////////////////////////////////
        inline U8 const* getDataBufferData(Size _offs = 0u) const
        {
            if (!m_dataBuffer)
                return nullptr;

            return m_dataBuffer->getDataUnsafe(_offs);
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
        SharedStringId getSharedStringId(HashedCString _name) const;

        //////////////////////////////////////////
        SharedStringId addSharedString(CString _str, Size _length);

        //////////////////////////////////////////
        SharedStringId getSharedStringId(CString _str, Size _length) const;

        //////////////////////////////////////////
        CString getSharedCString(SharedStringId _nameId) const;

        //////////////////////////////////////////
        String const& getSharedString(SharedStringId _nameId) const;

        //////////////////////////////////////////
        HashedCString getSharedHashedCString(SharedStringId _nameId) const;

    protected:
        DataBlockShared* m_shared = nullptr;
        DataBlockDataBuffer* m_dataBuffer = nullptr;
        SharedStringId m_nameIdAndFlags = 0;
        U16 m_paramsCount = 0u;
        U16 m_dataBlocksCount = 0u;
    };


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeDataBlock.inl"


#endif // _MazeDataBlock_h_
//////////////////////////////////////////