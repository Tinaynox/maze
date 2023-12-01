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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(DataBlock);
    class DataBlockShared;


    //////////////////////////////////////////
    enum class MAZE_CORE_API DataBlockParamType : U8
    {
        None,
        ParamS32,
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
    Size const MAZE_CORE_API c_dataBlockParamTypeSize[(Size)DataBlockParamType::MAX] = 
    {
        0u,
        sizeof(S32),
        sizeof(F32),
        sizeof(F64),
        sizeof(bool),
        sizeof(Vec2DS),
        sizeof(Vec3DS),
        sizeof(Vec4DS),
        sizeof(Vec2DU),
        sizeof(Vec3DU),
        sizeof(Vec4DU),
        sizeof(Vec2DF),
        sizeof(Vec3DF),
        sizeof(Vec4DF),
        sizeof(Vec2DB),
        sizeof(Vec3DB),
        sizeof(Vec4DB),
        sizeof(Mat3DF),
        sizeof(Mat4DF),
        0u
    };


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
        void copyFrom(DataBlock const* _from);

        //////////////////////////////////////////
        DataBlock* duplicate() const;

        //////////////////////////////////////////
        bool saveToByteBuffer(ByteBuffer& _byteBuffer) const;

        //////////////////////////////////////////
        ByteBufferPtr saveToByteBuffer() const;

        //////////////////////////////////////////
        bool loadFromByteBuffer(ByteBuffer const& _byteBuffer);


        //////////////////////////////////////////
        // Params API

        //////////////////////////////////////////
        inline U16 getParamsCount() const { return m_paramsCount; }

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
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(F32, F32);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(F64, F64);
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_VAL(bool, Bool);
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
        MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF(CString, String);

#undef MAZE_DECLARE_DATA_BLOCK_GET_SET_API_BASE
#undef MAZE_DECLARE_DATA_BLOCK_GET_SET_API
#undef MAZE_DECLARE_DATA_BLOCK_GET_SET_API_REF
    
        //////////////////////////////////////////
        ParamIndex findParamIndex(SharedStringId _nameId) const;

        //////////////////////////////////////////
        ParamIndex findParamIndexReverse(SharedStringId _nameId, ParamIndex _startBefore) const;

        //////////////////////////////////////////
        inline bool isParamExists(HashedCString _name) const { return findParamIndex(getStringId(_name)) >= 0; }

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
        template <class TValue>
        inline TValue getParamValueByNameId(SharedStringId _nameId, TValue const &_defaultValue) const;

        //////////////////////////////////////////
        template <class TValue>
        inline TValue getParamValueByName(HashedCString _name, TValue const& _defaultValue = TValue()) const;

        //////////////////////////////////////////
        template <class TValue>
        inline TValue getParamValueString(ParamValue _value) const;


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
        template <class TValue>
        inline ParamIndex addParamByNameId(SharedStringId _nameId, TValue const& _value);



        //////////////////////////////////////////
        inline Param* getParamsPtr() { return (Param*)getMemoryBufferData(); }

        //////////////////////////////////////////
        inline Param* getParamsEndPtr() { return getParamsPtr() + (Size)getParamsCount(); }

        //////////////////////////////////////////
        inline Param const* getParamsPtr() const { return (Param const*)getMemoryBufferData(); }

        //////////////////////////////////////////
        inline Param const* getParamsEndPtr() const { return getParamsPtr() + (Size)getParamsCount(); }

        //////////////////////////////////////////
        Param& getParam(ParamIndex _index);

        //////////////////////////////////////////
        Param const& getParam(ParamIndex _index) const;


        //////////////////////////////////////////
        template <class TValue, class TFrom>
        MAZE_FORCEINLINE static TValue castParamValue(TFrom const& _value);

        //////////////////////////////////////////
        template <class TValue>
        MAZE_FORCEINLINE static TValue castParamValue(U8 const* _value);

        //////////////////////////////////////////
        template <class TValue>
        MAZE_FORCEINLINE TValue castParamValueString(CString) const;

    private:

        //////////////////////////////////////////
        // DataBlocks private

        //////////////////////////////////////////
        inline Size getDataBlocksOffset() const { return m_dataBuffer->getDataSize() - getDataBlocksCount() * sizeof(DataBlock*); }

        //////////////////////////////////////////
        inline DataBlock** getDataBlocksPtr() { return (DataBlock**)(getMemoryBufferData() + getDataBlocksOffset()); }

        //////////////////////////////////////////
        inline DataBlock const** getDataBlocksPtr() const { return (DataBlock const**)(getMemoryBufferData() + getDataBlocksOffset()); }


    protected:

        //////////////////////////////////////////
        inline U8* getMemoryBufferData(Size _offs = 0u)
        {
            MAZE_ASSERT(m_dataBuffer);
            return m_dataBuffer->getDataUnsafe(_offs);
        }

        //////////////////////////////////////////
        inline U8 const* getMemoryBufferData(Size _offs = 0u) const
        {
            MAZE_ASSERT(m_dataBuffer);
            return m_dataBuffer->getDataUnsafe(_offs);
        }

        //////////////////////////////////////////
        void createDataBuffer();

        //////////////////////////////////////////
        void deleteBuffers();

        //////////////////////////////////////////
        U8* insertDataAt(U32 _at, Size _size, U8 const* _data);

        //////////////////////////////////////////
        SharedStringId addString(HashedCString _name);

        //////////////////////////////////////////
        SharedStringId getStringId(HashedCString _name) const;

        //////////////////////////////////////////
        SharedStringId addString(CString _str, Size _length);

        //////////////////////////////////////////
        SharedStringId getStringId(CString _str, Size _length) const;

        //////////////////////////////////////////
        CString getSharedCString(SharedStringId _nameId) const;

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
