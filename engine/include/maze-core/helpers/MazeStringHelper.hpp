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
#if (!defined(_MazeStringHelper_hpp_))
#define _MazeStringHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/data/MazeSpan.hpp"
#include <cstring>
#include <cctype>
#include <cstdio>
#include <cstdarg>
#include <fast_float/fast_float.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace StringHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(Char* _buffer, Size _buflen, CString _format, va_list _args);

        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(WChar* _buffer, Size _buflen, CWString _format, va_list _args);


        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(Char* _buffer, Size _buflen, CString _format, ...);

        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(WChar* _buffer, Size _buflen, CWString _format, ...);

        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(String& _buffer, CString _format, ...);

        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(WString& _buffer, CWString _format, ...);


        //////////////////////////////////////////
        MAZE_CORE_API S32 FormattedStringSize(CString _format, va_list _args);

        //////////////////////////////////////////
        MAZE_CORE_API S32 FormattedStringSize(CWString _format, va_list _args);


        //////////////////////////////////////////
        MAZE_CORE_API S32 FormattedStringSize(CString _format, ...);

        //////////////////////////////////////////
        MAZE_CORE_API S32 FormattedStringSize(CWString _format, ...);


        //////////////////////////////////////////
        inline void FormatVAString(CString _text, va_list& _arg, StdString& _destString) noexcept
        {
            S32 requiredSize = FormattedStringSize(_text, _arg);
            if (requiredSize > 0)
            {
                _destString.resize(requiredSize);
                FormatString(&_destString[0], (Size)requiredSize + 1, _text, _arg);
            }
            else
            if (requiredSize == 0)
            {
                _destString.clear();
            }
            else
            {
                MAZE_ERROR("Invalid required size for text '%ls'! requiredSize='%d'", _text, requiredSize);
            }
        }

        //////////////////////////////////////////
        inline void FormatVAString(CString _text, va_list& _arg, String& _destString) noexcept
        {
            S32 requiredSize = FormattedStringSize(_text, _arg);
            if (requiredSize > 0)
            {
                _destString.resize(requiredSize);
                FormatString(&_destString[0], (Size)requiredSize + 1, _text, _arg);
            }
            else
            if (requiredSize == 0)
            {
                _destString.clear();
            }
            else
            {
                MAZE_ERROR("Invalid required size for text '%ls'! requiredSize='%d'", _text, requiredSize);
            }
        }

        //////////////////////////////////////////
        inline void FormatVAString(CWString _text, va_list& _arg, StdWString& _destString) noexcept
        {
            S32 requiredSize = FormattedStringSize(_text, _arg);
            if (requiredSize > 0)
            {
                _destString.resize(requiredSize);
                FormatString(&_destString[0], (Size)requiredSize + 1, _text, _arg);
            }
            else
            if (requiredSize == 0)
            {
                _destString.clear();
            }
            else
            {
                MAZE_ERROR("Invalid required size for text '%ls'! requiredSize='%d'", _text, requiredSize);
            }
        }

        //////////////////////////////////////////
        inline void FormatVAString(CWString _text, va_list& _arg, WString& _destString) noexcept
        {
            S32 requiredSize = FormattedStringSize(_text, _arg);
            if (requiredSize > 0)
            {
                _destString.resize(requiredSize);
                FormatString(&_destString[0], (Size)requiredSize + 1, _text, _arg);
            }
            else
            if (requiredSize == 0)
            {
                _destString.clear();
            }
            else
            {
                MAZE_ERROR("Invalid required size for text '%ls'! requiredSize='%d'", _text, requiredSize);
            }
        }

        //////////////////////////////////////////
        template <
            class TElem,
            class TValue>
        inline TElem* UIntegralToBuff(TElem* _rnext, TValue _value)
        {
            #pragma warning(push)
            #pragma warning(disable: 4127) // Conditional expression is constant

            if (sizeof(TValue) > 4)
            {
                // For 64-bit numbers, work in chunks to avoid 64-bit divisions.
                while (_value > 0xFFFFFFFFU)
                {
                    auto value_chunk = static_cast<U32>(_value % 1000000000);
                    _value /= 1000000000;

                    for (S32 i = 0; i != 9; ++i)
                    {
                        *--_rnext = '0' + value_chunk % 10;
                        value_chunk /= 10;
                    }
                }
            }

            #pragma warning(pop)

            auto valueTrunc = static_cast<U32>(_value);

            do
            {
                *--_rnext = '0' + valueTrunc % 10;
                valueTrunc /= 10;
            }
            while (valueTrunc != 0);

            return _rnext;
        }


        //////////////////////////////////////////
        template <
            class TString,
            class TValue>
        inline TString IntegralToString(TValue const _value)
        {
            using TUValue = typename std::make_unsigned<TValue>::type;

            typename TString::value_type buff[21]; // -2^63 and 2^64 - 1, plus NUL
            typename TString::value_type* const buffEnd = std::end(buff);
            typename TString::value_type* rnext = buffEnd;

            auto uValue = static_cast<TUValue>(_value);

            if (_value < 0)
            {
                rnext = UIntegralToBuff(rnext, 0 - uValue);
                *--rnext = '-';
            }
            else
            {
                rnext = UIntegralToBuff(rnext, uValue);
            }

            return (TString(rnext, buffEnd));
        }

        //////////////////////////////////////////
        template <
            class TString,
            class TValue>
        inline TString FloatingToString(CString _format, TValue _value)
        {
            auto len = FormattedStringSize(_format, _value);
            TString str((Size)len + 1, '\0');
            FormatString(&str[0], (Size)len + 1, _format, _value);
            str.resize(len);
            return str;
        }

        //////////////////////////////////////////
        template <
            class TWString,
            class TValue>
        inline TWString FloatingToWString(CWString _format, TValue _value)
        {
            auto len = FormattedStringSize(_format, _value);
            TWString str((Size)len + 1, L'\0');
            FormatString(&str[0], (Size)len + 1, _format, _value);
            str.resize(len);
            return str;
        }

        //////////////////////////////////////////
        inline bool IsFloatNumber(CString _s, Size _size) noexcept
        {
            if (_s == 0 || _size == 0)
                return false;

            Char const* __restrict it = _s;
            Char const* __restrict end = _s + _size;

            if (it == end)
                return false;

            if (*it == '-')
                ++it;

            if (it == end)
                return false;

            S32 point = 1;
            return std::find_if(
                it,
                end,
                [&point](S8 _c)
                {
                    if (_c == '.' && point == 1)
                    {
                        point++;
                        return false;
                    }
                    return !std::isdigit(_c);
                }) == end;
        }

        //////////////////////////////////////////
        inline bool IsFloatNumber(CString _s) noexcept
        {
            if (_s == 0 || *_s == 0)
                return false;

            CString it = _s;

            if (*it == '-')
                ++it;

            S32 point = 1;
            for (; *it; it++)
            {
                if (*it == '.' && point == 1)
                {
                    point++;
                    continue;
                }
                if (!std::isdigit(*it))
                    return false;
            }
            return true;
        }

        //////////////////////////////////////////
        inline bool IsFloatNumber(StdString const& _s) noexcept
        {
            return IsFloatNumber(&_s[0], _s.size());
        }

        //////////////////////////////////////////
        inline bool IsFloatNumber(String const& _s) noexcept
        {
            return IsFloatNumber(&_s[0], _s.size());
        }

        //////////////////////////////////////////
        template <typename TIntType>
        inline CString ParseInteger(Char const* _str, Char const* _end, TIntType& _value)
        {
            if (!_str || !_end)
            {
                _value = TIntType(0);
                return nullptr;
            }

            bool isNegative = false;
            if (*_str == '-')
            {
                isNegative = true;
                ++_str;
            }
            else
            if (*_str == '+')
                ++_str;

            if (_str == _end || !std::isdigit(*_str))
            {
                _value = TIntType(0);
                return nullptr;
            }

            TIntType result = 0;

            // Hex
            if (_str[0] == '0' && !isNegative && (_end - _str) >= 2 && _str[1] == 'x')
            {
                for (_str += 2; _str != _end; ++_str)
                {
                    Char ch = *_str;
                    if (std::isdigit(ch))
                        ch = (ch - '0');
                    else
                    if (ch >= 'a' && ch <= 'f')
                        ch = (ch - ('a' - 10));
                    else
                    if (ch >= 'A' && ch <= 'F')
                        ch = (ch - ('A' - 10));
                    else
                        break;
                    result = (result << 4) | ch;
                }
                _value = result;
                return _str;
            }

            do
            {
                result *= 10;
                result += *_str - '0';
            }
            while (++_str != _end && std::isdigit(*_str));
            _value = isNegative ? TIntType(-(typename std::make_signed<TIntType>::type)(result)) : TIntType(result);
            return _str;
        }

        //////////////////////////////////////////
        template <typename TIntType>
        inline CString ParseInteger(Char const* __restrict _str, Size _size, TIntType& _value)
        {
            if (!_str || _size == 0)
            {
                _value = TIntType(0);
                return nullptr;
            }

            return ParseInteger<TIntType>(_str, (Char const* __restrict)(_str + _size), _value);
        }

        //////////////////////////////////////////
        inline CString SkipChar(Char const* _str, Char const* _end, Char _ch)
        {
            if (!_str)
                return nullptr;

            for (; _str != _end; ++_str)
                if (*_str != _ch)
                    break;

            return _str;
        }

        //////////////////////////////////////////
        inline CString SkipChar(Char const* __restrict _str, Size _size, Char _ch)
        {
            return _str && _size ? SkipChar(_str, (Char const* __restrict)(_str + _size), _ch) : _str;
        }

        //////////////////////////////////////////
        inline CString ExpectSkipChar(Char const* _str, Char const* _end, Char _ch, Size _count = 1)
        {
            if (!_count)
                return _str;

            if (!_str)
                return nullptr;

            for (; _str != _end && _count; ++_str)
                if (*_str == _ch)
                    --_count;
                else
                    return nullptr;

            return _count == 0 ? _str : nullptr;
        }


        //////////////////////////////////////////
        // Bool
        //
        //////////////////////////////////////////
        inline CString ParseBool(CString _str, CString _end, Bool& _value)
        {
            S8 temp = 0;
            _str = StringHelper::ParseInteger<S8>(_str, _end, temp);
            _value = (temp == 1);

            return _str;
        }

        //////////////////////////////////////////
        inline CString ParseBoolPretty(CString _str, CString _end, Bool& _value)
        {
            Size size = _end - _str;

            if (size >= 4 && MAZE_STRNICMP(_str, "true", 4) == 0)
            {
                _value = true;
                _str += 4;
            }
            else
            if (size >= 1 && MAZE_STRNICMP(_str, "1", 1) == 0)
            {
                _value = true;
                _str += 1;
            }
            else
            if (size >= 5 && MAZE_STRNICMP(_str, "false", 5) == 0)
            {
                _value = false;
                _str += 5;
            }
            else
            if (size >= 1 && MAZE_STRNICMP(_str, "0", 1) == 0)
            {
                _value = false;
                _str += 1;
            }

            return _str;
        }

        //////////////////////////////////////////
        inline bool StringToBool(CString _str) noexcept
        {
            return strcmp(_str, "1") == 0 ? true : false;
        }

        //////////////////////////////////////////
        inline bool StringToBool(String const& _str) noexcept
        {
            return (_str == "1");
        }


        //////////////////////////////////////////
        // S8
        //
        //////////////////////////////////////////
        inline CString ParseS8(Char const* _str, Size _size, S8& _value)
        {
            return ParseInteger<S8>(_str, std::min(_size, (Size)4u), _value);
        }

        //////////////////////////////////////////
        inline S8 StringToS8(CString _str) noexcept
        {
            return static_cast<S8>(std::atoi(_str));
        }

        //////////////////////////////////////////
        inline S8 StringToS8(StdString const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<S8>(std::stoi(_str));
        }

        //////////////////////////////////////////
        inline S8 StringToS8(String const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<S8>(std::stoi(_str.c_str()));
        }


        //////////////////////////////////////////
        // U8
        //
        //////////////////////////////////////////
        inline CString ParseU8(Char const* _str, Size _size, U8& _value)
        {
            return ParseInteger<U8>(_str, std::min(_size, (Size)4u), _value);
        }

        //////////////////////////////////////////
        inline U8 StringToU8(CString _str) noexcept
        {
            return static_cast<U8>(std::atoi(_str));
        }

        //////////////////////////////////////////
        inline U8 StringToU8(StdString const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<U8>(std::stoi(_str));
        }

        //////////////////////////////////////////
        inline U8 StringToU8(String const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<U8>(std::stoi(_str.c_str()));
        }


        //////////////////////////////////////////
        // S16
        //
        //////////////////////////////////////////
        inline CString ParseS16(Char const* _str, Size _size, S16& _value)
        {
            return ParseInteger<S16>(_str, std::min(_size, (Size)6u), _value);
        }

        //////////////////////////////////////////
        inline S16 StringToS16(CString _str) noexcept
        {
            return static_cast<S16>(std::atoi(_str));
        }

        //////////////////////////////////////////
        inline S16 StringToS16(StdString const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<S16>(std::stoi(_str));
        }

        //////////////////////////////////////////
        inline S16 StringToS16(String const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<S16>(std::stoi(_str.c_str()));
        }


        //////////////////////////////////////////
        // U16
        //
        //////////////////////////////////////////
        inline CString ParseU16(Char const* _str, Size _size, U16& _value)
        {
            return ParseInteger<U16>(_str, std::min(_size, (Size)6u), _value);
        }

        //////////////////////////////////////////
        inline U16 StringToU16(CString _str) noexcept
        {
            return static_cast<U16>(std::atoi(_str));
        }

        //////////////////////////////////////////
        inline U16 StringToU16(StdString const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<U16>(std::stoi(_str));
        }

        //////////////////////////////////////////
        inline U16 StringToU16(String const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<U16>(std::stoi(_str.c_str()));
        }


        //////////////////////////////////////////
        // S32
        //
        //////////////////////////////////////////
        inline CString ParseS32(Char const* _str, Size _size, S32& _value)
        {
            return ParseInteger<S32>(_str, std::min(_size, (Size)11u), _value);
        }

        //////////////////////////////////////////
        inline S32 StringToS32(CString _str) noexcept
        {
            return std::atoi(_str);
        }

        //////////////////////////////////////////
        inline S32 StringToS32(StdString const& _str) noexcept
        {
            return _str.empty() ? 0 : std::stoi(_str);
        }

        //////////////////////////////////////////
        inline S32 StringToS32(String const& _str) noexcept
        {
            return _str.empty() ? 0 : std::stoi(_str.c_str());
        }


        //////////////////////////////////////////
        // U32
        //
        //////////////////////////////////////////
        inline CString ParseU32(Char const* _str, Size _size, U32& _value)
        {
            return ParseInteger<U32>(_str, std::min(_size, (Size)11u), _value);
        }

        //////////////////////////////////////////
        inline U32 StringToU32(CString _str) noexcept
        {
            return static_cast<U32>(std::atoll(_str));
        }

        //////////////////////////////////////////
        inline U32 StringToU32(StdString const& _str) noexcept
        {
            return _str.empty() ? 0u : static_cast<U32>(std::stoll(_str));
        }

        //////////////////////////////////////////
        inline U32 StringToU32(String const& _str) noexcept
        {
            return _str.empty() ? 0u : static_cast<U32>(std::stoll(_str.c_str()));
        }


        //////////////////////////////////////////
        // S64
        //
        //////////////////////////////////////////
        inline CString ParseS64(Char const* _str, Size _size, S64& _value)
        {
            return ParseInteger<S64>(_str, std::min(_size, (Size)20u), _value);
        }

        //////////////////////////////////////////
        inline S64 StringToS64(String const& _str) noexcept
        {
            return _str.empty() ? 0 : static_cast<S64>(std::atoll(_str.c_str()));
        }


        //////////////////////////////////////////
        // U64
        //
        //////////////////////////////////////////
        inline CString ParseU64(Char const* _str, Size _size, U64& _value)
        {
            return ParseInteger<U64>(_str, std::min(_size, (Size)20u), _value);
        }

        //////////////////////////////////////////
        inline U64 StringToU64(CString _str) noexcept
        {
            return static_cast<U64>(std::strtoull(_str, nullptr, 10));
        }

        //////////////////////////////////////////
        inline U64 StringToU64(StdString const& _str) noexcept
        {
            return _str.empty() ? 0u : static_cast<U64>(std::stoull(_str));
        }

        //////////////////////////////////////////
        inline U64 StringToU64(String const& _str) noexcept
        {
            return _str.empty() ? 0u : static_cast<U64>(std::stoull(_str.c_str()));
        }


        //////////////////////////////////////////
        // F32
        //
        //////////////////////////////////////////
        inline CString ParseF32(CString _str, CString _end, F32& _value) noexcept
        {
            if (!_str || !_end)
            {
                _value = 0.0f;
                return nullptr;
            }

            auto result = fast_float::from_chars(_str, _end, _value);
            return result.ec == std::errc() ? result.ptr : nullptr;
        }

        //////////////////////////////////////////
        inline CString ParseF32(Char const* __restrict _str, Size _size, F32& _value) noexcept
        {
            if (!_str)
            {
                _value = 0.0f;
                return nullptr;
            }

            if (!_size)
            {
                _value = 0.0f;
                return _str;
            }

            return ParseF32(_str, (Char const* __restrict)(_str + _size), _value);
        }

        //////////////////////////////////////////
        inline F32 StringToF32(CString _str) noexcept
        {
            return _str ? static_cast<F32>(std::atof(_str)) : 0.0f;
        }

        //////////////////////////////////////////
        inline F32 StringToF32Safe(CString _str) noexcept
        {
            if (!IsFloatNumber(_str))
                return 0.0f;

            return static_cast<F32>(std::atof(_str));
        }

        //////////////////////////////////////////
        inline F32 StringToF32(CString _str, Size _size) noexcept
        {
            F32 value = 0.0f;
            fast_float::from_chars(_str, _str + _size, value);
            return value;
        }

        //////////////////////////////////////////
        inline F32 StringToF32(StdString const& _str) noexcept
        {
            return _str.empty() ? 0.0f : std::stof(_str);
        }

        //////////////////////////////////////////
        inline F32 StringToF32(String const& _str) noexcept
        {
            if (_str.empty())
                return 0.0f;

            return std::stof(_str.c_str());
        }

        //////////////////////////////////////////
        inline F32 StringToF32Safe(String const& _str) noexcept
        {
            if (_str.empty() || !IsFloatNumber(_str))
                return 0.0f;

            return std::stof(_str.c_str());
        }


        //////////////////////////////////////////
        // F64
        //
        //////////////////////////////////////////
        inline CString ParseF64(CString _str, CString _end, F64& _value) noexcept
        {
            if (!_str || !_end)
            {
                _value = 0.0;
                return nullptr;
            }

            auto result = fast_float::from_chars(_str, _end, _value);
            return result.ec == std::errc() ? result.ptr : nullptr;
        }

        //////////////////////////////////////////
        inline CString ParseF64(Char const* __restrict _str, Size _size, F64& _value) noexcept
        {
            if (!_str)
            {
                _value = 0.0;
                return nullptr;
            }

            if (!_size)
            {
                _value = 0.0;
                return _str;
            }

            return ParseF64(_str, (Char const* __restrict)(_str + _size), _value);
        }

        //////////////////////////////////////////
        inline F64 StringToF64(CString _str) noexcept
        {
            return _str ? std::atof(_str) : 0.0;
        }

        //////////////////////////////////////////
        inline F64 StringToF64Safe(CString _str) noexcept
        {
            if (!IsFloatNumber(_str))
                return 0.0;

            return std::atof(_str);
        }

        //////////////////////////////////////////
        inline F64 StringToF64(StdString const& _str) noexcept
        {
            return _str.empty() ? 0.0 : std::stod(_str);
        }

        //////////////////////////////////////////
        inline F64 StringToF64(String const& _str) noexcept
        {
            return std::stod(_str.c_str());
        }


        //////////////////////////////////////////
        // Bool
        //
        //////////////////////////////////////////
        inline StdString BoolToStdString(bool _value) noexcept
        {
            return _value ? StdString("1") : StdString("0");
        }

        ////////////////////////////////////
        inline String BoolToString(bool _value) noexcept
        {
            return _value ? String("1") : String("0");
        }

        //////////////////////////////////////////
        inline StdString ToStdString(bool _value) noexcept { return BoolToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(bool _value) noexcept { return BoolToString(_value); }



        //////////////////////////////////////////
        // CString
        //
        //////////////////////////////////////////
        inline StdString ToStdString(CString _value) noexcept { return _value ? StdString(_value)
                                                                              : StdString(); }

        //////////////////////////////////////////
        inline String ToString(CString _value) noexcept { return _value ? String(_value)
                                                                        : String(); }


        //////////////////////////////////////////
        // U8
        //
        //////////////////////////////////////////
        inline StdString U8ToStdString(U8 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String U8ToString(U8 _value) noexcept
        {
            return IntegralToString<String>((S32)_value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(U8 _value) noexcept { return U8ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(U8 _value) noexcept { return U8ToString(_value); }


        //////////////////////////////////////////
        // S32
        //
        //////////////////////////////////////////
        inline StdString S32ToStdString(S32 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String S32ToString(S32 _value) noexcept
        {
            return IntegralToString<String>(_value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(S32 _value) noexcept { return S32ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(S32 _value) noexcept { return S32ToString(_value); }


        //////////////////////////////////////////
        // U32
        //
        //////////////////////////////////////////
        inline StdString U32ToStdString(U32 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String U32ToString(U32 _value) noexcept
        {
            return IntegralToString<String>(_value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(U32 _value) noexcept { return U32ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(U32 _value) noexcept { return U32ToString(_value); }


        //////////////////////////////////////////
        // U64
        //
        //////////////////////////////////////////
        inline StdString U64ToStdString(U64 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String U64ToString(U64 _value) noexcept
        {
            return IntegralToString<String>(_value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(U64 _value) noexcept { return U64ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(U64 _value) noexcept { return U64ToString(_value); }


        //////////////////////////////////////////
        // S64
        //
        //////////////////////////////////////////
        inline StdString S64ToStdString(S64 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String S64ToString(S64 _value) noexcept
        {
            return IntegralToString<String>(_value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(S64 _value) noexcept { return S64ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(S64 _value) noexcept { return S64ToString(_value); }


        //////////////////////////////////////////
        // F32
        //
        //////////////////////////////////////////
        inline StdString F32ToStdString(F32 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String F32ToString(F32 _value) noexcept
        {
            return FloatingToString<String>("%f", _value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(F32 _value) noexcept { return F32ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(F32 _value) noexcept { return F32ToString(_value); }

        //////////////////////////////////////////
        inline StdString F32ToStdString(F32 _value, S32 _precision)
        {
            S32 s32 = S32(_value);
            if (_value == F32(s32))
                return S32ToStdString(s32);

            StdStringStream ss;
            ss << std::fixed << std::setprecision(_precision) << _value;
            return ss.str();
        }

        //////////////////////////////////////////
        inline String F32ToString(F32 _value, S32 _precision)
        {
            S32 s32 = S32(_value);
            if (_value == F32(s32))
                return S32ToString(s32);

            StringStream ss;
            ss << std::fixed << std::setprecision(_precision) << _value;
            return ss.str();
        }

        //////////////////////////////////////////
        inline StdString ToStdString(F32 _value, S32 _precision) noexcept { return F32ToStdString(_value, _precision); }

        //////////////////////////////////////////
        inline String ToString(F32 _value, S32 _precision) noexcept { return F32ToString(_value, _precision); }

        //////////////////////////////////////////
        inline StdString F32ToStdStringFormatted(F32 _value) noexcept
        {
            S8 buff[64];
            FormatString(buff, sizeof(buff), "%f", _value);
            StdString result(buff);
            if (result.find('.') != StdString::npos)
            {
                result.erase(result.find_last_not_of('0') + 1, StdString::npos);
                result.erase(result.find_last_not_of('.') + 1, StdString::npos);
            }
            return result;
        }

        //////////////////////////////////////////
        inline String F32ToStringFormatted(F32 _value) noexcept
        {
            S8 buff[64];
            FormatString(buff, sizeof(buff), "%f", _value);
            String result(buff);

            if (result.find('.') != String::npos)
            {
                result.erase(result.find_last_not_of('0') + 1, String::npos);
                result.erase(result.find_last_not_of('.') + 1, String::npos);
            }

            return result;
        }

        //////////////////////////////////////////
        inline StdString F32ToStdStringFormatted(F32 _value, S32 _precision) noexcept
        {
            StdString result = F32ToStdString(_value, _precision);

            if (result.find('.') != StdString::npos)
            {
                result.erase(result.find_last_not_of('0') + 1, StdString::npos);
                result.erase(result.find_last_not_of('.') + 1, StdString::npos);
            }

            return result;
        }

        //////////////////////////////////////////
        inline String F32ToStringFormatted(F32 _value, S32 _precision) noexcept
        {
            String result = F32ToString(_value, _precision);

            if (result.find('.') != String::npos)
            {
                result.erase(result.find_last_not_of('0') + 1, String::npos);
                result.erase(result.find_last_not_of('.') + 1, String::npos);
            }

            return result;
        }


        //////////////////////////////////////////
        // F64
        //
        //////////////////////////////////////////
        inline StdString F64ToStdString(F64 _value) noexcept
        {
            return std::to_string(_value);
        }

        //////////////////////////////////////////
        inline String F64ToString(F64 _value) noexcept
        {
            return FloatingToString<String>("%f", _value);
        }

        //////////////////////////////////////////
        inline StdString ToStdString(F64 _value) noexcept { return F64ToStdString(_value); }

        //////////////////////////////////////////
        inline String ToString(F64 _value) noexcept { return F64ToString(_value); }

        //////////////////////////////////////////
        inline StdString F64ToStdString(F64 _value, S32 _precision)
        {
            S64 s64 = S64(_value);
            if (_value == F64(s64))
                return S64ToStdString(s64);

            StdStringStream ss;
            ss << std::fixed << std::setprecision(_precision) << _value;
            return ss.str();
        }

        //////////////////////////////////////////
        inline String F64ToString(F64 _value, S32 _precision)
        {
            S64 s64 = S64(_value);
            if (_value == F64(s64))
                return S64ToString(s64);

            StringStream ss;
            ss << std::fixed << std::setprecision(_precision) << _value;
            return ss.str();
        }

        //////////////////////////////////////////
        inline StdString F64ToStdStringFormatted(F64 _value) noexcept
        {
            S8 buff[64];
            FormatString(buff, sizeof(buff), "%9.9f", _value);
            StdString result(buff);
            result.erase(result.find_last_not_of('0') + 1, StdString::npos);
            result.erase(result.find_last_not_of('.') + 1, StdString::npos);
            return result;
        }

        //////////////////////////////////////////
        inline String F64ToStringFormatted(F64 _value) noexcept
        {
            S8 buff[64];
            FormatString(buff, sizeof(buff), "%9.9f", _value);
            String result(buff);
            result.erase(result.find_last_not_of('0') + 1, String::npos);
            result.erase(result.find_last_not_of('.') + 1, String::npos);
            return result;
        }

        //////////////////////////////////////////
        inline StdString F64ToStdStringFormatted(F64 _value, S32 _precision) noexcept
        {
            StdString result = F64ToStdString(_value, _precision);
            if (result.find('.') != StdString::npos)
            {
                result.erase(result.find_last_not_of('0') + 1, StdString::npos);
                result.erase(result.find_last_not_of('.') + 1, StdString::npos);
            }

            return result;
        }

        //////////////////////////////////////////
        inline String F64ToStringFormatted(F64 _value, S32 _precision) noexcept
        {
            String result = F64ToString(_value, _precision);
            if (result.find('.') != String::npos)
            {
                result.erase(result.find_last_not_of('0') + 1, String::npos);
                result.erase(result.find_last_not_of('.') + 1, String::npos);
            }

            return result;
        }


#if (((MAZE_COMPILER == MAZE_COMPILER_CLANG) || (MAZE_COMPILER == MAZE_COMPILER_GNUC)) && (MAZE_PLATFORM != MAZE_PLATFORM_ANDROID))

        //////////////////////////////////////////
        inline StdString ToStdString(Size _value) noexcept { return ToStdString(static_cast<U64>(_value)); }

        //////////////////////////////////////////
        inline String ToString(Size _value) noexcept { return ToString(static_cast<U64>(_value)); }

        //////////////////////////////////////////
        inline StdString ToStdString(UnixTime _value) noexcept { return ToStdString(static_cast<U64>(_value)); }

        //////////////////////////////////////////
        inline String ToString(UnixTime _value) noexcept { return ToString(static_cast<U64>(_value)); }

#endif


        //////////////////////////////////////////
        inline StdString ToStdString(String const& _str)
        {
            return StdString(&_str[0], _str.size());
        }

        //////////////////////////////////////////
        inline String ToString(StdString const& _str)
        {
            return String(&_str[0], _str.size());
        }

        //////////////////////////////////////////
        inline StdString ToStdString(String&& _str)
        {
            return StdString(&_str[0], _str.size());
        }

        //////////////////////////////////////////
        inline String ToString(StdString&& _str)
        {
            return String(&_str[0], _str.size());
        }

        //////////////////////////////////////////
        inline StdString ToStdString(StdString const& _str)
        {
            return _str;
        }

        //////////////////////////////////////////
        inline String ToString(String const& _str)
        {
            return _str;
        }

        //////////////////////////////////////////
        template <typename TNumberType>
        inline CString ParseNumber(Char const* _str, Char const* _end, TNumberType& _value)
        {
            MAZE_ERROR("Unsupported number type!");
            return _str;
        }
        //////////////////////////////////////////
        template <>
        inline CString ParseNumber(Char const* _str, Char const* _end, S8& _value) { return ParseInteger<S8>(_str, _end, _value); }
        template <>
        inline CString ParseNumber(Char const* _str, Char const* _end, U8& _value) { return ParseInteger<U8>(_str, _end, _value); }
        template <>
        inline CString ParseNumber(Char const* _str, Char const* _end, S16& _value) { return ParseInteger<S16>(_str, _end, _value); }
        template <>
        inline CString ParseNumber(Char const* _str, Char const* _end, U16& _value) { return ParseInteger<U16>(_str, _end, _value); }
        template <>
        inline CString ParseNumber(Char const* _str, Char const* _end, S32& _value) { return ParseInteger<S32>(_str, _end, _value); }
        template <>
        inline CString ParseNumber(Char const* _str, Char const* _end, U32& _value) { return ParseInteger<U32>(_str, _end, _value); }
        template <>
        inline CString ParseNumber(Char const* _str, Char const* _end, S64& _value) { return ParseInteger<S64>(_str, _end, _value); }
        template <>
        inline CString ParseNumber(Char const* _str, Char const* _end, U64& _value) { return ParseInteger<U64>(_str, _end, _value); }
        template <>
        inline CString ParseNumber(Char const* _str, Char const* _end, F32& _value) { return ParseF32(_str, _end, _value); }
        template <>
        inline CString ParseNumber(Char const* _str, Char const* _end, F64& _value) { return ParseF64(_str, _end, _value); }


        //////////////////////////////////////////
        template <typename TChar>
        inline void SplitWords(TChar const* _line, Vector<ConstSpan<TChar>>& _words, TChar _separator = ' ') noexcept
        {
            _words.clear();
            Size prevPos = 0;

            TChar const* it = _line;

            for (; *it; it++)
            {
                if (*it == _separator)
                {
                    _words.emplace_back(_line + prevPos, it - (_line + prevPos));
                    prevPos = it - _line + 1;
                }
            }
            _words.emplace_back(_line + prevPos, it - (_line + prevPos));
        }

        //////////////////////////////////////////
        template <typename TChar>
        inline void SplitWords(TChar const* _line, Size _size, Vector<ConstSpan<TChar>>& _words, TChar _separator = ' ') noexcept
        {
            _words.clear();

            if (!_size)
                return;

            Size prevPos = 0;

            TChar const* __restrict it = _line;
            TChar const* __restrict end = _line + _size;

            for (; it != end; it++)
            {
                if (*it == _separator)
                {
                    _words.emplace_back(_line + prevPos, it - (_line + prevPos));
                    prevPos = it - _line + 1;
                }
            }
            _words.emplace_back(_line + prevPos, it - (_line + prevPos));
        }

        //////////////////////////////////////////
        template <typename TString, typename TChar = typename TString::value_type>
        inline void SplitWords(TString const& _line, Vector<TString>& _words, TChar _separator = ' ') noexcept
        {
            _words.clear();
            Size pos = 0;
            Size prevPos = 0;

            while ((pos = _line.find_first_of(_separator, pos)) != TString::npos)
            {
                _words.emplace_back(
                    _line.substr(prevPos, pos - prevPos));
                prevPos = ++pos;
            }

            _words.push_back(_line.substr(prevPos, pos - prevPos));
        }

        //////////////////////////////////////////
        template <typename TString, typename TChar = typename TString::value_type>
        inline bool IsNumber(TString const& _s) noexcept
        {
            return !_s.empty()
                && std::find_if(
                    _s.begin(),
                    _s.end(),
                    [](TChar _c)
                    {
                        return !std::isdigit(_c);
                    }) == _s.end();
        }

        //////////////////////////////////////////
        template <typename TString>
        inline void TrimString(TString& _str, bool _left = true, bool _right = true) noexcept
        {
            static TString const delims = " \t\r\n";

            if (_right)
                _str.erase(_str.find_last_not_of(delims) + 1);
            if (_left)
                _str.erase(0, _str.find_first_not_of(delims));
        }

        //////////////////////////////////////////
        template <typename TString>
        inline Size StringLength(TString const& _s)
        {
            return _s.size();
        }

        //////////////////////////////////////////
        inline Size StringLength(CString _s)
        {
            return strlen(_s);
        }

        //////////////////////////////////////////
        inline Size StringLength(CWString _s)
        {
            return wcslen(_s);
        }

        //////////////////////////////////////////
        template <typename TString, typename TSearchString, typename TReplaceString>
        inline void ReplaceSubstring(TString& _s, TSearchString const& _search, TReplaceString const& _replace) noexcept
        {
            Size searchSize = StringLength(_search);

            for (Size pos = 0, l = StringLength(_replace); ; pos += l)
            {
                // Locate the substring to replace
                pos = _s.find(_search, pos);
                if (pos == TString::npos)
                    break;

                // Replace by erasing and inserting
                _s.erase(pos, searchSize);
                _s.insert(pos, _replace);
            }
        }

        //////////////////////////////////////////
        template <typename TString, typename TSearchString>
        inline void RemoveSubstring(TString& _s, TSearchString const& _search) noexcept
        {
            Size searchSize = StringLength(_search);

            for (Size pos = 0; ; pos += 1)
            {
                pos = _s.find(_search, pos);
                if (pos == TString::npos)
                    break;

                _s.erase(pos, searchSize);
            }
        }

        //////////////////////////////////////////
        template <typename TString, typename TSymbolsString>
        inline void RemoveSymbols(TString& _s, TSymbolsString const& _symbols) noexcept
        {
            for (auto ch : _symbols)
                _s.erase(std::remove(_s.begin(), _s.end(), ch), _s.end());
        }

        //////////////////////////////////////////
        template <typename FloatType = F32>
        inline FloatType ParseFloatFast(CString _text) noexcept
        {
            CString p = _text;

            if (!*p || *p == '?')
                return 0;
            S32 s = 1;

            while (*p == ' ')
                p++;

            if (*p == '-')
            {
                s = -1;
                p++;
            }

            FloatType acc = 0;
            while (*p >= '0' && *p <= '9')
                acc = acc * 10 + *p++ - '0';

            if (*p == '.')
            {
                FloatType k = (FloatType)0.1;
                p++;
                while (*p >= '0' && *p <= '9')
                {
                    acc += (*p++ - '0') * k;
                    k *= (FloatType)0.1;
                }
            }

            return s * acc;
        }

        //////////////////////////////////////////
        template <typename TString>
        inline TString ToLower(TString const& _text) noexcept
        {
            TString copy = _text;
            std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
            return copy;
        }

        //////////////////////////////////////////
        template <typename TString>
        inline TString ToUpper(TString const& _text) noexcept
        {
            TString copy = _text;
            std::transform(copy.begin(), copy.end(), copy.begin(), ::toupper);
            return copy;
        }

        //////////////////////////////////////////
        template <typename TString>
        inline bool IsStartsWith(TString const& _value, TString const& _start) noexcept
        {
            if (_start.size() > _value.size())
                return false;

            return std::equal(_start.begin(), _start.end(), _value.begin());
        }

        //////////////////////////////////////////
        inline bool IsStartsWith(CString _value, CString _start) noexcept
        {
            return (strncmp(_value, _start, strlen(_start)) == 0);
        }

        //////////////////////////////////////////
        inline bool IsStartsWith(CWString _value, CWString _start) noexcept
        {
            return (wcsncmp(_value, _start, wcslen(_start)) == 0);
        }

        //////////////////////////////////////////
        template <typename TString>
        inline bool IsEndsWith(TString const& _value, TString const& _ending) noexcept
        {
            if (_ending.size() > _value.size())
                return false;

            return std::equal(_ending.rbegin(), _ending.rend(), _value.rbegin());
        }

        //////////////////////////////////////////
        inline bool IsEndsWith(CString _value, CString _ending) noexcept
        {
            Size valueSize = strlen(_value);
            Size endingSize = strlen(_ending);
            if (valueSize < endingSize)
                return false;

            return (strncmp(_value + valueSize - endingSize, _ending, endingSize) == 0);
        }

        //////////////////////////////////////////
        inline bool IsEndsWith(CWString _value, CWString _ending) noexcept
        {
            Size valueSize = wcslen(_value);
            Size endingSize = wcslen(_ending);
            if (valueSize < endingSize)
                return false;

            return (wcsncmp(_value + valueSize - endingSize, _ending, endingSize) == 0);
        }

        //////////////////////////////////////////
        template <typename TClass>
        inline void StringToObjectPtr(SharedPtr<TClass>& _value, String const& _str)
        {
            void* ptr;
            std::stringstream ss;
            ss << std::hex << _str;
            ss >> ptr;

            TClass* object = reinterpret_cast<TClass*>(ptr);
            if (object)
                _value = object->template cast<TClass>();
            else
                _value.reset();
        }

        //////////////////////////////////////////
        #define MAZE_FORMAT_VA_STRING(__text, __destString)                         \
        {                                                                           \
            va_list ___args;                                                        \
            va_start(___args, __text);                                              \
            Maze::StringHelper::FormatVAString(__text, ___args, __destString);      \
            va_end(___args);                                                        \
        }


    } // namespace StringHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeStringHelper_hpp_
