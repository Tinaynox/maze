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
namespace Maze
{
    //////////////////////////////////////////
    template <>
    struct DataBlock::TypeOf<S32> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamS32; };
    template <>
    struct DataBlock::TypeOf<S64> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamS64; };
    template <>
    struct DataBlock::TypeOf<U32> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamU32; };
    template <>
    struct DataBlock::TypeOf<U64> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamU64; };
    template <>
    struct DataBlock::TypeOf<F32> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamF32; };
    template <>
    struct DataBlock::TypeOf<F64> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamF64; };
    template <>
    struct DataBlock::TypeOf<bool> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamBool; };
    template <>
    struct DataBlock::TypeOf<Vec4S8> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec4S8; };
    template <>
    struct DataBlock::TypeOf<Vec4U8> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec4U8; };
    template <>
    struct DataBlock::TypeOf<Vec2S> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec2S32; };
    template <>
    struct DataBlock::TypeOf<Vec3S> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec3S32; };
    template <>
    struct DataBlock::TypeOf<Vec4S> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec4S32; };
    template <>
    struct DataBlock::TypeOf<Vec2U> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec2U32; };
    template <>
    struct DataBlock::TypeOf<Vec3U> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec3U32; };
    template <>
    struct DataBlock::TypeOf<Vec4U> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec4U32; };
    template <>
    struct DataBlock::TypeOf<Vec2F> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec2F32; };
    template <>
    struct DataBlock::TypeOf<Vec3F> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec3F32; };
    template <>
    struct DataBlock::TypeOf<Vec4F> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec4F32; };
    template <>
    struct DataBlock::TypeOf<Vec2B> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec2B; };
    template <>
    struct DataBlock::TypeOf<Vec3B> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec3B; };
    template <>
    struct DataBlock::TypeOf<Vec4B> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamVec4B; };
    template <>
    struct DataBlock::TypeOf<Mat3F> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamMat3F32; };
    template <>
    struct DataBlock::TypeOf<Mat4F> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamMat4F32; };
    template <>
    struct DataBlock::TypeOf<CString> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamString; };
    template <>
    struct DataBlock::TypeOf<String> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamString; };
    template <>
    struct DataBlock::TypeOf<HashedCString> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamString; };
    template <>
    struct DataBlock::TypeOf<HashedString> { static MAZE_CONSTEXPR DataBlockParamType type = DataBlockParamType::ParamString; };




    //////////////////////////////////////////
    template <class TValue>
    inline TValue DataBlock::getParamValue(DataBlock::ParamIndex _index) const
    {
        return getParamValue(_index, TValue());
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue DataBlock::getParamValue(DataBlock::ParamIndex _index, TValue const& _defaultValue) const
    {
        Param const& param = getParam(_index);
        ParamValue const& value = param.value;

        if ((U8)param.type != (U8)TypeOf<TValue>::type)
        {
            MAZE_ERROR(
                "Param type mismatch. Inner type: '%s'(%d). Requested type: '%s'(%d)",
                static_cast<CString>(c_dataBlockParamTypeInfo[(S32)param.type].name), (S32)param.type,
                static_cast<CString>(c_dataBlockParamTypeInfo[(S32)TypeOf<TValue>::type].name), (S32)TypeOf<TValue>::type);
            return _defaultValue;
        }

        if MAZE_CONSTEXPR14 (sizeof(TValue) <= MAZE_DATA_BLOCK_INPLACE_PARAM_SIZE)
            return castParamValue<TValue, ParamValue>(value);

        return castParamValue<TValue>(getDataBufferData(getParamsUsedSize() + value));
    }

    //////////////////////////////////////////
    template <>
    inline CString DataBlock::getParamValue(DataBlock::ParamIndex _index, CString const& _defaultValue) const
    {
        Param const& param = getParam(_index);
        ParamValue const& value = param.value;

        if ((U8)param.type != (U8)TypeOf<CString>::type)
        {
            MAZE_ERROR(
                "Param type mismatch. Inner type: '%s'(%d). Requested type: '%s'(%d)",
                static_cast<CString>(c_dataBlockParamTypeInfo[(S32)param.type].name), (S32)param.type,
                static_cast<CString>(c_dataBlockParamTypeInfo[(S32)TypeOf<CString>::type].name), (S32)TypeOf<CString>::type);
            return _defaultValue;
        }

        return getParamValueCString(value);
    }

    //////////////////////////////////////////
    template <>
    inline String DataBlock::getParamValue(DataBlock::ParamIndex _index, String const& _defaultValue) const
    {
        return getString(_index, _defaultValue);
    }

    //////////////////////////////////////////
    template <>
    inline HashedCString DataBlock::getParamValue(DataBlock::ParamIndex _index, HashedCString const& _defaultValue) const
    {
        return HashedCString(getParamValue(_index, _defaultValue.str));
    }

    //////////////////////////////////////////
    template <>
    inline HashedString DataBlock::getParamValue(DataBlock::ParamIndex _index, HashedString const& _defaultValue) const
    {
        return HashedString(getString(_index, _defaultValue.getString()));
    }

    //////////////////////////////////////////        
    inline String const& DataBlock::getParamValue(ParamIndex _index, String const& _defaultValue) const
    {
        Param const& param = getParam(_index);
        ParamValue const& value = param.value;

        if ((U8)param.type != (U8)TypeOf<CString>::type)
        {
            MAZE_ERROR(
                "Param type mismatch. Inner type: '%s'(%d). Requested type: '%s'(%d)",
                static_cast<CString>(c_dataBlockParamTypeInfo[(S32)param.type].name), (S32)param.type,
                static_cast<CString>(c_dataBlockParamTypeInfo[(S32)TypeOf<CString>::type].name), (S32)TypeOf<CString>::type);
            return _defaultValue;
        }

        return getParamValueString(value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue DataBlock::getParamValueByNameId(SharedStringId _nameId, TValue const& _defaultValue) const
    {
        ParamIndex paramIdx = findParamIndex(_nameId);
        return paramIdx < 0 ? _defaultValue : getParamValue(paramIdx, _defaultValue);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue DataBlock::getParamValueByName(HashedCString _name, TValue const& _defaultValue) const
    {
        SharedStringId nameId = getSharedStringId(_name);
        return nameId < 0 ? _defaultValue : getParamValueByNameId(nameId, _defaultValue);
    }

    //////////////////////////////////////////
    inline CString DataBlock::getParamValueCString(ParamValue _value) const
    {
        return getSharedCString(_value);
    }

    //////////////////////////////////////////
    inline String const& DataBlock::getParamValueString(ParamValue _value) const
    {
        return getSharedString(_value);
    }
    
    //////////////////////////////////////////
    template <class TValue>
    inline bool DataBlock::setParam(DataBlock::ParamIndex _index, TValue const& _value)
    {
        Param& param = getParam(_index);
        if ((U8)param.type != (U8)TypeOf<TValue>::type)
        {
            MAZE_ERROR(
                "Param type mismatch. Inner type: '%s'(%d). Requested type: '%s'(%d)",
                static_cast<CString>(c_dataBlockParamTypeInfo[(S32)param.type].name), (S32)param.type,
                static_cast<CString>(c_dataBlockParamTypeInfo[(S32)TypeOf<TValue>::type].name), (S32)TypeOf<TValue>::type);
            return false;
        }

        ParamValue& value = param.value;
        if (sizeof(TValue) <= MAZE_DATA_BLOCK_INPLACE_PARAM_SIZE)
            memcpy(&value, &_value, sizeof(TValue));
        else
            memcpy(getDataBufferData(getParamsUsedSize() + value), &_value, sizeof(TValue));

        return true;
    }

    //////////////////////////////////////////
    template <>
    inline bool DataBlock::setParam(DataBlock::ParamIndex _index, const CString& _value)
    {
        Param& param = getParam(_index);
        if ((U8)param.type != (U8)TypeOf<CString>::type)
        {
            MAZE_ERROR(
                "Param type mismatch. Inner type: '%s'(%d). Requested type: '%s'(%d)",
                static_cast<CString>(c_dataBlockParamTypeInfo[(S32)param.type].name), (S32)param.type,
                static_cast<CString>(c_dataBlockParamTypeInfo[(S32)TypeOf<CString>::type].name), (S32)TypeOf<CString>::type);
            return false;
        }

        ParamValue& value = param.value;
        Size length = _value ? strlen(_value) : 0;

        value = addSharedString(_value ? _value : "", length);
        return true;
    }

    //////////////////////////////////////////
    template <>
    inline bool DataBlock::setParam(DataBlock::ParamIndex _index, const String& _value)
    {
        Param& param = getParam(_index);
        if ((U8)param.type != (U8)TypeOf<CString>::type)
        {
            MAZE_ERROR(
                "Param type mismatch. Inner type: '%s'(%d). Requested type: '%s'(%d)",
                static_cast<CString>(c_dataBlockParamTypeInfo[(S32)param.type].name), (S32)param.type,
                static_cast<CString>(c_dataBlockParamTypeInfo[(S32)TypeOf<CString>::type].name), (S32)TypeOf<CString>::type);
            return false;
        }

        ParamValue& value = param.value;
        value = !_value.empty() ? addSharedString(_value.c_str(), _value.size() - 1) : addSharedString("", 0u);
        return true;
    }

    //////////////////////////////////////////
    template <>
    inline bool DataBlock::setParam(DataBlock::ParamIndex _index, const HashedCString& _value)
    {
        return setParam(_index, _value.str);
    }

    //////////////////////////////////////////
    template <>
    inline bool DataBlock::setParam(DataBlock::ParamIndex _index, const HashedString& _value)
    {
        return setParam(_index, _value.getString());
    }

    //////////////////////////////////////////
    template <class TValue>
    inline DataBlock::ParamIndex DataBlock::setParamByNameId(SharedStringId _nameId, TValue const& _value)
    {
        ParamIndex paramIdx = findParamIndex(_nameId);
        if (paramIdx < 0)
            return addParamByNameId<TValue>(_nameId, _value);
        setParam<TValue>(paramIdx, _value);
        return paramIdx;
    }

    //////////////////////////////////////////
    template <class TValue>
    int DataBlock::setParamByName(HashedCString _name, TValue const& _value)
    {
        SharedStringId nameId = addSharedString(_name);
        return setParamByNameId<TValue>(nameId, _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    int DataBlock::addParamByName(HashedCString _name, TValue const& _value)
    {
        SharedStringId nameId = addSharedString(_name);
        return addParamByNameId<TValue>(nameId, _value);
    }

    //////////////////////////////////////////
    inline void DataBlock::insertParamAt(
        DataBlock::ParamIndex _at,
        SharedStringId _nameId,
        DataBlockParamType _type,
        Size _size,
        U8 const* _data)
    {
        MAZE_ASSERT(_type != DataBlockParamType::None);
        MAZE_ASSERT(_type != DataBlockParamType::ParamString);

        Param param;
        param.nameId = (U32)_nameId;
        param.type = (U32)_type;

        if (_size <= MAZE_DATA_BLOCK_INPLACE_PARAM_SIZE)
        {
            // Implace
            param.value = 0u;
            memcpy(&param.value, _data, _size);
            insertDataAt(_at * sizeof(Param), sizeof(Param), (U8 const*)&param);
            ++m_paramsCount;
        }
        else
        {
            // Save to complex params
            param.value = getComplexParamsUsedSize();
            insertDataAt(_at * sizeof(Param), sizeof(Param), (U8 const*)&param);
            ++m_paramsCount;
            insertDataAt(getParamsUsedSize() + param.value, _size, _data);
        }
    }

    //////////////////////////////////////////
    template <class TValue>
    inline DataBlock::ParamIndex DataBlock::insertParamAt(DataBlock::ParamIndex _at, SharedStringId _nameId, TValue const& _value)
    {
        insertParamAt(_at, _nameId, TypeOf<TValue>::type, sizeof(TValue), (U8 const*)&_value);
        return _at;
    }

    //////////////////////////////////////////
    template <>
    inline DataBlock::ParamIndex DataBlock::insertParamAt(ParamIndex _at, SharedStringId _nameId, CString const& _value)
    {
        Param param;
        param.nameId = (U32)_nameId;
        param.type = (U32)TypeOf<CString>::type;
        param.value = addSharedString(_value ? _value : "", _value ? strlen(_value) : 0);
        insertDataAt(_at * sizeof(Param), sizeof(Param), (U8 const*)&param);
        ++m_paramsCount;
        return _at;
    }

    //////////////////////////////////////////
    inline DataBlock::ParamIndex DataBlock::insertParamAt(ParamIndex _at, SharedStringId _nameId, String const& _value)
    {
        Param param;
        param.nameId = (U32)_nameId;
        param.type = (U32)TypeOf<CString>::type;
        param.value = !_value.empty() ? addSharedString(_value.c_str(), _value.size() - 1) : addSharedString("", 0u);
        insertDataAt(_at * sizeof(Param), sizeof(Param), (U8 const*)&param);
        ++m_paramsCount;
        return _at;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline DataBlock::ParamIndex DataBlock::addParamByNameId(SharedStringId _nameId, TValue const& _value)
    {
        return insertParamAt<TValue>((ParamIndex)getParamsCount(), _nameId, _value);
    }

    //////////////////////////////////////////
    inline DataBlock::ParamIndex DataBlock::addParamByNameId(SharedStringId _nameId, String const& _value)
    {
        return insertParamAt((ParamIndex)getParamsCount(), _nameId, _value);
    }

    //////////////////////////////////////////
    template <class TValue, class TFrom>
    MAZE_FORCEINLINE TValue DataBlock::castParamValue(TFrom const& _value)
    {
        TValue result;
        memcpy(&result, &_value, sizeof(TValue));
        return result;
    }

    //////////////////////////////////////////
    template <class TValue>
    MAZE_FORCEINLINE TValue DataBlock::castParamValue(U8 const* _value)
    {
        TValue result;
        memcpy(&result, _value, sizeof(TValue));
        return result;
    }

    //////////////////////////////////////////
    inline DataBlockDataBuffer* DataBlock::ensureDataBuffer()
    {
        if (!m_dataBuffer)
            createDataBuffer();
        return m_dataBuffer;
    }

} // namespace Maze
//////////////////////////////////////////
