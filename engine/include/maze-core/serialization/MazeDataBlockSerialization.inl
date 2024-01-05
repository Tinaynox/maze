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
namespace Maze
{
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, TValue const& _value)
    {
        DataBlock dataBlock;
        if (TryValueToDataBlock<TValue>(_value, dataBlock))
        {
            *_dataBlock.addNewDataBlock(_name) = std::move(dataBlock);
            return true;
        }

        String stringValue;
        if (TryValueToString<TValue>(_value, stringValue))
            return AddDataToDataBlock(_dataBlock, _name, stringValue);

        return false;
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE TValue GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return TValue(); }



    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, S32 const& _value) { _dataBlock.addS32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, S64 const& _value) { _dataBlock.addS64(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, U32 const& _value) { _dataBlock.addU32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, U64 const& _value) { _dataBlock.addU64(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, F32 const& _value) { _dataBlock.addF32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, F64 const& _value) { _dataBlock.addF64(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Bool const& _value) { _dataBlock.addBool(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec4S8 const& _value) { _dataBlock.addVec4S8(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec4U8 const& _value) { _dataBlock.addVec4U8(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec2S32 const& _value) { _dataBlock.addVec2S32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec3S32 const& _value) { _dataBlock.addVec3S32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec4S32 const& _value) { _dataBlock.addVec4S32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec2U32 const& _value) { _dataBlock.addVec2U32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec3U32 const& _value) { _dataBlock.addVec3U32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec4U32 const& _value) { _dataBlock.addVec4U32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec2F32 const& _value) { _dataBlock.addVec2F32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec3F32 const& _value) { _dataBlock.addVec3F32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec4F32 const& _value) { _dataBlock.addVec4F32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec2B const& _value) { _dataBlock.addVec2B(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec3B const& _value) { _dataBlock.addVec3B(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Vec4B const& _value) { _dataBlock.addVec4B(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Mat3F32 const& _value) { _dataBlock.addMat3F32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, Mat4F32 const& _value) { _dataBlock.addMat4F32(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, String const& _value) { _dataBlock.addString(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, CString const& _value) { _dataBlock.addCString(_name, _value); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, HashedCString const& _value) { _dataBlock.addCString(_name, _value.str); return true; }
    template <>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, HashedString const& _value) { _dataBlock.addString(_name, _value.getString()); return true; }


    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE S32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getS32(_i); }
    template <>
    MAZE_FORCEINLINE S64 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getS64(_i); }
    template <>
    MAZE_FORCEINLINE U32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getU32(_i); }
    template <>
    MAZE_FORCEINLINE U64 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getU64(_i); }
    template <>
    MAZE_FORCEINLINE F32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getF32(_i); }
    template <>
    MAZE_FORCEINLINE F64 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getF64(_i); }
    template <>
    MAZE_FORCEINLINE Bool GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getBool(_i); }
    template <>
    MAZE_FORCEINLINE Vec4S8 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec4S8(_i); }
    template <>
    MAZE_FORCEINLINE Vec4U8 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec4U8(_i); }
    template <>
    MAZE_FORCEINLINE Vec2S32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec2S32(_i); }
    template <>
    MAZE_FORCEINLINE Vec3S32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec3S32(_i); }
    template <>
    MAZE_FORCEINLINE Vec4S32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec4S32(_i); }
    template <>
    MAZE_FORCEINLINE Vec2U32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec2U32(_i); }
    template <>
    MAZE_FORCEINLINE Vec3U32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec3U32(_i); }
    template <>
    MAZE_FORCEINLINE Vec4U32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec4U32(_i); }
    template <>
    MAZE_FORCEINLINE Vec2F32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec2F32(_i); }
    template <>
    MAZE_FORCEINLINE Vec3F32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec3F32(_i); }
    template <>
    MAZE_FORCEINLINE Vec4F32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec4F32(_i); }
    template <>
    MAZE_FORCEINLINE Vec2B GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec2B(_i); }
    template <>
    MAZE_FORCEINLINE Vec3B GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec3B(_i); }
    template <>
    MAZE_FORCEINLINE Vec4B GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getVec4B(_i); }
    template <>
    MAZE_FORCEINLINE Mat3F32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getMat3F32(_i); }
    template <>
    MAZE_FORCEINLINE Mat4F32 GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getMat4F32(_i); }
    template <>
    MAZE_FORCEINLINE CString GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getCString(_i); }
    template <>
    MAZE_FORCEINLINE String GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return _data.getString(_i); }
    template <>
    MAZE_FORCEINLINE HashedCString GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return HashedCString(_data.getCString(_i)); }
    template <>
    MAZE_FORCEINLINE HashedString GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i) { return HashedString(_data.getString(_i)); }


} // namespace Maze
//////////////////////////////////////////
