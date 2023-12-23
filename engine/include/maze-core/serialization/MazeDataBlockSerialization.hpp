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
#if (!defined(_MazeDataBlockSerialization_hpp_))
#define _MazeDataBlockSerialization_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/data/MazeDataBlock.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    template <typename TValue>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, TValue const& _value)
    {
        DataBlock* childBlock = _dataBlock.addNewDataBlock(_name);
        return TryValueToDataBlock<TValue>(_value, *childBlock);
    }

    //////////////////////////////////////////
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, S32 const& _value) { _dataBlock.addS32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, S64 const& _value) { _dataBlock.addS64(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, U32 const& _value) { _dataBlock.addU32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, U64 const& _value) { _dataBlock.addU64(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, F32 const& _value) { _dataBlock.addF32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, F64 const& _value) { _dataBlock.addF64(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Bool const& _value) { _dataBlock.addBool(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec2S32 const& _value) { _dataBlock.addVec2S32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec3S32 const& _value) { _dataBlock.addVec3S32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec4S32 const& _value) { _dataBlock.addVec4S32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec2U32 const& _value) { _dataBlock.addVec2U32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec3U32 const& _value) { _dataBlock.addVec3U32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec4U32 const& _value) { _dataBlock.addVec4U32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec2F32 const& _value) { _dataBlock.addVec2F32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec3F32 const& _value) { _dataBlock.addVec3F32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec4F32 const& _value) { _dataBlock.addVec4F32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec2B const& _value) { _dataBlock.addVec2B(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec3B const& _value) { _dataBlock.addVec3B(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec4B const& _value) { _dataBlock.addVec4B(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Mat3F32 const& _value) { _dataBlock.addMat3F32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Mat4F32 const& _value) { _dataBlock.addMat4F32(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, String const& _value) { _dataBlock.addString(_name, _value); return true; }
    template <>
    inline bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, CString const& _value) { _dataBlock.addCString(_name, _value); return true; }





    //////////////////////////////////////////
    // Type: IDataBlockSerializable
    //
    //////////////////////////////////////////
    inline void ValueToDataBlock(IDataBlockSerializable const& _value, DataBlock& _data)
    {
        _value.toDataBlock(_data);
    }

    //////////////////////////////////////////
    inline void ValueFromDataBlock(IDataBlockSerializable& _value, DataBlock const& _data)
    {
        _value.loadFromDataBlock(_data);
    }


    //////////////////////////////////////////
    // Type: SharedPtr
    //
    //////////////////////////////////////////
    template <typename UValue>
    inline void ValueToDataBlock(SharedPtr<UValue> const& _value, DataBlock& _data)
    {
        TryValueToDataBlock<UValue>(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data)
    {
        ValueToDataBlock(_value, _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    inline void ValueFromDataBlock(SharedPtr<UValue>& _value, DataBlock const& _data)
    {
        TryValueFromDataBlock<UValue>(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        ValueFromDataBlock(TValue& _value, DataBlock const& _data)
    {
        ValueFromDataBlock(_value, _data);
    }


    //////////////////////////////////////////
    // Type: std::function
    //
    //////////////////////////////////////////
    //template <typename TValue>
    //inline typename ::std::enable_if<(IsStdFunction<TValue>::value), void>::type
    //    ValueToDataBlock(TValue const& _value, DataBlock& _data)
    //{}

    ////////////////////////////////////////////
    //template <typename TValue>
    //inline typename ::std::enable_if<(IsStdFunction<TValue>::value), void>::type
    //    ValueFromDataBlock(TValue& _value, DataBlock const& _data)
    //{}



    //////////////////////////////////////////
    // Type: DataBlock
    //
    //////////////////////////////////////////
    inline void ValueToDataBlock(DataBlock const& _value, DataBlock& _data)
    {
        _data.copyFrom(&_value);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline void ValueFromDataBlock(DataBlock& _value, DataBlock const& _data)
    {
        _value.copyFrom(_data);
    }


    //////////////////////////////////////////
    // Type: Container
    //
    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    inline void ContainerMetaPropertyToDataBlock(
        TIterator _first,
        TIterator const _last,
        DataBlock& _data)
    {
        for (TIterator it = _first; it != _last; ++it)
        {
            AddDataToDataBlock(_data, MAZE_HS("i"), (*it));
        }
    }

    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    inline void ContainerMetaPropertyFromDataBlock(
        TIterator _it,
        DataBlock const& _data)
    {
        DataBlockParamType dataBlockParamType = DataBlock::TypeOf<TValue>::type;
        if (dataBlockParamType != DataBlockParamType::None)
        {
            for (DataBlock::ParamIndex i = 0; i < _data.getParamsCount(); ++i)
            {
                *_it++ = _data.getParamValue<TValue>(i);
            }
        }
        else
        {
            for (DataBlock const* dataBlock : _data)
            {
                TValue value;
                if (TryValueFromDataBlock<TValue>(value, *dataBlock))
                    *_it++ = value;
            }
        }
    }


    //////////////////////////////////////////
    // Type: Vector
    //
    //////////////////////////////////////////
    template <typename UValue>
    inline void ValueToDataBlock(Vector<UValue> const& _value, DataBlock& _data)
    {
        ContainerMetaPropertyToDataBlock<
            typename Vector<UValue>::const_iterator,
            UValue>(
                _value.begin(),
                _value.end(),
                _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data)
    {
        ValueToDataBlock(_value, _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    inline void ValueFromDataBlock(Vector<UValue>& _value, DataBlock const& _data)
    {
        ContainerMetaPropertyFromDataBlock<
            std::back_insert_iterator<Vector<UValue>>,
            UValue>(
                std::back_inserter(_value),
                _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueFromDataBlock(TValue& _value, DataBlock const& _data)
    {
        ValueFromDataBlock(_value, _data, _count);
    }


    //////////////////////////////////////////
    // Utils
    //
    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasValueFromDataBlock : std::false_type { };

    template <typename T>
    using ValueFromDataBlockType = void(&)(T&, DataBlock const&);
    template <typename T>
    struct HasValueFromDataBlock <T, decltype((ValueFromDataBlockType<T>)&ValueFromDataBlock, 0)> : std::true_type { };


    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasValueToDataBlock : std::false_type { };

    template <typename T>
    using ValueToDataBlockType = void(&)(T const&, DataBlock&);
    template <typename T>
    struct HasValueToDataBlock <T, decltype((ValueToDataBlockType<T>)&ValueToDataBlock, 0)> : std::true_type { };


    //////////////////////////////////////////
    // Try functions
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueFromDataBlock(typename ::std::enable_if<(HasValueFromDataBlock<TValue>::value), TValue>::type& _value, DataBlock const& _data)
    {
        ValueFromDataBlock(_value, _data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline  bool TryValueFromDataBlock(typename ::std::enable_if<(std::is_base_of<IDataBlockSerializable, TValue>::value), TValue>::type& _value, DataBlock const& _data)
    {
        _value.loadFromDataBlock(_data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline  bool TryValueFromDataBlock(typename ::std::enable_if<(
        !HasValueFromDataBlock<TValue>::value &&
        !std::is_base_of<IDataBlockSerializable, TValue>::value), TValue>::type& _value, DataBlock const& _data)
    {
        return false;
    }


    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueToDataBlock(typename ::std::enable_if<(HasValueFromDataBlock<TValue>::value), TValue>::type const& _value, DataBlock& _data)
    {
        ValueToDataBlock(_value, _data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueToDataBlock(typename ::std::enable_if<(std::is_base_of<IDataBlockSerializable, TValue>::value), TValue>::type const& _value, DataBlock& _data)
    {
        _value.toDataBlock(_data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueToDataBlock(typename ::std::enable_if<(
        !HasValueToDataBlock<TValue>::value &&
        !std::is_base_of<IDataBlockSerializable, TValue>::value), TValue>::type const& _value, DataBlock& _data)
    {
        return false;
    }



} // namespace Maze
//////////////////////////////////////////



//////////////////////////////////////////
#endif // _MazeDataBlockSerialization_hpp_
