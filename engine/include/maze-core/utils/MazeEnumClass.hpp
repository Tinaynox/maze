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
#if (!defined(_MazeEnumClass_hpp_))
#define _MazeEnumClass_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/serialization/MazeBinarySerializable.hpp"
#include "maze-core/serialization/MazeStringSerializable.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"


//////////////////////////////////////////
#if (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)

#    if (defined(None))
#        undef None
#    endif

#endif // (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)
//////////////////////////////////////////


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EnumClass
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EnumClass
        : public IBinarySerializable
        , public IStringSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(EnumClass);

    public:

        //////////////////////////////////////////
        virtual Vector<String> getAllValuesVectorString() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual Vector<S32> getAllValuesVectorS32() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual S32 asS32() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void set(S32 _value) MAZE_ABSTRACT;
    };


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_BASE(DClassAPI, DEnumClassName, DEnumBlock1, DEnumBlock2)    \
                                                                                    class DClassAPI DEnumClassName \
                                                                                        : public EnumClass\
                                                                                    {\
                                                                                    public:\
                                                                                        MAZE_DECLARE_METACLASS_WITH_PARENT(DEnumClassName, EnumClass);\
                                                                                    public:\
                                                                                        enum Enum\
                                                                                        {\
                                                                                            None = 0,\
                                                                                            DEnumBlock1\
                                                                                            MAX\
                                                                                        };\
                                                                                        \
                                                                                        DEnumClassName();\
                                                                                        DEnumClassName(Enum value);\
                                                                                        explicit DEnumClassName(Maze::U16 _value);\
                                                                                        explicit DEnumClassName(Maze::S32 _value);\
                                                                                        explicit DEnumClassName(Maze::U32 _value);\
                                                                                        \
                                                                                        StdString toStdString() const;\
                                                                                        CString toCString() const;\
                                                                                        void setString(String const& string) MAZE_OVERRIDE;\
                                                                                        void setString(StdString const& string);\
                                                                                        \
                                                                                        Enum toEnum() const;\
                                                                                        \
                                                                                        inline operator Enum() const { return m_value; }\
                                                                                        inline bool operator==(Enum _p) const { return m_value == _p; }\
                                                                                        inline bool operator==(DEnumClassName const& _p) const { return m_value == _p.m_value; }\
                                                                                        inline bool operator!=(Enum _p) const { return m_value != _p; }\
                                                                                        inline bool operator!=(DEnumClassName const& _p) const { return m_value != _p.m_value; }\
                                                                                        inline bool operator<(Enum _p) const { return m_value < _p; }\
                                                                                        inline bool operator<(DEnumClassName const& _p) const { return m_value < _p.m_value; }\
                                                                                        inline bool operator>(Enum _p) const { return m_value > _p; }\
                                                                                        inline bool operator>(DEnumClassName const& _p) const { return m_value > _p.m_value; }\
                                                                                        inline bool operator<=(Enum _p) const { return m_value <= _p; }\
                                                                                        inline bool operator<=(DEnumClassName const& _p) const { return m_value <= _p.m_value; }\
                                                                                        inline bool operator>=(Enum _p) const { return m_value >= _p; }\
                                                                                        inline bool operator>=(DEnumClassName const& _p) const { return m_value >= _p.m_value; }\
                                                                                        inline DEnumClassName& operator++() { m_value =  static_cast<Enum>((Maze::S32)m_value + 1); return *this; }\
                                                                                        inline DEnumClassName& operator--() { m_value =  static_cast<Enum>((Maze::S32)m_value - 1); return *this; }\
                                                                                        inline DEnumClassName operator++(Maze::S32) { DEnumClassName result(*this); ++(*this); return result; }\
                                                                                        inline DEnumClassName operator--(Maze::S32) { DEnumClassName result(*this); --(*this); return result; }\
                                                                                        inline DEnumClassName& operator=(DEnumClassName const& _p) { m_value = _p.m_value; return *this; }\
                                                                                        inline DEnumClassName& operator=(Enum _p) { m_value = _p; return *this; }\
                                                                                        inline static Maze::CString const* GetStringEnum()\
                                                                                        {\
                                                                                            static Maze::CString const s_stringEnum[] = { "None", DEnumBlock2 };\
                                                                                            return (Maze::CString*)s_stringEnum;\
                                                                                        }\
                                                                                        \
                                                                                        static StdString ToStdString(Enum _value);\
                                                                                        static String ToString(Enum _value);\
                                                                                        static CString ToCString(Enum _value);\
                                                                                        static Enum FromString(StdString const& _value);\
                                                                                        static Enum FromString(String const& _value);\
                                                                                        static Enum FromString(CString _value);\
                                                                                        \
                                                                                        static DEnumClassName Random();\
                                                                                        static Vector<DEnumClassName> All();\
                                                                                        static Vector<S32> AllS32();\
                                                                                        \
                                                                                        static Vector<String> AllStrings();\
                                                                                        static Vector<String> AllStringsWithNone();\
                                                                                        \
                                                                                    public:\
                                                                                        virtual Vector<String> getAllValuesVectorString() const MAZE_OVERRIDE;\
                                                                                        virtual Vector<S32> getAllValuesVectorS32() const MAZE_OVERRIDE;\
                                                                                        virtual S32 asS32() const MAZE_OVERRIDE;\
                                                                                        virtual void set(S32 _value) MAZE_OVERRIDE;\
                                                                                        \
                                                                                    public:\
                                                                                        virtual Maze::String toString() const MAZE_OVERRIDE;\
                                                                                        virtual void setString(Maze::CString _data, Maze::Size _count) MAZE_OVERRIDE;\
                                                                                        virtual Maze::U32 getValueSerializationSize() const MAZE_OVERRIDE;\
                                                                                        virtual void serialize(Maze::U8* _data) const MAZE_OVERRIDE;\
                                                                                        virtual void deserialize(Maze::U8 const* _data) MAZE_OVERRIDE;\
                                                                                    \
                                                                                    private:\
                                                                                        Enum m_value;\
                                                                                    };

    #define MAZE_IMPLEMENT_ENUMCLASS(DEnumClassName)                                \
                                                                                    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(DEnumClassName, EnumClass);\
                                                                                    DEnumClassName::DEnumClassName()\
                                                                                        : m_value(None)\
                                                                                    {};\
                                                                                    DEnumClassName::DEnumClassName(DEnumClassName::Enum _value)\
                                                                                        : m_value(_value)\
                                                                                    {};\
                                                                                    DEnumClassName::DEnumClassName(Maze::U16 _value)\
                                                                                        : m_value(Enum(_value))\
                                                                                    {};\
                                                                                    DEnumClassName::DEnumClassName(Maze::S32 _value)\
                                                                                        : m_value(Enum(_value))\
                                                                                    {};\
                                                                                    DEnumClassName::DEnumClassName(Maze::U32 _value)\
                                                                                        : m_value(Enum(_value))\
                                                                                    {};\
                                                                                    \
                                                                                    Maze::StdString DEnumClassName::toStdString() const { return DEnumClassName::ToStdString(m_value); }\
                                                                                    Maze::CString DEnumClassName::toCString() const { return DEnumClassName::ToCString(m_value); }\
                                                                                    void DEnumClassName::setString(Maze::String const& _string) { m_value = DEnumClassName::FromString(_string); }\
                                                                                    void DEnumClassName::setString(Maze::StdString const& _string) { m_value = DEnumClassName::FromString(_string); }\
                                                                                    \
                                                                                    DEnumClassName::Enum DEnumClassName::toEnum() const { return m_value; }\
                                                                                    \
                                                                                    StdString DEnumClassName::ToStdString(DEnumClassName::Enum _value)\
                                                                                    {\
                                                                                        return StdString(DEnumClassName::GetStringEnum()[_value]);\
                                                                                    }\
                                                                                    \
                                                                                    String DEnumClassName::ToString(DEnumClassName::Enum _value)\
                                                                                    {\
                                                                                        return String(DEnumClassName::GetStringEnum()[_value]);\
                                                                                    }\
                                                                                    CString DEnumClassName::ToCString(DEnumClassName::Enum _value)\
                                                                                    {\
                                                                                        return DEnumClassName::GetStringEnum()[_value];\
                                                                                    }\
                                                                                    DEnumClassName::Enum DEnumClassName::FromString(Maze::StdString const& _value)\
                                                                                    {\
                                                                                        for (DEnumClassName e = DEnumClassName(1); e < DEnumClassName::MAX; ++e)\
                                                                                            if (strcmp(DEnumClassName::GetStringEnum()[e], _value.c_str()) == 0)\
                                                                                                return e;\
                                                                                        return DEnumClassName::None;\
                                                                                    }\
                                                                                    DEnumClassName::Enum DEnumClassName::FromString(Maze::String const& _value)\
                                                                                    {\
                                                                                        for (DEnumClassName e = DEnumClassName(1); e < DEnumClassName::MAX; ++e)\
                                                                                            if (strcmp(DEnumClassName::GetStringEnum()[e], _value.c_str()) == 0)\
                                                                                                return e;\
                                                                                        return DEnumClassName::None;\
                                                                                    }\
                                                                                    DEnumClassName::Enum DEnumClassName::FromString(Maze::CString _value)\
                                                                                    {\
                                                                                        for (DEnumClassName e = DEnumClassName(1); e < DEnumClassName::MAX; ++e)\
                                                                                            if (strcmp(DEnumClassName::GetStringEnum()[e], _value) == 0)\
                                                                                                return e;\
                                                                                        return DEnumClassName::None;\
                                                                                    }\
                                                                                    \
                                                                                    DEnumClassName DEnumClassName::Random() { return DEnumClassName(rand()%(MAX - 1) + 1); }\
                                                                                    Vector<DEnumClassName> DEnumClassName::All()\
                                                                                    {\
                                                                                        Vector<DEnumClassName> result;\
                                                                                        for (DEnumClassName e = DEnumClassName(1); e < DEnumClassName::MAX; ++e)\
                                                                                            result.push_back(e);\
                                                                                        return result;\
                                                                                    }\
                                                                                    Vector<S32> DEnumClassName::AllS32()\
                                                                                    {\
                                                                                        Vector<S32> result;\
                                                                                        for (DEnumClassName e = DEnumClassName(1); e < DEnumClassName::MAX; ++e)\
                                                                                            result.push_back(e);\
                                                                                        return result;\
                                                                                    }\
                                                                                    Vector<String> DEnumClassName::AllStrings()\
                                                                                    {\
                                                                                        Vector<String> result;\
                                                                                        for (DEnumClassName e = DEnumClassName(1); e < DEnumClassName::MAX; ++e)\
                                                                                            result.push_back(ToString(e));\
                                                                                        return result;\
                                                                                    }\
                                                                                    Vector<String> DEnumClassName::AllStringsWithNone()\
                                                                                    {\
                                                                                        Vector<String> result;\
                                                                                        for (DEnumClassName e = DEnumClassName(0); e < DEnumClassName::MAX; ++e)\
                                                                                            result.push_back(ToString(e));\
                                                                                        return result;\
                                                                                    }\
                                                                                    Vector<String> DEnumClassName::getAllValuesVectorString() const { return AllStrings(); }\
                                                                                    Vector<S32> DEnumClassName::getAllValuesVectorS32() const { return AllS32(); }\
                                                                                    S32 DEnumClassName::asS32() const { return static_cast<S32>(m_value); }\
                                                                                    void DEnumClassName::set(S32 _value) { *this = static_cast<Enum>(_value); }\
                                                                                    Maze::String DEnumClassName::toString() const\
                                                                                    {\
                                                                                        return DEnumClassName::ToString(m_value);\
                                                                                    }\
                                                                                    void DEnumClassName::setString(Maze::CString _data, Maze::Size _count)\
                                                                                    {\
                                                                                        m_value = DEnumClassName::FromString(_data);\
                                                                                    }\
                                                                                    Maze::U32 DEnumClassName::getValueSerializationSize() const\
                                                                                    {\
                                                                                        return sizeof(Maze::U16);\
                                                                                    }\
                                                                                    void DEnumClassName::serialize(Maze::U8* _data) const\
                                                                                    {\
                                                                                        Maze::U16 valueU16 = static_cast<Maze::U16>(m_value);\
                                                                                        memcpy(_data, (Maze::U8 const*)(&valueU16), sizeof(Maze::U16));\
                                                                                    }\
                                                                                    void DEnumClassName::deserialize(Maze::U8 const* _data)\
                                                                                    {\
                                                                                        Maze::U16 valueU16;\
                                                                                        memcpy((Maze::U8*)(&valueU16), _data, sizeof(Maze::U16));\
                                                                                        m_value = (DEnumClassName)valueU16;\
                                                                                    }


    //////////////////////////////////////////
    #define MAZE_DECLARE_AND_IMPLEMENT_ENUMCLASS_PROPERTY_CONVERTER(DEnumClassName)\
    template <>\
    inline void MetaPropertyToString<DEnumClassName>(DEnumClassName const& _value, Maze::String& _data) { _data = DEnumClassName::ToString(_value); }\
    template <>\
    inline void MetaPropertyFromString<DEnumClassName>(DEnumClassName& _value, Maze::CString _data) { _value = DEnumClassName::FromString(_data); }\
    template <>\
    inline U32 GetMetaPropertySerializationSize<DEnumClassName>(DEnumClassName const& _value) { return sizeof(Maze::U16); }\
    template <>\
    inline void SerializeMetaProperty<DEnumClassName>(DEnumClassName const& _value, Maze::U8* _data)\
    {\
        Maze::U16 valueU16 = static_cast<Maze::U16>(_value.toEnum());\
        memcpy(_data, (Maze::U8 const*)(&valueU16), GetMetaPropertySerializationSize(_value));\
    }\
    template <>\
    inline void DeserializeMetaProperty<DEnumClassName>(DEnumClassName& _value, Maze::U8 const* _data)\
    {\
        Maze::U16 valueU16;\
        memcpy((Maze::U8*)(&valueU16), _data, GetMetaPropertySerializationSize(_value));\
        _value = (DEnumClassName)valueU16;\
    }

    //////////////////////////////////////////
    #define MAZE_IMPLEMENT_ENUMCLASS_SERIALIZATION(DEnumClass) \
        inline void ValueToString(DEnumClass const& _value, String& _data) \
        { \
            _data = DEnumClass::ToString(_value); \
        } \
        inline void ValueFromString(DEnumClass& _value, CString _data, Size _count) \
        { \
            _value = DEnumClass::FromString(String(_data, _count)); \
        } \
        inline U32 GetValueSerializationSize(DEnumClass const& _value) \
        { \
            return sizeof(DEnumClass); \
        } \
        inline void SerializeValue(DEnumClass const& _value, U8* _data) \
        { \
            memcpy(_data, (U8 const*)(&_value), sizeof(DEnumClass)); \
        } \
        inline void DeserializeValue(DEnumClass& _value, U8 const* _data) \
        { \
            memcpy((U8*)&_value, _data, sizeof(DEnumClass)); \
        }


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_1_BlOCK1(e1) e1,
    #define MAZE_DECLARE_ENUMCLASS_1_BlOCK2(e1) #e1
    #define MAZE_DECLARE_ENUMCLASS_1(name, e1) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_1_BlOCK1(e1), MAZE_DECLARE_ENUMCLASS_1_BlOCK2(e1))
    #define MAZE_DECLARE_ENUMCLASS_1_API(api, name, e1) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_1_BlOCK1(e1), MAZE_DECLARE_ENUMCLASS_1_BlOCK2(e1))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_2_BlOCK1(e1, e2) e1, e2,
    #define MAZE_DECLARE_ENUMCLASS_2_BlOCK2(e1, e2) #e1, #e2
    #define MAZE_DECLARE_ENUMCLASS_2(name, e1, e2) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_2_BlOCK1(e1, e2), MAZE_DECLARE_ENUMCLASS_2_BlOCK2(e1, e2))
    #define MAZE_DECLARE_ENUMCLASS_2_API(api, name, e1, e2) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_2_BlOCK1(e1, e2), MAZE_DECLARE_ENUMCLASS_2_BlOCK2(e1, e2))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_3_BlOCK1(e1, e2, e3) e1, e2, e3,
    #define MAZE_DECLARE_ENUMCLASS_3_BlOCK2(e1, e2, e3) #e1, #e2, #e3
    #define MAZE_DECLARE_ENUMCLASS_3(name, e1, e2, e3) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_3_BlOCK1(e1, e2, e3), MAZE_DECLARE_ENUMCLASS_3_BlOCK2(e1, e2, e3))
    #define MAZE_DECLARE_ENUMCLASS_3_API(api, name, e1, e2, e3) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_3_BlOCK1(e1, e2, e3), MAZE_DECLARE_ENUMCLASS_3_BlOCK2(e1, e2, e3))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_4_BlOCK1(e1, e2, e3, e4) e1, e2, e3, e4,
    #define MAZE_DECLARE_ENUMCLASS_4_BlOCK2(e1, e2, e3, e4) #e1, #e2, #e3, #e4
    #define MAZE_DECLARE_ENUMCLASS_4(name, e1, e2, e3, e4) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_4_BlOCK1(e1, e2, e3, e4), MAZE_DECLARE_ENUMCLASS_4_BlOCK2(e1, e2, e3, e4))
    #define MAZE_DECLARE_ENUMCLASS_4_API(api, name, e1, e2, e3, e4) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_4_BlOCK1(e1, e2, e3, e4), MAZE_DECLARE_ENUMCLASS_4_BlOCK2(e1, e2, e3, e4))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_5_BlOCK1(e1, e2, e3, e4, e5) e1, e2, e3, e4, e5,
    #define MAZE_DECLARE_ENUMCLASS_5_BlOCK2(e1, e2, e3, e4, e5) #e1, #e2, #e3, #e4, #e5
    #define MAZE_DECLARE_ENUMCLASS_5(name, e1, e2, e3, e4, e5) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_5_BlOCK1(e1, e2, e3, e4, e5), MAZE_DECLARE_ENUMCLASS_5_BlOCK2(e1, e2, e3, e4, e5))
    #define MAZE_DECLARE_ENUMCLASS_5_API(api, name, e1, e2, e3, e4, e5) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_5_BlOCK1(e1, e2, e3, e4, e5), MAZE_DECLARE_ENUMCLASS_5_BlOCK2(e1, e2, e3, e4, e5))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_6_BlOCK1(e1, e2, e3, e4, e5, e6) e1, e2, e3, e4, e5, e6,
    #define MAZE_DECLARE_ENUMCLASS_6_BlOCK2(e1, e2, e3, e4, e5, e6) #e1, #e2, #e3, #e4, #e5, #e6
    #define MAZE_DECLARE_ENUMCLASS_6(name, e1, e2, e3, e4, e5, e6) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_6_BlOCK1(e1, e2, e3, e4, e5, e6), MAZE_DECLARE_ENUMCLASS_6_BlOCK2(e1, e2, e3, e4, e5, e6))
    #define MAZE_DECLARE_ENUMCLASS_6_API(api, name, e1, e2, e3, e4, e5, e6) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_6_BlOCK1(e1, e2, e3, e4, e5, e6), MAZE_DECLARE_ENUMCLASS_6_BlOCK2(e1, e2, e3, e4, e5, e6))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_7_BlOCK1(e1, e2, e3, e4, e5, e6, e7) e1, e2, e3, e4, e5, e6, e7,
    #define MAZE_DECLARE_ENUMCLASS_7_BlOCK2(e1, e2, e3, e4, e5, e6, e7) #e1, #e2, #e3, #e4, #e5, #e6, #e7
    #define MAZE_DECLARE_ENUMCLASS_7(name, e1, e2, e3, e4, e5, e6, e7) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_7_BlOCK1(e1, e2, e3, e4, e5, e6, e7), MAZE_DECLARE_ENUMCLASS_7_BlOCK2(e1, e2, e3, e4, e5, e6, e7))
    #define MAZE_DECLARE_ENUMCLASS_7_API(api, name, e1, e2, e3, e4, e5, e6, e7) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_7_BlOCK1(e1, e2, e3, e4, e5, e6, e7), MAZE_DECLARE_ENUMCLASS_7_BlOCK2(e1, e2, e3, e4, e5, e6, e7))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_8_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8) e1, e2, e3, e4, e5, e6, e7, e8,
    #define MAZE_DECLARE_ENUMCLASS_8_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8
    #define MAZE_DECLARE_ENUMCLASS_8(name, e1, e2, e3, e4, e5, e6, e7, e8) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_8_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8), MAZE_DECLARE_ENUMCLASS_8_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8))
    #define MAZE_DECLARE_ENUMCLASS_8_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_8_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8), MAZE_DECLARE_ENUMCLASS_8_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_9_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9) e1, e2, e3, e4, e5, e6, e7, e8, e9,
    #define MAZE_DECLARE_ENUMCLASS_9_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9
    #define MAZE_DECLARE_ENUMCLASS_9(name, e1, e2, e3, e4, e5, e6, e7, e8, e9) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_9_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9), MAZE_DECLARE_ENUMCLASS_9_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9))
    #define MAZE_DECLARE_ENUMCLASS_9_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_9_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9), MAZE_DECLARE_ENUMCLASS_9_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_10_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10,
    #define MAZE_DECLARE_ENUMCLASS_10_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10
    #define MAZE_DECLARE_ENUMCLASS_10(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_10_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10), MAZE_DECLARE_ENUMCLASS_10_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10))
    #define MAZE_DECLARE_ENUMCLASS_10_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_10_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10), MAZE_DECLARE_ENUMCLASS_10_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_11_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11,
    #define MAZE_DECLARE_ENUMCLASS_11_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11
    #define MAZE_DECLARE_ENUMCLASS_11(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_11_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11), MAZE_DECLARE_ENUMCLASS_11_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11))
    #define MAZE_DECLARE_ENUMCLASS_11_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_11_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11), MAZE_DECLARE_ENUMCLASS_11_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_12_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12,
    #define MAZE_DECLARE_ENUMCLASS_12_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12
    #define MAZE_DECLARE_ENUMCLASS_12(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_12_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12), MAZE_DECLARE_ENUMCLASS_12_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12))
    #define MAZE_DECLARE_ENUMCLASS_12_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_12_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12), MAZE_DECLARE_ENUMCLASS_12_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_13_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
    #define MAZE_DECLARE_ENUMCLASS_13_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13
    #define MAZE_DECLARE_ENUMCLASS_13(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_13_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13), MAZE_DECLARE_ENUMCLASS_13_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13))
    #define MAZE_DECLARE_ENUMCLASS_13_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_13_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13), MAZE_DECLARE_ENUMCLASS_13_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_14_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14,
    #define MAZE_DECLARE_ENUMCLASS_14_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14
    #define MAZE_DECLARE_ENUMCLASS_14(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_14_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14), MAZE_DECLARE_ENUMCLASS_14_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14))
    #define MAZE_DECLARE_ENUMCLASS_14_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_14_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14), MAZE_DECLARE_ENUMCLASS_14_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_15_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
    #define MAZE_DECLARE_ENUMCLASS_15_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15
    #define MAZE_DECLARE_ENUMCLASS_15(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_15_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15), MAZE_DECLARE_ENUMCLASS_15_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15))
    #define MAZE_DECLARE_ENUMCLASS_15_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_15_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15), MAZE_DECLARE_ENUMCLASS_15_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_16_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16,
    #define MAZE_DECLARE_ENUMCLASS_16_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16
    #define MAZE_DECLARE_ENUMCLASS_16(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_16_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16), MAZE_DECLARE_ENUMCLASS_16_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16))
    #define MAZE_DECLARE_ENUMCLASS_16_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_16_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16), MAZE_DECLARE_ENUMCLASS_16_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_17_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17,
    #define MAZE_DECLARE_ENUMCLASS_17_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17
    #define MAZE_DECLARE_ENUMCLASS_17(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_17_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17), MAZE_DECLARE_ENUMCLASS_17_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17))
    #define MAZE_DECLARE_ENUMCLASS_17_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_17_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17), MAZE_DECLARE_ENUMCLASS_17_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17))



    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_18_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18,
    #define MAZE_DECLARE_ENUMCLASS_18_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18
    #define MAZE_DECLARE_ENUMCLASS_18(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_18_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18), MAZE_DECLARE_ENUMCLASS_18_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18))
    #define MAZE_DECLARE_ENUMCLASS_18_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_18_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18), MAZE_DECLARE_ENUMCLASS_18_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18))



    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_19_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19,
    #define MAZE_DECLARE_ENUMCLASS_19_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19
    #define MAZE_DECLARE_ENUMCLASS_19(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_19_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19), MAZE_DECLARE_ENUMCLASS_19_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19))
    #define MAZE_DECLARE_ENUMCLASS_19_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_19_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19), MAZE_DECLARE_ENUMCLASS_19_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_20_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20,
    #define MAZE_DECLARE_ENUMCLASS_20_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20
    #define MAZE_DECLARE_ENUMCLASS_20(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_20_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20), MAZE_DECLARE_ENUMCLASS_20_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20))
    #define MAZE_DECLARE_ENUMCLASS_20_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_20_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20), MAZE_DECLARE_ENUMCLASS_20_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_21_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21,
    #define MAZE_DECLARE_ENUMCLASS_21_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21
    #define MAZE_DECLARE_ENUMCLASS_21(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_21_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21), MAZE_DECLARE_ENUMCLASS_21_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21))
    #define MAZE_DECLARE_ENUMCLASS_21_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_21_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21), MAZE_DECLARE_ENUMCLASS_21_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_22_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22,
    #define MAZE_DECLARE_ENUMCLASS_22_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22
    #define MAZE_DECLARE_ENUMCLASS_22(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_22_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22), MAZE_DECLARE_ENUMCLASS_22_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22))
    #define MAZE_DECLARE_ENUMCLASS_22_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_22_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22), MAZE_DECLARE_ENUMCLASS_22_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_23_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23,
    #define MAZE_DECLARE_ENUMCLASS_23_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22, #e23
    #define MAZE_DECLARE_ENUMCLASS_23(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_23_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23), MAZE_DECLARE_ENUMCLASS_23_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23))
    #define MAZE_DECLARE_ENUMCLASS_23_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_23_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23), MAZE_DECLARE_ENUMCLASS_23_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_24_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24,
    #define MAZE_DECLARE_ENUMCLASS_24_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22, #e23, #e24
    #define MAZE_DECLARE_ENUMCLASS_24(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_24_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24), MAZE_DECLARE_ENUMCLASS_24_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24))
    #define MAZE_DECLARE_ENUMCLASS_24_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_24_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24), MAZE_DECLARE_ENUMCLASS_24_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_25_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
    #define MAZE_DECLARE_ENUMCLASS_25_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22, #e23, #e24, #e25
    #define MAZE_DECLARE_ENUMCLASS_25(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_25_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25), MAZE_DECLARE_ENUMCLASS_25_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25))
    #define MAZE_DECLARE_ENUMCLASS_25_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_25_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25), MAZE_DECLARE_ENUMCLASS_25_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_26_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26,
    #define MAZE_DECLARE_ENUMCLASS_26_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22, #e23, #e24, #e25, #e26
    #define MAZE_DECLARE_ENUMCLASS_26(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_26_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26), MAZE_DECLARE_ENUMCLASS_26_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26))
    #define MAZE_DECLARE_ENUMCLASS_26_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_26_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26), MAZE_DECLARE_ENUMCLASS_26_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_27_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27,
    #define MAZE_DECLARE_ENUMCLASS_27_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22, #e23, #e24, #e25, #e26, #e27
    #define MAZE_DECLARE_ENUMCLASS_27(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_27_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27), MAZE_DECLARE_ENUMCLASS_27_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27))
    #define MAZE_DECLARE_ENUMCLASS_27_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_27_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27), MAZE_DECLARE_ENUMCLASS_27_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_28_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
    #define MAZE_DECLARE_ENUMCLASS_28_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22, #e23, #e24, #e25, #e26, #e27, #e28
    #define MAZE_DECLARE_ENUMCLASS_28(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_28_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28), MAZE_DECLARE_ENUMCLASS_28_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28))
    #define MAZE_DECLARE_ENUMCLASS_28_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_28_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28), MAZE_DECLARE_ENUMCLASS_28_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_29_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29,
    #define MAZE_DECLARE_ENUMCLASS_29_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22, #e23, #e24, #e25, #e26, #e27, #e28, #e29
    #define MAZE_DECLARE_ENUMCLASS_29(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_29_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29), MAZE_DECLARE_ENUMCLASS_29_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29))
    #define MAZE_DECLARE_ENUMCLASS_29_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_29_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29), MAZE_DECLARE_ENUMCLASS_29_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_30_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30,
    #define MAZE_DECLARE_ENUMCLASS_30_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22, #e23, #e24, #e25, #e26, #e27, #e28, #e29, #e30
    #define MAZE_DECLARE_ENUMCLASS_30(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_30_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30), MAZE_DECLARE_ENUMCLASS_30_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30))
    #define MAZE_DECLARE_ENUMCLASS_30_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_30_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30), MAZE_DECLARE_ENUMCLASS_30_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_31_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31,
    #define MAZE_DECLARE_ENUMCLASS_31_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22, #e23, #e24, #e25, #e26, #e27, #e28, #e29, #e30, #e31
    #define MAZE_DECLARE_ENUMCLASS_31(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_31_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31), MAZE_DECLARE_ENUMCLASS_31_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31))
    #define MAZE_DECLARE_ENUMCLASS_31_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_31_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31), MAZE_DECLARE_ENUMCLASS_31_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_32_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32,
    #define MAZE_DECLARE_ENUMCLASS_32_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22, #e23, #e24, #e25, #e26, #e27, #e28, #e29, #e30, #e31, #e32
    #define MAZE_DECLARE_ENUMCLASS_32(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_32_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32), MAZE_DECLARE_ENUMCLASS_32_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32))
    #define MAZE_DECLARE_ENUMCLASS_32_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_32_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32), MAZE_DECLARE_ENUMCLASS_32_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_33_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33,
    #define MAZE_DECLARE_ENUMCLASS_33_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22, #e23, #e24, #e25, #e26, #e27, #e28, #e29, #e30, #e31, #e32, #e33
    #define MAZE_DECLARE_ENUMCLASS_33(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_33_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33), MAZE_DECLARE_ENUMCLASS_33_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33))
    #define MAZE_DECLARE_ENUMCLASS_33_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_33_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33), MAZE_DECLARE_ENUMCLASS_33_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33))


    //////////////////////////////////////////
    #define MAZE_DECLARE_ENUMCLASS_34_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34) e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34,
    #define MAZE_DECLARE_ENUMCLASS_34_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34) #e1, #e2, #e3, #e4, #e5, #e6, #e7, #e8, #e9, #e10, #e11, #e12, #e13, #e14, #e15, #e16, #e17, #e18, #e19, #e20, #e21, #e22, #e23, #e24, #e25, #e26, #e27, #e28, #e29, #e30, #e31, #e32, #e33, #e34
    #define MAZE_DECLARE_ENUMCLASS_34(name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34) MAZE_DECLARE_ENUMCLASS_BASE(, name, MAZE_DECLARE_ENUMCLASS_34_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34), MAZE_DECLARE_ENUMCLASS_34_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34))
    #define MAZE_DECLARE_ENUMCLASS_34_API(api, name, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34) MAZE_DECLARE_ENUMCLASS_BASE(api, name, MAZE_DECLARE_ENUMCLASS_34_BlOCK1(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34), MAZE_DECLARE_ENUMCLASS_34_BlOCK2(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34))



    //////////////////////////////////////////
    //#define MAZE_DECLARE_ENUMCLASS_MACRO_CHOOSE_FROM_ARG_COUNT(arg_, ...) MAZE_MACRO_FUNC_RECOMPOSER16((__VA_ARGS__, , , , , , arg_##_10, arg_##_9, arg_##_8, arg_##_7, arg_##_6, arg_##_5, arg_##_4, arg_##_3, arg_##_2, arg_##_1, ,))
    //#define MAZE_DECLARE_ENUMCLASS(...) MAZE_DECLARE_ENUMCLASS_MACRO_CHOOSE_FROM_ARG_COUNT(MAZE_DECLARE_ENUMCLASS, __VA_ARGS__)(__VA_ARGS__)


} // Namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeEnumClass_h_
